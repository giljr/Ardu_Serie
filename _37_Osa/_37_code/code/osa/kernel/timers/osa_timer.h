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
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    Services for work with system timer
 *                  This file directly included in osa.c
 *
 *  History:        02.10.2010 -    File updated
 *
 ************************************************************************************************
 */



/************************************************************************************************
 *                                                                                              *
 *                          S Y S T E M   T I M E R   W O R K                                   *
 *                                                                                              *
 ************************************************************************************************/

//******************************************************************************
//  VARIABLES
//******************************************************************************


//******************************************************************************
//  FUNCTION PROTOTYPES
//******************************************************************************


//******************************************************************************
//  MACROS
//******************************************************************************

#ifndef _OS_TIMER_H_
#define _OS_TIMER_H_


#ifdef OS_ENABLE_OS_TIMER

    #ifndef OS_USE_INLINE_TIMER
        extern void OS_Timer (void);
    #else
        #define OS_Timer()  __OS_TimerInline()
    #endif

#endif


//------------------------------------------------------------------------------
#ifndef _OS_DtimersWork_DEFINED
//------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    #ifndef OS_ENABLE_DTIMERS
    //------------------------------------------------------------------------------

        #define __OS_DtimersWork()

    //------------------------------------------------------------------------------
    #else
    //------------------------------------------------------------------------------


        #define __OS_DtimersWork()                                              \
            {                                                                   \
                _OS_DtimerWork = (OST_DTIMER*)&_OS_DTimers;                     \
                while (_OS_DtimerWork->Flags.bNextEnable)                       \
                {                                                               \
                  _OS_DtimerWork = (OST_DTIMER*)_OS_DtimerWork->Next;           \
                  if (!(++_OS_DtimerWork->Timer))                               \
                    _OS_DtimerWork->Flags.bTimeout |= 1;                        \
                }                                                               \
            }

    //------------------------------------------------------------------------------
    #endif  // OS_ENABLE_DTIMER
    //------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
#ifndef _OS_QtimersWork_DEFINED
//------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    #ifndef OS_ENABLE_QTIMERS
    //------------------------------------------------------------------------------

        #define __OS_QtimersWork()

    //------------------------------------------------------------------------------
    #else
    //------------------------------------------------------------------------------


        #define __OS_QtimersWork()                                          \
            if (_OS_Qtimers.Flags.bNextEnable)                              \
            {                                                               \
                _OS_QtimerWork = (OST_QTIMER*)_OS_Qtimers.Next;             \
                _OS_QtimerWork->Timer++;                                    \
                while (!_OS_QtimerWork->Timer)                              \
                {                                                           \
                    _OS_QtimerWork->Flags.bTimeout = 1;                     \
                    _OS_QtimerWork->Flags.bActive  = 0;                     \
                    if (!_OS_QtimerWork->Flags.bNextEnable)                 \
                    {                                                       \
                        _OS_Qtimers.Flags.bNextEnable = 0;                  \
                        break;                                              \
                    }                                                       \
                    _OS_Qtimers.Next = _OS_QtimerWork->Next;                \
                    _OS_QtimerWork = (OST_QTIMER*)_OS_Qtimers.Next;         \
                }                                                           \
            }                                                               \

    //------------------------------------------------------------------------------
    #endif  // OS_ENABLE_QTIMER
    //------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------






/************************************************************************/
/* Old style static timers                                              */
/************************************************************************/

//------------------------------------------------------------------------------
#ifndef __OS_Timer8Work
//------------------------------------------------------------------------------

#define __OS_Timer8Work(TIMER_ID)                                                               \
    {                                                                                           \
        if ((OS_Timeouts[(TIMER_ID)>>_OST_INT_SHIFT]&(1<<((TIMER_ID)&_OST_INT_MASK)))) {        \
            if (!++OS_Timers8[TIMER_ID])                                                        \
                OS_Timeouts[(TIMER_ID)>>_OST_INT_SHIFT] &= ~(1<<((TIMER_ID)&_OST_INT_MASK));    \
        }                                                                                       \
    }

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
#ifndef __OS_Timer16Work
//------------------------------------------------------------------------------

#define __OS_Timer16Work(TIMER_ID)                                                                                          \
    {                                                                                                                       \
        if ((OS_Timeouts[(TIMER_ID+_OS_TIMER16_POS)>>_OST_INT_SHIFT]&(1<<((TIMER_ID+_OS_TIMER16_POS)&_OST_INT_MASK)))) {    \
            if (!++OS_Timers16[TIMER_ID])                                                                                   \
                OS_Timeouts[(TIMER_ID+_OS_TIMER16_POS)>>_OST_INT_SHIFT] &= ~(1<<((TIMER_ID+_OS_TIMER16_POS)&_OST_INT_MASK));\
        }                                                                                                                   \
    }

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
#ifndef __OS_Timer24Work
//------------------------------------------------------------------------------

#define __OS_Timer24Work(TIMER_ID)                                                                                          \
    {                                                                                                                       \
        if ((OS_Timeouts[(TIMER_ID+_OS_TIMER24_POS)>>_OST_INT_SHIFT]&(1<<((TIMER_ID+_OS_TIMER24_POS)&_OST_INT_MASK)))) {    \
            if (!++OS_Timers24[TIMER_ID])                                                                                   \
                OS_Timeouts[(TIMER_ID+_OS_TIMER24_POS)>>_OST_INT_SHIFT] &= ~(1<<((TIMER_ID+_OS_TIMER24_POS)&_OST_INT_MASK));\
        }                                                                                                                   \
    }

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
#ifndef __OS_Timer32Work
//------------------------------------------------------------------------------

#define __OS_Timer32Work(TIMER_ID)                                                                                          \
    {                                                                                                                       \
        if ((OS_Timeouts[(TIMER_ID+_OS_TIMER32_POS)>>_OST_INT_SHIFT]&(1<<((TIMER_ID+_OS_TIMER32_POS)&_OST_INT_MASK)))) {    \
            if (!++OS_Timers32[TIMER_ID])                                                                                   \
                OS_Timeouts[(TIMER_ID+_OS_TIMER32_POS)>>_OST_INT_SHIFT] &= ~(1<<((TIMER_ID+_OS_TIMER32_POS)&_OST_INT_MASK));\
        }                                                                                                                   \
    }

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------






/************************************************************************/
/* Task timers                                                          */
/************************************************************************/

//------------------------------------------------------------------------------
#ifndef __OS_TtimerWork
//------------------------------------------------------------------------------

#define __OS_TtimerWork(TASK_ID)                                                    \
    if (_OS_Tasks[TASK_ID].State.bDelay)                                          \
    {                                                                               \
        if(!++_OS_Tasks[TASK_ID].Timer) _OS_Tasks[TASK_ID].State.bDelay = 0;    \
    }                                                                               \

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------




/************************************************************************/
/* New style static timers                                              */
/************************************************************************/

//------------------------------------------------------------------------------
#ifndef __OS_StimerWork
//------------------------------------------------------------------------------

#define __OS_StimerWork(STIMER_ID)                                                  \
    if (_OS_Stimers[STIMER_ID] & OS_STIMER_RUN_BIT)                                   \
    {                                                                               \
        _OS_Stimers[STIMER_ID]++;                                                     \
    }                                                                               \

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------









/************************************************************************************************
 *                                                                                              *
 *                         C O M M O N   T I M E R ' S   M A C R O S                             *
 *                                                                                               *
 ************************************************************************************************/

