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
 *  File:           osa_tasks.h
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    Services for work with tasks 
 *
 *  History:        12.09.2010 -    File updated
 *
 ************************************************************************************************
 */



/************************************************************************************************
 *                                                                                              *
 *                                   T A S K S                                                  *
 *                                                                                              *
 ************************************************************************************************/

//******************************************************************************
//  VARIABLES
//******************************************************************************


extern OS_TASKS_BANK    OST_TCB  _OS_Tasks[OS_TASKS];


//******************************************************************************
//  FUNCTION PROTOTYPES
//******************************************************************************

void _OS_Task_Create(OST_UINT priority, OST_CODE_POINTER TaskAddr);


//******************************************************************************
//  MACROS
//******************************************************************************






//******************************************************************************
//  Macro definitions
//******************************************************************************

#if defined(OS_PROTECT_MEMORY_ACCESS)
    #define __OS_TASK_DI_A()  _OS_DI_INT()
    #define __OS_TASK_RI_A()  _OS_RI_INT()
#else
    #define __OS_TASK_DI_A()
    #define __OS_TASK_RI_A()
#endif


#define _OS_TASK_ATOMIC_WRITE_A(expr)       \
    OSM_BEGIN {                             \
        __OS_TASK_DI_A();                   \
        expr;                               \
        __OS_TASK_RI_A();                   \
    } OSM_END

#define _OS_TASK_ATOMIC_READ_A(expr)                                            \
    (__OS_TASK_DI_A(), _OS_Temp = (expr), __OS_TASK_RI_A(), _OS_Temp)           \

//------------------------------------------------------------------------------





//------------------------------------------------------------------------------
#ifdef _OS_CUR_FLAGS_IN_OS_STATE            
//------------------------------------------------------------------------------

    // for cases when working with State through _OS_State

    #define __OS_CLEAR_STATE_ENABLE()               _OS_TASK_ATOMIC_WRITE_A(_OS_State.bEnable = 0)
    #define __OS_SET_STATE_PAUSE()                  _OS_TASK_ATOMIC_WRITE_A(_OS_State.bPaused = 1)
    #define __OS_SET_STATE_PRIORITY(new_priority)   _OS_TASK_ATOMIC_WRITE_A(_OS_State.cPriority = new_priority)

#else   
    
    // for cases when working with State through INDF

    #define __OS_CLEAR_STATE_ENABLE()
    #define __OS_SET_STATE_PAUSE()
    #define __OS_SET_STATE_PRIORITY(new_priority)

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------





#define this_task   _OS_CurTask

/************************************************************************/
/*                                                                      */
/* Stop current task                                                    */
/*                                                                      */
/************************************************************************/

#define OS_Task_Stop()   OS_Task_Delete(this_task)


/************************************************************************/
/*                                                                      */
/* Delete task                                                          */
/*                                                                      */
/************************************************************************/

//------------------------------------------------------------------------------
#if defined(__OSACCS__)
//------------------------------------------------------------------------------
    #define OS_Task_Delete(pTask)                                           \
        {                                                                   \
            _OS_TASK_ATOMIC_WRITE_A(pTask->State.bEnable = 0);              \
            if ((pTask) == (_OS_CurTask))                                   \
            {                                                               \
                __OS_CLEAR_STATE_ENABLE();                                  \
                _OS_RETURN_NO_SAVE();                                       \
            }                                                               \
        }
//------------------------------------------------------------------------------
#elif defined(__OSA_STM8_COSMIC__)
//------------------------------------------------------------------------------
    #define OS_Task_Delete(pTask)                                           \
        {                                                                   \
            _OS_TASK_ATOMIC_WRITE_A((pTask)->State.bEnable &= ~1);          \
            if ((pTask) == (_OS_CurTask))                                   \
            {                                                               \
                __OS_CLEAR_STATE_ENABLE();                                  \
                _OS_RETURN_NO_SAVE();                                       \
            }                                                               \
        }
