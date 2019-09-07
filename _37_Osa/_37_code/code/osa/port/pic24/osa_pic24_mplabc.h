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
 *  File:           osa_pic24_mplabc.h
 *
 *  Compilers:      Mplab C30
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    MplabC30 specific definition
 *                  This file directly included in osa.h
 *
 *  History:        13.09.2010 -    File updated
 *
 ************************************************************************************************
 */


#ifndef __OSAMCC30_H__
#define __OSAMCC30_H__



#if   defined(__PIC24F__)

    #include <p24Fxxxx.h>
    #define __OSA_PIC24__
    #define __OSA_PIC24F__

#elif defined(__PIC24FK__)

    #include <p24Fxxxx.h>
    #define __OSA_PIC24__
    #define __OSA_PIC24FK__

#elif defined(__PIC24H__)

    #define __OSA_PIC24__
    #define __OSA_PIC24H__
    #include <p24Hxxxx.h>

#elif defined(__dsPIC30F__)

    #include <p30Fxxxx.h>
    #define __OSA_DSPIC30__
    #define __OSA_DSPIC__

#elif defined(__dsPIC33F__)

    #include <p33Fxxxx.h>
    #define __OSA_DSPIC33__
    #define __OSA_DSPIC__

#else
#warning "WARNING! PIC type not defined!"
#endif




/************************************************************************************************
 *                                                                                              *
 *     Integer types definitions                                                                *
 *                                                                                              *
 ************************************************************************************************/

typedef unsigned char   OST_UINT8;
typedef unsigned int    OST_UINT16;
typedef unsigned long   OST_UINT32;
typedef unsigned int    OST_BOOL;

typedef OST_UINT16    OST_UINT;
#define _OST_INT_MASK    15
#define _OST_INT_SHIFT   4
#define _OST_INT_SIZE    16


/************************************************************************************************
 *                                                                                              *
 *     Location keywords                                                                        *
 *                                                                                              *
 ************************************************************************************************/

#define OS_RAM_NEAR     __attribute__((near))
#define OS_RAM_BANK0
#define OS_RAM_BANK1
#define OS_RAM_BANK2
#define OS_RAM_BANK3

#define OST_BOOL             OST_UINT

#define _OS_CUR_FLAGS_IN_OS_STATE   // Work with tasks state througth _OS_State variable



/************************************************************************************************
 *                                                                                              *
 *     Processor specified definitions for system flags                                         *
 *     (see OST_SYSTEM_FLAGS and OST_TCB definition in file "OSA.h")                            *
 *                                                                                              *
 ************************************************************************************************/

#define _OS_SYSTEM_FLAGS_PROC_SPEC()                                                            \
                                                                                                \
        /*5..7*/OST_UINT   cIPL_Temp   : 3; /* Saved InterruptPriotityLevel    */               \
                                            /* when entering critical section  */               \
                                                                                                \
        /*8*/   OST_UINT   bTimeout    : 1; /* Event waiting timeout           */               \
                                                                                                \
        /*------------------------------------------------------*/                              \
        /*  Tells that task was replaced (with OS_Task_Replace) */                              \
        /*  so we don't need to save stack pointer for MCC30    */                              \
        /*------------------------------------------------------*/                              \
                                                                                                \
        /*9*/   OST_UINT   bTaskReplaced                   : 1;                                 \
                                                                                                \
         /*-----------------------------------------------------*/                              \
         /* Theese 5 bit variables contains information         */                              \
         /* about enablity "lnk" instruction in the             */                              \
         /* begin of 5 switching context routines.              */                              \
         /*-----------------------------------------------------*/                              \
                                                                                                \
                                                                                                \
        /*10*/  OST_UINT   bOSRetPoint_LNK                 : 1;                                 \
        /*11*/  OST_UINT   bOSRetPointNoSave_LNK           : 1;                                 \
        /*12*/  OST_UINT   bOSClearReady_LNK               : 1;                                 \
        /*13*/  OST_UINT   bOSClearReadySetCanContinue_LNK : 1;                                 \
        /*14*/  OST_UINT   bOSCheckEvent_LNK               : 1;                                 \


#define _OS_TCB_PROC_SPEC()                                                                     \
                                                                                                \
        OST_UINT16    W8_Temp;                                                                  \
        OST_UINT16    W9_Temp;                                                                  \
        OST_UINT16    W10_Temp;                                                                 \
        OST_UINT16    W11_Temp;                                                                 \
        OST_UINT16    W12_Temp;                                                                 \
        OST_UINT16    W13_Temp;                                                                 \
        OST_UINT16    W14_Temp;                                                                 \
        OST_UINT16    W15_Temp;                                                                 \


/************************************************************************************************
 *                                                                                              *
 *     Constants and types                                                                      *
 *                                                                                              *
 ************************************************************************************************/


#ifndef OS_SMSG_SIZE
#define OS_SMSG_SIZE    0   // We can not use sizeof() because it does not
                            // work with #ifdef directive
#endif

//------------------------------------------------------------------------------
// For all memory models pointer to code mast be 16-bit value (manual 5.6)
// (For model large used segment .handles)

typedef unsigned int                OST_CODE_POINTER;
#define OS_CODE_POINTER_SIZE        2


#define __OSA_C30_24BIT__