#if OS_TIMERS8 > 0
#define __OS_Timer8Work0()    __OS_Timer8Work(0)
#else
#define __OS_Timer8Work0()
#endif
//..............................................................................
#if OS_TIMERS8 > 1
#define __OS_Timer8Work1()    __OS_Timer8Work(1)
#else
#define __OS_Timer8Work1()
#endif
//..............................................................................
#if OS_TIMERS8 > 2
#define __OS_Timer8Work2()    __OS_Timer8Work(2)
#else
#define __OS_Timer8Work2()
#endif
//..............................................................................
#if OS_TIMERS8 > 3
#define __OS_Timer8Work3()    __OS_Timer8Work(3)
#else
#define __OS_Timer8Work3()
#endif
//..............................................................................
#if OS_TIMERS8 > 4
#define __OS_Timer8Work4()    __OS_Timer8Work(4)
#else
#define __OS_Timer8Work4()
#endif
//..............................................................................
#if OS_TIMERS8 > 5
#define __OS_Timer8Work5()    __OS_Timer8Work(5)
#else
#define __OS_Timer8Work5()
#endif
//..............................................................................
#if OS_TIMERS8 > 6
#define __OS_Timer8Work6()    __OS_Timer8Work(6)
#else
#define __OS_Timer8Work6()
#endif
//..............................................................................
#if OS_TIMERS8 > 7
#define __OS_Timer8Work7()    __OS_Timer8Work(7)
#else
#define __OS_Timer8Work7()
#endif
//..............................................................................
#if OS_TIMERS8 > 8
#define __OS_Timer8Work8()    __OS_Timer8Work(8)
#else
#define __OS_Timer8Work8()
#endif
//..............................................................................
#if OS_TIMERS8 > 9
#define __OS_Timer8Work9()    __OS_Timer8Work(9)
#else
#define __OS_Timer8Work9()
#endif
//..............................................................................
#if OS_TIMERS8 > 10
#define __OS_Timer8Work10()    __OS_Timer8Work(10)
#else
#define __OS_Timer8Work10()
#endif
//..............................................................................
#if OS_TIMERS8 > 11
#define __OS_Timer8Work11()    __OS_Timer8Work(11)
#else
#define __OS_Timer8Work11()
#endif
//..............................................................................
#if OS_TIMERS8 > 12
#define __OS_Timer8Work12()    __OS_Timer8Work(12)
#else
#define __OS_Timer8Work12()
#endif
//..............................................................................
#if OS_TIMERS8 > 13
#define __OS_Timer8Work13()    __OS_Timer8Work(13)
#else
#define __OS_Timer8Work13()
#endif
//..............................................................................
#if OS_TIMERS8 > 14
#define __OS_Timer8Work14()    __OS_Timer8Work(14)
#else
#define __OS_Timer8Work14()
#endif
//..............................................................................
#if OS_TIMERS8 > 15
#define __OS_Timer8Work15()    __OS_Timer8Work(15)
#else
#define __OS_Timer8Work15()
#endif
//..............................................................................
#if OS_TIMERS8 > 16
#define __OS_Timer8Work16()    __OS_Timer8Work(16)
#else
#define __OS_Timer8Work16()
#endif
//..............................................................................
#if OS_TIMERS8 > 17
#define __OS_Timer8Work17()    __OS_Timer8Work(17)
#else
#define __OS_Timer8Work17()
#endif
//..............................................................................
#if OS_TIMERS8 > 18
#define __OS_Timer8Work18()    __OS_Timer8Work(18)
#else
#define __OS_Timer8Work18()
#endif
//..............................................................................
#if OS_TIMERS8 > 19
#define __OS_Timer8Work19()    __OS_Timer8Work(19)
#else
#define __OS_Timer8Work19()
#endif
//..............................................................................
#if OS_TIMERS8 > 20
#define __OS_Timer8Work20()    __OS_Timer8Work(20)
#else
#define __OS_Timer8Work20()
#endif
//..............................................................................
#if OS_TIMERS8 > 21
#define __OS_Timer8Work21()    __OS_Timer8Work(21)
#else
#define __OS_Timer8Work21()
#endif
//..............................................................................
#if OS_TIMERS8 > 22
#define __OS_Timer8Work22()    __OS_Timer8Work(22)
#else
#define __OS_Timer8Work22()
#endif
//..............................................................................
#if OS_TIMERS8 > 23
#define __OS_Timer8Work23()    __OS_Timer8Work(23)
#else
#define __OS_Timer8Work23()
#endif
//..............................................................................
#if OS_TIMERS8 > 24
#define __OS_Timer8Work24()    __OS_Timer8Work(24)
#else
#define __OS_Timer8Work24()
#endif
//..............................................................................
#if OS_TIMERS8 > 25
#define __OS_Timer8Work25()    __OS_Timer8Work(25)
#else
#define __OS_Timer8Work25()
#endif
//..............................................................................
#if OS_TIMERS8 > 26
#define __OS_Timer8Work26()    __OS_Timer8Work(26)
#else
#define __OS_Timer8Work26()
#endif
//..............................................................................
#if OS_TIMERS8 > 27
#define __OS_Timer8Work27()    __OS_Timer8Work(27)
#else
#define __OS_Timer8Work27()
#endif
//..............................................................................
#if OS_TIMERS8 > 28
#define __OS_Timer8Work28()    __OS_Timer8Work(28)
#else
#define __OS_Timer8Work28()
#endif
//..............................................................................
#if OS_TIMERS8 > 29
#define __OS_Timer8Work29()    __OS_Timer8Work(29)
#else
#define __OS_Timer8Work29()
#endif
//..............................................................................
#if OS_TIMERS8 > 30
#define __OS_Timer8Work30()    __OS_Timer8Work(30)
#else
#define __OS_Timer8Work30()
#endif
//..............................................................................
#if OS_TIMERS8 > 31
#define __OS_Timer8Work31()    __OS_Timer8Work(31)
#else
#define __OS_Timer8Work31()
#endif
//..............................................................................


//______________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//..............................................................................
#if OS_TIMERS16 > 0
#define __OS_Timer16Work0()    __OS_Timer16Work(0)
#else
#define __OS_Timer16Work0()
#endif
//..............................................................................
#if OS_TIMERS16 > 1
#define __OS_Timer16Work1()    __OS_Timer16Work(1)
#else
#define __OS_Timer16Work1()
#endif
//..............................................................................
#if OS_TIMERS16 > 2
#define __OS_Timer16Work2()    __OS_Timer16Work(2)
#else
#define __OS_Timer16Work2()
#endif
//..............................................................................
#if OS_TIMERS16 > 3
#define __OS_Timer16Work3()    __OS_Timer16Work(3)
#else
#define __OS_Timer16Work3()
#endif
//..............................................................................
#if OS_TIMERS16 > 4
#define __OS_Timer16Work4()    __OS_Timer16Work(4)
#else
#define __OS_Timer16Work4()
#endif
//..............................................................................
#if OS_TIMERS16 > 5
#define __OS_Timer16Work5()    __OS_Timer16Work(5)
#else
#define __OS_Timer16Work5()
#endif
//..............................................................................
#if OS_TIMERS16 > 6
#define __OS_Timer16Work6()    __OS_Timer16Work(6)
#else
#define __OS_Timer16Work6()
#endif
//..............................................................................
#if OS_TIMERS16 > 7
#define __OS_Timer16Work7()    __OS_Timer16Work(7)
#else
#define __OS_Timer16Work7()
#endif
//..............................................................................
#if OS_TIMERS16 > 8
#define __OS_Timer16Work8()    __OS_Timer16Work(8)
#else
#define __OS_Timer16Work8()
#endif
//..............................................................................
#if OS_TIMERS16 > 9
#define __OS_Timer16Work9()    __OS_Timer16Work(9)
#else
#define __OS_Timer16Work9()
#endif
//..............................................................................
#if OS_TIMERS16 > 10
#define __OS_Timer16Work10()    __OS_Timer16Work(10)
#else
#define __OS_Timer16Work10()
#endif
//..............................................................................
#if OS_TIMERS16 > 11
#define __OS_Timer16Work11()    __OS_Timer16Work(11)
#else
#define __OS_Timer16Work11()
#endif
//..............................................................................
#if OS_TIMERS16 > 12
#define __OS_Timer16Work12()    __OS_Timer16Work(12)
#else
#define __OS_Timer16Work12()
#endif
//..............................................................................
#if OS_TIMERS16 > 13
#define __OS_Timer16Work13()    __OS_Timer16Work(13)
#else
#define __OS_Timer16Work13()
#endif
//..............................................................................
#if OS_TIMERS16 > 14
#define __OS_Timer16Work14()    __OS_Timer16Work(14)
#else
#define __OS_Timer16Work14()
#endif
//..............................................................................
#if OS_TIMERS16 > 15
#define __OS_Timer16Work15()    __OS_Timer16Work(15)
#else
#define __OS_Timer16Work15()
#endif
//..............................................................................
#if OS_TIMERS16 > 16
#define __OS_Timer16Work16()    __OS_Timer16Work(16)
#else
#define __OS_Timer16Work16()
#endif
//..............................................................................
#if OS_TIMERS16 > 17
#define __OS_Timer16Work17()    __OS_Timer16Work(17)
#else
#define __OS_Timer16Work17()
#endif
//..............................................................................
#if OS_TIMERS16 > 18
#define __OS_Timer16Work18()    __OS_Timer16Work(18)
#else
#define __OS_Timer16Work18()
#endif
//..............................................................................
#if OS_TIMERS16 > 19
#define __OS_Timer16Work19()    __OS_Timer16Work(19)
#else
#define __OS_Timer16Work19()
#endif
//..............................................................................
#if OS_TIMERS16 > 20
#define __OS_Timer16Work20()    __OS_Timer16Work(20)
#else
#define __OS_Timer16Work20()
#endif
//..............................................................................
#if OS_TIMERS16 > 21
#define __OS_Timer16Work21()    __OS_Timer16Work(21)
#else
#define __OS_Timer16Work21()
#endif
//..............................................................................
#if OS_TIMERS16 > 22
#define __OS_Timer16Work22()    __OS_Timer16Work(22)
#else
#define __OS_Timer16Work22()
#endif
//..............................................................................
#if OS_TIMERS16 > 23
#define __OS_Timer16Work23()    __OS_Timer16Work(23)
#else
#define __OS_Timer16Work23()
#endif
//..............................................................................
#if OS_TIMERS16 > 24
#define __OS_Timer16Work24()    __OS_Timer16Work(24)
#else
#define __OS_Timer16Work24()
#endif
//..............................................................................
#if OS_TIMERS16 > 25
#define __OS_Timer16Work25()    __OS_Timer16Work(25)
#else
#define __OS_Timer16Work25()
#endif
//..............................................................................
#if OS_TIMERS16 > 26
#define __OS_Timer16Work26()    __OS_Timer16Work(26)
#else
#define __OS_Timer16Work26()
#endif
//..............................................................................
#if OS_TIMERS16 > 27
#define __OS_Timer16Work27()    __OS_Timer16Work(27)
#else
#define __OS_Timer16Work27()
#endif
//..............................................................................
#if OS_TIMERS16 > 28
#define __OS_Timer16Work28()    __OS_Timer16Work(28)
#else
#define __OS_Timer16Work28()
#endif
//..............................................................................
#if OS_TIMERS16 > 29
#define __OS_Timer16Work29()    __OS_Timer16Work(29)
#else
#define __OS_Timer16Work29()
#endif
//..............................................................................
#if OS_TIMERS16 > 30
#define __OS_Timer16Work30()    __OS_Timer16Work(30)
#else
#define __OS_Timer16Work30()
#endif
//..............................................................................
#if OS_TIMERS16 > 31
#define __OS_Timer16Work31()    __OS_Timer16Work(31)
#else
#define __OS_Timer16Work31()
#endif
//..............................................................................






