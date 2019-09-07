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
 *  File:           osa_qtimer.h
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    Queue of timers specific macros
 *                  This file directly included in osa.h
 *
 *  History:        02.10.2010 -    File updated
 *
 ************************************************************************************************
 */


#ifdef OS_ENABLE_QTIMERS


//******************************************************************************
//  VARIABLES
//******************************************************************************

extern OS_BANK  OST_QTIMER_CB   _OS_Qtimers;


#ifndef _OS_QtimersWork_DEFINED
extern OST_QTIMER    * OS_BANK        _OS_QtimerWork;
#endif



//******************************************************************************
//  FUNCTION PROTOTYPES
//******************************************************************************


//******************************************************************************
//  MACROS
//******************************************************************************





//------------------------------------------------------------------------------
// Macros for atomic access

#if defined(OS_PROTECT_MEMORY_ACCESS)
    #define __OS_QTIMER_DI_A()  _OS_DI_INT()
    #define __OS_QTIMER_RI_A()  _OS_RI_INT()
#else
    #define __OS_QTIMER_DI_A()
    #define __OS_QTIMER_RI_A()
#endif


#define OS_QTIMER_ATOMIC_WRITE_A(expr)  \
    OSM_BEGIN {                         \
        __OS_QTIMER_DI_A();             \
        expr;                           \
        __OS_QTIMER_RI_A();             \
    } OSM_END

#define OS_QTIMER_ATOMIC_READ_A(expr)                                     \
    (__OS_QTIMER_DI_A(), _OS_Temp = (expr), __OS_QTIMER_RI_A(), _OS_Temp) \

//------------------------------------------------------------------------------



OS_QTIMER_TYPE _OS_Qtimer_List  (OS_QTIMER_TYPE time, OST_QTIMER *ftimer);



#define __OS_Qtimer_Add(ftimer, time)                                   \
    OSM_BEGIN {                                                         \
        _OS_Qtimer_List(time, &(ftimer));                               \
    } OSM_END                                                           \


#define __OS_Qtimer_Delete(ftimer)                                      \
    OSM_BEGIN {                                                         \
        _OS_Qtimers.Flags.bListFunction = OS_QTIMER_LIST_FUNCTION_DELETE;\
        _OS_Qtimer_List(0, &(ftimer));                                  \
    } OSM_END                                                           \


#define OS_Qtimer_Check(ftimer)      ((ftimer).Flags.bTimeout)       // overflowed
#define OS_Qtimer_IsRun(ftimer)      ((ftimer).Flags.bActive)        // created

#define OS_Qtimer_Run(ftimer, time)                                     \
    OSM_BEGIN {                                                         \
        __OS_Qtimer_Add(ftimer, time);                                  \
    } OSM_END                                                           \


#define OS_Qtimer_Delete(ftimer)                                        \
    OSM_BEGIN {                                                         \
        __OS_Qtimer_Delete(ftimer);                                     \
    } OSM_END


#define OS_Qtimer_Break(ftimer)                                         \
    OSM_BEGIN {                                                         \
        __OS_Qtimer_Delete(ftimer);                                     \
        OS_QTIMER_ATOMIC_WRITE_A((ftimer).Flags.bTimeout = 1);          \
    } OSM_END



#define OS_Qtimer_Create(ftimer)                                        \
    OS_QTIMER_ATOMIC_WRITE_A(                                           \
        (ftimer).Flags.bActive  = 0;                                    \
        (ftimer).Flags.bTimeout = 0;                                    \
    )


#define OS_Qtimer_Get(ftimer)                                                       \
    (_OS_Qtimers.Flags.bListGetTime = 1, _OS_Qtimer_List(1, &(ftimer)))             \



#define OS_Qtimer_Wait(ftimer)                  \
    OSM_BEGIN {                                 \
        OS_Wait(OS_Qtimer_Check(ftimer));\
    } OSM_END


#define OS_Qtimer_Delay(ftimer, delay)          \
    {                                           \
        OS_Qtimer_Run(ftimer, delay);           \
        OS_Qtimer_Wait(ftimer);                 \
    }






 







#endif  // OS_ENABLE_QTIMERS