#if defined __OSA_C30_24BIT__
#define OSA_TASK_STATE_PROC_SPEC()   OST_UINT8     pTaskPointerH;
#endif



/************************************************************************************************
 *                                                                                              *
 *     Platform macros                                                                          *
 *                                                                                              *
 ************************************************************************************************/


#define _OS_SET_TASK_IRP()      // Empty macro for this type of PICs
#define OS_CLRWDT()   asm volatile(" clrwdt     ")
#define OS_ClrWdt()   asm volatile(" clrwdt     ")
#define OS_SLEEP()    asm volatile(" pwrsav #0  ")
#define OS_Sleep()    asm volatile(" pwrsav #0  ")
#define OS_IDLE()     asm volatile(" pwrsav #1  ")
#define OS_Idle()     asm volatile(" pwrsav #1  ")






/************************************************************************************************
 *                                                                                              *
 *     S C H E D U L E R                                                                        *
 *                                                                                              *
 ************************************************************************************************/


//------------------------------------------------------------------------------
#if (OS_PRIORITY_LEVEL == OS_PRIORITY_DISABLED) || (OS_PRIORITY_LEVEL == OS_PRIORITY_NORMAL)
//------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    #ifdef OS_ENABLE_CRITICAL_SECTION
    //------------------------------------------------------------------------------
    
        #define _OS_SCHED_SKIP_IF_CRITICAL_SECTION()                                \
            asm volatile("   btsc    __OS_Flags, #2  ");                            \
            asm volatile("   bra     2f             "); /*  SCHED_CHECK_ENABLE   */ \
    
        #define IF_IN_CRITICAL_SECTION_THEN_BREAK()                                 \
            asm volatile("   btsc    __OS_Flags, #2  ");                            \
            asm volatile("   bra     6f             "); /*  SCHED_BREAK */          \
    
    //------------------------------------------------------------------------------
    #else
    //------------------------------------------------------------------------------
    
        #define _OS_SCHED_SKIP_IF_CRITICAL_SECTION()
        #define IF_IN_CRITICAL_SECTION_THEN_BREAK()
    
    //------------------------------------------------------------------------------
    #endif  // OS_ENABLE_CRITICAL_SECTION
    //------------------------------------------------------------------------------
    
    
    
    
    //------------------------------------------------------------------------------
    #ifdef OS_ENABLE_TTIMERS
    //------------------------------------------------------------------------------
    
        #define _OS_SCHED_IF_DELAIED_GOTO_CONTINUE()                                        \
            asm volatile("   btss    __OS_State, #4         "); /*  bDelay          */      \
            asm volatile("   bra     $ + 6                  "); /*  SCHED_NO_DELAY  */      \
            asm volatile("   btss    __OS_State, #5         "); /* bCanContinue */          \
            asm volatile("   bra     5f   ;SCHED_CONTINUE   ");                             \
            asm volatile(";SCHED_NO_DELAY:                  ");                             \
    
    //------------------------------------------------------------------------------
    #else
    //------------------------------------------------------------------------------
    
        #define _OS_SCHED_IF_DELAIED_GOTO_CONTINUE()
    
    //------------------------------------------------------------------------------
    #endif
    //------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#endif  // PRIORITY_NORMAL || PRIORITY_DISABLED
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#if OS_PRIORITY_LEVEL == OS_PRIORITY_NORMAL
//------------------------------------------------------------------------------

    /********************************************************************************************
     *                                                                                          *
     *     P R I O R I T Y   M O D E   S C H E D U L E R                                        *
     *                                                                                          *
     ********************************************************************************************/

    // This macro is to determine were there any ready task or system is in idle state
    #define OS_IsIdle()     (!OS_IsInCriticalSection() && !_OS_Flags.bBestTaskFound)

    #define OS_Sched()                                                                          \
    {                                                                                           \
        asm volatile("   bset    __OS_Flags, #3                     ");  /* bCheckingTasks */   \
        asm volatile("   bclr    __OS_Flags, #4                     ");  /* bBestTaskFound */   \
        asm volatile("   mov     #%0, w0"::"i"(OS_WORST_PRIORITY + 1));                         \
        asm volatile("   mov     w0, __OS_Best_Priority             ");                         \
                        _OS_SCHED_SKIP_IF_CRITICAL_SECTION();                                   \
        asm volatile("   mov     __OS_LastTask, w0                  ");                         \
        asm volatile("   mov     w0, __OS_CurTask                   ");                         \
        asm volatile("                                              ");                         \
        asm volatile("1:;SCHED_REPEAT:                              ");                         \
        asm volatile("   mov     #%0, w0"::"i"(_OS_Tasks));                                     \
        asm volatile("   add     #%0, w0"::"i"((OS_TASKS-1)*sizeof(OST_TCB)));                  \
        asm volatile("   cp      __OS_CurTask                       ");                         \
        asm volatile("   bra     nc, $ + 8                          "); /* SCHED_INC_CUR_TASK */\
        asm volatile("   mov     #%0, w0"::"i"(_OS_Tasks)            );                         \
        asm volatile("   mov w0, __OS_CurTask                       ");                         \
        asm volatile("   bra     2f    ;SCHED_CHECK_ENABLE          ");                         \
        asm volatile(";SCHED_INC_CUR_TASK:                          ");                         \
        asm volatile("   mov     #%0, w0"::"i"(sizeof(OST_TCB)));                               \
        asm volatile("   add     __OS_CurTask                       ");                         \
        asm volatile("                                              ");                         \
        asm volatile("2:;SCHED_CHECK_ENABLE:                        ");                         \
        asm volatile("   mov     __OS_CurTask, w1                   ");                         \
        asm volatile("   mov     [w1], w0                           ");                         \
        asm volatile("   mov     w0, __OS_State                     ");                         \
        asm volatile("   btsc    __OS_State, #6                     ");  /* bEnable */          \
        asm volatile("   btsc    __OS_State, #7                     ");  /* bPaused */          \
        asm volatile("   bra     5f   ;SCHED_CONTINUE               ");                         \
        asm volatile("                                              ");                         \
        asm volatile("   and     #7, w0                             ");                         \
        asm volatile("   cp      __OS_Best_Priority                 ");                         \
        asm volatile("   bra     leu, 5f  ;SCHED_CONTINUE           ");                         \
        asm volatile("                                              ");                         \
        asm volatile("   btsc    __OS_State, #3                     ");  /* bReady */           \
        asm volatile("   bra     4f    ;SCHED_SKIP_RUN              ");                         \
        asm volatile("3:;_OS_SCHED_RUN:                             ");                         \
                        _OS_JUMP_TO_TASK();                                                     \
                        _OS_BACK_TO_SCEDULER();                                                 \
        asm volatile("   btss    __OS_Flags, #3                     ");  /* bCheckingTasks */   \
        asm volatile("   bra     7f ;SCHED_END                      ");                         \
        asm volatile("                                              ");                         \
        asm volatile("4:;SCHED_SKIP_RUN:                            ");                         \
                        _OS_SCHED_IF_DELAIED_GOTO_CONTINUE();                                   \
        asm volatile("   btss    __OS_State, #3                     ");  /* bReady */           \
        asm volatile("   bra     5f   ;SCHED_CONTINUE               ");                         \
        asm volatile("   bset    __OS_Flags, #4                     ");  /* bBestTaskFound */   \
        asm volatile("   mov     __OS_CurTask, w0                   ");                         \
        asm volatile("   mov     w0, __OS_BestTask                  ");                         \
        asm volatile("   mov     [w0], w1                           ");                         \
        asm volatile("   and     #7, w1                             ");                         \
        asm volatile("   mov     w1, __OS_Best_Priority             ");                         \
        asm volatile("   bra     z, 6f  ;SCHED_BREAK                ");                         \
        asm volatile("                                              ");                         \
        asm volatile("5:;SCHED_CONTINUE:                            ");                         \
                        IF_IN_CRITICAL_SECTION_THEN_BREAK();                                    \
        asm volatile("   mov     __OS_CurTask, w0                   ");                         \
        asm volatile("   cp      __OS_LastTask                      ");                         \
        asm volatile("   bra     nz, 1b ;SCHED_REPEAT               ");                         \
        asm volatile("                                              ");                         \
        asm volatile("6:;SCHED_BREAK:                               ");                         \
        asm volatile("   btss    __OS_Flags, #4                     ");  /* bBestTaskFound */   \
        asm volatile("   bra     7f ;SCHED_END                      ");                         \
        asm volatile("   bclr    __OS_Flags, #3                     ");  /* bCheckingTasks */   \
        asm volatile("   mov     __OS_BestTask, w0                  ");                         \
        asm volatile("   mov     w0, __OS_CurTask                   ");                         \
        asm volatile("   mov     w0, __OS_LastTask                  ");                         \
        asm volatile("   bra     3b    ;_OS_SCHED_RUN               ");                         \
                                                                                                \
        asm volatile("7:    ;SCHED_END:                             ");                         \
    }