//______________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#if OS_TIMERS24 > 0
#define __OS_CheckInc24()   if (!(OS_Ticks & 0xFF))
#else
#define __OS_CheckInc24()
#endif


//..............................................................................
#if OS_TIMERS24 > 0
#define __OS_Timer24Work0()    __OS_Timer24Work(0)
#else
#define __OS_Timer24Work0()
#endif
//..............................................................................
#if OS_TIMERS24 > 1
#define __OS_Timer24Work1()    __OS_Timer24Work(1)
#else
#define __OS_Timer24Work1()
#endif
//..............................................................................
#if OS_TIMERS24 > 2
#define __OS_Timer24Work2()    __OS_Timer24Work(2)
#else
#define __OS_Timer24Work2()
#endif
//..............................................................................
#if OS_TIMERS24 > 3
#define __OS_Timer24Work3()    __OS_Timer24Work(3)
#else
#define __OS_Timer24Work3()
#endif
//..............................................................................
#if OS_TIMERS24 > 4
#define __OS_Timer24Work4()    __OS_Timer24Work(4)
#else
#define __OS_Timer24Work4()
#endif
//..............................................................................
#if OS_TIMERS24 > 5
#define __OS_Timer24Work5()    __OS_Timer24Work(5)
#else
#define __OS_Timer24Work5()
#endif
//..............................................................................
#if OS_TIMERS24 > 6
#define __OS_Timer24Work6()    __OS_Timer24Work(6)
#else
#define __OS_Timer24Work6()
#endif
//..............................................................................
#if OS_TIMERS24 > 7
#define __OS_Timer24Work7()    __OS_Timer24Work(7)
#else
#define __OS_Timer24Work7()
#endif
//..............................................................................
#if OS_TIMERS24 > 8
#define __OS_Timer24Work8()    __OS_Timer24Work(8)
#else
#define __OS_Timer24Work8()
#endif
//..............................................................................
#if OS_TIMERS24 > 9
#define __OS_Timer24Work9()    __OS_Timer24Work(9)
#else
#define __OS_Timer24Work9()
#endif
//..............................................................................
#if OS_TIMERS24 > 10
#define __OS_Timer24Work10()    __OS_Timer24Work(10)
#else
#define __OS_Timer24Work10()
#endif
//..............................................................................
#if OS_TIMERS24 > 11
#define __OS_Timer24Work11()    __OS_Timer24Work(11)
#else
#define __OS_Timer24Work11()
#endif
//..............................................................................
#if OS_TIMERS24 > 12
#define __OS_Timer24Work12()    __OS_Timer24Work(12)
#else
#define __OS_Timer24Work12()
#endif
//..............................................................................
#if OS_TIMERS24 > 13
#define __OS_Timer24Work13()    __OS_Timer24Work(13)
#else
#define __OS_Timer24Work13()
#endif
//..............................................................................
#if OS_TIMERS24 > 14
#define __OS_Timer24Work14()    __OS_Timer24Work(14)
#else
#define __OS_Timer24Work14()
#endif
//..............................................................................
#if OS_TIMERS24 > 15
#define __OS_Timer24Work15()    __OS_Timer24Work(15)
#else
#define __OS_Timer24Work15()
#endif
//..............................................................................
#if OS_TIMERS24 > 16
#define __OS_Timer24Work16()    __OS_Timer24Work(16)
#else
#define __OS_Timer24Work16()
#endif
//..............................................................................
#if OS_TIMERS24 > 17
#define __OS_Timer24Work17()    __OS_Timer24Work(17)
#else
#define __OS_Timer24Work17()
#endif
//..............................................................................
#if OS_TIMERS24 > 18
#define __OS_Timer24Work18()    __OS_Timer24Work(18)
#else
#define __OS_Timer24Work18()
#endif
//..............................................................................
#if OS_TIMERS24 > 19
#define __OS_Timer24Work19()    __OS_Timer24Work(19)
#else
#define __OS_Timer24Work19()
#endif
//..............................................................................
#if OS_TIMERS24 > 20
#define __OS_Timer24Work20()    __OS_Timer24Work(20)
#else
#define __OS_Timer24Work20()
#endif
//..............................................................................
#if OS_TIMERS24 > 21
#define __OS_Timer24Work21()    __OS_Timer24Work(21)
#else
#define __OS_Timer24Work21()
#endif
//..............................................................................
#if OS_TIMERS24 > 22
#define __OS_Timer24Work22()    __OS_Timer24Work(22)
#else
#define __OS_Timer24Work22()
#endif
//..............................................................................
#if OS_TIMERS24 > 23
#define __OS_Timer24Work23()    __OS_Timer24Work(23)
#else
#define __OS_Timer24Work23()
#endif
//..............................................................................
#if OS_TIMERS24 > 24
#define __OS_Timer24Work24()    __OS_Timer24Work(24)
#else
#define __OS_Timer24Work24()
#endif
//..............................................................................
#if OS_TIMERS24 > 25
#define __OS_Timer24Work25()    __OS_Timer24Work(25)
#else
#define __OS_Timer24Work25()
#endif
//..............................................................................
#if OS_TIMERS24 > 26
#define __OS_Timer24Work26()    __OS_Timer24Work(26)
#else
#define __OS_Timer24Work26()
#endif
//..............................................................................
#if OS_TIMERS24 > 27
#define __OS_Timer24Work27()    __OS_Timer24Work(27)
#else
#define __OS_Timer24Work27()
#endif
//..............................................................................
#if OS_TIMERS24 > 28
#define __OS_Timer24Work28()    __OS_Timer24Work(28)
#else
#define __OS_Timer24Work28()
#endif
//..............................................................................
#if OS_TIMERS24 > 29
#define __OS_Timer24Work29()    __OS_Timer24Work(29)
#else
#define __OS_Timer24Work29()
#endif
//..............................................................................
#if OS_TIMERS24 > 30
#define __OS_Timer24Work30()    __OS_Timer24Work(30)
#else
#define __OS_Timer24Work30()
#endif
//..............................................................................
#if OS_TIMERS24 > 31
#define __OS_Timer24Work31()    __OS_Timer24Work(31)
#else
#define __OS_Timer24Work31()
#endif
//..............................................................................













