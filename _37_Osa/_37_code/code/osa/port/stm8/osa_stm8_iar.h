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
 *  File:           osa_stm8_iar.h
 *
 *  Compilers:      IAR
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    IAR for STM8 specific definition
 *                  This file directly included in osa.h
 *
 *  History:        13.09.2010 -    File updated
 *
 ************************************************************************************************
 */




#ifndef __OSAIARSTM8_H__
#define __OSAIARSTM8_H__

//#error "OSA error #26: IAR for STM8 is not supported by OSA!"


#ifdef __cplusplus
#error "OSA: This is CPLUSPLUS mode!!!"
#endif

#define OS_PROTECT_MEMORY_ACCESS            // for atomic access protection



/************************************************************************************************
 *                                                                                              *
 *     Integer types definitions                                                                *
 *                                                                                              *
 ************************************************************************************************/

typedef unsigned char   OST_UINT8;
typedef unsigned int    OST_UINT16;
typedef unsigned long   OST_UINT32;
typedef unsigned char   OST_BOOL;

typedef OST_UINT8       OST_UINT;
#define _OST_INT_MASK    7
#define _OST_INT_SHIFT   3
#define _OST_INT_SIZE    8


/************************************************************************************************
 *                                                                                              *
 *     Location keywords                                                                        *
 *                                                                                              *
 ************************************************************************************************/

#define OS_RAM_NEAR     __tiny
#define OS_RAM_BANK0    
#define OS_RAM_BANK1    
#define OS_RAM_BANK2    
#define OS_RAM_BANK3    

#define _OS_CUR_FLAGS_IN_OS_STATE        // Work with tasks state througth _OS_State


#define _OS_SET_STATE()                         \
    asm("   ldw     X, _OS_CurTask  ");         \
    asm("   ld      A, (X)          ");         \
    asm("   ld      _OS_State, A    ");

#define _OS_RESTORE_STATE()


/************************************************************************************************
 *                                                                                              *
 *     Processor specified definitions for system flags                                         *
 *     (see OST_SYSTEM_FLAGS and OST_TCB definition in file "OSA.h")                            *
 *                                                                                              *
 ************************************************************************************************/

#define _OS_SYSTEM_FLAGS_PROC_SPEC()                                                          \
                                                                                              \
        /*5*/   OST_UINT   bTimeout    : 1; /* Event waiting timeout                */        \
        /*6*/   OST_UINT   bI1_CTemp   : 1; /* Saved I1 while in critical section */          \
        /*7*/   OST_UINT   bI0_CTemp   : 1; /* Saved I0 while in critical section */          \

#define _OS_TCB_PROC_SPEC()                                               \
                OST_UINT16   nSP_Temp;                                    \
                OST_UINT8    b8_Temp;       /* Saved temp registers */    \
                OST_UINT8    b9_Temp;                                     \
                OST_UINT8    b10_Temp;                                    \
                OST_UINT8    b11_Temp;                                    \
                OST_UINT8    b12_Temp;                                    \
                OST_UINT8    b13_Temp;                                    \
                OST_UINT8    b14_Temp;                                    \
                OST_UINT8    b15_Temp;                                    \


#define OST_TASK_POINTER   OS_TASKS_BANK OST_TCB *

/************************************************************************************************
 *                                                                                              *
 *     Registers definitions                                                                    *
 *                                                                                              *
 ************************************************************************************************/

extern OS_RAM_NEAR OST_UINT16 _OS_SP_Temp;


/************************************************************************************************
 *                                                                                              *
 *     Constants and types                                                                      *
 *                                                                                              *
 ************************************************************************************************/

#ifndef OS_SMSG_SIZE
#define OS_SMSG_SIZE    0   // We can not use sizeof() because it does not work with #ifdef directive
#endif

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#if __CODE_MODEL__ == __SMALL_CODE_MODEL__

    #define OST_CODE_POINTER        OST_UINT16
    #define OS_CODE_POINTER_SIZE    2

#else

    #define OST_CODE_POINTER        OST_UINT32
    #define OS_CODE_POINTER_SIZE    4

#endif





