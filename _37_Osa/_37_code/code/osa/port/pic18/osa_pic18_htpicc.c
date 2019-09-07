/*
 ************************************************************************************************
 *
 *  OSA cooperative RTOS for microcontrollers PIC, AVR and STM8
 *
 *  OSA is distributed under BSD license (see license.txt)
 *
 *  URL:            http://wiki.pic24.ru/doku.php/en/osa/ref/intro
 *
 *----------------------------------------------------------------------------------------------
 *
 *  File:           osa_pic18_htpicc.c
 *
 *  Compilers:      HT-PICC18 STD
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    HT-PICC18 STD specific functions definition
 *                  This file directly included in osa.c
 *
 *  History:        13.09.2010 -    File updated
 *
 ************************************************************************************************
 */




#define _OS_INIT_PROC_SPEC()      // Empty macro for PICC18



/************************************************************************************************
 *                                                                                              *
 *                           K E R N E L   F U N C T I O N S                                    *
 *                                                                                              *
 ************************************************************************************************/


void _OS_ClrReadySetClrCanContinue (void);  /* see description below    */



void _OS_JumpToTask (void)
{
    _OS_SET_FSR_CUR_TASK();
    *((near char*)&_OS_State) = _postinc;
    _OS_SET_PC();
}



/*
 ********************************************************************************
 *
 *  void _OS_ReturnSave (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Return to sheduler with saving current task return point
 *
 *  parameters:     none
 *
 *  on return:      none
 *
 ********************************************************************************
 */

void _OS_ReturnSave (void)
{
    _OS_SET_FSR_CUR_TASK();
    _OS_SAVE_PC();
    _OS_bTaskReady = 1;
    asm("    global __OS_ReturnNoSave   ");
    asm("__OS_ReturnNoSave:             ");
    _OS_POP();
}


/*
 ********************************************************************************
 *
 *  void __OS_EnterWaitMode (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Clear bReady flag and return to the sheduler with seaving
 *                  task return point
 *
 *  parameters:     none
 *
 *  on return:      none
 *
 *
 ********************************************************************************
 */

asm("   global  __OS_EnterWaitMode");
asm("__OS_EnterWaitMode:     ");
asm("   clrf    __OS_Temp, c    ");
asm("   global  __OS_ClrReadySetClrCanContinue  ");
asm("   goto    __OS_ClrReadySetClrCanContinue  ");


/*
 ********************************************************************************
 *
 *  void __OS_EnterWaitModeTO (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Clear bReady flag, set bCanContinue flag and return to the
 *                  sheduler with seaving task return point
 *
 *  parameters:     none
 *
 *  on return:      none
 *
 *
 ********************************************************************************
 */

asm("   global  __OS_EnterWaitModeTO");
asm("__OS_EnterWaitModeTO:     ");
asm("   setf    __OS_Temp, c    ");
asm("   global  __OS_ClrReadySetClrCanContinue  ");
asm("   goto    __OS_ClrReadySetClrCanContinue  ");



/*
 ********************************************************************************
 *
 *  void _OS_ClrReadySetClrCanContinue (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Addition function. Called only by GOTO from:
 *                      _OS_EnterWaitMode()
 *                      _OS_EnterWaitModeTO()
 *                  Stack has pointer of task who called theese services.
 *
 *  parameters:     none
 *
 *  on return:      none
 *
 ********************************************************************************
 */

void _OS_ClrReadySetClrCanContinue (void)
{


    _OS_Flags.bEventError = 0;

    _OS_SET_FSR_CUR_TASK();
    _OS_SAVE_PC();

    _OS_bTaskReady = 0;

    #if defined(OS_ENABLE_TTIMERS)

        if (!_OS_bTaskDelay) _OS_bTaskCanContinue = 0;
        if (_OS_Temp & 0x01) _OS_bTaskCanContinue = 1;

    #endif

}
















/************************************************************************************************
 *                                                                                              *
 *  MAKE FSR POINTER TO OS_CUR_TASK                                                             *
 *                                                                                              *
 *  (for code reducing)                                                                         *
 *                                                                                              *
 ************************************************************************************************/

    asm("   global __OS_SET_FSR_CUR_TASK        ");
    asm("__OS_SET_FSR_CUR_TASK:                 ");
    _OS_PIC18_ERRATA_NOP();
    asm("       movf        __OS_CurTask+0, w, c");
    asm("       movwf       __fsr + 0, c        ");
#if OS_BANK_TASKS == 0
    asm("       clrf        __fsr + 1, c        ");
#else
    asm("       movf        __OS_CurTask+1, w, c");
    asm("       movwf       __fsr + 1, c        ");
#endif
    asm("       return                          ");



