//______________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//..............................................................................
#if OS_TIMERS32 > 0
#define __OS_Timer32Work0()    __OS_Timer32Work(0)
#else
#define __OS_Timer32Work0()
#endif
//..............................................................................
#if OS_TIMERS32 > 1
#define __OS_Timer32Work1()    __OS_Timer32Work(1)
#else
#define __OS_Timer32Work1()
#endif
//..............................................................................
#if OS_TIMERS32 > 2
#define __OS_Timer32Work2()    __OS_Timer32Work(2)
#else
#define __OS_Timer32Work2()
#endif
//..............................................................................
#if OS_TIMERS32 > 3
#define __OS_Timer32Work3()    __OS_Timer32Work(3)
#else
#define __OS_Timer32Work3()
#endif
//..............................................................................
#if OS_TIMERS32 > 4
#define __OS_Timer32Work4()    __OS_Timer32Work(4)
#else
#define __OS_Timer32Work4()
#endif
//..............................................................................
#if OS_TIMERS32 > 5
#define __OS_Timer32Work5()    __OS_Timer32Work(5)
#else
#define __OS_Timer32Work5()
#endif
//..............................................................................
#if OS_TIMERS32 > 6
#define __OS_Timer32Work6()    __OS_Timer32Work(6)
#else
#define __OS_Timer32Work6()
#endif
//..............................................................................
#if OS_TIMERS32 > 7
#define __OS_Timer32Work7()    __OS_Timer32Work(7)
#else
#define __OS_Timer32Work7()
#endif
//..............................................................................
#if OS_TIMERS32 > 8
#define __OS_Timer32Work8()    __OS_Timer32Work(8)
#else
#define __OS_Timer32Work8()
#endif
//..............................................................................
#if OS_TIMERS32 > 9
#define __OS_Timer32Work9()    __OS_Timer32Work(9)
#else
#define __OS_Timer32Work9()
#endif
//..............................................................................
#if OS_TIMERS32 > 10
#define __OS_Timer32Work10()    __OS_Timer32Work(10)
#else
#define __OS_Timer32Work10()
#endif
//..............................................................................
#if OS_TIMERS32 > 11
#define __OS_Timer32Work11()    __OS_Timer32Work(11)
#else
#define __OS_Timer32Work11()
#endif
//..............................................................................
#if OS_TIMERS32 > 12
#define __OS_Timer32Work12()    __OS_Timer32Work(12)
#else
#define __OS_Timer32Work12()
#endif
//..............................................................................
#if OS_TIMERS32 > 13
#define __OS_Timer32Work13()    __OS_Timer32Work(13)
#else
#define __OS_Timer32Work13()
#endif
//..............................................................................
#if OS_TIMERS32 > 14
#define __OS_Timer32Work14()    __OS_Timer32Work(14)
#else
#define __OS_Timer32Work14()
#endif
//..............................................................................
#if OS_TIMERS32 > 15
#define __OS_Timer32Work15()    __OS_Timer32Work(15)
#else
#define __OS_Timer32Work15()
#endif
//..............................................................................
#if OS_TIMERS32 > 16
#define __OS_Timer32Work16()    __OS_Timer32Work(16)
#else
#define __OS_Timer32Work16()
#endif
//..............................................................................
#if OS_TIMERS32 > 17
#define __OS_Timer32Work17()    __OS_Timer32Work(17)
#else
#define __OS_Timer32Work17()
#endif
//..............................................................................
#if OS_TIMERS32 > 18
#define __OS_Timer32Work18()    __OS_Timer32Work(18)
#else
#define __OS_Timer32Work18()
#endif
//..............................................................................
#if OS_TIMERS32 > 19
#define __OS_Timer32Work19()    __OS_Timer32Work(19)
#else
#define __OS_Timer32Work19()
#endif
//..............................................................................
#if OS_TIMERS32 > 20
#define __OS_Timer32Work20()    __OS_Timer32Work(20)
#else
#define __OS_Timer32Work20()
#endif
//..............................................................................
#if OS_TIMERS32 > 21
#define __OS_Timer32Work21()    __OS_Timer32Work(21)
#else
#define __OS_Timer32Work21()
#endif
//..............................................................................
#if OS_TIMERS32 > 22
#define __OS_Timer32Work22()    __OS_Timer32Work(22)
#else
#define __OS_Timer32Work22()
#endif
//..............................................................................
#if OS_TIMERS32 > 23
#define __OS_Timer32Work23()    __OS_Timer32Work(23)
#else
#define __OS_Timer32Work23()
#endif
//..............................................................................
#if OS_TIMERS32 > 24
#define __OS_Timer32Work24()    __OS_Timer32Work(24)
#else
#define __OS_Timer32Work24()
#endif
//..............................................................................
#if OS_TIMERS32 > 25
#define __OS_Timer32Work25()    __OS_Timer32Work(25)
#else
#define __OS_Timer32Work25()
#endif
//..............................................................................
#if OS_TIMERS32 > 26
#define __OS_Timer32Work26()    __OS_Timer32Work(26)
#else
#define __OS_Timer32Work26()
#endif
//..............................................................................
#if OS_TIMERS32 > 27
#define __OS_Timer32Work27()    __OS_Timer32Work(27)
#else
#define __OS_Timer32Work27()
#endif
//..............................................................................
#if OS_TIMERS32 > 28
#define __OS_Timer32Work28()    __OS_Timer32Work(28)
#else
#define __OS_Timer32Work28()
#endif
//..............................................................................
#if OS_TIMERS32 > 29
#define __OS_Timer32Work29()    __OS_Timer32Work(29)
#else
#define __OS_Timer32Work29()
#endif
//..............................................................................
#if OS_TIMERS32 > 30
#define __OS_Timer32Work30()    __OS_Timer32Work(30)
#else
#define __OS_Timer32Work30()
#endif
//..............................................................................
#if OS_TIMERS32 > 31
#define __OS_Timer32Work31()    __OS_Timer32Work(31)
#else
#define __OS_Timer32Work31()
#endif
//..............................................................................







//______________________________________________________________________________
//******************************************************************************
/**/


#ifndef ASM_OST_TCB_SIZE_CONST
#define ASM_OST_TCB_SIZE_CONST
#endif

#ifndef ASM_SET_BANK
#define ASM_SET_BANK
#endif