//------------------------------------------------------------------------------
#endif   // OS_PRIORITY_NORMAL
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
#if OS_PRIORITY_LEVEL == OS_PRIORITY_EXTENDED
//------------------------------------------------------------------------------

/**** The OS_Sched for EXTENDED mode is defined in osa_system.h ****/

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
#if OS_PRIORITY_LEVEL == OS_PRIORITY_DISABLED
//------------------------------------------------------------------------------

    /********************************************************************************************
     *                                                                                          *
     *     N O N   P R I O R I T Y   M O D E   S C H E D U L E R                                *
     *                                                                                          *
     ********************************************************************************************/

    #define OS_IsIdle()     0    // for non priority mode supposed that system always not in idle

    // Scheduler macro for NON PRIORITY MODE

    #define OS_Sched()                                                                          \
    {                                                                                           \
        asm volatile("   clr     __OS_Temp                          ");                         \
                        _OS_SCHED_SKIP_IF_CRITICAL_SECTION();                                   \
        asm volatile("   mov     #%0, w0"::"i"(_OS_Tasks));                                     \
        asm volatile("   add     #%0, w0"::"i"((OS_TASKS-1)*sizeof(OST_TCB)));                  \
        asm volatile("   cp      __OS_CurTask                       ");                         \
        asm volatile("   bra     nc, $ + 8                          "); /* SCHED_INC_CUR_TASK */\
        asm volatile("   mov     #%0, w0"::"i"(_OS_Tasks));                                     \
        asm volatile("   mov     w0, __OS_CurTask                   ");                         \
        asm volatile("   bra     2f    ;SCHED_CHECK_ENABLE          ");                         \
        asm volatile(";SCHED_INC_CUR_TASK:                          ");                         \
        asm volatile("   mov     #%0, w0"::"i"(sizeof(OST_TCB)));                               \
        asm volatile("   add     __OS_CurTask                       ");                         \
        asm volatile("                                              ");                         \
        asm volatile("2:;SCHED_CHECK_ENABLE:                        ");                         \
        asm volatile("   mov     __OS_CurTask, w1                   ");                         \
        asm volatile("   mov     [w1], w0                           ");                         \
        asm volatile("   mov     w0, __OS_State                      ");                        \
        asm volatile("   btsc    __OS_State, #6                      "); /* bEnable */          \
        asm volatile("   btsc    __OS_State, #7                      "); /* bPaused */          \
        asm volatile("   bra     5f   ;SCHED_CONTINUE               ");                         \
                        _OS_SCHED_IF_DELAIED_GOTO_CONTINUE();                                   \
                        _OS_JUMP_TO_TASK();                                                     \
                        _OS_BACK_TO_SCEDULER();                                                 \
        asm volatile("5:;SCHED_CONTINUE                             ");                         \
    }



