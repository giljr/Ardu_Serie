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
 *  File:           osa_system.h
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    Kernel definitions and services
 *                  This file directly included in osa.c
 *
 *  History:        15.09.2010 -    File updated
 *
 ************************************************************************************************
 */



//******************************************************************************
//  VARIABLES
//******************************************************************************


//******************************************************************************
//  FUNCTION PROTOTYPES
//******************************************************************************


//******************************************************************************
//  MACROS
//******************************************************************************


//******************************************************************************
//  Function prototypes
//******************************************************************************

//------------------------------------------------------------------------------
// System variables initialization
extern void  OS_Init (void);

//------------------------------------------------------------------------------
// Critical sections
extern void OS_EnterCriticalSection (void);
extern void OS_LeaveCriticalSection (void);






//******************************************************************************
//  Macro definitions
//******************************************************************************




/************************************************************************************************
 *                                                                                              *
 *                                      S H C E D U L E R                                       *
 *                                                                                              *
 *                      !!!(Sheduler for MCC30 is in "port\osa_mcc30.h")!!!                     *
 *                                                                                              *
 ************************************************************************************************/


//------------------------------------------------------------------------------
#ifndef OS_Sched
//------------------------------------------------------------------------------



#define _OS_SCHED_LABEL_SCHED_CONTINUE      _OS_SCHED_CONTINUE:;

#define _OS_IF_NOT_TASK_ENABLE_CONTINUE()   if (!_OS_bTaskEnable || _OS_bTaskPaused) goto _OS_SCHED_CONTINUE;

#define _OS_IF_NOT_TASK_READY()             if (!_OS_bTaskReady)

#define _OS_IF_TASK_READY()                 if (_OS_bTaskReady)


//------------------------------------------------------------------------------
#ifdef OS_ENABLE_TTIMERS

    #define _OS_SCHED_IF_DELAIED_GOTO_CONTINUE()   if (_OS_bTaskDelay && !_OS_bTaskCanContinue) goto _OS_SCHED_CONTINUE;
#else

    #define _OS_SCHED_IF_DELAIED_GOTO_CONTINUE()

