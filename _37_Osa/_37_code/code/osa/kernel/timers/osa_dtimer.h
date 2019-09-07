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
 *  File:           osa_dtimer.h
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    Dynamic timers specific macros
 *                  This file directly included in osa.h
 *
 *  History:        02.10.2010 -    File updated
 *
 ************************************************************************************************
 */




/************************************************************************************************
 *                                                                                              *
 *                                 D Y N A M I C   T I M E R S                                  *
 *                                                                                              *
 ************************************************************************************************/



//------------------------------------------------------------------------------
#ifdef OS_ENABLE_DTIMERS
//------------------------------------------------------------------------------



//******************************************************************************
//  VARIABLES
//******************************************************************************

extern OS_BANK  OST_DTIMER_CB   _OS_DTimers;


#ifndef _OS_DtimersWork_DEFINED
extern OST_DTIMER *    _OS_DtimerWork;
#endif


//******************************************************************************
//  FUNCTION PROTOTYPES
//******************************************************************************

void _OS_Dtimer_Create (OST_DTIMER *dtimer);
void _OS_Dtimer_Delete (OST_DTIMER *dtimer);

//******************************************************************************
//  MACROS
//******************************************************************************


//------------------------------------------------------------------------------

#if defined(OS_PROTECT_MEMORY_ACCESS)
    #define __OS_DTIMER_DI_A()  _OS_DI_INT()
    #define __OS_DTIMER_RI_A()  _OS_RI_INT()
#else
    #define __OS_DTIMER_DI_A()
    #define __OS_DTIMER_RI_A()
#endif


#define OS_DTIMER_ATOMIC_WRITE_A(expr)  \
    OSM_BEGIN {                         \
        __OS_DTIMER_DI_A();             \
        expr;                           \
        __OS_DTIMER_RI_A();             \
    } OSM_END

#define OS_DTIMER_ATOMIC_READ_A(expr)                                       \
    (__OS_DTIMER_DI_A(), _OS_Temp = (expr), __OS_DTIMER_RI_A(), _OS_Temp)   \

//------------------------------------------------------------------------------

/************************************************************************/
/* Create and delete dynamic timers                                     */
/************************************************************************/

#define OS_Dtimer_Create(dtimer)     _OS_Dtimer_Create(&dtimer)
#define OS_Dtimer_Delete(dtimer)     _OS_Dtimer_Delete(&dtimer)

/************************************************************************/
/* Check dynamic timer's states                                         */
/************************************************************************/

#define OS_Dtimer_Check(dtimer)         ((dtimer).Flags.bTimeout)       // overflowed
#define OS_Dtimer_IsActive(dtimer)      ((dtimer).Flags.bActive)        // created
#define OS_Dtimer_IsStopped(dtimer)     (!((dtimer).Flags.bRun))        // stopped
#define OS_Dtimer_IsRun(dtimer)         ((dtimer).Flags.bRun)           // run

#define OS_Dtimer_Check_I(dtimer)       ((dtimer).Flags.bTimeout)       // overflowed
#define OS_Dtimer_IsActive_I(dtimer)    ((dtimer).Flags.bActive)        // created
#define OS_Dtimer_IsStopped_I(dtimer)   (!((dtimer).Flags.bRun))        // stopped
#define OS_Dtimer_IsRun_I(dtimer)       ((dtimer).Flags.bRun)           // run

//------------------------------------------------------------------------------
// Get remaining time of counting
#define OS_Dtimer_Get(dtimer)      (-(dtimer).Timer)
#define OS_Dtimer_Get_I(dtimer)    (-(dtimer).Timer)



/************************************************************************/
/* Stop, pause and continue counting                                    */
/************************************************************************/

#define __OS_Dtimer_Stop(dtimer)        (dtimer).Flags.bRun = 0        // Stop timer (pause it)
#define __OS_Dtimer_Pause(dtimer)       (dtimer).Flags.bRun = 0        // Stop timer (pause it)
#define __OS_Dtimer_Continue(dtimer)    (dtimer).Flags.bRun = 1        // Continue after pause
#define __OS_Dtimer_Break(dtimer)       (dtimer).Flags.bTimeout = 1    //

