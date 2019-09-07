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
 *  File:           osa_system.c
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    System functions
 *                  This file directly included in osa.c
 *
 *  History:        12.09.2010 -    File updated
 *
 ************************************************************************************************
 */






/************************************************************************************************
 *                                                                                              *
 *                                                                                              *
 *                         F U N C T I O N   D E F I N I T I O N S                              *
 *                                                                                              *
 *                                                                                              *
 ************************************************************************************************/




/*
 ********************************************************************************
 *
 *   void OS_Init (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    (Internal function called by system kernel)
 *
 *                  Initialize all system variables:
 *                  - Clear all binary semaphores
 *                  - Free all task descriptors
 *                  - Clear all timeouts for old style static timers
 *                  - Set _OS_LastTask and _OS_CurTask variables at last task
 *                    descriptor
 *
 *  parameters:     none
 *
 *  on return:      _OSCurTask points to last task descriptor.
 *
 *  Overloaded in:  -
 *
 ********************************************************************************
 */


void OS_Init (void)
{
    #ifdef __OSA_PIC24_MPLABC__
    _OS_Flags = (OST_SYSTEM_FLAGS){0};
    #else
    *(OS_RAM_NEAR OST_WORD*)&_OS_Flags = 0;
    #endif

    _OS_INIT_PROC_SPEC();     /* Specific controller type's init operations   */



    #if OS_PRIORITY_LEVEL == OS_PRIORITY_NORMAL

        _OS_LastTask = (OST_TASK_POINTER) _OS_Tasks + (OS_TASKS - 1);

    #endif



    /*--------------------------------------*
     *                                      *
     *  Zero all old style static timers    *
     *                                      *
     *--------------------------------------*/

    #if OS_TIMERS > 11*8 || ((OS_TIMERS > 0) && defined(__OSA_PIC24_MPLABC__))

        _OS_Temp = sizeof(OS_Timeouts) / sizeof(OST_WORD);
        do
        {
            OS_Timeouts[_OS_Temp-1] = 0;
        } while (--_OS_Temp);

    #else
        #if OS_TIMERS > 0
            OS_Timeouts[0] = 0;
        #endif

        #if OS_TIMERS > 1*8
            OS_Timeouts[1] = 0;
        #endif

        #if OS_TIMERS > 2*8
            OS_Timeouts[2] = 0;
        #endif

        #if OS_TIMERS > 3*8
            OS_Timeouts[3] = 0;
        #endif

        #if OS_TIMERS > 4*8
            OS_Timeouts[4] = 0;
        #endif

        #if OS_TIMERS > 5*8
            OS_Timeouts[5] = 0;
        #endif

        #if OS_TIMERS > 6*8
            OS_Timeouts[6] = 0;
        #endif

        #if OS_TIMERS > 7*8
            OS_Timeouts[7] = 0;
        #endif

        #if OS_TIMERS > 8*8
            OS_Timeouts[8] = 0;
        #endif

        #if OS_TIMERS > 9*8
            OS_Timeouts[9] = 0;
        #endif

        #if OS_TIMERS > 10*8
            OS_Timeouts[10] = 0;
        #endif

    #endif


    /*--------------------------------------*
     *                                      *
     *  Clear list of dynamic timers        *
     *                                      *
     *--------------------------------------*/

    #if defined(OS_ENABLE_DTIMERS)
        *((OS_BANK OST_WORD*)&_OS_DTimers.Flags) = 0;
        _OS_DTimers.Flags.bNextEnable = 0;
        _OS_DTimers.Flags.bActive = 1;
    #endif

    /*--------------------------------------*
     *                                      *
     *  Clear list of fast timers           *
     *                                      *
     *--------------------------------------*/


    #if defined(OS_ENABLE_QTIMERS)
        *((OS_BANK OST_WORD*)&_OS_Qtimers.Flags) = 0;
        // Default:
        //  bNextEnable     = 0
        //  bListFunction   = 0 (FUNCTION_ADD)
        //  bListGetTime    = 0
        _OS_Qtimers.Flags.bActive = 1;
    #endif

    /*--------------------------------------*
     *                                      *
     *  Make all static timers free         *
     *                                      *
     *--------------------------------------*/

    #if defined (OS_ENABLE_STIMERS)
    #if defined (OS_STIMERS_ENABLE_ALLOCATION)

    #if OS_STIMERS > 11*8 || ((OS_STIMERS > 0) && defined(__OSA_PIC24_MPLABC__))

        _OS_Temp = sizeof(_OS_StimersFree) / sizeof(OST_WORD);
        do
        {
            _OS_StimersFree[_OS_Temp-1] = (OST_WORD)-1;
        } while (--_OS_Temp);

    #else
        #if OS_STIMERS > 1*8
            _OS_StimersFree[0] = (OST_WORD)-1;
        #endif

        #if OS_STIMERS > 2*8
            _OS_StimersFree[1] = (OST_WORD)-1;
        #endif

        #if OS_STIMERS > 3*8
            _OS_StimersFree[2] = (OST_WORD)-1;
        #endif

        #if OS_STIMERS > 4*8
            _OS_StimersFree[3] = (OST_WORD)-1;
        #endif

        #if OS_STIMERS > 5*8
            _OS_StimersFree[4] = (OST_WORD)-1;
        #endif

        #if OS_STIMERS > 6*8
            _OS_StimersFree[5] = (OST_WORD)-1;
        #endif

        #if OS_STIMERS > 7*8
            _OS_StimersFree[6] = (OST_WORD)-1;
        #endif

        #if OS_STIMERS > 8*8
            _OS_StimersFree[7] = (OST_WORD)-1;
        #endif

        #if OS_STIMERS > 9*8
            _OS_StimersFree[8] = (OST_WORD)-1;
        #endif

        #if OS_STIMERS > 10*8
            _OS_StimersFree[ 9] = (OST_WORD)-1;
        #endif

    #endif

        #if (OS_STIMERS & _OST_INT_MASK) != 0
        _OS_StimersFree[OS_STIMERS >> _OST_INT_SHIFT] = (1 << (OS_STIMERS & _OST_INT_MASK)) - 1;
        #endif

    #endif
    #endif

    /*--------------------------------------*
     *                                      *
     *  Zero all binary semaphores          *
     *                                      *
     *--------------------------------------*/

    #if OS_BSEMS > 11*8 || ((OS_BSEMS > 0) && defined(__OSA_PIC24_MPLABC__))

        _OS_Temp = sizeof(_OS_Bsems) / sizeof(OST_WORD);
        do
        {
            _OS_Bsems[_OS_Temp-1] = 0;
        } while (--_OS_Temp);

    #else
        #if OS_BSEMS > 0
            _OS_Bsems[0] = 0;
        #endif

        #if OS_BSEMS > 1*8
            _OS_Bsems[1] = 0;
        #endif

        #if OS_BSEMS > 2*8
            _OS_Bsems[2] = 0;
        #endif

        #if OS_BSEMS > 3*8
            _OS_Bsems[3] = 0;
        #endif

        #if OS_BSEMS > 4*8
            _OS_Bsems[4] = 0;
        #endif

        #if OS_BSEMS > 5*8
            _OS_Bsems[5] = 0;
        #endif

        #if OS_BSEMS > 6*8
            _OS_Bsems[6] = 0;
        #endif

        #if OS_BSEMS > 7*8
            _OS_Bsems[7] = 0;
        #endif

        #if OS_BSEMS > 8*8
            _OS_Bsems[8] = 0;
        #endif

        #if OS_BSEMS > 9*8
            _OS_Bsems[9] = 0;
        #endif

        #if OS_BSEMS > 10*8
            _OS_Bsems[10] = 0;
        #endif
    #endif


    /*--------------------------------------*
     *                                      *
     *  Zero all task descriptors           *
     *                                      *
     *--------------------------------------*/

    #if OS_TASKS > 10
        _OS_SET_IRP_CUR_TASK();
        _OS_CurTask = (OST_TASK_POINTER) _OS_Tasks;
        _OS_Temp = OS_TASKS;
        do {
            _OS_CurTask->State.bEnable = 0;
            _OS_CurTask ++;
        } while (--_OS_Temp);
        _OS_CLR_IRP();
    #else

        #if OS_PRIORITY_LEVEL == OS_PRIORITY_DISABLED
            _OS_CurTask = (OST_TASK_POINTER) _OS_Tasks + (OS_TASKS-1);
        #endif

        _OS_Tasks[0].State.bEnable = 0;

        #if OS_TASKS > 1
           _OS_Tasks[1].State.bEnable = 0;
        #endif

        #if OS_TASKS > 2
           _OS_Tasks[2].State.bEnable = 0;
        #endif

        #if OS_TASKS > 3
           _OS_Tasks[3].State.bEnable = 0;
        #endif

        #if OS_TASKS > 4
           _OS_Tasks[4].State.bEnable = 0;
        #endif

        #if OS_TASKS > 5
           _OS_Tasks[5].State.bEnable = 0;
        #endif

        #if OS_TASKS > 6
           _OS_Tasks[6].State.bEnable = 0;
        #endif

        #if OS_TASKS > 7
           _OS_Tasks[7].State.bEnable = 0;
        #endif

        #if OS_TASKS > 8
           _OS_Tasks[8].State.bEnable = 0;
        #endif

        #if OS_TASKS > 9
           _OS_Tasks[9].State.bEnable = 0;
        #endif

    #endif


    /*--------------------------------------*
     *                                      *
     *  Zero task queue for extended        *
     *  priority mode                       *
     *                                      *
     *--------------------------------------*/

    //------------------------------------------------------------------------------
    #if (OS_PRIORITY_LEVEL == OS_PRIORITY_EXTENDED)
    //------------------------------------------------------------------------------


    _OS_SET_IRP_CUR_TASK();
    _OS_CurTask = (OST_TASK_POINTER) _OS_Tasks;
    _OS_Temp = OS_TASKS;
    do
    {
        _OS_TaskQueue[_OS_Temp-1] = _OS_Temp-1;
        _OS_TaskLevel[_OS_Temp-1] = 0;
    } while (--_OS_Temp);
    _OS_CLR_IRP();

    //------------------------------------------------------------------------------
    #endif
    //------------------------------------------------------------------------------


}



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
 *   void OS_EnterCriticalSection (void)
 *
 *------------------------------------------------------------------------------
 *
 *   description:   Enter critical section.
 *                  This function disables interrupts (with saving current
 *                  state) and sets system flag bInCriticalSection
 *
 *
 *  parameters:     none
 *
 *  on return:      none
 *
 *  Overloaded in:  "osa_pic16_htpicc.c"
 *                  "osa_pic16_mikroc.c"
 *                  "osa_pic16_ccs.c"
 *                  "osa_pic24_mplabc.c"
 *                  "osa_stm8_cosmic.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#if !defined(OS_EnterCriticalSection_DEFINED)
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#ifdef      __OSA18__
//------------------------------------------------------------------------------

    void OS_EnterCriticalSection (void)
    {
        OST_UINT temp;

        temp = OS_DI();
        _OS_Flags.bInCriticalSection = 1;

        _OS_Flags.bGIE_CTemp = 0;
        if (temp & 0x80) _OS_Flags.bGIE_CTemp = 1;

        _OS_Flags.bGIEL_CTemp = 0;
        if (temp & 0x40) _OS_Flags.bGIEL_CTemp = 1;
    }