//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------
    #define OS_Task_Delete(pTask)                                           \
        {                                                                   \
            _OS_SET_IRP_CUR_TASK();   /* for mikroC for PIC16 only */       \
            _OS_TASK_ATOMIC_WRITE_A((pTask)->State.bEnable = 0);            \
            if ((pTask) == (_OS_CurTask))                                   \
            {                                                               \
                __OS_CLEAR_STATE_ENABLE();                                  \
                _OS_CLR_IRP();                  /* for mikroC PIC16 only */ \
                _OS_RETURN_NO_SAVE();                                       \
            }                                                               \
            _OS_CLR_IRP();       /* for mikroC PIC16 only */                \
        }
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------




/************************************************************************/
/*                                                                      */
/* Pause task                                                           */
/*                                                                      */
/************************************************************************/

//------------------------------------------------------------------------------
#if defined(__OSACCS__)
//------------------------------------------------------------------------------
    #define OS_Task_Pause(pTask)                                            \
        {                                                                   \
            _OS_TASK_ATOMIC_WRITE_A(pTask->State.bPaused = 1);              \
            if ((pTask) == (_OS_CurTask))                                   \
            {                                                               \
                __OS_SET_STATE_PAUSE();                                     \
                OS_Yield();                                                 \
            }                                                               \
        }
//------------------------------------------------------------------------------
#elif defined(__OSA_STM8_COSMIC__)
//------------------------------------------------------------------------------
    #define OS_Task_Pause(pTask)                                            \
        {                                                                   \
            _OS_TASK_ATOMIC_WRITE_A((pTask)->State.bPaused != 1);           \
            if ((pTask) == (_OS_CurTask))                                   \
            {                                                               \
                __OS_SET_STATE_PAUSE();                                     \
                OS_Yield();                                                 \
            }                                                               \
        }
//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------
    #define OS_Task_Pause(pTask)                                            \
        {                                                                   \
            _OS_SET_IRP_CUR_TASK(); /* for mikroC for PIC16 only */         \
            _OS_TASK_ATOMIC_WRITE_A((pTask)->State.bPaused = 1);            \
            if ((pTask) == (_OS_CurTask))                                   \
            {                                                               \
                __OS_SET_STATE_PAUSE();                                     \
                OS_Yield();                                                 \
            }                                                               \
            _OS_CLR_IRP();          /* for mikroC for PIC16 only */         \
        }
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------

/************************************************************************/
/*                                                                      */
/* Continue paused task                                                 */
/*                                                                      */
/************************************************************************/

//------------------------------------------------------------------------------
#if defined(__OSACCS__)
//------------------------------------------------------------------------------
    #define OS_Task_Continue(pTask)                                         \
        {                                                                   \
            _OS_TASK_ATOMIC_WRITE_A(pTask->State.bPaused = 0);              \
        }
//------------------------------------------------------------------------------
#elif defined(__OSA_STM8_COSMIC__)
//------------------------------------------------------------------------------
    #define OS_Task_Continue(pTask)                                         \
        {                                                                   \
            _OS_TASK_ATOMIC_WRITE_A(pTask->State.bPaused &= ~1);            \
        }
//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------
    #define OS_Task_Continue(pTask)                                         \
        {                                                                   \
            _OS_SET_IRP_CUR_TASK();   /* for mikroC for PIC16 only */       \
            _OS_TASK_ATOMIC_WRITE_A((pTask)->State.bPaused = 0);            \
            _OS_CLR_IRP();            /* for mikroC for PIC16 only */       \
        }
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------


/************************************************************************/
/*                                                                      */
/* Replace current task with new (current task will be deleted)         */
/*                                                                      */
/************************************************************************/

// See proc.spec. h-file in "port" folder.



/****************************************************************************/
/*                                                                          */
/* Create task                                                              */
/*                                                                          */
/****************************************************************************/

// See proc.spec. h-file in "port" folder.



/************************************************************************/
/*                                                                      */
/* Create task by indirrect address                                     */
/*                                                                      */
/************************************************************************/

