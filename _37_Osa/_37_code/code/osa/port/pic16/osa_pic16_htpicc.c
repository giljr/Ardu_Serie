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
 *  File:           osa_pic16_htpicc.c
 *
 *  Compilers:      HT-PICC STD
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    HT-PICC STD specific functions definition
 *                  This file directly included in osa.c
 *
 *  History:        10.09.2010 -    File updated
 * 
 *                  25.10.2010  -   Bug fixed: OS_Qtimer_Break did not deleted timer from the list
 *                              -   Bug fixed: interrupt became disables after calling OS_Qtimer_Run for the timer
 *                                  already presented in queue
 * 
 *                  30.10.2010  -   Another bug in qtimers fixed
 * 
 *
 ************************************************************************************************
 */



#define _OS_INIT_PROC_SPEC()      // Empty macro for PIC16


/************************************************************************************************
 *                                                                                              *
 *  MAKE FSR POINTER TO OS_CUR_TASK                                                             *
 *                                                                                              *
 *  (for code reducing)                                                                         *
 *                                                                                              *
 ************************************************************************************************/


#if defined(_ROMSIZE) && (_ROMSIZE <= 0x800)

    //----------------------------------------------------------------------
    #if !defined(__OSAPICCPRO__)
    //----------------------------------------------------------------------

            asm("   global __OS_SET_FSR_CUR_TASK        ");
            asm("   global __OS_SET_FSR_CUR_TASK_W      ");
            asm("__OS_SET_FSR_CUR_TASK:                 ");
            asm("       movlw       0                   ");
            asm("__OS_SET_FSR_CUR_TASK_W:               ");
            asm("       addwf       __OS_CurTask, w     ");
            asm("       movwf       __fsr               ");

        #if OS_BANK_TASKS >= 2

            asm("       bsf         __status, 7         ");

        #else

            asm("       bcf         __status, 7         ");

        #endif

            asm("       return                          ");

    //----------------------------------------------------------------------
    #else
    //----------------------------------------------------------------------
        void _OS_SET_FSR_CUR_TASK (void)
        {
            _fsr = (OS_FSR_TYPE) _OS_CurTask;
        }

        void _OS_SET_FSR_CUR_TASK_W (char temp)
        {
            _fsr = (OS_FSR_TYPE) _OS_CurTask + temp;
        }
    //----------------------------------------------------------------------
    #endif
    //----------------------------------------------------------------------

#endif









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
    _OS_Flags.bEventError = 0;       // Semaphore overflow flag

    //------------------------------------------------------------------------------
    // For one-byte semaphores
    #if OS_CSEM_SIZE == 1
        _fsr = (OS_FSR_TYPE)pCSem;
        _status &= 0x7F;        // IRP = 0
        _indf++;
        if (!_indf)
        {
            _indf--;
            _OS_Flags.bEventError = 1;   // Set error flag if overflow
        }

    //------------------------------------------------------------------------------
    // for two-bytes semaphores
    #elif OS_CSEM_SIZE == 2
        _fsr = (OS_FSR_TYPE)pCSem;
        _status &= 0x7F;        // IRP = 0
        asm("\t    movlw    1     ");
        asm("\t    addwf    __indf,f    ");
        asm("\t    incf     __fsr, f    ");
        asm("\t    btfsc    __status, 0     ");
        asm("\t    incf     __indf,f     ");
        asm("\t    movf     __indf, w     ");
        asm("\t    decf     __fsr, f     ");
        asm("\t    iorwf    __indf, w      ");
        if (ZERO) {
            asm("\t    movlw    -1     ");
            asm("\t    movwf    __indf     ");
            asm("\t    incf     __fsr,f     ");
            asm("\t    movwf    __indf     ");
            _OS_Flags.bEventError = 1;   // Set error flag if overflow
        }
    //------------------------------------------------------------------------------
    // for four-bytes semaphores
    #else
        (*pCSem)++;
        if (!*pCSem) {
            (*pCSem) = -1;
            _OS_Flags.bEventError = 1;   // Set error flag if overflow
        }
    #endif
    //------------------------------------------------------------------------------
}