#endif
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef _OS_SET_STATE   /*see osa_stm8_iar.h for details one this definition */
//------------------------------------------------------------------------------
    #if defined(_OS_CUR_FLAGS_IN_OS_STATE)
    //------------------------------------------------------------------------------
    
        #define _OS_SET_STATE()     _OS_State = _OS_CurTask->State
        #define _OS_RESTORE_STATE()
    
    //------------------------------------------------------------------------------
    #else
    //------------------------------------------------------------------------------
    
        #define _OS_SET_STATE()     _OS_SET_FSR_CUR_TASK()
        #define _OS_RESTORE_STATE() _OS_SET_FSR_CUR_TASK()
    
    //------------------------------------------------------------------------------
    #endif
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
#if (OS_PRIORITY_LEVEL == OS_PRIORITY_NORMAL)
//------------------------------------------------------------------------------

    /********************************************************************************************
     *                                                                                          *
     *     N O R M A L     P R I O R I T Y     M O D E     S C H E D U L E R                    * 
     *                                                                                          *
     ********************************************************************************************/


    //------------------------------------------------------------------------------
    #ifdef OS_ENABLE_CRITICAL_SECTION
    
        #define _OS_SCHED_SKIP_IF_CRITICAL_SECTION() if (OS_IsInCriticalSection()) goto _OS_SCHED_CHECK_READY;
        #define _OS_SCHED_LABEL_CHECK_READY          _OS_SCHED_CHECK_READY:
        #define _OS_NOT_IN_CRITICAL_SECTION()        !OS_IsInCriticalSection()
    
    #else
    
        #define _OS_SCHED_SKIP_IF_CRITICAL_SECTION()
        #define _OS_SCHED_LABEL_CHECK_READY
        #define _OS_NOT_IN_CRITICAL_SECTION()           1
    
    #endif
    //------------------------------------------------------------------------------

    #define OS_IsIdle()     (!OS_IsInCriticalSection() && !_OS_Flags.bBestTaskFound)

    #define OS_Sched()                                                                          \
    {                                                                                           \
                                                                                                \
        /* First we suppose that there is no ready task*/                                       \
        _OS_Flags.bBestTaskFound = 0;                                                           \
        /* MODE: Search of best task                  */                                        \
        _OS_Flags.bCheckingTasks = 1;                                                           \
        /* Worst of lowest priority                   */                                        \
        _OS_Best_Priority = OS_WORST_PRIORITY + 1;                                              \
        _OS_SCHED_SKIP_IF_CRITICAL_SECTION();                                                   \
        /* List all tasks starting from last executed   */                                      \
        _OS_CurTask = _OS_LastTask;                                                             \
        do                                                                                      \
        {                                                                                       \
            if (_OS_CurTask >= (OST_TASK_POINTER)_OS_Tasks + (OS_TASKS-1))                      \
                _OS_CurTask = (OST_TASK_POINTER)_OS_Tasks;                                      \
            else                                                                                \
                _OS_CurTask++;                                                                  \
                                                                                                \
    _OS_SCHED_LABEL_CHECK_READY                                                                 \
                                                                                                \
            _OS_SET_STATE();                                                                    \
            _OS_IF_NOT_TASK_ENABLE_CONTINUE();                       /* Skip empty descriptor */\
            if (_OS_Best_Priority <= (_OS_Temp = _OS_cTaskPriority)) goto _OS_SCHED_CONTINUE;   \
            _OS_IF_NOT_TASK_READY()                                                             \
            {                                                                                   \
                /* Task is not ready yet. Check for it has became ready    */                   \
    _OS_SCHED_RUN:;                                                                             \
                _OS_JUMP_TO_TASK();                                                             \
                _OS_BACK_TO_SCEDULER();                                                         \
                if (!_OS_Flags.bCheckingTasks) goto SCHED_END;                                  \
                _OS_RESTORE_STATE();                                                            \
            }                                                                                   \
            /* Skip if task is delayed              */                                          \
            _OS_SCHED_IF_DELAIED_GOTO_CONTINUE();                                               \
            /* Compare priority for all ready tasks      */                                     \
            _OS_IF_TASK_READY()                                                                 \
            {                                                                                   \
                _OS_Flags.bBestTaskFound = 1;                                                   \
                _OS_BestTask = _OS_CurTask;                                                     \
                _OS_Best_Priority = _OS_cTaskPriority;                                          \
                                                                                                \
                /* If this task has highest priority then stop searching */                     \
                if (_OS_Best_Priority == OS_BEST_PRIORITY) break;                               \
            }                                                                                   \
                                                                                                \
    _OS_SCHED_LABEL_SCHED_CONTINUE                                                              \
                                                                                                \
        } while (_OS_NOT_IN_CRITICAL_SECTION() && _OS_CurTask != _OS_LastTask);                 \
                                                                                                \
        /* If there is at least one ready task, then execute it */                              \
                                                                                                \
        if (_OS_Flags.bBestTaskFound)                                                           \
        {                                                                                       \
            _OS_Flags.bCheckingTasks = 0; /* MODE: executing best task    */                    \
            _OS_LastTask = _OS_CurTask = _OS_BestTask;                                          \
            _OS_SET_STATE();                                                                    \
            if (_OS_bTaskEnable && !_OS_bTaskPaused) goto _OS_SCHED_RUN;                        \
        }                                                                                       \
                                                                                                \
    SCHED_END:;                                                                                 \
                                                                                                \
    }                                                                                           \

    //-------------------------------------------------------------------------------------------
    // Explaining of some moments:
    //
    // 1.  OS_Temp used here to reduce used RAM by 1 byte. Compiler while comparing
    //     _OS_Best_Priority with (indf&7) generates one temporary variable. We set it manualy,
    //     so compiler will not use local variable.
    //
    //-------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#endif      // OS_PRIORITY_NORMAL
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#if (OS_PRIORITY_LEVEL == OS_PRIORITY_EXTENDED)
//------------------------------------------------------------------------------


    //------------------------------------------------------------------------------
    
    #ifndef _OS_UPDATE_TASK_QUEUE
    #define _OS_UPDATE_TASK_QUEUE()                                                         \
                _OS_n = _OS_Best_n;                                                         \
                while (_OS_n)                                                               \
                {                                                                           \
                    _OS_TaskQueue[_OS_n] = _OS_TaskQueue[_OS_n-1];                          \
                    _OS_n--;                                                                \
                }                                                                           \
                _OS_TaskQueue[0] = _OS_Cur_Pos;
    #endif

    //------------------------------------------------------------------------------

    #ifndef _OS_INC_PRIORITY
    #define _OS_INC_PRIORITY()                                                              \
                if (!(_OS_Temp & 0x80))                                                     \
                {                                                                           \
                    _OS_Temp += 8;                                                          \
                    _OS_Temp -= _OS_CurTask->State.cPriority;                               \
                    _OS_TaskLevel[_OS_Cur_Pos] = _OS_Temp;                                  \
                }                                                                           \

    #endif

    //------------------------------------------------------------------------------



    /********************************************************************************************
     *                                                                                          *
     *     E X T E N D E D    P R I O R I T Y    M O D E    S C H E D U L E R                   * 
     *                                                                                          *
     ********************************************************************************************/

    //------------------------------------------------------------------------------
    #ifdef OS_ENABLE_CRITICAL_SECTION
    
        #define _OS_SCHED_SKIP_IF_CRITICAL_SECTION()    if (OS_IsInCriticalSection()) goto _OS_SCHED_CHECK_READY;
        #define _OS_SCHED_LABEL_CHECK_READY             _OS_SCHED_CHECK_READY:
        #define _OS_NOT_IN_CRITICAL_SECTION()           !OS_IsInCriticalSection()
    
    #else
    
        #define _OS_SCHED_SKIP_IF_CRITICAL_SECTION()
        #define _OS_SCHED_LABEL_CHECK_READY
        #define _OS_NOT_IN_CRITICAL_SECTION()           1
    
    #endif
    //------------------------------------------------------------------------------

    #define OS_IsIdle()     (!OS_IsInCriticalSection() && !_OS_Flags.bBestTaskFound)

    #define OS_Sched()                                                                          \
    {                                                                                           \
                                                                                                \
        /* First we suppose that there is no ready task*/                                       \
        _OS_Flags.bBestTaskFound = 0;                                                           \
        /* MODE: Search of best task                  */                                        \
        _OS_Flags.bCheckingTasks = 1;                                                           \
        /* Worst of lowest priority                   */                                        \
        _OS_Best_Priority = 0;                                                                  \
        _OS_Worst_Priority = 0;                                                                 \
                                                                                                \
        _OS_SCHED_SKIP_IF_CRITICAL_SECTION();                                                   \
                                                                                                \
        _OS_n = OS_TASKS;                                                                       \
        do                                                                                      \
        {                                                                                       \
            _OS_Cur_Pos = _OS_TaskQueue[_OS_n-1];                                               \
                                                                                                \
    _OS_SCHED_LABEL_CHECK_READY                                                                 \
            _OS_CurTask = &_OS_Tasks[_OS_Cur_Pos];                                              \
                                                                                                \
            _OS_SET_STATE();                                                                    \
            _OS_IF_NOT_TASK_ENABLE_CONTINUE();                       /* Skip empty descriptor */\
            _OS_IF_NOT_TASK_READY()                                                             \
            {                                                                                   \
                /* Task is not ready yet. Check for it has became ready    */                   \
    _OS_SCHED_RUN:;                                                                             \
                _OS_JUMP_TO_TASK();                                                             \
                _OS_BACK_TO_SCEDULER();                                                         \
                _OS_RESTORE_STATE();                                                            \
                                                                                                \
                if (!_OS_Flags.bCheckingTasks)                                                  \
                {                                                                               \
                    if (_OS_bTaskReady || _OS_Flags.bEventOK)                                   \
                    {                                                                           \
                        _OS_TaskLevel[_OS_Cur_Pos] -= _OS_Worst_Priority;                       \
                        _OS_UPDATE_TASK_QUEUE();                                                \
                    }                                                                           \
                    goto SCHED_END;                                                             \
                }                                                                               \
            }                                                                                   \
                                                                                                \
            /* Skip if task is delayed              */                                          \
            _OS_SCHED_IF_DELAIED_GOTO_CONTINUE();                                               \
                                                                                                \
            /* Compare priority for all ready tasks      */                                     \
            _OS_IF_TASK_READY()                                                                 \
            {                                                                                   \
                _OS_Temp = _OS_TaskLevel[_OS_Cur_Pos];                                          \
                _OS_INC_PRIORITY();                                                             \
                if (_OS_Temp > _OS_Best_Priority)                                               \
                {                                                                               \
                    _OS_Worst_Priority = _OS_Best_Priority;                                     \
                    _OS_Best_Priority = _OS_Temp;                                               \
                    _OS_Best_n = _OS_n-1;                                                       \
                    _OS_Flags.bBestTaskFound = 1;                                               \
                }                                                                               \
                else if (_OS_Temp > _OS_Worst_Priority)                                         \
                {                                                                               \
                    _OS_Worst_Priority = _OS_Temp;                                              \
                }                                                                               \
            }                                                                                   \
                                                                                                \
    _OS_SCHED_LABEL_SCHED_CONTINUE                                                              \
                                                                                                \
        } while (_OS_NOT_IN_CRITICAL_SECTION() && --_OS_n);                                     \
                                                                                                \
        /* If there is at least one ready task, then execute it */                              \
                                                                                                \
        if (_OS_Flags.bBestTaskFound)                                                           \
        {                                                                                       \
            _OS_Flags.bCheckingTasks = 0; /* MODE: executing best task    */                    \
            _OS_Flags.bEventOK = 0;                                                             \
            _OS_Cur_Pos = _OS_TaskQueue[_OS_Best_n];                                            \
            _OS_CurTask = &_OS_Tasks[_OS_Cur_Pos];                                              \
            _OS_SET_STATE();                                                                    \
            if (_OS_bTaskEnable && !_OS_bTaskPaused) goto _OS_SCHED_RUN;                        \
        }                                                                                       \
                                                                                                \
    SCHED_END:;                                                                                 \
                                                                                                \
    }                                                                                           \

    //-------------------------------------------------------------------------------------------
    // Explaining of some moments:
    //
    // 1.  OS_Temp used here to reduce used RAM by 1 byte. Compiler while comparing
    //     _OS_Best_Priority with (indf&7) generates one temporary variable. We set it manualy,
    //     so compiler will not use local variable.
    //
    //-------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#endif      // OS_PRIORITY_EXTENDED
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#if OS_PRIORITY_LEVEL == OS_PRIORITY_DISABLED
//------------------------------------------------------------------------------

    /********************************************************************************************
     *                                                                                          *
     *     N O N   P R I O R I T Y   M O D E   S C H E D U L E R                                *
     *                                                                                          *
     ********************************************************************************************/

    #define OS_IsIdle()     0

    //-------------------------------------------------------------------------------------------
    //
    // Non priority mode can be used when there is no need to give more preference to
    // some tasks. (For example there are no time rcitical code). This mode allows to reduce ROM
    // and RAM usage and also increases speed of // scheduler work in OS_TASKS times
    //
    //-------------------------------------------------------------------------------------------


    #ifdef OS_ENABLE_CRITICAL_SECTION
        #define _OS_IF_NOT_IN_CRITICAL_SECTION()        if (!OS_IsInCriticalSection())
    #else
        #define _OS_IF_NOT_IN_CRITICAL_SECTION()
    #endif

    #define OS_Sched()                                                                          \
    {                                                                                           \
        _OS_Temp = 0;                                                                           \
                                                                                                \
        _OS_IF_NOT_IN_CRITICAL_SECTION()                                                        \
        {                                                                                       \
            if (_OS_CurTask >= (OST_TASK_POINTER)_OS_Tasks + (OS_TASKS-1))                      \
                _OS_CurTask = (OST_TASK_POINTER)_OS_Tasks;                                      \
            else                                                                                \
                _OS_CurTask++;                                                                  \
        }                                                                                       \
                                                                                                \
        _OS_SET_STATE();                                                                        \
                                                                                                \
        /* Skip empty descriptor              */                                                \
        if (!_OS_bTaskEnable || _OS_bTaskPaused) goto _OS_SCHED_CONTINUE;                       \
                                                                                                \
        _OS_SCHED_IF_DELAIED_GOTO_CONTINUE();                                                   \
                                                                                                \
        _OS_JUMP_TO_TASK();                                                                     \
        _OS_BACK_TO_SCEDULER();                                                                 \
                                                                                                \
    _OS_SCHED_CONTINUE:;                                                                        \
                                                                                                \
    }