#if defined(OS_ENABLE_TTIMERS)

    #if OS_TASKS > 0
    #define __OS_TtimerWork0()    __OS_TtimerWork(0)
    #else
    #define __OS_TtimerWork0()
    #endif
    //...........................................................
    #if OS_TASKS > 1
    #define __OS_TtimerWork1()    __OS_TtimerWork(1)
    #else
    #define __OS_TtimerWork1()
    #endif
    //...........................................................
    #if OS_TASKS > 2
    #define __OS_TtimerWork2()    __OS_TtimerWork(2)
    #else
    #define __OS_TtimerWork2()
    #endif
    //...........................................................
    #if OS_TASKS > 3
    #define __OS_TtimerWork3()    __OS_TtimerWork(3)
    #else
    #define __OS_TtimerWork3()
    #endif
    //...........................................................
    #if OS_TASKS > 4
    #define __OS_TtimerWork4()    __OS_TtimerWork(4)
    #else
    #define __OS_TtimerWork4()
    #endif
    //...........................................................
    #if OS_TASKS > 5
    #define __OS_TtimerWork5()    __OS_TtimerWork(5)
    #else
    #define __OS_TtimerWork5()
    #endif
    //...........................................................
    #if OS_TASKS > 6
    #define __OS_TtimerWork6()    __OS_TtimerWork(6)
    #else
    #define __OS_TtimerWork6()
    #endif
    //...........................................................
    #if OS_TASKS > 7
    #define __OS_TtimerWork7()    __OS_TtimerWork(7)
    #else
    #define __OS_TtimerWork7()
    #endif
    //...........................................................
    #if OS_TASKS > 8
    #define __OS_TtimerWork8()    __OS_TtimerWork(8)
    #else
    #define __OS_TtimerWork8()
    #endif
    //...........................................................
    #if OS_TASKS > 9
    #define __OS_TtimerWork9()    __OS_TtimerWork(9)
    #else
    #define __OS_TtimerWork9()
    #endif
    //...........................................................
    #if OS_TASKS > 10
    #define __OS_TtimerWork10()    __OS_TtimerWork(10)
    #else
    #define __OS_TtimerWork10()
    #endif
    //...........................................................
    #if OS_TASKS > 11
    #define __OS_TtimerWork11()    __OS_TtimerWork(11)
    #else
    #define __OS_TtimerWork11()
    #endif
    //...........................................................
    #if OS_TASKS > 12
    #define __OS_TtimerWork12()    __OS_TtimerWork(12)
    #else
    #define __OS_TtimerWork12()
    #endif
    //...........................................................
    #if OS_TASKS > 13
    #define __OS_TtimerWork13()    __OS_TtimerWork(13)
    #else
    #define __OS_TtimerWork13()
    #endif
    //...........................................................
    #if OS_TASKS > 14
    #define __OS_TtimerWork14()    __OS_TtimerWork(14)
    #else
    #define __OS_TtimerWork14()
    #endif
    //...........................................................
    #if OS_TASKS > 15
    #define __OS_TtimerWork15()    __OS_TtimerWork(15)
    #else
    #define __OS_TtimerWork15()
    #endif
    //...........................................................
    #if OS_TASKS > 16
    #define __OS_TtimerWork16()    __OS_TtimerWork(16)
    #else
    #define __OS_TtimerWork16()
    #endif
    //...........................................................
    #if OS_TASKS > 17
    #define __OS_TtimerWork17()    __OS_TtimerWork(17)
    #else
    #define __OS_TtimerWork17()
    #endif
    //...........................................................
    #if OS_TASKS > 18
    #define __OS_TtimerWork18()    __OS_TtimerWork(18)
    #else
    #define __OS_TtimerWork18()
    #endif
    //...........................................................
    #if OS_TASKS > 19
    #define __OS_TtimerWork19()    __OS_TtimerWork(19)
    #else
    #define __OS_TtimerWork19()
    #endif
    //...........................................................
    #if OS_TASKS > 20
    #define __OS_TtimerWork20()    __OS_TtimerWork(20)
    #else
    #define __OS_TtimerWork20()
    #endif
    //...........................................................
    #if OS_TASKS > 21
    #define __OS_TtimerWork21()    __OS_TtimerWork(21)
    #else
    #define __OS_TtimerWork21()
    #endif
    //...........................................................
    #if OS_TASKS > 22
    #define __OS_TtimerWork22()    __OS_TtimerWork(22)
    #else
    #define __OS_TtimerWork22()
    #endif
    //...........................................................
    #if OS_TASKS > 23
    #define __OS_TtimerWork23()    __OS_TtimerWork(23)
    #else
    #define __OS_TtimerWork23()
    #endif
    //...........................................................
    #if OS_TASKS > 24
    #define __OS_TtimerWork24()    __OS_TtimerWork(24)
    #else
    #define __OS_TtimerWork24()
    #endif
    //...........................................................
    #if OS_TASKS > 25
    #define __OS_TtimerWork25()    __OS_TtimerWork(25)
    #else
    #define __OS_TtimerWork25()
    #endif
    //...........................................................
    #if OS_TASKS > 26
    #define __OS_TtimerWork26()    __OS_TtimerWork(26)
    #else
    #define __OS_TtimerWork26()
    #endif
    //...........................................................
    #if OS_TASKS > 27
    #define __OS_TtimerWork27()    __OS_TtimerWork(27)
    #else
    #define __OS_TtimerWork27()
    #endif
    //...........................................................
    #if OS_TASKS > 28
    #define __OS_TtimerWork28()    __OS_TtimerWork(28)
    #else
    #define __OS_TtimerWork28()
    #endif
    //...........................................................
    #if OS_TASKS > 29
    #define __OS_TtimerWork29()    __OS_TtimerWork(29)
    #else
    #define __OS_TtimerWork29()
    #endif
    //...........................................................
    #if OS_TASKS > 30
    #define __OS_TtimerWork30()    __OS_TtimerWork(30)
    #else
    #define __OS_TtimerWork30()
    #endif
    //...........................................................
    #if OS_TASKS > 31
    #define __OS_TtimerWork31()    __OS_TtimerWork(31)
    #else
    #define __OS_TtimerWork31()
    #endif
    //...........................................................
    #if OS_TASKS > 32
    #define __OS_TtimerWork32()    __OS_TtimerWork(32)
    #else
    #define __OS_TtimerWork32()
    #endif
    //...........................................................
    #if OS_TASKS > 33
    #define __OS_TtimerWork33()    __OS_TtimerWork(33)
    #else
    #define __OS_TtimerWork33()
    #endif
    //...........................................................
    #if OS_TASKS > 34
    #define __OS_TtimerWork34()    __OS_TtimerWork(34)
    #else
    #define __OS_TtimerWork34()
    #endif
    //...........................................................
    #if OS_TASKS > 35
    #define __OS_TtimerWork35()    __OS_TtimerWork(35)
    #else
    #define __OS_TtimerWork35()
    #endif
    //...........................................................
    #if OS_TASKS > 36
    #define __OS_TtimerWork36()    __OS_TtimerWork(36)
    #else
    #define __OS_TtimerWork36()
    #endif
    //...........................................................
    #if OS_TASKS > 37
    #define __OS_TtimerWork37()    __OS_TtimerWork(37)
    #else
    #define __OS_TtimerWork37()
    #endif
    //...........................................................
    #if OS_TASKS > 38
    #define __OS_TtimerWork38()    __OS_TtimerWork(38)
    #else
    #define __OS_TtimerWork38()
    #endif
    //...........................................................
    #if OS_TASKS > 39
    #define __OS_TtimerWork39()    __OS_TtimerWork(39)
    #else
    #define __OS_TtimerWork39()
    #endif
    //...........................................................
    #if OS_TASKS > 40
    #define __OS_TtimerWork40()    __OS_TtimerWork(40)
    #else
    #define __OS_TtimerWork40()
    #endif
    //...........................................................
    #if OS_TASKS > 41
    #define __OS_TtimerWork41()    __OS_TtimerWork(41)
    #else
    #define __OS_TtimerWork41()
    #endif
    //...........................................................
    #if OS_TASKS > 42
    #define __OS_TtimerWork42()    __OS_TtimerWork(42)
    #else
    #define __OS_TtimerWork42()
    #endif
    //...........................................................
    #if OS_TASKS > 43
    #define __OS_TtimerWork43()    __OS_TtimerWork(43)
    #else
    #define __OS_TtimerWork43()
    #endif
    //...........................................................
    #if OS_TASKS > 44
    #define __OS_TtimerWork44()    __OS_TtimerWork(44)
    #else
    #define __OS_TtimerWork44()
    #endif
    //...........................................................
    #if OS_TASKS > 45
    #define __OS_TtimerWork45()    __OS_TtimerWork(45)
    #else
    #define __OS_TtimerWork45()
    #endif
    //...........................................................
    #if OS_TASKS > 46
    #define __OS_TtimerWork46()    __OS_TtimerWork(46)
    #else
    #define __OS_TtimerWork46()
    #endif
    //...........................................................
    #if OS_TASKS > 47
    #define __OS_TtimerWork47()    __OS_TtimerWork(47)
    #else
    #define __OS_TtimerWork47()
    #endif
    //...........................................................
    #if OS_TASKS > 48
    #define __OS_TtimerWork48()    __OS_TtimerWork(48)
    #else
    #define __OS_TtimerWork48()
    #endif
    //...........................................................
    #if OS_TASKS > 49
    #define __OS_TtimerWork49()    __OS_TtimerWork(49)
    #else
    #define __OS_TtimerWork49()
    #endif
    //...........................................................
    #if OS_TASKS > 50
    #define __OS_TtimerWork50()    __OS_TtimerWork(50)
    #else
    #define __OS_TtimerWork50()
    #endif
    //...........................................................
    #if OS_TASKS > 51
    #define __OS_TtimerWork51()    __OS_TtimerWork(51)
    #else
    #define __OS_TtimerWork51()
    #endif
    //...........................................................
    #if OS_TASKS > 52
    #define __OS_TtimerWork52()    __OS_TtimerWork(52)
    #else
    #define __OS_TtimerWork52()
    #endif
    //...........................................................
    #if OS_TASKS > 53
    #define __OS_TtimerWork53()    __OS_TtimerWork(53)
    #else
    #define __OS_TtimerWork53()
    #endif
    //...........................................................
    #if OS_TASKS > 54
    #define __OS_TtimerWork54()    __OS_TtimerWork(54)
    #else
    #define __OS_TtimerWork54()
    #endif
    //...........................................................
    #if OS_TASKS > 55
    #define __OS_TtimerWork55()    __OS_TtimerWork(55)
    #else
    #define __OS_TtimerWork55()
    #endif
    //...........................................................
    #if OS_TASKS > 56
    #define __OS_TtimerWork56()    __OS_TtimerWork(56)
    #else
    #define __OS_TtimerWork56()
    #endif
    //...........................................................
    #if OS_TASKS > 57
    #define __OS_TtimerWork57()    __OS_TtimerWork(57)
    #else
    #define __OS_TtimerWork57()
    #endif
    //...........................................................
    #if OS_TASKS > 58
    #define __OS_TtimerWork58()    __OS_TtimerWork(58)
    #else
    #define __OS_TtimerWork58()
    #endif
    //...........................................................
    #if OS_TASKS > 59
    #define __OS_TtimerWork59()    __OS_TtimerWork(59)
    #else
    #define __OS_TtimerWork59()
    #endif
    //...........................................................
    #if OS_TASKS > 60
    #define __OS_TtimerWork60()    __OS_TtimerWork(60)
    #else
    #define __OS_TtimerWork60()
    #endif
    //...........................................................
    #if OS_TASKS > 61
    #define __OS_TtimerWork61()    __OS_TtimerWork(61)
    #else
    #define __OS_TtimerWork61()
    #endif
    //...........................................................
    #if OS_TASKS > 62
    #define __OS_TtimerWork62()    __OS_TtimerWork(62)
    #else
    #define __OS_TtimerWork62()
    #endif
    //...........................................................
    #if OS_TASKS > 63
    #define __OS_TtimerWork63()    __OS_TtimerWork(63)
    #else
    #define __OS_TtimerWork63()
    #endif
    //...........................................................


    #define __OS_TtimersWorkSpeed()      \
    {                                    \
        ASM_OST_TCB_SIZE_CONST;          \
        ASM_SET_BANK;                    \
        __OS_TtimerWork0();         \
        __OS_TtimerWork1();         \
        __OS_TtimerWork2();         \
        __OS_TtimerWork3();         \
        __OS_TtimerWork4();         \
        __OS_TtimerWork5();         \
        __OS_TtimerWork6();         \
        __OS_TtimerWork7();         \
        __OS_TtimerWork8();         \
        __OS_TtimerWork9();         \
        __OS_TtimerWork10();        \
        __OS_TtimerWork11();        \
        __OS_TtimerWork12();        \
        __OS_TtimerWork13();        \
        __OS_TtimerWork14();        \
        __OS_TtimerWork15();        \
        __OS_TtimerWork16();        \
        __OS_TtimerWork17();        \
        __OS_TtimerWork18();        \
        __OS_TtimerWork19();        \
        __OS_TtimerWork20();        \
        __OS_TtimerWork21();        \
        __OS_TtimerWork22();        \
        __OS_TtimerWork23();        \
        __OS_TtimerWork24();        \
        __OS_TtimerWork25();        \
        __OS_TtimerWork26();        \
        __OS_TtimerWork27();        \
        __OS_TtimerWork28();        \
        __OS_TtimerWork29();        \
        __OS_TtimerWork30();        \
        __OS_TtimerWork31();        \
        __OS_TtimerWork32();        \
        __OS_TtimerWork33();        \
        __OS_TtimerWork34();        \
        __OS_TtimerWork35();        \
        __OS_TtimerWork36();        \
        __OS_TtimerWork37();        \
        __OS_TtimerWork38();        \
        __OS_TtimerWork39();        \
        __OS_TtimerWork40();        \
        __OS_TtimerWork41();        \
        __OS_TtimerWork42();        \
        __OS_TtimerWork43();        \
        __OS_TtimerWork44();        \
        __OS_TtimerWork45();        \
        __OS_TtimerWork46();        \
        __OS_TtimerWork47();        \
        __OS_TtimerWork48();        \
        __OS_TtimerWork49();        \
        __OS_TtimerWork50();        \
        __OS_TtimerWork51();        \
        __OS_TtimerWork52();        \
        __OS_TtimerWork53();        \
        __OS_TtimerWork54();        \
        __OS_TtimerWork55();        \
        __OS_TtimerWork56();        \
        __OS_TtimerWork57();        \
        __OS_TtimerWork58();        \
        __OS_TtimerWork59();        \
        __OS_TtimerWork60();        \
        __OS_TtimerWork61();        \
        __OS_TtimerWork62();        \
        __OS_TtimerWork63();        \
    }

    #ifndef __OS_TtimersWorkSize

        #define __OS_TtimersWorkSize()                                                      \
        {                                                                                   \
            OST_UINT _os_i;                                                            \
            ASM_OST_TCB_SIZE_CONST;                                                         \
            ASM_SET_BANK;                                                                   \
            for (_os_i = 0; _os_i < OS_TASKS; _os_i++)                                      \
            {                                                                               \
                if (_OS_Tasks[_os_i].State.bDelay){                                       \
                    if (!++_OS_Tasks[_os_i].Timer) _OS_Tasks[_os_i].State.bDelay = 0;   \
                }                                                                           \
            }                                                                               \
        }
    #endif

