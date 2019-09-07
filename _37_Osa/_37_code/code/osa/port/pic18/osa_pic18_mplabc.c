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
 *  File:           osa_pic18_mplabc.c
 *
 *  Compilers:      Mplab C18
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    MPLABC18 specific functions definition
 *                  This file directly included in osa.c
 *
 *  History:        10.09.2010 -    File updated
 *
 ************************************************************************************************
 */




#define _OS_INIT_PROC_SPEC()      // Empty macro for MCC18






/************************************************************************************************
 *                                                                                              *
 *                           K E R N E L   F U N C T I O N S                                    *
 *                                                                                              *
 ************************************************************************************************/


void _OS_ClrReadySetClrCanContinue (void);  /* see description below    */



void _OS_JumpToTask (void)
{
    _OS_SET_FSR_CUR_TASK_W(OS_CODE_POINTER_SIZE + _OS_TASK_POINTER_OFFSET);

    if (_indf)
    {
        _indf1 = _fsr2l;
        _fsr2l = _fsr1l + 1;
    }
    _OS_TempH = _fsr1l;
    _fsr1l += _postdec;
    _OS_SET_PC();
    /* Now FSR points to OS_CurTask->State    */
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
    _OS_SET_FSR_CUR_TASK_W(OS_CODE_POINTER_SIZE + _OS_TASK_POINTER_OFFSET);
    _postdec = _fsr1l - _OS_TempH;

    _OS_SAVE_PC();

    _OS_bTaskReady = 1;

    _OS_POP();
}



/*
 ********************************************************************************
 *
 *  void _OS_ReturnNoSave (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Return to scheduler without saving context
 *
 *  parameters:     none
 *
 *  on return:      none
 *
 ********************************************************************************
 */