#define OS_Task_CreateP(priority, TaskAddr)      _OS_Task_Create(priority, TaskAddr)
#define OS_Task_Create_P(priority, TaskAddr)     _OS_Task_Create(priority, TaskAddr)

// If task created indirrectly then it is nessesary to insert in
// the main() service OS_ReserveTasks:
// see proc.spec. h-file in "port" folder.



/************************************************************************/
/*                                                                      */
/* Get/set current task priority                                        */
/*                                                                      */
/************************************************************************/

#if defined(__OSA_PIC16_MIKROC__)
    #define OS_Task_GetPriority(pTask)      _OS_Task_GetPriority(pTask)
#elif defined (__OSACCS__)
    #define OS_Task_GetPriority(pTask)      pTask->State.cPriority
#else
    #define OS_Task_GetPriority(pTask)      (pTask)->State.cPriority
#endif


//------------------------------------------------------------------------------
#if defined(__OSACCS__)
//------------------------------------------------------------------------------
    #define OS_Task_SetPriority(pTask, new_priority)                        \
        {                                                                   \
            pTask->State.cPriority = new_priority;                          \
            if ((pTask) == (_OS_CurTask))                                   \
                __OS_SET_STATE_PRIORITY(new_priority);                      \
        }
//------------------------------------------------------------------------------
#elif defined(__OSA_STM8_COSMIC__)
//------------------------------------------------------------------------------
    #define OS_Task_SetPriority(pTask, new_priority)                        \
        {                                                                   \
            (pTask)->State.cPriority &= ~7;                                 \
            (pTask)->State.cPriority |= new_priority;                       \
            if ((pTask) == (_OS_CurTask))                                   \
                __OS_SET_STATE_PRIORITY(new_priority);                      \
        }
//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------
    #define OS_Task_SetPriority(pTask, new_priority)                        \
        {                                                                   \
            _OS_SET_IRP_CUR_TASK();       /* for mikroC for PIC16 only */   \
            (pTask)->State.cPriority = new_priority;                        \
            if ((pTask) == (_OS_CurTask))                                   \
                __OS_SET_STATE_PRIORITY(new_priority);                      \
            _OS_CLR_IRP();               /* for mikroC for PIC16 only */    \
        }
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------




/************************************************************************/
/*                                                                      */
/* Get current task's descriptor address                                */
/*                                                                      */
/************************************************************************/

#define OS_GetCurTask()                 this_task
#define OS_Task_GetCur()                this_task


/************************************************************************/
/*                                                                      */
/* Check for task is paused                                             */
/*                                                                      */
/************************************************************************/

#if defined(__OSA_PIC16_MIKROC__)

    #define OS_Task_IsPaused(pTask)         _OS_Task_IsPaused(pTask)
    #define OS_Task_IsEnable(pTasK)         _OS_Task_IsEnable(pTask)

#elif defined(__OSACCS16__)

    #define OS_Task_IsPaused(pTask)         pTask->State.bPaused
    #define OS_Task_IsEnable(pTask)         pTask->State.bEnable

#else

    #define OS_Task_IsPaused(pTask)         (pTask)->State.bPaused
    #define OS_Task_IsEnable(pTask)         (pTask)->State.bEnable

#endif






/************************************************************************/
// Pause timer
/************************************************************************/

#define OS_Ttimer_Pause()           _OS_TASK_ATOMIC_WRITE_A(_OS_CurTask->State.bDelay = 0)

/************************************************************************/
// Continue paused timer
/************************************************************************/

#define OS_Ttimer_Continue()        _OS_TASK_ATOMIC_WRITE_A(_OS_CurTask->State.bDelay = 1)

/************************************************************************/
// Create task inside main() (don't requires OS_Task_Reserve usage)
/************************************************************************/

#ifndef OS_Task_Create_M
#define OS_Task_Create_M(p, t)      OS_Task_Create(p, t)
#endif


#define OS_Task_GetCreated()        &_OS_Tasks[_OS_Temp]  



//******************************************************************************
//  END OF FILE osa_tasks.h
//******************************************************************************

