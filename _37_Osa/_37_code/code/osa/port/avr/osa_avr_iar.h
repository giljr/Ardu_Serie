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
 *  File:           osa_avr_iar.h
 *
 *  Compilers:      IAR
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    IAR for AVR specific definition
 *                  This file directly included in osa.h
 *
 *  History:        13.09.2010 -    File updated
 *                  22.11.2010 -    Services _OS_STORE_SP and _OS_RESTORE_SP updated. They did
 *                                  not support 16-bit SP register
 *                             -    Bug in _OS_SAVE_Y() fixed (it was X saved, but not Y)
 *                             -    Bug fixed: _OS_SAVE_PC(): "ld  r29,r23" -> "mov   r29, r23"
 *
 ************************************************************************************************
 */

#ifndef __OSAIARAVR_H__
#define __OSAIARAVR_H__


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

typedef OST_UINT8     OST_UINT;
#define _OST_INT_MASK    7
#define _OST_INT_SHIFT   3
#define _OST_INT_SIZE    8


/************************************************************************************************
 *                                                                                              *
 *     Location keywords                                                                        *
 *                                                                                              *
 ************************************************************************************************/

#define OS_RAM_NEAR     
#define OS_RAM_BANK0
#define OS_RAM_BANK1
#define OS_RAM_BANK2
#define OS_RAM_BANK3

#define _OS_CUR_FLAGS_IN_OS_STATE        // Work with tasks state througth _OS_State



/************************************************************************************************
 *                                                                                              *
 *     Processor specified definitions for system flags                                         *
 *     (see OST_SYSTEM_FLAGS and OST_TCB definition in file "OSA.h")                            *
 *                                                                                              *
 ************************************************************************************************/

#define _OS_SYSTEM_FLAGS_PROC_SPEC()                                                    \
                                                                                        \
        /*5*/   OST_UINT   bTimeout    : 1; /* Event waiting timeout                */  \
        /*6*/   OST_UINT   bGIE_CTemp  : 1; /* Saved GIEH while in critical section */  \


#define _OS_TCB_PROC_SPEC()         OST_UINT16    nY_Temp;         /* Saved R28:R29 */


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

#if __CORE__ <= 4

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


#define OS_CLRWDT()             asm("  wdr   ")
#define OS_SLEEP()              asm("  sleep   ")








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
        asm("   ld      r22, z+         ");     \
        asm("   sts     _OS_State, r22   ");    \
        /* push task entry point into stack */  \
        asm("   ld      r22, z+         ");     \
        asm("   push    r22             ");     \
        asm("   ld      r22, z+         ");     \
        asm("   push    r22             ");     \
        /* restore saved Y value            */  \
        asm("   ld      r22, z+         ");     \
        asm("   ld      r23, z+         ");     \
        /* skip if not initialized yet      */  \
        asm("   sbrc    r23, 7          ");     \
        asm("   rjmp    $+12            ");     \
        asm("   in      r24, 0x3F       ");     \
        asm("   cli                     ");     \
        asm("   mov     r28, r22        ");     \
        asm("   mov     r29, r23        ");     \
        asm("   out     0x3F, r24       ");     \


    #define _OS_SAVE_PC()                       \
        asm("   ld      r22, z+     ");         \
        /* get return address               */  \
        asm("   pop     r22         ");         \
        asm("   pop     r23         ");         \
        /* and save it into TCB             */  \
        asm("   st      z+, r23     ");         \
        asm("   st      z+, r22     ");         \

    #define _OS_PUSH()                          \
        /* push return address back to stack */ \
        asm("   push    r23         ");         \
        asm("   push    r22         ");         \

    #define _OS_POP()                           \
        asm("   pop     r22         ");         \
        asm("   pop     r23         ");


    #define     _OS_STORE_SP()      _OS_SP_Temp = SP        /*101122*/

    #define     _OS_RESTORE_SP()    SP = _OS_SP_Temp        /*101122*/


//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------

    #define _OS_SET_PC()                        \
        asm("   ld      r22, z+         ");     \
        asm("   sts     _OS_State, r22   ");    \
        /* push task entry point into stack */  \
        asm("   ld      r22, z+         ");     \
        asm("   push    r22             ");     \
        asm("   ld      r22, z+         ");     \
        asm("   push    r22             ");     \
        asm("   ld      r22, z+         ");     \
        asm("   push    r22             ");     \
        /* skip highes byte of dwaddress    */  \
        asm("   ld      r22, z+         ");     \
        /* restore saved Y value            */  \
        asm("   ld      r22, z+         ");     \
        asm("   ld      r23, z+         ");     \
        /* skip if not initialized yet      */  \
        asm("   sbrc    r23, 7          ");     \
        asm("   rjmp    $+12            ");     \
        asm("   in      r24, 0x3F       ");     \
        asm("   cli                     ");     \
        asm("   mov     r28, r22        ");     \
        asm("   mov     r29, r23        "); /*101122*/  \
        asm("   out     0x3F, r24       ");     \


    #define _OS_SAVE_PC()                       \
        asm("   ld      r22, z+     ");         \
        /* get return address               */  \
        asm("   pop     r22         ");         \
        asm("   pop     r23         ");         \
        asm("   pop     r21         ");         \
        /* and save it into TCB             */  \
        asm("   st      z+, r21     ");         \
        asm("   st      z+, r23     ");         \
        asm("   st      z+, r22     ");         \
        /* write 0 to highest byte of dw addr */\
        asm("   ldi     r22, 0      ");         \
        asm("   st      z+, r22     ");         \


    #define _OS_PUSH()                          \
        /* push return address back to stack */ \
        asm("   push    r21         ");         \
        asm("   push    r23         ");         \
        asm("   push    r22         ");         \

    #define _OS_POP()                           \
        asm("   pop     r22         ");         \
        asm("   pop     r23         ");         \
        asm("   pop     r21         ");


    #define  _OS_STORE_SP()          /*101122*/ \
        _OS_SP_Temp = 0;                        \
        _OS_SP_Temp =(SPH<<8);                  \
        _OS_SP_Temp += SPL;                     \

    #define  _OS_RESTORE_SP()        /*101122*/ \
            _OS_Temp = SREG;                    \
            asm("   cli              ");        \
            SPH = _OS_SP_Temp>>8;               \
            SPL = _OS_SP_Temp;                  \
            SREG = _OS_Temp;                    \

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------