//------------------------------------------------------------------------------
#if defined(OS_ENABLE_CSEM_INT)
//------------------------------------------------------------------------------

    #define _OS_Csem_Signal_I_DEFINED

    void _OS_Csem_Signal_I (OST_CSEM *pCSem)
    {
        _OS_Flags.bEventError = 0;       // Semaphore overflow flag

        //------------------------------------------------------------------------------
        // For one-byte semaphores
        #if OS_CSEM_SIZE == 1
            _fsr = (OS_FSR_TYPE)pCSem;
            _status &= 0x7F;        // IRP = 0
            _indf++;
            if (!_indf)
            {
                _indf--;
                _OS_Flags.bEventError = 1;   // Set error flag if overflow
            }

        //------------------------------------------------------------------------------
        // for two-bytes semaphores
        #elif OS_CSEM_SIZE == 2
            _fsr = (OS_FSR_TYPE)pCSem;
            _status &= 0x7F;        // IRP = 0
            asm("\t    movlw    1     ");
            asm("\t    addwf    __indf,f    ");
            asm("\t    incf     __fsr, f    ");
            asm("\t    btfsc    __status, 0     ");
            asm("\t    incf     __indf,f     ");
            asm("\t    movf     __indf, w     ");
            asm("\t    decf     __fsr, f     ");
            asm("\t    iorwf    __indf, w      ");
            if (ZERO) {
                asm("\t    movlw    -1     ");
                asm("\t    movwf    __indf     ");
                asm("\t    incf     __fsr,f     ");
                asm("\t    movwf    __indf     ");
                _OS_Flags.bEventError = 1;   // Set error flag if overflow
            }
        //------------------------------------------------------------------------------
        // for four-bytes semaphores
        #else
            (*pCSem)++;
            if (!*pCSem) {
                (*pCSem) = -1;
                _OS_Flags.bEventError = 1;   // Set error flag if overflow
            }
        #endif
        //------------------------------------------------------------------------------
    }

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------




//------------------------------------------------------------------------------
#endif  // OS_ENABLE_CSEM
//------------------------------------------------------------------------------



















/*
 ************************************************************************************************
 *                                                                                              *
 *                         C R I T I C A L   S E C T I O N S                                    *
 *                                                                                              *
 ************************************************************************************************
 */

//------------------------------------------------------------------------------
#ifdef OS_ENABLE_CRITICAL_SECTION
//------------------------------------------------------------------------------


/*
 ********************************************************************************
 *
 *  void OS_EnterCriticalSection (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Enter critical section.
 *                  This function disables interrupts (with saving current
 *                  state) and sets system flag bInCriticalSection
 *
 *  parameters:     none
 *
 *  on return:      none
 *
 *  note:           This function overloads "osa.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define OS_EnterCriticalSection_DEFINED
//------------------------------------------------------------------------------

void OS_EnterCriticalSection (void)
{
    OST_UINT temp;
    temp = OS_DI();
    _OS_Flags.bInCriticalSection = 1;

    _OS_Flags.bGIE_CTemp = 0;
    if (temp & 0x80) _OS_Flags.bGIE_CTemp = 1;

}



/*
 ********************************************************************************
 *
 *   void OS_LeaveCriticalSection (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Leave critical section.
 *                  This function restore interrupt state from OS_Flag temp bits
 *                  and clears system flag bInCriticalSection
 *
 *  parameters:     none
 *
 *  on return:      none
 *
 *  note:           This function overloads "osa.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define OS_LeaveCriticalSection_DEFINED
//------------------------------------------------------------------------------

void OS_LeaveCriticalSection (void)
{
    char temp;
    _OS_Flags.bInCriticalSection = 0;
    temp = 0;
    if (_OS_Flags.bGIE_CTemp) temp |= 0x80;
    OS_RI(temp);
}

//------------------------------------------------------------------------------
#endif      //     #ifdef OS_ENABLE_CRITICAL_SECTION
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

    _OS_SET_FSR_CUR_TASK();
    _OS_bTaskCanContinue = 0;
    _OS_bTaskDelay = 0;         // Clear bDelay to prevent timer changing in interrupt

    if (Delay)
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
 *  on return:      char - bit 7 = previous GIE value, bits 0..6 = 0
 *
 ********************************************************************************
 */