/************************************************************************************************
 *                                                                                              *
 *     Platform macros                                                                          *
 *                                                                                              *
 ************************************************************************************************/



#define OS_CLRWDT()
#define OS_SLEEP()





/************************************************************************************************
 *                                                                                              *
 *     Context switching macros                                                                 *
 *                                                                                              *
 ************************************************************************************************/


//------------------------------------------------------------------------------
// Save/restore PC macros
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#if OS_CODE_POINTER_SIZE == 2
//------------------------------------------------------------------------------

    #define _OS_SET_PC()                        \
        asm("   ldw     X, Y            ");     \
        asm("   ldw     X, (1,X)        ");     \
        asm("   pushw   X               ");     \
        asm("   ret                     ");     \

    #define _OS_Restore_TaskSP()                \
        asm("   ldw     X, Y            ");     \
        asm("   ldw     X, (3,X)        ");     \
        asm("   tnzw    X               ");     \
        asm("   jreq    SKIP_SP         ");     \
        asm("   ldw     SP, X           ");     \
        asm("SKIP_SP:                   ");     \

    #define _OS_Restore_Btemp()                 \
        asm("   ldw     X, Y            ");     \
        asm("   ld      A, (5,X)        ");     \
        asm("   ld      ?b0+8, A        ");     \
        asm("   ld      A, (6,X)        ");     \
        asm("   ld      ?b0+9, A        ");     \
        asm("   ld      A, (7,X)        ");     \
        asm("   ld      ?b0+10, A       ");     \
        asm("   ld      A, (8,X)        ");     \
        asm("   ld      ?b0+11, A       ");     \
        asm("   ld      A, (9,X)        ");     \
        asm("   ld      ?b0+12, A       ");     \
        asm("   ld      A, (10,X)       ");     \
        asm("   ld      ?b0+13, A       ");     \
        asm("   ld      A, (11,X)       ");     \
        asm("   ld      ?b0+14, A       ");     \
        asm("   ld      A, (12,X)       ");     \
        asm("   ld      ?b0+15, A       ");     \

    #define _OS_SAVE_PC()                       \
        asm("   popw    X               ");     \
        asm("   ldw     (1,Y), X        ");     \


    #define _OS_Store_TaskSP()                  \
        asm("   ldw     X, SP           ");     \
        asm("   ldw     (3,Y), X        ");     \

    #define _OS_Store_TaskSP2()                 \
        asm("   ldw     X, SP           ");     \
        asm("   addw    X, #2           ");     \
        asm("   ldw     (3,Y), X        ");     \

    #define _OS_Store_Btemp()                   \
        asm("   ldw     X, Y            ");     \
        asm("   ld      A, ?b0+8    ");         \
        asm("   ld      (5,X), A    ");         \
        asm("   ld      A, ?b0+9    ");         \
        asm("   ld      (6,X), A    ");         \
        asm("   ld      A, ?b0+10   ");         \
        asm("   ld      (7,X), A    ");         \
        asm("   ld      A, ?b0+11   ");         \
        asm("   ld      (8,X), A    ");         \
        asm("   ld      A, ?b0+12   ");         \
        asm("   ld      (9,X), A    ");         \
        asm("   ld      A, ?b0+13   ");         \
        asm("   ld      (10,X), A    ");        \
        asm("   ld      A, ?b0+14   ");         \
        asm("   ld      (11,X), A    ");        \
        asm("   ld      A, ?b0+15   ");         \
        asm("   ld      (12,X), A   ");         \

//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------




#define     _OS_STORE_SP()                          \
    {                                               \
        asm("   ldw     X, SP           ");         \
        asm("   ldw     _OS_SP_Temp, X  ");         \
    }

#define     _OS_RESTORE_SP()                        \
    {                                               \
        asm("   ldw     X, _OS_SP_Temp  ");         \
        asm("   ldw     SP, X           ");         \
    }

#define     _OS_POP()       asm("   popw    X   ")
#define     _OS_PUSH()      asm("   pushw   X   ")


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

void _OS_JumpToTask (void);
void _OS_JumpToTask_ASM (void);