//------------------------------------------------------------------------------
#endif  // OS_PRIORITY_DISABLED
//------------------------------------------------------------------------------

//______________________________________________________________________________





/************************************************************************************************
 *                                                                                              *
 *     Context switching macros                                                                 *
 *                                                                                              *
 ************************************************************************************************/

/*
 ********************************************************************************
 *
 *  _OS_JUMP_TO_TASK()
 *
 *------------------------------------------------------------------------------
 *
 *  description:        jump indirectly from kernel (OS_Sched) into task
 *
 ********************************************************************************
 */


#ifdef __OSA_C30_24BIT__
#define _OS_GET_TASK_ADDR_H()   asm volatile("  swap    w3                  "); \
                                asm volatile("  and     #0x00FF, w3         ");
#else
#define _OS_GET_TASK_ADDR_H()   asm volatile("  clr     w3                  ")
#endif


#define _OS_JUMP_TO_TASK()                                                                      \
    {                                                                                           \
        asm volatile("  push.d  w8                  ");                                         \
        asm volatile("  push.d  w10                 ");                                         \
        asm volatile("  push.d  w12                 ");                                         \
        asm volatile("  push.w  w14                 ");                                         \
        asm volatile("  call    $+12                ");                                         \
        asm volatile("  pop.w   w14                 ");                                         \
        asm volatile("  pop.d   w12                 ");                                         \
        asm volatile("  pop.d   w10                 ");                                         \
        asm volatile("  pop.d   w8                  ");                                         \
        asm volatile("  bra     9f                  ");                                         \
                                                                                                \
        asm volatile(";SCHED_GOTO_TASK:             ");                                         \
        asm volatile("   mov     __OS_CurTask, w0   ");                                         \
        asm volatile("   mov.w   [w0++], w3         ");                                         \
        asm volatile("   mov     w3, __OS_State      ");                                        \
        asm volatile("   mov.w   [w0++], w2         "); /* task addr */                         \
                                                                                                \
        /* Restore registers (C30 manual 4.12 Function call conversions) */                     \
        asm volatile("  mov.d   [w0++], w8          ");                                         \
        asm volatile("  mov.d   [w0++], w10         ");                                         \
        asm volatile("  mov.d   [w0++], w12         ");                                         \
        asm volatile("  mov.w   [w0++], w14         ");                                         \
                                                                                                \
        asm volatile("  mov     [w0], w4            "); /* w15 value */                         \
        asm volatile("  mov     w15, [w0]           "); /* save stack pointer */                \
        asm volatile("  cp0     w4                  ");                                         \
        asm volatile("  bra     z, $ + #4           ");                                         \
        asm volatile("  mov     w4, w15             ");                                         \
                                                                                                \
        /* Put task address into stack and make return */                                       \
        _OS_GET_TASK_ADDR_H();                                                                  \
        asm volatile("  push.d  w2                  ");                                         \
        asm volatile("  return                      ");                                         \
    }


#define _OS_BACK_TO_SCEDULER()                                                                  \
    {                                                                                           \
        asm volatile("9:   ;SCHED_SAVE_RET_ADDR:    ");                                         \
                                                                                                \
        /* Save OS_State      */                                                                \
        asm volatile("   mov     __OS_CurTask, w2   ");                                         \
                                                                                                \
        /* Disable interrupts to avoid changing bDelay flag by interrupt */                     \
        asm volatile("   disi    #7                 ");                                         \
        asm volatile("   mov     [w2], w1           ");                                         \
        asm volatile("   bclr    w1, #3             ");  /* bReady */                           \
        asm volatile("   bclr    w1, #5             ");  /* bCanContinue */                     \
        asm volatile("   mov     #0x28, w0          ");  /* bReady | bCanContinue */            \
        asm volatile("   and     __OS_State, WREG   ");                                         \
        asm volatile("   ior     w1, w0, w1         ");                                         \
        asm volatile("   mov     w1, [w2]           ");                                         \
        /* Here interrupts are enabling     */                                                  \
    }







/************************************************************************************************
 *                                                                                              *
 *     Return from task to OS_Sched macros                                                      *
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
 ************************************************************************************/