//-------------------------------------------------------------------------------
#define OS_DI_DEFINED
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
#if !defined(__OSAPICCPRO__)
//-------------------------------------------------------------------------------

    asm("   global _OS_DI           ");
    asm("_OS_DI:                    ");
    asm("   clrf    __status        ");
    asm("   movf    __intcon, w     ");
    asm("   andlw   0x80            ");
    #ifdef _OS_PIC16_GIE_LOOPED
        asm("_DI_REPEAT:                ");
        asm("   bcf     __intcon, 7     ");
        asm("   btfss   __intcon, 7     ");
        asm("   return                  ");
        asm("   goto    _DI_REPEAT      ");
    #else
        asm("   bcf     __intcon, 7     ");
        asm("   return                  ");
    #endif

//-------------------------------------------------------------------------------
#else
//-------------------------------------------------------------------------------

    char OS_DI (void)
    {
        char temp;
        temp = _intcon & 0x80;
        _intcon &= 0x7F;
        return temp;
    }

//-------------------------------------------------------------------------------
#endif //   __OSAPICCPRO__
//-------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#ifdef OS_ENABLE_INT
//------------------------------------------------------------------------------

    #if defined(_ROMSIZE) && (_ROMSIZE <= 0x800)

        asm("   global __OS_DI_INT       ");
        asm("__OS_DI_INT:                ");
        asm("   clrf    __status        ");
        asm("   rlf     __intcon, w     ");
        asm("_DI_INT_REPEAT:            ");
        asm("   bcf     __intcon, 7     ");
        #ifdef _OS_PIC16_GIE_LOOPED
            asm("   btfsc   __intcon, 7     ");
            asm("   goto    _DI_INT_REPEAT  ");
        #endif
        asm("   rrf     __OS_Temp_I, f  ");
        asm("   return                  ");



        asm("   global  __OS_RI_INT      ");
        asm("__OS_RI_INT:                ");
        asm("   clrf    __status        ");
        asm("   btfsc   __OS_Temp_I, 7  ");
        asm("   bsf     __intcon, 7     ");
        asm("   return                  ");

    #endif
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------








/*
 ********************************************************************************
 *
 *  OST_BOOL _OS_CheckEvent (char bEvent)
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
 *  on return:      1 - when condition is true, or timeout occured
 *                  0 - condition is false and no timeout
 *
 *  note:           This function overloads "osa.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define _OS_CheckEvent_DEFINED
//------------------------------------------------------------------------------

OST_BOOL _OS_CheckEvent (char bEvent)
{
    //------------------------------------------------------------------------------
    #if OS_PRIORITY_LEVEL == OS_PRIORITY_DISABLED
    //------------------------------------------------------------------------------

        _OS_Flags.bTimeout = 0;
        _OS_SET_FSR_CUR_TASK();

        _carry = 0;

        if (bEvent)
        {
            #if defined(OS_ENABLE_TTIMERS)
                _OS_bTaskDelay = 0;
            #endif

            return 1;
        }

        #if defined(OS_ENABLE_TTIMERS)

            asm("   btfss   __indf, 4     ");
            asm("   btfss   __indf, 5     ");
            asm("   return                ");
            _OS_Flags.bTimeout = 1;

            return 1;
        #else

            asm("   return  ");

        #endif

    //------------------------------------------------------------------------------
    #else
    //------------------------------------------------------------------------------

        _OS_Flags.bTimeout = 0;
        _OS_SET_FSR_CUR_TASK();
        _carry = 0;

        if (bEvent)
        {
            if (_OS_bTaskReady)
            {
                #if defined(OS_ENABLE_TTIMERS)
            EXIT_OK:
                #endif

                _carry = 1;
            }

            #if OS_PRIORITY_LEVEL == OS_PRIORITY_EXTENDED
            _OS_Flags.bEventOK = 1;
            #endif

            _OS_bTaskReady = 1;

            #if defined(OS_ENABLE_TTIMERS)
                if (_carry) _OS_bTaskDelay = 0;
            #endif

            asm("   return     ");
        }

        _OS_bTaskReady = 0;

        #if defined(OS_ENABLE_TTIMERS)

            asm("   btfss   __indf, 4     ");
            asm("   btfss   __indf, 5     ");
            asm("   return                ");
            _OS_Flags.bTimeout = 1;
            goto EXIT_OK;

        #else

            asm("   return  ");

        #endif

    //------------------------------------------------------------------------------
    #endif
    //------------------------------------------------------------------------------

}





/************************************************************************************************
 *                                                                                              *
 *                     W O R K   W I T H   D Y N A M I C   T I M E R S                          *
 *                                                                                              *
 ************************************************************************************************/