void _OS_ReturnNoSave (void)
{
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

void _OS_EnterWaitMode (void)
{
    _OS_Temp = 0;
    _asm    goto    _OS_ClrReadySetClrCanContinue   _endasm
}


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

void _OS_EnterWaitModeTO (void)
{
    _OS_Temp = 0xFF;
    _asm    goto    _OS_ClrReadySetClrCanContinue   _endasm
}


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

    _OS_SET_FSR_CUR_TASK_W(OS_CODE_POINTER_SIZE + _OS_TASK_POINTER_OFFSET);
    _postdec = _fsr1l - _OS_TempH;
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

void _OS_SET_FSR_CUR_TASK (void)
{
    _asm
        movlw   0
        goto __OS_SET_FSR_CUR_TASK_W
    _endasm
}


void __OS_SET_FSR_CUR_TASK_W (void)
{
    _asm
        addwf   _OS_CurTask + 0, 0, 0
        movwf   _fsr + 0, 0
        #if OS_BANK_TASKS == 0
            clrf    _fsr + 1, 0
        #else
            movlw   0
            addwfc  _OS_CurTask + 1, 0, 0
            movwf   _fsr + 1, 0
        #endif
    _endasm
}





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

    char temp;
    temp = 0;               // This variable used to prevent FSR changing
    if (Delay) temp = 1;    // when checking Delay after FSR set to task->Timer
                            // See "if (temp & 1)" below

    _OS_SET_FSR_CUR_TASK();
    _OS_bTaskCanContinue = 0;
    _OS_bTaskDelay = 0;     // Clear bDelay to prevent timer changing in interrupt

    if (temp & 1)
    {

        Delay ^= -1;
        Delay ++;

        _OS_CurTask->Timer = Delay;
        _OS_SET_FSR_CUR_TASK();
        _OS_bTaskDelay = 1;
        _OS_bTaskReady = 1;

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

char OS_DI (void)
{
    _asm
        movf    _intcon, 0, 0
        andlw   0xC0
        bcf     _intcon, 7, 0
        btfsc   _rcon, 7, 0
        bcf     _intcon, 6, 0
    _endasm
}

//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT)
//------------------------------------------------------------------------------

    void _OS_DI_INT (void)
    {
        _asm
            movf    _intcon, 0, 0
            andlw   0xC0
            bcf     _intcon, 7, 0
            btfsc   _rcon, 7, 0
            bcf     _intcon, 6, 0
            movwf   _OS_Temp_I, 0
        _endasm
    }

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

void _OS_RI ()
{
    _asm
        btfsc   _wreg, 7, 0
        bsf     _intcon, 7, 0
        btfsc   _rcon, 7, 0
        btfss   _wreg, 6, 0
        bra     _RI_EXIT
        bsf     _intcon, 6, 0
    _RI_EXIT:
    _endasm

}

//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT)
//------------------------------------------------------------------------------

    void _OS_RI_INT ()
    {
        _asm
            btfsc   _OS_Temp_I, 7, 0
            bsf     _intcon, 7, 0
            btfsc   _rcon, 7, 0
            btfss   _OS_Temp_I, 6, 0
            bra     _RI_INT_EXIT
            bsf     _intcon, 6, 0
        _RI_INT_EXIT:
        _endasm

    }

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
 *  parameters:     bEvent (passing througth _OS_TempH)
 *                  - zero, or non-zero condition
 *
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

#define bEvent  _OS_Temp

void _OS_CheckEvent (void)
{
    //------------------------------------------------------------------------------
    #if OS_PRIORITY_LEVEL == OS_PRIORITY_DISABLED
    //------------------------------------------------------------------------------

        _OS_Flags.bTimeout = 0;
        _OS_SET_FSR_CUR_TASK();

        if (bEvent)
        {
            #ifdef OS_ENABLE_TTIMERS
                _OS_bTaskDelay = 0;
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
                _OS_bTaskDelay = 0;
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
//------------------------------------------------------------------------------

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
 *  Overloaded in:  osa_pic18_htpicc.c
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
            if (!(bCreate & 1)) return 0;

            _asm
                movlw   _OS_StimersFree
                subwf   _fsr, 0, 0
                rlcf    _wreg, 1, 0
                rlcf    _wreg, 1, 0
                rlcf    _wreg, 0, 0
                andlw   0xF8

                movff   _indf, _OS_TempH  // Search for first "1" in _indf
                movwf   _OS_Temp, 0
                movlw   1
             OS_ST_GF_REPEAT:
                rrcf    _OS_TempH, 1, 0
                bc      OS_ST_GF_OK
                rlcf    _wreg, 1, 0
                incf    _OS_Temp, 1, 0
                bra     OS_ST_GF_REPEAT
             OS_ST_GF_OK:

                xorwf  _indf, 1, 0         // Clear bit of selected timer

            _endasm
            return _OS_Temp;

        }
        _fsr++;
    } while (--_OS_Temp);

    _OS_Flags.bError = 1;
    return 0xFF;
}


void    _OS_Stimer_Free (OST_UINT8 ID)
{
    _OS_Temp = ID;
    _asm
    __OS_Stimer_Free:
        lfsr    0, _OS_StimersFree
        movf    _OS_Temp, 0, 0
        rrcf    _wreg, 0, 0          //  wreg = ID / 8
        rrcf    _wreg, 0, 0
        rrcf    _wreg, 0, 0
        andlw   0x1F
        addwf   _fsr, 1, 0

        movlw   0x01                  // W = 1 << (ID & 7)
        btfsc   _OS_Temp, 2, 0
        swapf   _wreg, 0, 0
        btfsc   _OS_Temp, 1, 0
        rlcf    _wreg, 0, 0
        btfsc   _OS_Temp, 1, 0
        rlcf    _wreg, 0, 0
        btfsc   _OS_Temp, 0, 0
        rlcf    _wreg, 0, 0
        iorwf   _indf, 1, 0          // Free timer
    _endasm
    return   ;
}
//------------------------------------------------------------------------------
#endif      //  OS_STIMERS_ENABLE_ALLOCATION
//-----------------------------------------------------------------



//******************************************************************************
//  END OF FILE osa_pic18_mplabc.c
//******************************************************************************