#define OS_Dtimer_Stop(dtimer)          OS_DTIMER_ATOMIC_WRITE_A(__OS_Dtimer_Stop(dtimer))
#define OS_Dtimer_Pause(dtimer)         OS_DTIMER_ATOMIC_WRITE_A(__OS_Dtimer_Pause(dtimer))
#define OS_Dtimer_Continue(dtimer)      OS_DTIMER_ATOMIC_WRITE_A(__OS_Dtimer_Continue(dtimer))
#define OS_Dtimer_Break(dtimer)         OS_DTIMER_ATOMIC_WRITE_A(__OS_Dtimer_Break(dtimer))

#define OS_Dtimer_Stop_I(dtimer)        __OS_Dtimer_Stop(dtimer)
#define OS_Dtimer_Pause_I(dtimer)       __OS_Dtimer_Pause(dtimer)
#define OS_Dtimer_Continue_I(dtimer)    __OS_Dtimer_Continue(dtimer)
#define OS_Dtimer_Break_I(dtimer)       __OS_Dtimer_Break(dtimer)



/************************************************************************/
/* Services for run dynamic timers                                      */
/************************************************************************/

//------------------------------------------------------------------------------
// Start counting with given time. Timeout flag is cleared.

#define OS_Dtimer_Run(dtimer, time)                         \
    OSM_BEGIN {                                             \
        OS_DTIMER_ATOMIC_WRITE_A((dtimer).Flags.bRun = 0;); \
        (dtimer).Timer =-(OS_DTIMER_TYPE)(time);            \
        OS_DTIMER_ATOMIC_WRITE_A(                           \
            (dtimer).Flags.bTimeout = 0;                    \
            (dtimer).Flags.bRun = 1;                        \
        );                                                  \
    } OSM_END

#define OS_Dtimer_Run_I(dtimer, time)                       \
    OSM_BEGIN {                                             \
        (dtimer).Flags.bRun = 0;                            \
        (dtimer).Timer = -(OS_DTIMER_TYPE)(time);           \
        (dtimer).Flags.bTimeout = 0;                        \
        (dtimer).Flags.bRun = 1;                            \
    } OSM_END

//------------------------------------------------------------------------------
// Re-run static timer for time time since last overflow. Timeout flag is cleared.

#define OS_Dtimer_Update(dtimer, time)                      \
    OSM_BEGIN {                                             \
        OS_DTIMER_ATOMIC_WRITE_A((dtimer).Flags.bRun = 0;); \
        (dtimer).Timer =-(OS_DTIMER_TYPE)(time);            \
        OS_DTIMER_ATOMIC_WRITE_A(                           \
            (dtimer).Flags.bTimeout = 0;                    \
            (dtimer).Flags.bRun = 1;                        \
        );                                                  \
    } OSM_END

#define OS_Dtimer_Update_I(dtimer, time)                    \
    OSM_BEGIN {                                             \
        (dtimer).Flags.bRun = 0;                            \
        (dtimer).Timer = -(OS_DTIMER_TYPE)(time);           \
        (dtimer).Flags.bTimeout = 0;                        \
        (dtimer).Flags.bRun = 1;                            \
    } OSM_END

//------------------------------------------------------------------------------
// Increase time of dinamic timer counter. Timeout flag remain unchanged.

#define OS_Dtimer_Add(dtimer, time)                         \
    OSM_BEGIN {                                             \
        OS_DTIMER_ATOMIC_WRITE_A((dtimer).Flags.bRun = 0;); \
        (dtimer).Timer -= (OS_DTIMER_TYPE)(time);           \
        OS_DTIMER_ATOMIC_WRITE_A((dtimer).Flags.bRun = 1;); \
    } OSM_END

#define OS_Dtimer_Add_I(dtimer, time)                       \
    OSM_BEGIN {                                             \
        (dtimer).Flags.bRun = 0;                            \
        (dtimer).Timer -= (OS_DTIMER_TYPE)(time);           \
        (dtimer).Flags.bRun = 1;                            \
    } OSM_END






/************************************************************************/
/* Wait services                                                        */
/************************************************************************/

//------------------------------------------------------------------------------
// Wait for dynamic timer overflowed

#define OS_Dtimer_Wait(dtimer)                  \
    {                                           \
        OS_Wait(OS_Dtimer_Check(dtimer));\
    }


//------------------------------------------------------------------------------
// Delay current task using dynamic timer

#define OS_Dtimer_Delay(dtimer, delay)          \
    {                                           \
        OS_Dtimer_Run(dtimer, delay);           \
        OS_Dtimer_Wait(dtimer);                 \
    }








//------------------------------------------------------------------------------
#endif  // OS_ENABLE_DTIMERS
//------------------------------------------------------------------------------
