//-----------------------------------------------------------------
#ifdef OS_ENABLE_DTIMERS
//-----------------------------------------------------------------


/*
 ********************************************************************************
 *
 *  void _OS_Dtimer_Create (OST_DTIMER *dtimer)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    (Internal function called by system kernel througth
 *                  service OS_Dtimer_Create)
 *
 *                  Add dynamic timer into list of active dynamic timers. This
 *                  timer will be added at first position in list.
 *
 *
 *  parameters:     dtimer - pointer to dynamic timer.
 *                  (for PICC16 can be allocated only in OS_RAM_BANK0 and OS_RAM_BANK1)
 *
 *  on return:      none
 *
 *  note:           This function overloads "osa.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define _OS_Dtimer_Create_DEFINED
//------------------------------------------------------------------------------

void _OS_Dtimer_Create (OST_DTIMER *dtimer)
{
    _OS_Temp = OS_DI();

    _irp = 0;
    _fsr = (OS_FSR_TYPE) dtimer;
    _indf = *((OS_BANK char*)&_OS_DTimers.Flags);
    // Here: bActive is set,
    //       bTimeout is cleared,
    //       bRun is cleared
    //       bNextEnable is copied

    _fsr++;
    _indf = (OS_FSR_TYPE)_OS_DTimers.Next;

    _fsr++;
    _indf = 0;
    _fsr++;
    #if  OS_DTIMER_SIZE >= 2
        _indf = 0;
        _fsr++;
    #endif
    #if  OS_DTIMER_SIZE == 4
        _indf = 0;
        _fsr++;
        _indf = 0;
    #endif


    _OS_DTimers.Next = (OST_DTIMER*)dtimer;
    _OS_DTimers.Flags.bNextEnable = 1;

    #if !defined(__OSA_PIC12_HTPICC__)
    OS_RI(_OS_Temp);
    #endif
}





/*
 ********************************************************************************
 *
 *  void _OS_Dtimer_Delete (OST_DTIMER *dtimer)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    (Internal function called by system kernel througth
 *                  service OS_Dtimer_Delete)
 *
 *                  Delete dynamic timer from list of active timers. It is
 *                  recommended to delete all unused dynamic timers to avoid
 *                  speed down.
 *
 *  parameters:     dtimer - pointer to deleting dynamic timer
 *
 *  on return:      none
 *
 *  note:           This function overloads "osa.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define _OS_Dtimer_Delete_DEFINED
//------------------------------------------------------------------------------

void _OS_Dtimer_Delete (OST_DTIMER *dtimer)
{

    OST_DTIMER *ptcb;
    char flags;

    _irp = 0;
    _fsr = ((OS_FSR_TYPE)dtimer);
    flags = _indf;
    _OS_IndfTimerActive = 0;
    _fsr++;
    ptcb = (OST_DTIMER *)_indf;
    _fsr = (OS_FSR_TYPE)&_OS_DTimers;
    // Search timer in list of active timers
REPEAT:
    if (_OS_IndfTimerNextEnable)   // exit if timer not found
    {
        _fsr++;
        if ((OS_FSR_TYPE)_indf == (OS_FSR_TYPE)dtimer)  // Check pointer to next timer
        {
            _OS_Temp = OS_DI();         // Disable interrupts
            _indf = *((char*)&ptcb);    // Change pointer
            _fsr--;
            // Clear NextEnable flag if it was last timer
            if (!((OST_DTIMER_FLAGS*)&flags)->bNextEnable) _OS_IndfTimerNextEnable = 0;
            OS_RI(_OS_Temp);
            return;
        }
        _fsr = (OS_FSR_TYPE)_indf;
        goto REPEAT;
    }

}



//------------------------------------------------------------------------------
#endif      //  OS_ENABLE_DTIMERS
//------------------------------------------------------------------------------








//-----------------------------------------------------------------
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
 *
 ********************************************************************************
 */