//------------------------------------------------------------------------------
#else   //  __OSA18__
//------------------------------------------------------------------------------

    void OS_EnterCriticalSection (void)
    {
        _OS_Flags.bGIE_CTemp = 0;
        if (OS_DI() & 0x80) _OS_Flags.bGIE_CTemp = 1;
        _OS_Flags.bInCriticalSection = 1;

    }

//------------------------------------------------------------------------------
#endif  //  __OSA18__
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------




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
 *
 *  Overloaded in:  "osa_pic16_htpicc.c"
 *                  "osa_pic16_ccs.c"
 *                  "osa_pic16_mikroc.c"
 *                  "osa_pic24_mplabc.c"
 *                  "osa_stm8_cosmic.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#if !defined(OS_LeaveCriticalSection_DEFINED)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef      __OSA18__
//------------------------------------------------------------------------------

    void OS_LeaveCriticalSection (void)
    {
        char temp;
        _OS_Flags.bInCriticalSection = 0;
        temp = 0;
        if (_OS_Flags.bGIE_CTemp)  temp |= 0x80;
        if (_OS_Flags.bGIEL_CTemp) temp |= 0x40;
        OS_RI(temp);
    }

//------------------------------------------------------------------------------
#else   //  __OSA18__
//------------------------------------------------------------------------------

    void OS_LeaveCriticalSection (void)
    {
        _OS_Flags.bInCriticalSection = 0;
        if (_OS_Flags.bGIE_CTemp) OS_RI(0x80);
    }