#else


    #ifndef __OS_TtimersWorkSize
        #define __OS_TtimersWorkSize()
    #endif
    #define __OS_TtimersWorkSpeed()

#endif





/************************************************************************/
/*                                                                      */
/************************************************************************/

//------------------------------------------------------------------------------
#ifdef OS_ENABLE_STIMERS
//------------------------------------------------------------------------------


    //...........................................................
    #if OS_STIMERS > 0
    #define __OS_StimerWork0()    __OS_StimerWork(0)
    #else
    #define __OS_StimerWork0()
    #endif
    //...........................................................
    #if OS_STIMERS > 1
    #define __OS_StimerWork1()    __OS_StimerWork(1)
    #else
    #define __OS_StimerWork1()
    #endif
    //...........................................................
    #if OS_STIMERS > 2
    #define __OS_StimerWork2()    __OS_StimerWork(2)
    #else
    #define __OS_StimerWork2()
    #endif
    //...........................................................
    #if OS_STIMERS > 3
    #define __OS_StimerWork3()    __OS_StimerWork(3)
    #else
    #define __OS_StimerWork3()
    #endif
    //...........................................................
    #if OS_STIMERS > 4
    #define __OS_StimerWork4()    __OS_StimerWork(4)
    #else
    #define __OS_StimerWork4()
    #endif
    //...........................................................
    #if OS_STIMERS > 5
    #define __OS_StimerWork5()    __OS_StimerWork(5)
    #else
    #define __OS_StimerWork5()
    #endif
    //...........................................................
    #if OS_STIMERS > 6
    #define __OS_StimerWork6()    __OS_StimerWork(6)
    #else
    #define __OS_StimerWork6()
    #endif
    //...........................................................
    #if OS_STIMERS > 7
    #define __OS_StimerWork7()    __OS_StimerWork(7)
    #else
    #define __OS_StimerWork7()
    #endif
    //...........................................................
    #if OS_STIMERS > 8
    #define __OS_StimerWork8()    __OS_StimerWork(8)
    #else
    #define __OS_StimerWork8()
    #endif
    //...........................................................
    #if OS_STIMERS > 9
    #define __OS_StimerWork9()    __OS_StimerWork(9)
    #else
    #define __OS_StimerWork9()
    #endif
    //...........................................................
    #if OS_STIMERS > 10
    #define __OS_StimerWork10()    __OS_StimerWork(10)
    #else
    #define __OS_StimerWork10()
    #endif
    //...........................................................
    #if OS_STIMERS > 11
    #define __OS_StimerWork11()    __OS_StimerWork(11)
    #else
    #define __OS_StimerWork11()
    #endif
    //...........................................................
    #if OS_STIMERS > 12
    #define __OS_StimerWork12()    __OS_StimerWork(12)
    #else
    #define __OS_StimerWork12()
    #endif
    //...........................................................
    #if OS_STIMERS > 13
    #define __OS_StimerWork13()    __OS_StimerWork(13)
    #else
    #define __OS_StimerWork13()
    #endif
    //...........................................................
    #if OS_STIMERS > 14
    #define __OS_StimerWork14()    __OS_StimerWork(14)
    #else
    #define __OS_StimerWork14()
    #endif
    //...........................................................
    #if OS_STIMERS > 15
    #define __OS_StimerWork15()    __OS_StimerWork(15)
    #else
    #define __OS_StimerWork15()
    #endif
    //...........................................................
    #if OS_STIMERS > 16
    #define __OS_StimerWork16()    __OS_StimerWork(16)
    #else
    #define __OS_StimerWork16()
    #endif
    //...........................................................
    #if OS_STIMERS > 17
    #define __OS_StimerWork17()    __OS_StimerWork(17)
    #else
    #define __OS_StimerWork17()
    #endif
    //...........................................................
    #if OS_STIMERS > 18
    #define __OS_StimerWork18()    __OS_StimerWork(18)
    #else
    #define __OS_StimerWork18()
    #endif
    //...........................................................
    #if OS_STIMERS > 19
    #define __OS_StimerWork19()    __OS_StimerWork(19)
    #else
    #define __OS_StimerWork19()
    #endif
    //...........................................................
    #if OS_STIMERS > 20
    #define __OS_StimerWork20()    __OS_StimerWork(20)
    #else
    #define __OS_StimerWork20()
    #endif
    //...........................................................
    #if OS_STIMERS > 21
    #define __OS_StimerWork21()    __OS_StimerWork(21)
    #else
    #define __OS_StimerWork21()
    #endif
    //...........................................................
    #if OS_STIMERS > 22
    #define __OS_StimerWork22()    __OS_StimerWork(22)
    #else
    #define __OS_StimerWork22()
    #endif
    //...........................................................
    #if OS_STIMERS > 23
    #define __OS_StimerWork23()    __OS_StimerWork(23)
    #else
    #define __OS_StimerWork23()
    #endif
    //...........................................................
    #if OS_STIMERS > 24
    #define __OS_StimerWork24()    __OS_StimerWork(24)
    #else
    #define __OS_StimerWork24()
    #endif
    //...........................................................
    #if OS_STIMERS > 25
    #define __OS_StimerWork25()    __OS_StimerWork(25)
    #else
    #define __OS_StimerWork25()
    #endif
    //...........................................................
    #if OS_STIMERS > 26
    #define __OS_StimerWork26()    __OS_StimerWork(26)
    #else
    #define __OS_StimerWork26()
    #endif
    //...........................................................
    #if OS_STIMERS > 27
    #define __OS_StimerWork27()    __OS_StimerWork(27)
    #else
    #define __OS_StimerWork27()
    #endif
    //...........................................................
    #if OS_STIMERS > 28
    #define __OS_StimerWork28()    __OS_StimerWork(28)
    #else
    #define __OS_StimerWork28()
    #endif
    //...........................................................
    #if OS_STIMERS > 29
    #define __OS_StimerWork29()    __OS_StimerWork(29)
    #else
    #define __OS_StimerWork29()
    #endif
    //...........................................................
    #if OS_STIMERS > 30
    #define __OS_StimerWork30()    __OS_StimerWork(30)
    #else
    #define __OS_StimerWork30()
    #endif
    //...........................................................
    #if OS_STIMERS > 31
    #define __OS_StimerWork31()    __OS_StimerWork(31)
    #else
    #define __OS_StimerWork31()
    #endif
    //...........................................................
    #if OS_STIMERS > 32
    #define __OS_StimerWork32()    __OS_StimerWork(32)
    #else
    #define __OS_StimerWork32()
    #endif
    //...........................................................
    #if OS_STIMERS > 33
    #define __OS_StimerWork33()    __OS_StimerWork(33)
    #else
    #define __OS_StimerWork33()
    #endif
    //...........................................................
    #if OS_STIMERS > 34
    #define __OS_StimerWork34()    __OS_StimerWork(34)
    #else
    #define __OS_StimerWork34()
    #endif
    //...........................................................
    #if OS_STIMERS > 35
    #define __OS_StimerWork35()    __OS_StimerWork(35)
    #else
    #define __OS_StimerWork35()
    #endif
    //...........................................................
    #if OS_STIMERS > 36
    #define __OS_StimerWork36()    __OS_StimerWork(36)
    #else
    #define __OS_StimerWork36()
    #endif
    //...........................................................
    #if OS_STIMERS > 37
    #define __OS_StimerWork37()    __OS_StimerWork(37)
    #else
    #define __OS_StimerWork37()
    #endif
    //...........................................................
    #if OS_STIMERS > 38
    #define __OS_StimerWork38()    __OS_StimerWork(38)
    #else
    #define __OS_StimerWork38()
    #endif
    //...........................................................
    #if OS_STIMERS > 39
    #define __OS_StimerWork39()    __OS_StimerWork(39)
    #else
    #define __OS_StimerWork39()
    #endif
    //...........................................................
    #if OS_STIMERS > 40
    #define __OS_StimerWork40()    __OS_StimerWork(40)
    #else
    #define __OS_StimerWork40()
    #endif
    //...........................................................
    #if OS_STIMERS > 41
    #define __OS_StimerWork41()    __OS_StimerWork(41)
    #else
    #define __OS_StimerWork41()
    #endif
    //...........................................................
    #if OS_STIMERS > 42
    #define __OS_StimerWork42()    __OS_StimerWork(42)
    #else
    #define __OS_StimerWork42()
    #endif
    //...........................................................
    #if OS_STIMERS > 43
    #define __OS_StimerWork43()    __OS_StimerWork(43)
    #else
    #define __OS_StimerWork43()
    #endif
    //...........................................................
    #if OS_STIMERS > 44
    #define __OS_StimerWork44()    __OS_StimerWork(44)
    #else
    #define __OS_StimerWork44()
    #endif
    //...........................................................
    #if OS_STIMERS > 45
    #define __OS_StimerWork45()    __OS_StimerWork(45)
    #else
    #define __OS_StimerWork45()
    #endif
    //...........................................................
    #if OS_STIMERS > 46
    #define __OS_StimerWork46()    __OS_StimerWork(46)
    #else
    #define __OS_StimerWork46()
    #endif
    //...........................................................
    #if OS_STIMERS > 47
    #define __OS_StimerWork47()    __OS_StimerWork(47)
    #else
    #define __OS_StimerWork47()
    #endif
    //...........................................................
    #if OS_STIMERS > 48
    #define __OS_StimerWork48()    __OS_StimerWork(48)
    #else
    #define __OS_StimerWork48()
    #endif
    //...........................................................
    #if OS_STIMERS > 49
    #define __OS_StimerWork49()    __OS_StimerWork(49)
    #else
    #define __OS_StimerWork49()
    #endif
    //...........................................................
    #if OS_STIMERS > 50
    #define __OS_StimerWork50()    __OS_StimerWork(50)
    #else
    #define __OS_StimerWork50()
    #endif
    //...........................................................
    #if OS_STIMERS > 51
    #define __OS_StimerWork51()    __OS_StimerWork(51)
    #else
    #define __OS_StimerWork51()
    #endif
    //...........................................................
    #if OS_STIMERS > 52
    #define __OS_StimerWork52()    __OS_StimerWork(52)
    #else
    #define __OS_StimerWork52()
    #endif
    //...........................................................
    #if OS_STIMERS > 53
    #define __OS_StimerWork53()    __OS_StimerWork(53)
    #else
    #define __OS_StimerWork53()
    #endif
    //...........................................................
    #if OS_STIMERS > 54
    #define __OS_StimerWork54()    __OS_StimerWork(54)
    #else
    #define __OS_StimerWork54()
    #endif
    //...........................................................
    #if OS_STIMERS > 55
    #define __OS_StimerWork55()    __OS_StimerWork(55)
    #else
    #define __OS_StimerWork55()
    #endif
    //...........................................................
    #if OS_STIMERS > 56
    #define __OS_StimerWork56()    __OS_StimerWork(56)
    #else
    #define __OS_StimerWork56()
    #endif
    //...........................................................
    #if OS_STIMERS > 57
    #define __OS_StimerWork57()    __OS_StimerWork(57)
    #else
    #define __OS_StimerWork57()
    #endif
    //...........................................................
    #if OS_STIMERS > 58
    #define __OS_StimerWork58()    __OS_StimerWork(58)
    #else
    #define __OS_StimerWork58()
    #endif
    //...........................................................
    #if OS_STIMERS > 59
    #define __OS_StimerWork59()    __OS_StimerWork(59)
    #else
    #define __OS_StimerWork59()
    #endif
    //...........................................................
    #if OS_STIMERS > 60
    #define __OS_StimerWork60()    __OS_StimerWork(60)
    #else
    #define __OS_StimerWork60()
    #endif
    //...........................................................
    #if OS_STIMERS > 61
    #define __OS_StimerWork61()    __OS_StimerWork(61)
    #else
    #define __OS_StimerWork61()
    #endif
    //...........................................................
    #if OS_STIMERS > 62
    #define __OS_StimerWork62()    __OS_StimerWork(62)
    #else
    #define __OS_StimerWork62()
    #endif
    //...........................................................
    #if OS_STIMERS > 63
    #define __OS_StimerWork63()    __OS_StimerWork(63)
    #else
    #define __OS_StimerWork63()
    #endif
    //...........................................................

    #ifndef __OS_StimerBank
    #define __OS_StimerBank()
    #endif


    #define __OS_StimersWorkSpeed()    \
    {                                  \
        __OS_StimerBank();             \
        __OS_StimerWork0();            \
        __OS_StimerWork1();            \
        __OS_StimerWork2();            \
        __OS_StimerWork3();            \
        __OS_StimerWork4();            \
        __OS_StimerWork5();            \
        __OS_StimerWork6();            \
        __OS_StimerWork7();            \
        __OS_StimerWork8();            \
        __OS_StimerWork9();            \
        __OS_StimerWork10();           \
        __OS_StimerWork11();           \
        __OS_StimerWork12();           \
        __OS_StimerWork13();           \
        __OS_StimerWork14();           \
        __OS_StimerWork15();           \
        __OS_StimerWork16();           \
        __OS_StimerWork17();           \
        __OS_StimerWork18();           \
        __OS_StimerWork19();           \
        __OS_StimerWork20();           \
        __OS_StimerWork21();           \
        __OS_StimerWork22();           \
        __OS_StimerWork23();           \
        __OS_StimerWork24();           \
        __OS_StimerWork25();           \
        __OS_StimerWork26();           \
        __OS_StimerWork27();           \
        __OS_StimerWork28();           \
        __OS_StimerWork29();           \
        __OS_StimerWork30();           \
        __OS_StimerWork31();           \
        __OS_StimerWork32();           \
        __OS_StimerWork33();           \
        __OS_StimerWork34();           \
        __OS_StimerWork35();           \
        __OS_StimerWork36();           \
        __OS_StimerWork37();           \
        __OS_StimerWork38();           \
        __OS_StimerWork39();           \
        __OS_StimerWork40();           \
        __OS_StimerWork41();           \
        __OS_StimerWork42();           \
        __OS_StimerWork43();           \
        __OS_StimerWork44();           \
        __OS_StimerWork45();           \
        __OS_StimerWork46();           \
        __OS_StimerWork47();           \
        __OS_StimerWork48();           \
        __OS_StimerWork49();           \
        __OS_StimerWork50();           \
        __OS_StimerWork51();           \
        __OS_StimerWork52();           \
        __OS_StimerWork53();           \
        __OS_StimerWork54();           \
        __OS_StimerWork55();           \
        __OS_StimerWork56();           \
        __OS_StimerWork57();           \
        __OS_StimerWork58();           \
        __OS_StimerWork59();           \
        __OS_StimerWork60();           \
        __OS_StimerWork61();           \
        __OS_StimerWork62();           \
        __OS_StimerWork63();           \
    }


    #ifndef __OS_StimersWorkSize

        #define __OS_StimersWorkSize()                                          \
        {                                                                       \
            OST_UINT _os_i;                                                \
            for (_os_i = 0; _os_i < OS_STIMERS; _os_i++)                        \
            {                                                                   \
                if (_OS_Stimers[_os_i] & OS_STIMER_RUN_BIT) _OS_Stimers[_os_i]++;   \
            }                                                                   \
        }

    #endif