/*
 ************************************************************************************************
 *                                                                                              *
 *                            C O U N T I N G    S E M A P H O R E S                            *
 *                                                                                              *
 ************************************************************************************************
 */


//------------------------------------------------------------------------------
#ifdef OS_ENABLE_CSEM
//------------------------------------------------------------------------------


/*
 ********************************************************************************
 *
 *  void _OS_Csem_Signal (OST_CSEM *pCSem)
 *
 *------------------------------------------------------------------------------
 *
 *  Increase counting semaphore. Set EventError if csem is FF...
 *
 *
 *  parameters:     pCSem - pointer to counting semaphore
 *
 *  on return:      OS_IsEventError()
 *
 *  note:           This function overloads "osa.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define _OS_Csem_Signal_DEFINED
//------------------------------------------------------------------------------

void _OS_Csem_Signal (OST_CSEM *pCSem)
{
    _OS_Flags.bEventError = 0;               // Counting semaphore overflow bit

    //------------------------------------------------------------------------------
    //
    #if OS_CSEM_SIZE == 1

        _fsr = (OS_FSR_TYPE)pCSem;
        _indf++;
        if (!_indf)
        {
            _indf--;
            _OS_Flags.bEventError = 1;   // Set overflow bit
        }
    //------------------------------------------------------------------------------
    // For two- and four-bytes semaphores
    //
    #else
        (*pCSem)++;
        if (!*pCSem) {
            (*pCSem) = -1;
            _OS_Flags.bEventError = 1;   // Set overflow bit
        }
    #endif
    //------------------------------------------------------------------------------

}


//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT_CSEM)
//------------------------------------------------------------------------------

    #define _OS_Csem_Signal_I_DEFINED

    void _OS_Csem_Signal_I (OST_CSEM *pCSem)
    {
        _OS_Flags.bEventError = 0;               // Counting semaphore overflow bit

        //------------------------------------------------------------------------------
        //
        #if OS_CSEM_SIZE == 1

            _fsr = (OS_FSR_TYPE)pCSem;
            _indf++;
            if (!_indf)
            {
                _indf--;
                _OS_Flags.bEventError = 1;   // Set overflow bit
            }
        //------------------------------------------------------------------------------
        // For two- and four-bytes semaphores
        //
        #else
            (*pCSem)++;
            if (!*pCSem) {
                (*pCSem) = -1;
                _OS_Flags.bEventError = 1;   // Set overflow bit
            }
        #endif
        //------------------------------------------------------------------------------

    }

//------------------------------------------------------------------------------
#endif      // OS_ENABLE_INT_CSEM
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
#endif      // OS_INABLE_CSEM
//------------------------------------------------------------------------------



























/*
 ************************************************************************************************
 *                                                                                              *
 *                                   D E L A Y S   I N   T A S K                                *
 *                                                                                              *
 ************************************************************************************************
 */

//------------------------------------------------------------------------------
#ifdef OS_ENABLE_TTIMERS
//------------------------------------------------------------------------------

/*
 ********************************************************************************
 *
 *   void _OS_InitDelay (OS_TTIMER_TYPE Delay)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    (Internal function called by system kernel)
 *
 *                  Init task timer delay.Set timer variable in task descriptor,
 *                  sets bit bDelay and clears bit bCanContinue.
 *
 *                  _OS_CurTask must point to descriptor.
 *
 *
 *  parameters:     Delay   - time of delay in system ticks
 *
 *  on return:      none
 *
 *  note:           This function overloads "osa.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define _OS_InitDelay_DEFINED
//------------------------------------------------------------------------------

void _OS_InitDelay (OS_TTIMER_TYPE Delay)
{

    _OS_bTaskCanContinue = 0;
    _OS_bTaskDelay = 0;         // Clear bDelay to prevent timer changing in interrupt
    _OS_SET_FSR_CUR_TASK();
    _indf = *((near char*)&_OS_State);

    if (Delay)
    {

        Delay ^= -1;
        Delay ++;

        _OS_CurTask->Timer = Delay;

        _OS_bTaskDelay = 1;
        _OS_bTaskReady = 1;
        _OS_SET_FSR_CUR_TASK();
        _indf = *((near char*)&_OS_State);

    }
}

//------------------------------------------------------------------------------
#endif  // #ifdef OS_ENABLE_TTIMERS
//------------------------------------------------------------------------------












/************************************************************************************************
 *                                                                                              *
 *                            S Y S T E M   F U N C T I O N S                                   *
 *                                                                                              *
 ************************************************************************************************/




/*
 ********************************************************************************
 *
 *  char OS_DI (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Disable interrupt with GIE saving
 *
 *  parameters:     none
 *
 *  on return:      char - bit 7 = previous GIEH value,
 *                         bit 6 = previous GIEL value
 *                         bits 0..5 = 0
 *
 ********************************************************************************
 */