#define _OS_JUMP_TO_TASK()      _OS_JumpToTask_ASM()

#define _OS_BACK_TO_SCEDULER()                              \
    {                                                       \
        _OS_SET_FSR_CUR_TASK();                             \
        /* cli() */                                         \
        asm("   push    CC              ");                 \
        asm("   pop     A               ");                 \
        asm("   ld      XL, A           ");                 \
        asm("   sim                     ");                 \
        /* _OS_CurTask->State &= 0xD7; */                   \
        asm("   ld      A, (Y)          ");                 \
        asm("   and     A, #0xD7        ");                 \
        asm("   ld      (Y), A          ");                 \
        /* _OS_CurTask->State |= _OS_State & 0x28; */       \
        asm("   ld      A, #0x28        ");                 \
        asm("   and     A, _OS_State     ");                \
        asm("   or      A, (Y)          ");                 \
        asm("   ld      (Y), A          ");                 \
        /* ri() */                                          \
        asm("   ld      A, XL           ");                 \
        asm("   push    A               ");                 \
        asm("   pop     CC              ");                 \
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


void _OS_ReturnSave          (void);
void _OS_ReturnNoSave        (void);
void _OS_EnterWaitMode       (void);
void _OS_EnterWaitModeTO     (void);
void _OS_CheckEvent (OST_UINT);
void _OS_ClrReadySetClrCanContinue (void);

#define _OS_SET_FSR_CUR_TASK()      asm("   ldw     Y, _OS_CurTask     ");

//------------------------------------------------------------------------------

#define _OS_RETURN_SAVE()           _OS_ReturnSave()
#define _OS_ENTER_WAIT_MODE()       _OS_EnterWaitMode()
#define _OS_ENTER_WAIT_MODE_TO()    _OS_EnterWaitModeTO()
#define _OS_RETURN_NO_SAVE()        _OS_ReturnNoSave()
#define _OS_CHECK_EVENT(event)      _OS_CheckEvent(event)












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

#define OS_Task_Replace(priority, TaskName)                                             \
    {                                                                                   \
        *((OS_RAM_NEAR OST_UINT*)&_OS_State) = priority | 0x48; /* bEnable è bReady */  \
        _OS_CurTask->State = _OS_State;                                                 \
        _OS_CurTask->pTaskPointer = (OST_CODE_POINTER)(&TaskName);                      \
        _OS_CurTask->nSP_Temp = 0x0000;                                                 \
        _OS_ReturnNoSave();                                                             \
    }

#define OS_Task_Replace_P(priority, TaskAddr)                                           \
    {                                                                                   \
        *((OS_RAM_NEAR OST_UINT*)&_OS_State) = priority | 0x48; /* bEnable è bReady */  \
        _OS_CurTask->State = _OS_State;                                                 \
        _OS_CurTask->pTaskPointer = (OST_CODE_POINTER)(TaskAddr);                       \
        _OS_CurTask->nSP_Temp = 0x0000;                                                 \
        _OS_ReturnNoSave();                                                             \
    }


/************************************************************************/
/*                                                                      */
/* Create task                                                          */
/*                                                                      */
/************************************************************************/

#define OS_Task_Create(priority, TaskName)                          \
    {                                                               \
        _OS_Task_Create(priority, (OST_CODE_POINTER)&(TaskName));   \
    }


/************************************************************************/
/*                                                                      */
/* Create task by indirrect address                                     */
/*                                                                      */
/************************************************************************/

// If task created indirrectly then it is nessesary to insert in
// the main():
// #pragma funcall main TaskName

#define OS_Task_Reserve(TaskName)       // Empty macro for this compiler



/************************************************************************************************
 *                                                                                              *
 *                                     I N T E R R U P T S                                      *
 *                                                                                              *
 ************************************************************************************************/

char        OS_DI (void);
void        OS_RI (char);
#define     OS_EI()         asm(" rim ");

//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT)
//------------------------------------------------------------------------------
    void _OS_DI_INT (void);
    void _OS_RI_INT (void);
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



#endif




//******************************************************************************
//  END OF FILE osa_stm8_iar.h
//******************************************************************************