//-----------------------------------------------------------------
#define _OS_Stimer_GetFree_DEFINED
//-----------------------------------------------------------------

OST_UINT8   _OS_Stimer_GetFree (OST_UINT bCreate)
{
    char i;

    _OS_Flags.bError = 0;
    _fsr = (OS_FSR_TYPE)_OS_StimersFree;
    _OS_SET_STIMER_IRP();
    _OS_Temp = (OS_STIMERS >> 3) + 1;
    i = 0;
    do
    {
        asm("   movf    __indf, w               ");
        asm("   btfsc   __status, 2             ");
        asm("   goto    OS_ST_GF_NEXT           ");

            if (!(bCreate & 1)) asm("   return ");

            asm("   movwf   __OS_TempH          ");
            asm("   movlw   __OS_StimersFree & 0xFF"); // w = (position in array) * 8
            asm("   subwf   __fsr, w            ");
            asm("   movwf   __OS_Temp           ");
            asm("   rlf     __OS_Temp, f        ");
            asm("   rlf     __OS_Temp, f        ");
            asm("   rlf     __OS_Temp, w        ");
            asm("   andlw   0xF8                ");

            asm("   clrf    __OS_Stimer_GetFree$i");  // Search for first "1" in _indf
            asm("   incf    __OS_Stimer_GetFree$i, f");
            asm("OS_ST_GF_REPEAT:               ");
            asm("   rrf     __OS_TempH, f       ");
            asm("   btfsc   __status, 0         ");
            asm("   goto    OS_ST_GF_OK         ");
            asm("   rlf     __OS_Stimer_GetFree$i, f");
            asm("   addlw   0x01                ");
            asm("   goto     OS_ST_GF_REPEAT    ");
            asm("OS_ST_GF_OK:                   ");

            asm("   movwf   __OS_Temp            ");
            asm("   comf    __OS_Stimer_GetFree$i, w");
            asm("   andwf   __indf, f            "); // Clear bit of selected timer
            asm("   movf    __OS_Temp, w         "); // Return bit position (timer's ID)
            asm("   return                      ");

        asm("OS_ST_GF_NEXT:                     ");
        _fsr++;
    } while (--_OS_Temp);

    _OS_Flags.bError = 1;
    return 0xFF;
}


//void    _OS_Stimer_Free (OST_UINT8 ID)
asm("   global  __OS_Stimer_Free            ");
asm("__OS_Stimer_Free:                      ");
asm("   clrf    __status                    ");
asm("   movwf   __OS_TempH                  ");

asm("   movwf   __OS_Temp                   ");     // fsr = &_OS_Stimer_Free[ID >> 3]
asm("   rrf     __OS_Temp, f                ");
asm("   rrf     __OS_Temp, f                ");
asm("   rrf     __OS_Temp, w                ");
asm("   andlw   0x1F                        ");
asm("   addlw   __OS_StimersFree & 0xFF       ");
asm("   movwf   __fsr                       ");
_OS_SET_STIMER_IRP();

asm("   clrf    __OS_Temp                   ");     // W = 1 << (IS & 7)
asm("   incf    __OS_Temp, f                ");
asm("   btfsc   __OS_TempH, 2               ");
asm("   swapf   __OS_Temp, f                ");
asm("   btfsc   __OS_TempH, 0               ");     // CARRY = 0 now
asm("   rlf     __OS_Temp, f                ");     // CARRY = 0 now
asm("   btfsc   __OS_TempH, 1               ");     // CARRY = 0 now
asm("   rlf     __OS_Temp, f                ");     // CARRY = 0 now
asm("   btfsc   __OS_TempH, 1               ");     // CARRY = 0 now
asm("   rlf     __OS_Temp, f                ");     // CARRY = 0 now