//-------------------------------------------------------------------------------
#define OS_DI_DEFINED
//-------------------------------------------------------------------------------

    asm("   global _OS_DI           ");
    asm("_OS_DI:                    ");
    asm("   dw      0xFFFF          ");
    asm("   movf    __intcon, w, c  ");
    asm("   andlw   0xC0            ");
    asm("   bcf     __intcon, 7, c  ");
    asm("   btfsc   __rcon, 7, c    ");
    asm("   bcf     __intcon, 6, c  ");
    asm("   return                  ");
    asm("   dw      0xFFFF          ");

//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT)
//------------------------------------------------------------------------------

    asm("   global __OS_DI_INT       ");
    asm("__OS_DI_INT:                ");
    asm("   dw      0xFFFF          ");
    asm("   movf    __intcon, w, c  ");
    asm("   andlw   0xC0            ");
    asm("   bcf     __intcon, 7, c  ");
    asm("   btfsc   __rcon, 7, c    ");
    asm("   bcf     __intcon, 6, c   ");
    asm("   movwf   __OS_Temp_I, c  ");
    asm("   return                  ");
    asm("   dw      0xFFFF          ");

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------


/*
 ********************************************************************************
 *
 *  char OS_RI (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Restore saved by OS_DI() GIEx states
 *
 *  parameters:     char - bit 7 = previos GIE value, bits 0..6 = 0
 *
 *  on return:      none
 *
 ********************************************************************************
 */

//-------------------------------------------------------------------------------
#define OS_RI_DEFINED
//-------------------------------------------------------------------------------

    asm("   global  _OS_RI          ");
    asm("_OS_RI:                    ");
    asm("   dw      0xFFFF          ");
    asm("   btfsc   __wreg, 7, c     ");
    asm("   bsf     __intcon, 7, c   ");
    asm("   btfsc   __rcon, 7, c     ");
    asm("   btfss   __wreg, 6, c     ");
    asm("   bra     _RI_EXIT        ");
    asm("   bsf     __intcon, 6, c   ");
    asm("_RI_EXIT:                  ");
    asm("   return                  ");
    asm("   dw      0xFFFF          ");


//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT)
//------------------------------------------------------------------------------

    asm("   global  __OS_RI_INT          ");
    asm("__OS_RI_INT:                    ");
    asm("   dw      0xFFFF              ");
    asm("   btfsc   __OS_Temp_I, 7, c   ");
    asm("   bsf     __intcon, 7, c      ");
    asm("   btfsc   __rcon, 7, c        ");
    asm("   btfss   __OS_Temp_I, 6, c   ");
    asm("   bra     _RI_INT_EXIT        ");
    asm("   bsf     __intcon, 6, c      ");
    asm("_RI_INT_EXIT:                  ");
    asm("   return                      ");
    asm("   dw      0xFFFF              ");

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------





/*
 ********************************************************************************
 *
 *  void _OS_CheckEvent (char bEvent)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    (Internal function called by system kernel througth
 *                  waiting services)
 *
 *                  Check condition of bEvent. Accordint to bEvent value and
 *                  current state of bReady, bDelay and bCanContinue flags,
 *                  task becames ready to execute or still remains in waiting
 *                  mode.
 *
 *  parameters:     bEvent - zero, or non-zero condition
 *
 *  on return:      none
 *
 *  note:           This function overloads "osa.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define _OS_CheckEvent_DEFINED
//------------------------------------------------------------------------------

void _OS_CheckEvent (OST_UINT bEvent)
{
    //------------------------------------------------------------------------------
    #if OS_PRIORITY_LEVEL == OS_PRIORITY_DISABLED
    //------------------------------------------------------------------------------

        _OS_Flags.bTimeout = 0;
        _OS_SET_FSR_CUR_TASK();

        if (bEvent)
        {
            #ifdef OS_ENABLE_TTIMERS

                _OS_bTaskDelay = 0; // Clear bDelay in temporary variable _OS_State
                _indf &= ~0x10;     // Clear bDelay in task descriptor

            #endif
            return;
        }

        #ifdef OS_ENABLE_TTIMERS

            if (_OS_bTaskTimeout && _OS_bTaskCanContinue)
            {
                _OS_Flags.bTimeout = 1;
                return;
            }

        #endif

        _OS_POP();

    //------------------------------------------------------------------------------
    #else
    //------------------------------------------------------------------------------

        _OS_Flags.bTimeout = 0;
        _OS_SET_FSR_CUR_TASK();

        if (bEvent)
        {
            if (_OS_bTaskReady)
            {
                #ifdef OS_ENABLE_TTIMERS

                    _OS_bTaskDelay = 0; // Clear bDelay in temporary variable _OS_State
                    _indf &= ~0x10;     // Clear bDelay in task descriptor

                #endif

                #if OS_PRIORITY_LEVEL == OS_PRIORITY_EXTENDED
                _OS_Flags.bEventOK = 1;
                #endif

                return;
            }

            _OS_bTaskReady = 1;

        } else {
            _OS_bTaskReady = 0;
        }

        #ifdef OS_ENABLE_TTIMERS

            if (_OS_bTaskTimeout && _OS_bTaskCanContinue)
            {
                _OS_bTaskReady = 1;
                _OS_Flags.bTimeout = 1;

                #if OS_PRIORITY_LEVEL == OS_PRIORITY_EXTENDED
                _OS_Flags.bEventOK = 1;
                #endif

                return;
            }

        #endif

        _OS_POP();

    //------------------------------------------------------------------------------
    #endif
    //------------------------------------------------------------------------------
}







//------------------------------------------------------------------------------
#if (OS_STIMERS > 0 ) && defined(OS_STIMERS_ENABLE_ALLOCATION)
//-----------------------------------------------------------------

/*
 ********************************************************************************
 *
 *  _OS_Stimer_GetFree
 *
 *------------------------------------------------------------------------------
 *
 *  description:    (Internal function called by system kernel througth service OS
 *                  Searches free static timer and returns it's ID
 *
 *
 *  parameters:     OST_UINT bCreate - =0 - do not created timer, just check for
 *                                               aviability
 *                                          =1 - create timer and mark it asm used
 *
 *  on return:      none
 *
 ********************************************************************************
 */