//------------------------------------------------------------------------------
#endif  //  __OSA18__
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#endif  //  OS_ENABLE_CRITICAL_SECTION
//------------------------------------------------------------------------------








/*
 ************************************************************************************************
 *                                                                                              *
 *                                   S Y S T E M   T I M E R                                    *
 *                                                                                              *
 ************************************************************************************************
 */



//-----------------------------------------------------------------
#ifdef OS_ENABLE_OS_TIMER
#ifndef OS_USE_INLINE_TIMER
//-----------------------------------------------------------------
void OS_Timer (void)
{
    __OS_TimerInline();
}
//-----------------------------------------------------------------
#endif  // OS_ENABLE_OS_TIMER
#endif  // OS_USE_INLINE_TIMER
//-----------------------------------------------------------------






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
 *  Overloaded in:  "osa_pic12_htpicc.c"
 *                  "osa_pic16_htpicc.c"
 *                  "osa_pic18_htpicc.c"
 *                  "osa_pic16_mikroc.c"
 *                  "osa_pic18_mikroc.c"
 *                  "osa_pic18_mplabc.c"
 *                  "osa_pic24_mplabc.c"
 *                  "osa_pic16_ccs.c"
 *                  "osa_pic18_ccs.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#if !defined(_OS_CheckEvent_DEFINED)
