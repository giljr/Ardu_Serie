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
 *  File:           osa_pic16_ccs.c
 *
 *  Compilers:      CCS
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    CCS for PIC16 specific functions definition
 *                  This file directly included in osa.c
 *
 *  History:        13.09.2010 -    File updated
 *
 ************************************************************************************************
 */






//******************************************************************************
//
//  _OS_INIT_PROC_SPEC.
//
//******************************************************************************
//
//  Specific macro for CCS compiler:
//
//  The problem is that system functions that operate with stack can be
//  called through 'goto' operator in case when there is only one call of
//  function. To avoid it we call all these functions once to be sure that
//  in case of they will be used in program, compilser will call them by 'call',
//  not 'goto'.
//
//******************************************************************************

#define _OS_INIT_PROC_SPEC()                      \
    _OS_Temp = 0;                               \
    if (_OS_Temp & 1)                           \
    {                                           \
        _OS_ReturnSave(0);                      \
        _OS_CheckEvent(0);                      \
        _OS_JumpToTask();                       \
    }



OST_UINT16    _OS_RetAddr_Temp;


/************************************************************************************************
 *                                                                                              *
 *                           K E R N E L   F U N C T I O N S                                    *
 *                                                                                              *
 ************************************************************************************************/



/*
 ********************************************************************************
 *
 *  void _OS_ReturnSave (char mode)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Return to sheduler with saving current task return point
 *
 *  parameters:     char mode:
 *                  =0 - only save point of return
 *                  =1 - save retpoint, clear ready, set cancontinue
 *                  =2 - save retpoint and clear ready
 *
 *  on return:      none
 *
 ********************************************************************************
 */

#separate void _OS_ReturnSave (char mode)
{
    _OS_Flags.bEventError = 0;

    _OS_SET_FSR_CUR_TASK_W(_OS_TASK_POINTER_OFFSET + 1);

    _indf = _OS_RetAddr_Temp >> 8;
    _fsr--;
    _indf = _OS_RetAddr_Temp & 0xFF;
    _fsr--;

    if (mode)
    {
        _OS_bTaskReady = 0;

        #if defined(OS_ENABLE_TTIMERS)

            if (!_OS_bTaskDelay) _OS_bTaskCanContinue = 0;
            if (mode & 0x02) _OS_bTaskCanContinue = 1;

        #endif
    }
}



#separate void _OS_JumpToTask (void)
{
    _OS_SET_FSR_CUR_TASK_W(_OS_TASK_POINTER_OFFSET + 1);
    _pclath = _indf;
    _fsr--;
    #asm
        movf    _indf, w
        decf    _fsr, f
        movwf   _pcl
    #endasm
}


void __OS_SET_FSR_CUR_TASK (void)
{
    _fsr = ((OS_FSR_TYPE)&_OS_CurTask->State);
}



/*
 ********************************************************************************
 *
 *   void _OS_Task_Create(OST_UINT priority, OST_CODE_POINTER TaskAddr)
 *
 *------------------------------------------------------------------------------
 *
 *   description:   (Internal function called by system kernel from service
 *                  OS_Task_Create)
 *                  Create task in free descriptor.
 *
 *  parameters:     priority - value from 0 (highest) to 7 (lowest)
 *                  TaskAddr - pointer to C-function that contains task
 *
 *  on return:      check OS_IsError
 *                  _OS_Temp - task position in _OS_Tasks array
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define _OS_Task_Create_DEFINED
//------------------------------------------------------------------------------


void _OS_Task_Create(OST_UINT priority, OST_CODE_POINTER TaskAddr)
{
    OST_TASK_POINTER Task;

    _OS_Flags.bError = 0;

    #if OS_PRIORITY_LEVEL == OS_PRIORITY_DISABLED
    priority = 0;   // To increase scheduler speed
    #endif



    /*--------------------------------------*
     *                                      *
     *  Start search from first task in     *
     *  OS_TASKS descriptors.               *
     *                                      *
     *--------------------------------------*/
    Task = (OST_TASK_POINTER)_OS_Tasks;
    _OS_Temp = 0;

    do {

        if (!Task->State.bEnable)               // Is descriptor free?
        {                                       // Yes
            ((OST_TASK_STATE)priority).bEnable = 1;
            ((OST_TASK_STATE)priority).bReady = 1;

            Task->pTaskPointer = TaskAddr;

            #ifdef OS_ENABLE_TTIMERS
                Task->Timer = 0;
            #endif

            #ifdef _OS_TASK_CREATE_PROC_SPEC
            _OS_TASK_CREATE_PROC_SPEC();
            #endif

            *(OS_TASKS_BANK char*)&Task->State = priority;

            #if defined(_OS_CUR_FLAGS_IN_OS_STATE)
            if (Task == _OS_CurTask) ((OS_RAM_NEAR char)_OS_State) = priority;
            #endif

            _OS_Flags.bError = 0;

            return ;
        }

        Task ++;

    } while (++_OS_Temp < OS_TASKS);

    // There is no free descriptor. Task was not created.
    _OS_Flags.bError = 1;

    return ;
}















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

char OS_DI (void)
{
    char temp;
    temp = _intcon & 0x80;
    _gie = 0;
    return temp;
}


//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT)
//------------------------------------------------------------------------------

    void _OS_DI_INT (void)
    {
        _OS_Temp_I = _intcon & 0x80;
        _gie = 0;
    }

//------------------------------------------------------------------------------
#endif
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

//______________________________________________________________________________
//******************************************************************************

//------------------------------------------------------------------------------
#endif  // #ifdef OS_ENABLE_TTIMERS
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


//------------------------------------------------------------------------------
#separate OST_BOOL _OS_CheckEvent (OST_UINT bEvent)
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


            #endif

            return 1;
        }

        #ifdef OS_ENABLE_TTIMERS

            if (_OS_bTaskTimeout && _OS_bTaskCanContinue)
            {
                _OS_Flags.bTimeout = 1;
                return 1;
            }

        #endif

        return 0;

    //------------------------------------------------------------------------------
    #else
    //------------------------------------------------------------------------------


        _OS_Flags.bTimeout = 0;
        _OS_SET_FSR_CUR_TASK();

        if (bEvent)
        {
            if (_OS_bTaskReady)
            {
        EXIT_GOOD:
                #ifdef OS_ENABLE_TTIMERS

                    _OS_bTaskDelay = 0; // Clear bDelay in temporary variable _OS_State

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

            if (_OS_bTaskTimeout && _OS_bTaskCanContinue)
            {
                _OS_Flags.bTimeout = 1;
                _OS_bTaskReady = 1;
                #if OS_PRIORITY_LEVEL == OS_PRIORITY_EXTENDED
                _OS_Flags.bEventOK = 1;
                #endif
                return 1;
            }

        #endif

        return 0;
    //------------------------------------------------------------------------------
    #endif
    //------------------------------------------------------------------------------
}






//******************************************************************************
//  END OF FILE osa_pic16_ccs.c
//******************************************************************************