void _OS_ReturnSave         (void);
void _OS_ReturnNoSave       (void);
void _OS_EnterWaitMode      (void);
void _OS_EnterWaitModeTO    (void);
void _OS_CheckEvent (OST_UINT);


//------------------------------------------------------------------------------
#define _OS_RETURN_SAVE()           _OS_ReturnSave()
#define _OS_RETURN_NO_SAVE()        _OS_ReturnNoSave()
#define _OS_ENTER_WAIT_MODE()       _OS_EnterWaitMode()
#define _OS_ENTER_WAIT_MODE_TO()    _OS_EnterWaitModeTO()
#define _OS_CHECK_EVENT(event)      _OS_CheckEvent(event)

#define _OS_SET_FSR_CUR_TASK_W()
#define _OS_SET_FSR_CUR_TASK()
//------------------------------------------------------------------------------



/************************************************************************************************
 *                                                                                              *
 *     Create, replace, reserve task specific services                                          *
 *                                                                                              *
 ************************************************************************************************/

/************************************************************************/
/*                                                                      */
/* Replace current task with new (current task will be deleted)         */
/*                                                                      */
/************************************************************************/

#ifdef __OSA_C30_24BIT__
#define _OS_CLEAR_TASK_ADDR_HIGH()    _OS_State.pTaskPointerH = 0
#else
#define _OS_CLEAR_TASK_ADDR_HIGH()
#endif

#define OS_Task_Replace(priority, TaskName)                                     \
{                                                                               \
    OS_Task_Reserve(TaskName);                                                  \
    *((OST_UINT*)&_OS_State) = priority | 0x48; /* bEnable è bReady */          \
    _OS_CurTask->pTaskPointer = (OST_CODE_POINTER)TaskName;                     \
    _OS_CLEAR_TASK_ADDR_HIGH();                                                 \
    _OS_CurTask->State = _OS_State;                                             \
    _OS_Flags.bTaskReplaced = 1;                                                \
    _OS_RETURN_NO_SAVE();                                                       \
}

#define OS_Task_Replace_P(p,t)   OS_Task_Replace(p,t)


/************************************************************************/
/*                                                                      */
/* Create task                                                          */
/*                                                                      */
/************************************************************************/

#define OS_Task_Create(priority, TaskName) _OS_Task_Create(priority, (OST_CODE_POINTER)TaskName)


/************************************************************************/
/*                                                                      */
/* Create task by indirrect address                                     */
/*                                                                      */
/************************************************************************/

#define OS_Task_Reserve(TaskName)




/************************************************************************************************
 *                                                                                              *
 *                                     I N T E R R U P T S                                      *
 *                                                                                              *
 ************************************************************************************************/

extern OST_UINT OS_DI();
extern void OS_RI(OST_UINT);
#define OS_EI()     SR &= 0xFF1F;

//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT)
//------------------------------------------------------------------------------
    extern void _OS_DI_INT (void);
    extern void _OS_RI_INT (void);
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------































//____________________________________________________________________________________________________________
//************************************************************************************************************
//
//                    S P E C I F I C      M A C R O S       O V E R L O A D
//
//                    (used ro increasing the speed and reducing the code size)
//
//************************************************************************************************************



/******************************************************************************************
 *
 *  WORK WITH DYNAMIC TIMERS
 *
 ******************************************************************************************/

//------------------------------------------------------------------------------
#ifdef OS_ENABLE_DTIMERS
//------------------------------------------------------------------------------

#define _OS_DtimersWork_DEFINED


//------------------------------------------------------------------------------
#if   OS_DTIMER_SIZE == 1
//------------------------------------------------------------------------------

    #define _OS_INC_DTIMER()                                                             \
        {                                                                               \
            asm volatile("   mov     [w0 + #4], w2   ");                                \
            asm volatile("   inc     w2, w2          ");                                \
            asm volatile("   and     #0xFF, w2       ");                                \
            asm volatile("   mov     w2, [w0 + #4]   ");                                \
        }

//------------------------------------------------------------------------------
#elif OS_DTIMER_SIZE == 2
//------------------------------------------------------------------------------

    #define _OS_INC_DTIMER()                                                             \
        {                                                                               \
            asm volatile("   mov     [w0 + #4], w2   ");                                \
            asm volatile("   inc     w2, w2          ");                                \
            asm volatile("   mov     w2, [w0 + #4]   ");                                \
        }

//------------------------------------------------------------------------------
#elif OS_DTIMER_SIZE == 4
//------------------------------------------------------------------------------

    #define _OS_INC_DTIMER()                                                             \
        {                                                                               \
            asm volatile("   mov     [w0 + #4], w2   ");                                \
            asm volatile("   inc     w2, w2          ");                                \
            asm volatile("   mov     w2, [w0 + #4]   ");                                \
            asm volatile("   bra     NZ,1b           ");                                \
            asm volatile("   mov     [w0 + #6], w2   ");                                \
            asm volatile("   inc     w2, w2          ");                                \
            asm volatile("   mov     w2, [w0 + #6]   ");                                \
        }

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------