//------------------------------------------------------------------------------
#endif     //   OS_PRIORITY_DISABLED
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#endif  // ifndef OS_Shed
//------------------------------------------------------------------------------

/************************************************************************************************/









/************************************************************************************************
 *                                                                                              *
 *     C O M M O N   S Y S T E M   S E R V I C E S                                              *
 *                                                                                              *
 ************************************************************************************************/


#define OS_Run()    for(;;) OS_Sched()




/************************************************************************/
/* Check for timeout expired in last service                            */
/************************************************************************/

#define OS_IsTimeout()              _OS_Flags.bTimeout

/************************************************************************/
/* Check for one of task is in critical section                         */
/************************************************************************/

#define OS_IsInCriticalSection()    _OS_Flags.bInCriticalSection


/************************************************************************/
// Check for object creation error (task, stimers)
/************************************************************************/

#define OS_IsError()                _OS_Flags.bError

/************************************************************************/
// Check for event error
/************************************************************************/

#define OS_IsEventError()           _OS_Flags.bEventError





/************************************************************************************************
 *                                                                                              *
 *                                    I N T E R R U P T S                                       *
 *                                                                                              *
 ************************************************************************************************/


//------------------------------------------------------------------------------
// Enable/disable/restore interrupts are in h-files for each processor
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
#if defined(__OSA_PIC16_HTPICC__) || defined(__OSA_PIC18_HTPICC__)    
//------------------------------------------------------------------------------

    /************************************************************************/
    /*                                                                      */
    /* This service must be called in the begin of ISR                      */
    /*                                                                      */
    /************************************************************************/

    #define OS_EnterInt()                                   \
        static OS_FSR_TYPE   _OS_FSR_Temp;                  \
        _OS_FSR_Temp = _fsr;                                \

    /************************************************************************/
    /*                                                                      */
    /* This service must be called at the end of ISR                        */
    /*                                                                      */
    /************************************************************************/

    #define OS_LeaveInt()   _fsr = _OS_FSR_Temp