#define _OS_SAVE_Y()                            \
    /* save Y                           */      \
    asm("   st      z+, r28     "); /*101122*/  \
    asm("   st      z+, r29     "); /*101122*/

#define _OS_SAVE_Y_PLUS_7()                     \
    /* save Y                           */      \
    asm("   mov     r26, r28    ");             \
    asm("   mov     r27, r29    ");             \
    asm("   adiw    r26, 7      ");             \
    asm("   st      z+, r26     ");             \
    asm("   st      z+, r27     ");




/*
 ********************************************************************************
 *                                                                              *
 *  _OS_JUMP_TO_TASK()                                                          *
 *                                                                              *
 *------------------------------------------------------------------------------*
 *                                                                              *
 *  description:        jump indirectly from kernel (OS_Sched) into task        *
 *                                                                              *
 ********************************************************************************
 */

void _OS_JumpToTask (void);


#define _OS_JUMP_TO_TASK()                                              \
        asm("   push  r28     ");                                       \
        asm("   push  r29     ");                                       \
        _OS_JumpToTask();                                               \


#define _OS_BACK_TO_SCEDULER()                                          \
    {                                                                   \
        _OS_SET_FSR_CUR_TASK();                                         \
        asm("   in      r24, 0x3F       ");                             \
        asm("   cli                     ");                             \
        asm("   ld      r23, z          ");                             \
        /* r23 &= ~(OS_TASK_READY_MASK | OS_TASK_CANCONTINUE_MASK) */   \
        asm("   andi    r23, 0xD7       ");                             \
        asm("   lds     r22, _OS_State  ");                             \
        /* r22 |= (OS_TASK_READY_MASK | OS_TASK_CANCONTINUE_MASK) */    \
        asm("   andi    r22, 0x28       ");                             \
        asm("   or      r23, r22        ");                             \
        asm("   st      z, r23          ");                             \
        asm("   out     0x3F, r24       ");                             \
        asm("   pop     r29             ");                             \
        asm("   pop     r28             ");                             \
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
void _OS_SET_FSR_CUR_TASK   (void);
void _OS_CheckEvent         (OST_UINT);



//------------------------------------------------------------------------------
#define _OS_RETURN_SAVE()           _OS_ReturnSave()
#define _OS_ENTER_WAIT_MODE()       _OS_EnterWaitMode()
#define _OS_ENTER_WAIT_MODE_TO()    _OS_EnterWaitModeTO()
#define _OS_RETURN_NO_SAVE()        _OS_ReturnNoSave()
#define _OS_CHECK_EVENT(event)      _OS_CheckEvent(event)

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

#define OS_Task_Replace(priority, TaskName)                                             \
    {                                                                                   \
        *((OS_RAM_NEAR OST_UINT*)&_OS_State) = priority | 0x48; /* bEnable è bReady */  \
        _OS_CurTask->State = _OS_State;                                                 \
        _OS_CurTask->pTaskPointer = (OST_CODE_POINTER)(&TaskName);                      \
        _OS_CurTask->nY_Temp = 0x8000;                                                  \
        _OS_ReturnNoSave();                                                             \
    }

#define OS_Task_Replace_P(priority, TaskAddr)                                           \
    {                                                                                   \
        *((OS_RAM_NEAR OST_UINT*)&_OS_State) = priority | 0x48; /* bEnable è bReady */  \
        _OS_CurTask->State = _OS_State;                                                 \
        _OS_CurTask->pTaskPointer = (OST_CODE_POINTER)(TaskAddr);                       \
        _OS_CurTask->nY_Temp = 0x8000;                                                  \
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

extern char OS_DI (void);
extern void OS_RI (char);
#define     OS_EI()         asm(" sei ")

//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT)
//------------------------------------------------------------------------------
    extern void _OS_DI_INT (void);
    extern void _OS_RI_INT (void);
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------






#endif




//******************************************************************************
//  END OF FILE osa_avr_iar.h
//******************************************************************************