#define __OS_DtimersWork()                                                              \
    {                                                                                   \
        asm volatile("   mov     #__OS_DTimers, w0       ");                            \
        asm volatile("   mov     [w0], w1                "); /* w1 = flags */           \
        asm volatile("1:                                 "); /*_OS_DTIMERS_WORK_NEXT:*/ \
        asm volatile("   btss    w1, #3                  "); /* check bNextEnable */    \
        asm volatile("   goto    2f                      ");                            \
        asm volatile("   mov     [w0 + #2], w0           ");                            \
        asm volatile("   mov     [w0], w1                "); /* w1 = flags */           \
        asm volatile("   btss    w1, #2                  "); /* check bRun */           \
        asm volatile("   goto    1b                      ");                            \
        _OS_INC_DTIMER();                                                                \
        asm volatile("   bra     NZ,1b                   ");                            \
        asm volatile("   bset    w1, #0                  "); /* set timeout */          \
        asm volatile("   mov     w1, [w0]                "); /* save flags */           \
        asm volatile("   bra     1b                      ");                            \
        asm volatile("2:                                 "); /*END*/                    \
    }


//------------------------------------------------------------------------------
#endif  // OS_ENABLE_DTIMER
//------------------------------------------------------------------------------








//______________________________________________________________________________
/******************************************************************************************
 *
 * MACROS FOR STATIC TIMERS WORK
 *
 ******************************************************************************************/

//------------------------------------------------------------------------------
#ifdef OS_ENABLE_STIMERS
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#if     OS_STIMER_SIZE == 1
//------------------------------------------------------------------------------

    #define __OS_StimerWork(STIMER_ID)                                              \
        {                                                                           \
            asm volatile("  btsc.b     __OS_Stimers + %0, #7"::"i"(STIMER_ID));     \
            asm volatile("  inc.b      __OS_Stimers + %0    "::"i"(STIMER_ID));     \
        }



    #define __OS_StimersWorkSize()                                  \
        asm volatile("  mov     #__OS_Stimers, w0             ");     \
        asm volatile("  mov     #%0, w1"::"i"(OS_STIMERS*1)  );     \
        asm volatile("  add     w0,w1,w1                    ");     \
        asm volatile("0:                                    ");     \
        asm volatile("  mov.b   [w0++], w2                  ");     \
        asm volatile("  btsc    w2, #7                      ");     \
        asm volatile("  inc.b   [--w0], [w0++]              ");     \
        asm volatile("  cp      w0,w1                       ");     \
        asm volatile("  bra     nz, 0b                      ");     \


//------------------------------------------------------------------------------
#elif   OS_STIMER_SIZE == 2
//------------------------------------------------------------------------------

    #define __OS_StimerWork(STIMER_ID)                                              \
        {                                                                           \
            asm volatile("  btsc       __OS_Stimers + %0,#15"::"i"(STIMER_ID*2));   \
            asm volatile("  inc        __OS_Stimers + %0    "::"i"(STIMER_ID*2));   \
        }


    #define __OS_StimersWorkSize()                                  \
        asm volatile("  mov     #__OS_Stimers, w0             ");     \
        asm volatile("  mov     #%0, w1"::"i"(OS_STIMERS*2)  );     \
        asm volatile("  add     w0,w1,w1                    ");     \
        asm volatile("0:                                    ");     \
        asm volatile("  btsc    [w0++], #15                 ");     \
        asm volatile("  inc     [--w0], [w0++]              ");     \
        asm volatile("  cp      w0,w1                       ");     \
        asm volatile("  bra     nz, 0b                      ");     \


//------------------------------------------------------------------------------
#elif   OS_STIMER_SIZE == 4
//------------------------------------------------------------------------------

    #define __OS_StimerWork(STIMER_ID)                                              \
        {                                                                           \
            asm volatile("  btss       __OS_Stimers + %0,#15"::"i"(STIMER_ID*4+2)); \
            asm volatile("  bra        0f                                       "); \
            asm volatile("  inc        __OS_Stimers + %0    "::"i"(STIMER_ID*4));   \
            asm volatile("  bra        nc, 0f                                   "); \
            asm volatile("  inc        __OS_Stimers + %0    "::"i"(STIMER_ID*4+2)); \
            asm volatile("0:                                                    "); \
        }


    #define __OS_StimersWorkSize()                                  \
        asm volatile("  mov     #__OS_Stimers, w0             ");     \
        asm volatile("  mov     #%0, w1"::"i"(OS_STIMERS*4)  );     \
        asm volatile("  add     w0,w1,w1                    ");     \
        asm volatile("0:                                    ");     \
        asm volatile("  btss    [++w0], #15                 ");     \
        asm volatile("  bra     1f                          ");     \
        asm volatile("  inc     [--w0], [w0++]              ");     \
        asm volatile("  bra     nc, 1f                      ");     \
        asm volatile("  inc     [w0],[w0]                   ");     \
        asm volatile("1:                                    ");     \
        asm volatile("  inc2    w0, w0                      ");     \
        asm volatile("  cp      w0,w1                       ");     \
        asm volatile("  bra     nz, 0b                      ");     \


//------------------------------------------------------------------------------
#endif  // OS_STIMER_SIZE
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
#endif  // OS_ENABLE_STIMERS
//------------------------------------------------------------------------------












/******************************************************************************************
 *
 *  WORK WITH TASK TIMERS
 *
 ******************************************************************************************/
