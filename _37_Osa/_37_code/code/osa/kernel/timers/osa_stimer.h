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
 *  File:           osa_stimer.h
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    Services for work with static timers 
 *                  This file directly included in osa.c
 *
 *  History:        02.10.2010 -    File updated
 *
 ************************************************************************************************
 */



/************************************************************************************************
 *                                                                                              *
 *                         S T A T I C   T I M E R S   S E R V I C E S                          *
 *                                                                                              *
 ************************************************************************************************/



//------------------------------------------------------------------------------
#if OS_STIMERS > 0
//------------------------------------------------------------------------------

//******************************************************************************
//  VARIABLES
//******************************************************************************

extern  volatile OS_STIMERS_BANK     OS_STIMER_TYPE     _OS_Stimers[OS_STIMERS]    ;

#ifdef OS_STIMERS_ENABLE_ALLOCATION    
extern  OS_STIMERS_BANK     OST_UINT   _OS_StimersFree[(OS_STIMERS + _OST_INT_SIZE-1) / _OST_INT_SIZE];
#endif


//******************************************************************************
//  FUNCTION PROTOTYPES
//******************************************************************************

#ifdef OS_STIMERS_ENABLE_ALLOCATION

    OST_UINT8   _OS_Stimer_GetFree  (OST_UINT bCreate);
    void        _OS_Stimer_Free     (OST_UINT8 ID);

#endif

//******************************************************************************
//  MACROS
//******************************************************************************


//------------------------------------------------------------------------------

#if defined(OS_PROTECT_MEMORY_ACCESS)
    #define __OS_STIMER_DI_A()  _OS_DI_INT()
    #define __OS_STIMER_RI_A()  _OS_RI_INT()
#else
    #define __OS_STIMER_DI_A()
    #define __OS_STIMER_RI_A()
#endif


#define OS_STIMER_ATOMIC_WRITE_A(expr)  \
    OSM_BEGIN {                         \
        __OS_STIMER_DI_A();             \
        expr;                           \
        __OS_STIMER_RI_A();             \
    } OSM_END

//------------------------------------------------------------------------------



#if     OS_STIMER_SIZE == 1
    #define OS_STIMER_RUN_BIT   0x80
#elif   OS_STIMER_SIZE == 2
    #define OS_STIMER_RUN_BIT   0x8000
#elif   OS_STIMER_SIZE == 4
    #define OS_STIMER_RUN_BIT   0x80000000UL
#endif





#ifdef OS_STIMERS_ENABLE_ALLOCATION 

    #define OS_Stimer_Alloc(var_stimer_id)     var_stimer_id = _OS_Stimer_GetFree(1)
    #define OS_Stimer_Free(stimer_id)         _OS_Stimer_Free(stimer_id)

    #define OS_Stimer_Found()                   (_OS_Stimer_GetFree (0) != 0xFF)

#endif



/************************************************************************/
/* Check timer's states                                                 */
/************************************************************************/

#ifdef __OSA_PIC16_MIKROC__
    #define OS_Stimer_Check(stimer_id)   (!(*((char*)&_OS_Stimers[stimer_id]+(OS_STIMER_SIZE-1)) & 0x80))
    #define OS_Stimer_IsRun(stimer_id)     (*((char*)&_OS_Stimers[stimer_id]+(OS_STIMER_SIZE-1)) & 0x80)
#else
    #define OS_Stimer_Check(stimer_id)   (!(_OS_Stimers[stimer_id] & OS_STIMER_RUN_BIT))
    #define OS_Stimer_IsRun(stimer_id)     (_OS_Stimers[stimer_id] & OS_STIMER_RUN_BIT)
#endif


/************************************************************************/
/* Run/Break timer for counting                                         */
/************************************************************************/

#define OS_Stimer_Run(stimer_id, time)                                  \
    {                                                                   \
        OS_Stimer_Stop(stimer_id);                                      \
        _OS_Stimers[stimer_id] = -(OS_STIMER_TYPE)(time);               \
    }

    // At the end of this macro we can't set bit Run since before we do it,
    // flag can became clear in interrupt.

#define OS_Stimer_Break(stimer_id)   OS_STIMER_ATOMIC_WRITE_A(_OS_Stimers[stimer_id] = 0)   


/************************************************************************/
/* Pause/continue timer                                                 */
/************************************************************************/

#define OS_Stimer_Pause(stimer_id)      OS_STIMER_ATOMIC_WRITE_A(_OS_Stimers[stimer_id] &= ~OS_STIMER_RUN_BIT)
#define OS_Stimer_Continue(stimer_id)   OS_STIMER_ATOMIC_WRITE_A(_OS_Stimers[stimer_id] |= OS_STIMER_RUN_BIT)


/************************************************************************/
/* Get remaining time of counting                                       */
/* It is nessesary to disable interrupt ehile reading STimer value      */
/************************************************************************/

#define OS_Stimer_Get(stimer_id)    (OS_Stimer_Check(stimer_id)? (_OS_Stimers[stimer_id]):(-_OS_Stimers[stimer_id]))


/************************************************************************/
/* Continue counting after pause                                        */
/************************************************************************/



/************************************************************************/
/* Wait for timer overflow                                              */
/************************************************************************/

#define OS_Stimer_Wait(stimer_id)                   \
    {                                               \
        OS_Wait(OS_Stimer_Check(stimer_id)); \
    }


/************************************************************************/
/* Delay current task using static timer                                */
/************************************************************************/

#define OS_Stimer_Delay(stimer_id, delay)           \
    {                                               \
        OS_Stimer_Run(stimer_id, delay);            \
        OS_Stimer_Wait(stimer_id);                  \
    }



//------------------------------------------------------------------------------
// OLD SERVICES
//------------------------------------------------------------------------------
#define OS_Stimer_Stop(stimer_id)    OS_STIMER_ATOMIC_WRITE_A(_OS_Stimers[stimer_id] &= ~OS_STIMER_RUN_BIT)



//------------------------------------------------------------------------------
#endif  // OS_ENABLE_STIMERS
//------------------------------------------------------------------------------




