asm("   movf    __OS_Temp, w                ");
asm("   iorwf   __indf, f                   ");     // Free timer
asm("   return                              ");


//------------------------------------------------------------------------------
#endif      // OS_STIMERS_ENABLE_ALLOCATION
//-----------------------------------------------------------------












//-----------------------------------------------------------------
#ifdef OS_ENABLE_QTIMERS
//-----------------------------------------------------------------



/*
 ********************************************************************************
 *
 *  _OS_Qtimer_List
 *
 *------------------------------------------------------------------------------
 *
 *  description:    (Internal function called by system kernel througth service
 *                  OS)
 *
 *                  This function works with timer's list. It adds, deletes,
 *                  counts timers according to value of ftimer->Flags.
 *
 *  parameters:     OS_QTIMER_TYPE time   - new time for added timer (when
 *                                      _OS_Qtimers.Flags.bListFunction =
 *                                      FUNCTION_ADD)
 *                  OST_QTIMER *ftimer - pointer to timer to work on.
 *
 *  on return:      OS_QTIMER_TYPE:
 *                  1.  When bListGetTime == "1", this function returns remaining
                        time for current timer (flag bListFunction does not matter)
                    2.  When bListFunction == OS_QTIMER_LIST_FUNCTION_ADD, this
                        function adds new timer into list (before adding it checks
                        for timer is already in list; if so, then timer deleted first
                        and function executes twice (see "goto RESTART"))
                    3.  When bListFunction == OS_QTIMER_LIST_FUNCTION_DELETE, this
                        function deletes current timer
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define _OS_Qtimer_List_DEFINED
//------------------------------------------------------------------------------


OS_QTIMER_TYPE _OS_Qtimer_List (OS_QTIMER_TYPE time, OST_QTIMER *ftimer)
{
    OST_QTIMER_FLAGS    flags;
    OS_QTIMER_TYPE         temp, remain;
    unsigned char       _temp_i;        // For GIE save


RESTART:
    _fsr = (OS_FSR_TYPE)ftimer;           //flags = ftimer->Flags;
    _irp = 0;
    *(unsigned char*)&flags = _indf;

// *101030*    if (time == 0  && _OS_Qtimers.Flags.bListFunction == OS_QTIMER_LIST_FUNCTION_ADD)   /*101025*/
// *101030*    {
// *101030*        _OS_IndfFTimerTimeout = 1;
// *101030*        _OS_IndfFTimerActive  = 0;
// *101030*        goto EXIT;
// *101030*    }


    if (_OS_Qtimers.Flags.bListGetTime)
    {
        //--------------------------------------------
        // Clear ADD function if GET function present
        //--------------------------------------------
        _OS_Qtimers.Flags.bListFunction = !OS_QTIMER_LIST_FUNCTION_ADD;
    }
    else
    {
        //---------------------------------------
        // Skip deleting if timer is not in list
        //---------------------------------------
        if (_OS_Qtimers.Flags.bListFunction == OS_QTIMER_LIST_FUNCTION_DELETE && !flags.bActive) goto EXIT;
    }

    _OS_Qtimers.Flags.bAddAfterDelete = 0;

    if (_OS_Qtimers.Flags.bListFunction == OS_QTIMER_LIST_FUNCTION_ADD)
    {
        if (flags.bActive)              // If timer is already in list, it should be deleted first
        {
            _OS_Qtimers.Flags.bAddAfterDelete = 1;  // Set flag to execute ADD after deleting
            _OS_Qtimers.Flags.bListFunction = OS_QTIMER_LIST_FUNCTION_DELETE;
        }
        else
        {
            //---------------------------------------
            // Prepare timer for adding
            //---------------------------------------

            if (time == 0)                      /*101030*/
            {
                _OS_IndfFTimerTimeout = 1;
                _OS_IndfFTimerActive  = 0;
                goto EXIT;
            }
            _OS_IndfFTimerNextEnable = 0;       //ftimer->Flags.bNextEnable = 0;
            time ^= -1; // time = -time
            time++;
        }
    }
    remain = 0;                                 // Count remaining time

    _fsr = (OS_FSR_TYPE)&_OS_Qtimers;             // Start search from header
    // irp is already cleared

    _temp_i = _intcon;
    di();

    if (_OS_Qtimers.Flags.bNextEnable)
    {
        do
        {
            _fsr ++;    // Now FSR points to ftcb->next

            if (_OS_Qtimers.Flags.bListFunction == OS_QTIMER_LIST_FUNCTION_DELETE && !_OS_Qtimers.Flags.bListGetTime)
            {
                if (_indf == (unsigned char)ftimer)     //********* Deleting ***************
                {
                    _fsr--;
                    _OS_IndfFTimerNextEnable = 0;
                    if (flags.bNextEnable) _OS_IndfFTimerNextEnable = 1;

                    _fsr++;
                    *((unsigned char*)&temp + 0) = _fsr;    // Save address of ftcb->Next
                    _fsr = _indf;
                    _OS_IndfFTimerActive = 0;               // ftimer->Falgs.bActive = 0;
                    _fsr++;
                    *((unsigned char*)&remain + 0) = _indf; // Save ftimer->Next

                    if (flags.bNextEnable)                  // Save ftimer->Timer
                    {
                        _fsr++;

                        *((unsigned char *)&time + 0) = _indf;
                    #if OS_QTIMER_SIZE > 1
                        _fsr++; *((unsigned char*)&time + 1) = _indf;
                    #if OS_QTIMER_SIZE > 2
                        _fsr++; *((unsigned char*)&time + 2) = _indf;
                        _fsr++; *((unsigned char*)&time + 3) = _indf;
                    #endif
                    #endif
                    }

                    _fsr = *((unsigned char*)&temp + 0);    // Restore address of ftcb->Next
                    _indf = *((unsigned char*)&remain + 0); // ftcb->Next = ftimer->Next

                    if (flags.bNextEnable)                  // Add ftimer->Timer to ftcb->Timer
                    {
                        _fsr = _indf + 2;

                        _indf += *((unsigned char*)&time + 0);
                    #if OS_QTIMER_SIZE == 2
                        _fsr++;
                        if (CARRY) _indf++;
                        _indf += *((unsigned char*)&time + 1);
                    #endif
                    #if OS_QTIMER_SIZE == 4
                        if (CARRY) time -= 0x100;
                        _fsr++;
                        _indf += *((unsigned char *)&time + 1);
                        if (CARRY) time -= 0x10000;
                        _fsr++;
                        _indf += *((unsigned char *)&time + 2);
                        if (CARRY) time -= 0x1000000;
                        _fsr++;
                        _indf += *((unsigned char *)&time + 3);
                    #endif
                    }

                    if (_OS_Qtimers.Flags.bAddAfterDelete)
                    {
                        //-----------------------------------------------------------
                        // It was deleting before adding.
                        // Set ADD flag and restart searching
                        //-----------------------------------------------------------

                        _OS_Qtimers.Flags.bListFunction = OS_QTIMER_LIST_FUNCTION_ADD;
                        if (_temp_i & 0x80) _gie = 1;   /*101025*/
                        goto RESTART;
                    }
                    goto EXIT_RI;

                }   // Deleting: if frs->Next = ftimer
            }
            else   // _OS_Qtimers.Flags.bListFunction == OS_QTIMER_LIST_FUNCTION_DELETE && !_OS_Qtimers.Flags.bListGetTime
            {
                                                    //********* Adding or Reading **************
                _OS_Temp = _fsr;        // Store pointer to current timer->Next
                _fsr = _indf + 2;       // Fsr -> fsr->Next->Timer

                //------------------------------------------------------------------------------
                // Copy time value into temp variable

                *((unsigned char *)&temp + 0) = _indf;
                #if   OS_QTIMER_SIZE > 1
                    _fsr++; *((unsigned char *)&temp + 1) = _indf;
                #if   OS_QTIMER_SIZE > 2
                    _fsr++; *((unsigned char *)&temp + 2) = _indf;
                    _fsr++; *((unsigned char *)&temp + 3) = _indf;
                #endif
                #endif

                _fsr = _OS_Temp;    // fsr points to ftcb->Next

                if (_OS_Qtimers.Flags.bListGetTime)
                {
                    if ((unsigned char)(_fsr-1) == (OS_FSR_TYPE)ftimer) goto EXIT_RI;
                    remain -= temp;
                }
                else
                {
                    if (temp && temp <= time)
                    {
                        _fsr--;
                        break;
                    }
                }
            }

            //------------------------------------------------------------------------------
            // Now FSR points to ftcb->next
            //------------------------------------------------------------------------------
            if (_temp_i & 0x80) _gie = 1;
            time -= temp;
            _fsr = _indf;
            di();

        } while (_OS_IndfFTimerNextEnable);
    }

    //**************************************
    //               Adding
    //**************************************
    if (_OS_Qtimers.Flags.bListFunction == OS_QTIMER_LIST_FUNCTION_ADD)
    {
        *((unsigned char *)&remain+0) = _fsr;   // Store &ftcb
        *((unsigned char *)&flags)    = _indf;  // store "flags"
        _fsr++;
        *((unsigned char *)&temp + 0) = _indf;  // Store ftcb->Next
        _fsr = ((OS_FSR_TYPE)ftimer);             // fsr = ftimer
        _OS_IndfFTimerActive        = 1;
        _OS_IndfFTimerTimeout       = 0;
        _fsr++;
        _indf = *((unsigned char *)&temp + 0);  // ftimer->Next = stored "Next"

        _fsr++;                                 // ftimer->Timer = time
        //------------------------------------------------------------------------------
        _indf = *((unsigned char *)&time + 0);
    #if     OS_QTIMER_SIZE > 1
        _fsr++; _indf = *((unsigned char *)&time + 1);
    #if     OS_QTIMER_SIZE > 2
        _fsr++; _indf = *((unsigned char *)&time + 2);
        _fsr++; _indf = *((unsigned char *)&time + 3);
    #endif
    #endif
        //------------------------------------------------------------------------------

        _fsr = *((unsigned char *)&remain + 0);     // Restore: _fsr = ftcb
        if (flags.bTimeout) _fsr = (OS_FSR_TYPE)&_OS_Qtimers;

        _OS_IndfFTimerNextEnable = 1;               // ftcb->Next = ftimer
        _fsr++;
        _indf = (unsigned char) ftimer;

        _fsr = (OS_FSR_TYPE)ftimer;

        if (flags.bNextEnable)               // Decrement next timer: ftimer->Next->Timer -= time
        {
            _OS_IndfFTimerNextEnable = 1;
            _fsr = *((unsigned char *)&temp + 0) + 2;    // _fsr = &ftimer->Next->Timer

            //------------------------------------------------------------------------------
            _indf -= *((unsigned char *)&time + 0);

            #if   OS_QTIMER_SIZE == 2

                _fsr++;
                if (!CARRY) _indf--;
                _indf -= *((unsigned char*)&time + 1);

            #endif

            #if   OS_QTIMER_SIZE == 4

                if (!CARRY) time += 0x100;
                _fsr++;
                _indf -= *((unsigned char *)&time + 1);
                if (!CARRY) time += 0x10000;
                _fsr++;
                _indf -= *((unsigned char *)&time + 2);
                if (!CARRY) time += 0x1000000;
                _fsr++;
                _indf -= *((unsigned char *)&time + 3);

            #endif
            //------------------------------------------------------------------------------
        }


    }

EXIT_RI:
    if (_temp_i & 0x80) _gie = 1;
EXIT:
    _OS_Qtimers.Flags.bListFunction = OS_QTIMER_LIST_FUNCTION_ADD;
    _OS_Qtimers.Flags.bListGetTime  = 0;
    return remain;
}




//------------------------------------------------------------------------------
#endif      // OS_ENABLE_QTIMERS
//------------------------------------------------------------------------------



//******************************************************************************
//  END OF FILE osa_pic16_htpicc.c
//******************************************************************************