#else

    #define __OS_StimersWorkSpeed()
    #ifndef __OS_StimersWorkSize
        #define __OS_StimersWorkSize()
    #endif

//------------------------------------------------------------------------------
#endif  // OS_ENABLE_STIMERS
//------------------------------------------------------------------------------





//______________________________________________________________________________
//******************************************************************************


#define OS_TimerInline()    __OS_TimerInline()


#if defined(OS_SLOW_TIMERS) || OS_TIMERS24 > 0
#define _OS_IncOSTicks()    OS_Ticks++;
#else
#define _OS_IncOSTicks()
#endif





/************************************************************************************************
 *                                                                                              *
 *     S Y S T E M   T I M E R   W O R K   M A C R O                                            *
 *                                                                                              *
 ************************************************************************************************/


#define __OS_OldTimersWork()               \
{                                          \
    _OS_IncOSTicks();                      \
                                           \
    __OS_Timer8Work0();                    \
    __OS_Timer8Work1();                    \
    __OS_Timer8Work2();                    \
    __OS_Timer8Work3();                    \
    __OS_Timer8Work4();                    \
    __OS_Timer8Work5();                    \
    __OS_Timer8Work6();                    \
    __OS_Timer8Work7();                    \
    __OS_Timer8Work8();                    \
    __OS_Timer8Work9();                    \
    __OS_Timer8Work10();                   \
    __OS_Timer8Work11();                   \
    __OS_Timer8Work12();                   \
    __OS_Timer8Work13();                   \
    __OS_Timer8Work14();                   \
    __OS_Timer8Work15();                   \
    __OS_Timer8Work16();                   \
    __OS_Timer8Work17();                   \
    __OS_Timer8Work18();                   \
    __OS_Timer8Work19();                   \
    __OS_Timer8Work20();                   \
    __OS_Timer8Work21();                   \
    __OS_Timer8Work22();                   \
    __OS_Timer8Work23();                   \
    __OS_Timer8Work24();                   \
    __OS_Timer8Work25();                   \
    __OS_Timer8Work26();                   \
    __OS_Timer8Work27();                   \
    __OS_Timer8Work28();                   \
    __OS_Timer8Work29();                   \
    __OS_Timer8Work30();                   \
    __OS_Timer8Work31();                   \
                                           \
    __OS_Timer16Work0();                   \
    __OS_Timer16Work1();                   \
    __OS_Timer16Work2();                   \
    __OS_Timer16Work3();                   \
    __OS_Timer16Work4();                   \
    __OS_Timer16Work5();                   \
    __OS_Timer16Work6();                   \
    __OS_Timer16Work7();                   \
    __OS_Timer16Work8();                   \
    __OS_Timer16Work9();                   \
    __OS_Timer16Work10();                  \
    __OS_Timer16Work11();                  \
    __OS_Timer16Work12();                  \
    __OS_Timer16Work13();                  \
    __OS_Timer16Work14();                  \
    __OS_Timer16Work15();                  \
    __OS_Timer16Work16();                  \
    __OS_Timer16Work17();                  \
    __OS_Timer16Work18();                  \
    __OS_Timer16Work19();                  \
    __OS_Timer16Work20();                  \
    __OS_Timer16Work21();                  \
    __OS_Timer16Work22();                  \
    __OS_Timer16Work23();                  \
    __OS_Timer16Work24();                  \
    __OS_Timer16Work25();                  \
    __OS_Timer16Work26();                  \
    __OS_Timer16Work27();                  \
    __OS_Timer16Work28();                  \
    __OS_Timer16Work29();                  \
    __OS_Timer16Work30();                  \
    __OS_Timer16Work31();                  \
                                           \
    __OS_Timer32Work0();                   \
    __OS_Timer32Work1();                   \
    __OS_Timer32Work2();                   \
    __OS_Timer32Work3();                   \
    __OS_Timer32Work4();                   \
    __OS_Timer32Work5();                   \
    __OS_Timer32Work6();                   \
    __OS_Timer32Work7();                   \
    __OS_Timer32Work8();                   \
    __OS_Timer32Work9();                   \
    __OS_Timer32Work10();                  \
    __OS_Timer32Work11();                  \
    __OS_Timer32Work12();                  \
    __OS_Timer32Work13();                  \
    __OS_Timer32Work14();                  \
    __OS_Timer32Work15();                  \
    __OS_Timer32Work16();                  \
    __OS_Timer32Work17();                  \
    __OS_Timer32Work18();                  \
    __OS_Timer32Work19();                  \
    __OS_Timer32Work20();                  \
    __OS_Timer32Work21();                  \
    __OS_Timer32Work22();                  \
    __OS_Timer32Work23();                  \
    __OS_Timer32Work24();                  \
    __OS_Timer32Work25();                  \
    __OS_Timer32Work26();                  \
    __OS_Timer32Work27();                  \
    __OS_Timer32Work28();                  \
    __OS_Timer32Work29();                  \
    __OS_Timer32Work30();                  \
    __OS_Timer32Work31();                  \
                                           \
    __OS_CheckInc24()                      \
    {                                      \
        __OS_Timer24Work0();               \
        __OS_Timer24Work1();               \
        __OS_Timer24Work2();               \
        __OS_Timer24Work3();               \
        __OS_Timer24Work4();               \
        __OS_Timer24Work5();               \
        __OS_Timer24Work6();               \
        __OS_Timer24Work7();               \
        __OS_Timer24Work8();               \
        __OS_Timer24Work9();               \
        __OS_Timer24Work10();              \
        __OS_Timer24Work11();              \
        __OS_Timer24Work12();              \
        __OS_Timer24Work13();              \
        __OS_Timer24Work14();              \
        __OS_Timer24Work15();              \
        __OS_Timer24Work16();              \
        __OS_Timer24Work17();              \
        __OS_Timer24Work18();              \
        __OS_Timer24Work19();              \
        __OS_Timer24Work20();              \
        __OS_Timer24Work21();              \
        __OS_Timer24Work22();              \
        __OS_Timer24Work23();              \
        __OS_Timer24Work24();              \
        __OS_Timer24Work25();              \
        __OS_Timer24Work26();              \
        __OS_Timer24Work27();              \
        __OS_Timer24Work28();              \
        __OS_Timer24Work29();              \
        __OS_Timer24Work30();              \
        __OS_Timer24Work31();              \
    }                                      \
}



#define OS_OldTimer()           __OS_OldTimersWork()

#ifdef OS_TTIMERS_OPTIMIZE_SIZE
    #define OS_Ttimer()             __OS_TtimersWorkSize()
#else
    #define OS_Ttimer()             __OS_TtimersWorkSpeed()
#endif

#ifdef OS_STIMERS_OPTIMIZE_SIZE
    #define OS_Stimer()             __OS_StimersWorkSize()
#else
    #define OS_Stimer()             __OS_StimersWorkSpeed()
#endif


#define OS_Dtimer()                 __OS_DtimersWork()
#define OS_Qtimer()                 __OS_QtimersWork()



#define __OS_TimerInline()          \
{                                   \
    OS_OldTimer();                  \
    OS_Ttimer();                    \
    OS_Stimer();                    \
    OS_Dtimer();                    \
    OS_Qtimer();                    \
}





//------------------------------------------------------------------------------
#endif      // _OS_TIMER_H_
//------------------------------------------------------------------------------



