//------------------------------------------------------------------------------
#ifdef  OS_ENABLE_TTIMERS
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#if   OS_TTIMER_SIZE == 1
//------------------------------------------------------------------------------

    #define __OS_TtimerWork(TASK_ID)                                                \
        asm volatile("  btss    __OS_Tasks + %0, #4"::"i"(22 * TASK_ID));           \
        asm volatile("  bra     0f                                      ");         \
        asm volatile("  inc.b   __OS_Tasks + %0"::"i"(22 * TASK_ID+20) );           \
        asm volatile("  bra     nc, 0f                                  ");         \
        asm volatile("  bclr    __OS_Tasks + %0, #4"::"i"(22 * TASK_ID));           \
        asm volatile("0:                                                ");         \

    #define __OS_TtimersWorkSize()                                                  \
        asm volatile("  mov     #__OS_Tasks, w0           ");                       \
        asm volatile("  mov     #%0, w1"::"i"(OS_TASKS*22)   );                     \
        asm volatile("  add     w0,w1,w1                    ");                     \
        asm volatile("0:                                    ");                     \
        asm volatile("  btss    [w0], #4                    ");                     \
        asm volatile("  bra     1f                          ");                     \
        asm volatile("  add     w0,#20,w2                   ");                     \
        asm volatile("  inc.b   [w2], [w2++]                ");                     \
        asm volatile("  bra     nc, 1f                      ");                     \
        asm volatile("  bclr    [w0], #4                    ");                     \
        asm volatile("1:                                    ");                     \
        asm volatile("  add     w0, #22, w0                 ");                     \
        asm volatile("  cp      w0,w1                       ");                     \
        asm volatile("  bra     nz, 0b                      ");                     \


//------------------------------------------------------------------------------
#elif OS_TTIMER_SIZE == 2
//------------------------------------------------------------------------------

    #define __OS_TtimerWork(TASK_ID)                                                \
        asm volatile("  btss    __OS_Tasks + %0, #4"::"i"(22 * TASK_ID));           \
        asm volatile("  bra     0f                                      ");         \
        asm volatile("  inc     __OS_Tasks + %0"::"i"(22 * TASK_ID+20) );           \
        asm volatile("  bra     nc, 0f                                  ");         \
        asm volatile("  bclr    __OS_Tasks + %0, #4"::"i"(22 * TASK_ID));           \
        asm volatile("0:                                                ");         \

    #define __OS_TtimersWorkSize()                                                  \
        asm volatile("  mov     #__OS_Tasks, w0           ");                       \
        asm volatile("  mov     #%0, w1"::"i"(OS_TASKS*22)   );                     \
        asm volatile("  add     w0,w1,w1                    ");                     \
        asm volatile("0:                                    ");                     \
        asm volatile("  btss    [w0], #4                    ");                     \
        asm volatile("  bra     1f                          ");                     \
        asm volatile("  add     w0,#20,w2                   ");                     \
        asm volatile("  inc     [w2], [w2++]                ");                     \
        asm volatile("  bra     nc, 1f                      ");                     \
        asm volatile("  bclr    [w0], #4                    ");                     \
        asm volatile("1:                                    ");                     \
        asm volatile("  add     w0, #22, w0                 ");                     \
        asm volatile("  cp      w0,w1                       ");                     \
        asm volatile("  bra     nz, 0b                      ");                     \



//------------------------------------------------------------------------------
#elif OS_TTIMER_SIZE == 4
//------------------------------------------------------------------------------

    #define __OS_TtimerWork(TASK_ID)                                                \
        asm volatile("  btss    __OS_Tasks + %0, #4"::"i"(24 * TASK_ID));           \
        asm volatile("  bra     0f                                      ");         \
        asm volatile("  inc     __OS_Tasks + %0"::"i"(24 * TASK_ID+20)    );        \
        asm volatile("  bra     nc, 0f                                  ");         \
        asm volatile("  inc     __OS_Tasks + %0"::"i"(24 * TASK_ID+22)    );        \
        asm volatile("  bra     nc, 0f                                  ");         \
        asm volatile("  bclr    __OS_Tasks + %0, #4"::"i"(24 * TASK_ID));           \
        asm volatile("0:                                                ");         \


    #define __OS_TtimersWorkSize()                                                  \
        asm volatile("  mov     #__OS_Tasks, w0           ");                       \
        asm volatile("  mov     #%0, w1"::"i"(OS_TASKS*24)   );                     \
        asm volatile("  add     w0,w1,w1                    ");                     \
        asm volatile("0:                                    ");                     \
        asm volatile("  btss    [w0], #4                    ");                     \
        asm volatile("  bra     1f                          ");                     \
        asm volatile("  add     w0,#20,w2                   ");                     \
        asm volatile("  inc     [w2], [w2++]                ");                     \
        asm volatile("  bra     nc, 1f                      ");                     \
        asm volatile("  inc     [w2], [w2++]                ");                     \
        asm volatile("  bra     nc, 1f                      ");                     \
        asm volatile("  bclr    [w0], #4                    ");                     \
        asm volatile("1:                                    ");                     \
        asm volatile("  add     w0, #24, w0                 ");                     \
        asm volatile("  cp      w0,w1                       ");                     \
        asm volatile("  bra     nz, 0b                      ");                     \


//------------------------------------------------------------------------------
#endif      // if sizeof(OS_TIMER_TYPE)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#endif      // OS_ENABLE_TTIMERS
//------------------------------------------------------------------------------






