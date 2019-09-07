/*
 ***********************************************************************************************
 *                                                                                              
 *  OSA cooperative RTOS for microcontrollers PIC (Microchip) and 8-bit AVR (Atmel)
 *                                                                                              
 *  OSA is distributed under BSD license (see license.txt)                                      
 *                                                                                              
 *  URL:        http://wiki.pic24.ru/doku.php/en/osa/ref/intro                                  
 *              http://picosa.narod.ru                                                          
 *                                                                                              
 *----------------------------------------------------------------------------------------------
 *                                                                                              
 *  File:       osa_stimer_old.h                                                                
 *                                                                                              
 *  Programmer: Timofeev Victor                                                                 
 *              osa@pic24.ru, testerplus@mail.ru                                                
 *                                                                                              
 *  Definition: Services with old static services                                               
 *                                                                                              
 *  History:    21.01.2009                                                                      
 *                                                                                              
 *  (not recommended to use. use STimers instead (see osa_stimers.h)                            
 *                                                                                              
 *                                                                                              
 ***********************************************************************************************
 */



/************************************************************************************************
 *                                                                                              *
 *                           O L D   S T Y L E   S T A T I C   T I M E R S                      *
 *                                                                                              *
 ************************************************************************************************/


//******************************************************************************
//  VARIABLES
//******************************************************************************

#if OS_TIMERS > 0
extern  OS_TIMEOUTS_BANK    OST_UINT      OS_Timeouts[];
#endif

#if OS_TIMERS32 > 0
extern  OS_TIMERS32_BANK    OST_UINT32    OS_Timers32[];
#endif

#if OS_TIMERS24 > 0
extern  OS_TIMERS24_BANK    OST_UINT16    OS_Timers24[];
#endif

#if OS_TIMERS16 > 0
extern  OS_TIMERS16_BANK    OST_UINT16    OS_Timers16[];
#endif

#if OS_TIMERS8  > 0
extern  OS_TIMERS8_BANK     OST_UINT8     OS_Timers8[];
#endif

//------------------------------------------------------------------------------
#if OS_TIMERS24 > 0
extern volatile OS_BANK OST_UINT    OS_Ticks;   // Prescaler for 24-bit old style static timers
#endif
//------------------------------------------------------------------------------


//******************************************************************************
//  FUNCTION PROTOTYPES
//******************************************************************************


//******************************************************************************
//  MACROS
//******************************************************************************


#define _OS_TMR_POS(timer_id)   ((timer_id) >> _OST_INT_SHIFT)
#define _OS_TMR_MASK(timer_id)  (1 << ((timer_id) & _OST_INT_MASK))

//------------------------------------------------------------------------------
// Stop timer

#define OS_Oldtimer_Pause(timer_id)     OS_Timeouts[_OS_TMR_POS(timer_id)] &= ~_OS_TMR_MASK(timer_id)   
#define OS_Oldtimer_Continue(timer_id)  OS_Timeouts[_OS_TMR_POS(timer_id)] |=  _OS_TMR_MASK(timer_id)   

//------------------------------------------------------------------------------
// Run timer (internal system macro)

#define _OS_RunTimer(timer_id)  OS_Timeouts[_OS_TMR_POS(timer_id)] |=  _OS_TMR_MASK(timer_id)

//------------------------------------------------------------------------------
// Check for overflow

#define OS_Oldtimer_Check(timer_id) !(OS_Timeouts[_OS_TMR_POS(timer_id)] & _OS_TMR_MASK(timer_id))

//------------------------------------------------------------------------------
// Check for timer is running

#define OS_IsTimerRun(timer_id) (OS_Timeouts[_OS_TMR_POS(timer_id)] & _OS_TMR_MASK(timer_id))


//------------------------------------------------------------------------------
// Run 8-bit timer

#define OS_Oldtimer_Run8(timer_id, time)                                    \
    {                                                                       \
        OS_Oldtimer_Pause(timer_id);                                         \
        OS_Timers8 [timer_id] = -(time);                                    \
        _OS_RunTimer(timer_id);                                             \
    }


//------------------------------------------------------------------------------
// Run 16-bit timer

#define OS_Oldtimer_Run16(timer_id, time)                                   \
    {                                                                       \
        OS_Oldtimer_Pause(timer_id);                                         \
        OS_Timers16[(timer_id) - _OS_TIMER16_POS] = -(time);                \
        _OS_RunTimer(timer_id);                                             \
    }

//------------------------------------------------------------------------------
// Run 24-bit timer

#define OS_Oldtimer_Run24(timer_id, time)                                   \
    {                                                                       \
        OS_Oldtimer_Pause(timer_id);                                         \
        OS_Timers24[(timer_id) - _OS_TIMER24_POS] = - (((time) + 128) >> 8);\
        _OS_RunTimer(timer_id);                                             \
    }

//------------------------------------------------------------------------------
// Run 24-bit timer withowt recounting prescaler (not recomended to use)

#define OS_Oldtimer_Run24prs(timer_id, time)                                \
    {                                                                       \
        OS_Oldtimer_Pause(timer_id);                                         \
        OS_Timers24[(timer_id) - _OS_TIMER24_POS] = -(time);                \
        _OS_RunTimer(timer_id);                                             \
    }

//------------------------------------------------------------------------------
// Run 32-bit timer

#define OS_Oldtimer_Run32(timer_id, time)                                   \
    {                                                                       \
        OS_Oldtimer_Pause(timer_id);                                         \
        OS_Timers32[(timer_id) - _OS_TIMER32_POS] = -(time);                \
        _OS_RunTimer(timer_id);                                             \
    }
//------------------------------------------------------------------------------
// Wait for timer overflowed

#define OS_Oldtimer_Wait(timer_id)                                          \
    {                                                                       \
        OS_Wait(OS_Oldtimer_Check(timer_id));                        \
    }

//------------------------------------------------------------------------------
// Delay current task using old style static timer
#define OS_DelayTimer8(timer_id, delay)                                     \
    {                                                                       \
        OS_Oldtimer_Run8(timer_id, delay);                                  \
        OS_Oldtimer_Wait(timer_id);                                         \
    }

//------------------------------------------------------------------------------
#define OS_DelayTimer16(timer_id, delay)                                    \
    {                                                                       \
        OS_Oldtimer_Run16(timer_id, delay);                                 \
        OS_Oldtimer_Wait(timer_id);                                         \
    }

//------------------------------------------------------------------------------
#define OS_DelayTimer24(timer_id, delay)                                    \
    {                                                                       \
        OS_Oldtimer_Run24(timer_id, delay);                                 \
        OS_Oldtimer_Wait(timer_id);                                         \
    }

//------------------------------------------------------------------------------
#define OS_DelayTimer32(timer_id, delay)                                    \
    {                                                                       \
        OS_Oldtimer_Run32(timer_id, delay);                                 \
        OS_Oldtimer_Wait(timer_id);                                         \
    }


