//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------

    #define OS_EnterInt()
    #define OS_LeaveInt()

#endif







/************************************************************************************************
 *                                                                                              *
 *                      C O N T E X T    S W I T C H I N G                                      * 
 *                                                                                              *
 ************************************************************************************************/



/************************************************************************************
 *
 *  All context switches are based one five macros:
 * 
 *  _OS_RETURN_SAVE()         - save context and return to scheduler
 * 
 *  _OS_RETURN_NO_SAVE()      - returning to scheduler without saving context
 * 
 *  _OS_ENTER_WAIT_MODE()     - switch the task to wait mode
 * 
 *  _OS_ENTER_WAIT_MODE_TO()  - switch the task to wait mode with exit on timeout
 * 
 *  _OS_CHECK_EVENT()         - check for event is true. Continue task execution if
 *                              then event is true, or switch context otherwise
 * 
 *  See definitions of these macros for each compiler in specific .h-files (in the
 *  "port" folder)
 *
 ************************************************************************************/


#define OS_Yield()                          _OS_RETURN_SAVE()

/*
 ********************************************************************************
 *                                                                              *
 *   Put task in wait mode till "event" will not became true                    *
 *                                                                              *
 ********************************************************************************
 */

#define OS_Wait(event)                              \
{                                                   \
    _OS_ENTER_WAIT_MODE();                          \
    _OS_CHECK_EVENT((event) != 0);                  \
}

/*
 ********************************************************************************
 *                                                                              *
 *   Put task in wait mode till "event" will not became true or timeout will not*
 *   expired                                                                    *
 *                                                                              *
 ********************************************************************************
 */

#define OS_Wait_TO(event, timeout)                  \
{                                                   \
    _OS_InitDelay(timeout);                         \
    _OS_ENTER_WAIT_MODE_TO();                       \
    _OS_CHECK_EVENT((event) != 0);                  \
}






//******************************************************************************
//  END OF FILE osa_system.h
//******************************************************************************