//______________________________________________________________________________
/******************************************************************************************
 *
 * MACROS FOR OLD STYLE STATIC TIMERS WORK
 *
 ******************************************************************************************/

//------------------------------------------------------------------------------

#define __OS_Timer8Work(TIMER_ID,L)                                                                                     \
{                                                                                                                       \
    asm volatile("   btss   _OS_Timeouts + #%0, #%1"::"i"((TIMER_ID >> _OST_INT_SHIFT)),"i"(TIMER_ID & _OST_INT_MASK)); \
    asm volatile("   bra    $ + #12 ");                                                                                 \
    asm volatile("   mov.b    _OS_Timers8+%0, wreg"::"i"(TIMER_ID));                                                    \
    asm volatile("   inc.b    w0, w0     ");                                                                            \
    asm volatile("   mov.b    wreg, _OS_Timers8+%0"::"i"(TIMER_ID));                                                    \
    asm volatile("   bra    NZ, $ + #4");                                                                               \
    asm volatile("   bclr   _OS_Timeouts + #%0, #%1"::"i"((TIMER_ID >> _OST_INT_SHIFT)),"i"(TIMER_ID & _OST_INT_MASK)); \
}

//------------------------------------------------------------------------------

#define __OS_Timer16Work(TIMER_ID,L)                                                                                \
{                                                                                                               \
    asm volatile("   btss   _OS_Timeouts + #%0, #%1"::"i"(((TIMER_ID+_OS_TIMER16_POS) >> _OST_INT_SHIFT)),"i"((TIMER_ID+_OS_TIMER16_POS) & _OST_INT_MASK));    \
    asm volatile("   bra    $ + #12 ");                                                                                   \
    asm volatile("   mov    _OS_Timers16+%0, w0"::"i"((TIMER_ID)*2));                                                    \
    asm volatile("   inc    w0, w0     ");                                                                               \
    asm volatile("   mov    w0, _OS_Timers16+%0"::"i"((TIMER_ID)*2));                                                    \
    asm volatile("   bra    NZ, $ + #4");                                                                                \
    asm volatile("   bclr   _OS_Timeouts + #%0, #%1"::"i"(((TIMER_ID+_OS_TIMER16_POS) >> _OST_INT_SHIFT)),"i"((TIMER_ID+_OS_TIMER16_POS) & _OST_INT_MASK));      \
}

//------------------------------------------------------------------------------

#define __OS_Timer24Work(TIMER_ID,L)                                                                                \
{                                                                                                                   \
    asm volatile("   btss   _OS_Timeouts + #%0, #%1"::"i"(((TIMER_ID+_OS_TIMER24_POS) >> _OST_INT_SHIFT)),"i"((TIMER_ID+_OS_TIMER24_POS) & _OST_INT_MASK));    \
    asm volatile("   bra    $ + 12 ");                                                                              \
    asm volatile("   mov    _OS_Timers24+%0, w0"::"i"((TIMER_ID)*2));                                               \
    asm volatile("   inc    w0, w0     ");                                                                          \
    asm volatile("   mov    w0, _OS_Timers24+%0"::"i"((TIMER_ID)*2));                                               \
    asm volatile("   bra    NZ, $ + #4");                                                                           \
    asm volatile("   bclr   _OS_Timeouts + #%0, #%1"::"i"(((TIMER_ID+_OS_TIMER24_POS) >> _OST_INT_SHIFT)),"i"((TIMER_ID+_OS_TIMER24_POS) & _OST_INT_MASK));      \
}

//------------------------------------------------------------------------------

#define __OS_Timer32Work(TIMER_ID,L)                                                                                \
{                                                                                                               \
    asm volatile("   btss   _OS_Timeouts + #%0, #%1"::"i"(((TIMER_ID+_OS_TIMER32_POS) >> _OST_INT_SHIFT)),"i"((TIMER_ID+_OS_TIMER32_POS) & _OST_INT_MASK));    \
    asm volatile("   bra    $ + #20 ");                                                                                   \
    asm volatile("   mov    _OS_Timers32+%0, w0"::"i"((TIMER_ID+_OS_TIMER32_POS)*4));                                    \
    asm volatile("   inc    w0, w0     ");                                                                               \
    asm volatile("   mov    w0, _OS_Timers32+%0"::"i"((TIMER_ID+_OS_TIMER32_POS)*4));                                    \
    asm volatile("   bra    NZ, $ + #12");                                                                                \
    asm volatile("   mov    _OS_Timers32+%0, w0"::"i"((TIMER_ID+_OS_TIMER32_POS)*4 + 2));                                \
    asm volatile("   inc    w0, w0     ");                                                                               \
    asm volatile("   mov    w0, _OS_Timers32+%0"::"i"((TIMER_ID+_OS_TIMER32_POS)*4 + 2));                                \
    asm volatile("   bra    NZ, $ + #4");                                                                                \
    asm volatile("   bclr   _OS_Timeouts + #%0, #%1"::"i"(((TIMER_ID+_OS_TIMER32_POS) >> _OST_INT_SHIFT)),"i"((TIMER_ID+_OS_TIMER32_POS) & _OST_INT_MASK));    \
}






#endif



//******************************************************************************
//  END OF FILE osa_pic24_mplabc.h
//******************************************************************************