//------------------------------------------------------------------------------

    OST_BOOL _OS_CheckEvent (char bEvent)
    {
        _OS_Flags.bTimeout = 0;

        if (bEvent)
        {
            if (_OS_CurTask->State.bReady)
            {
                #ifdef OS_ENABLE_TTIMERS
                    _OS_TASK_ATOMIC_WRITE_A(_OS_CurTask->State.bDelay = 0);
                #endif

                #if OS_PRIORITY_LEVEL == OS_PRIORITY_EXTENDED
                _OS_Flags.bEventOK = 1;
                #endif

                return 1;
            }

            _OS_bTaskReady = 1;

        } else {
            _OS_bTaskReady = 0;
        }

        #ifdef OS_ENABLE_TTIMERS

            if (!_OS_CurTask->State.bDelay && _OS_CurTask->State.bCanContinue)
            {
                _OS_bTaskReady = 1;
                _OS_Flags.bTimeout = 1;

                #if OS_PRIORITY_LEVEL == OS_PRIORITY_EXTENDED
                _OS_Flags.bEventOK = 1;
                #endif

                return 1;

            }

        #endif

        return 0;

    }

//------------------------------------------------------------------------------
#endif  // !defined(_OS_CheckEvent_DEFINED)
//------------------------------------------------------------------------------



//******************************************************************************
//  END OF FILE osa_system.c
//******************************************************************************