//-----------------------------------------------------------------
#define _OS_Stimer_GetFree_DEFINED
//-----------------------------------------------------------------


OST_UINT8   _OS_Stimer_GetFree (OST_UINT bCreate)
{
    _OS_Flags.bError = 0;
    _fsr = (OS_FSR_TYPE)_OS_StimersFree;
    _OS_Temp = (OS_STIMERS >> 3) + 1;
    do
    {
        if (_indf)
        {
            if (!(bCreate & 1)) asm("   return ");

            asm("   movlw   low(__OS_StimersFree) "); // w = (position in array) * 8
            asm("   subwf   __fsr, w, c         ");
            asm("   rlcf    __wreg, f, c        ");
            asm("   rlcf    __wreg, f, c        ");
            asm("   rlcf    __wreg, w, c        ");
            asm("   andlw   0xF8                ");

            asm("   movff   __indf, __OS_TempH  "); // Search for first "1" in _indf
            asm("   movwf   __OS_Temp, c        ");
            asm("   movlw   1                   ");
            asm("OS_ST_GF_REPEAT:               ");
            asm("   rrcf    __OS_TempH, f, c    ");
            asm("   bc      OS_ST_GF_OK         ");
            asm("   rlcf    __wreg, f, c        ");
            asm("   incf    __OS_Temp, f, c     ");
            asm("   bra     OS_ST_GF_REPEAT     ");
            asm("OS_ST_GF_OK:                   ");

            asm("   xorwf  __indf, f, c         "); // Clear bit of selected timer
            asm("   movf   __OS_Temp, w, c      "); // Return bit position (timer's ID)
            asm("   return                      ");
        }
        _fsr++;
    } while (--_OS_Temp);

    _OS_Flags.bError = 1;
    return 0xFF;
}


//void    _OS_Stimer_Free (OST_UINT8 ID)
asm("   global  __OS_Stimer_Free            ");
asm("__OS_Stimer_Free:                      ");
asm("   dw      0xFFFF                      ");
asm("   lfsr    0, __OS_StimersFree         ");
asm("   movwf   __OS_Temp, c                ");
asm("   rrcf    __wreg, w, c                ");     //  wreg = ID / 8
asm("   rrcf    __wreg, w, c                ");
asm("   rrcf    __wreg, w, c                ");
asm("   andlw   0x1F                        ");
asm("   addwf   __fsr, f, c                 ");

asm("   movlw   0x01                        ");     // W = 1 << (ID & 7)
asm("   btfsc   __OS_Temp, 2, c             ");
asm("   swapf   __wreg, w, c                ");
asm("   btfsc   __OS_Temp, 1, c             ");
asm("   rlcf    __wreg, w, c                ");
asm("   btfsc   __OS_Temp, 1, c             ");
asm("   rlcf    __wreg, w, c                ");
asm("   btfsc   __OS_Temp, 0, c             ");
asm("   rlcf    __wreg, w, c                ");

asm("   iorwf   __indf, f, c                ");     // Free timer
asm("   return                              ");


//------------------------------------------------------------------------------
#endif          //  OS_STIMERS_ENABLE_ALLOCATION
//-----------------------------------------------------------------



//******************************************************************************
//  END OF FILE osa_pic18_htpicc.c
//******************************************************************************

