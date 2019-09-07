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
 *  File:           osa_avr_winavr.h
 *
 *  Compilers:      WinAVR
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    WinAVR specific definition
 *                  This file directly included in osa.h
 *
 *  History:        13.09.2010 -    File updated
 * 
 *                  06.03.2011 -    Bug fixed: local variables were not saved correctly when
 *                                  task exited through the OS_CheckEvent function
 *
 ************************************************************************************************
 */



#ifndef __OSAWINAVR_H__
#define __OSAWINAVR_H__



#include <avr/io.h>


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

#define _OS_CUR_FLAGS_IN_OS_STATE           // Work with tasks state througth _OS_State

/************************************************************************************************
 *                                                                                              *
 *     Processor specified definitions for system flags                                         *
 *     (see OST_SYSTEM_FLAGS and OST_TCB definition in file "OSA.h")                            *
 *                                                                                              *
 ************************************************************************************************/

#define _OS_SYSTEM_FLAGS_PROC_SPEC()                                                            \
                                                                                                \
        /*5*/   OST_UINT   bTimeout    : 1; /* Event waiting timeout                */          \
        /*6*/   OST_UINT   bGIE_CTemp  : 1; /* Saved GIEH while in critical section */          \

#define _OS_TCB_PROC_SPEC()                                                                     \
                OST_UINT16    nY_Temp;        /* Saved R28:R29 */                               \
                OST_UINT8     c_NumOfTemp;    /* Number of saved temp variables */              \
                OST_UINT8     Temp[4];        /* Saved temp variables */                        \



#define OST_TASK_POINTER   OS_TASKS_BANK OST_TCB *


/************************************************************************************************
 *                                                                                              *
 *     Registers definitions                                                                    *
 *                                                                                              *
 ************************************************************************************************/



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

#ifdef __AVR_2_BYTE_PC__

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

#define _OS_POP()             asm(" pop r22 "); asm(" pop r23 ");


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

//******************************************************************************
//  RAM size depended macros
//******************************************************************************

//------------------------------------------------------------------------------
#if XRAMEND < 0x100
//------------------------------------------------------------------------------

    extern OST_UINT8  _OS_SP_Temp;

    #define     _OS_MOV_Y_SP()                      \
        asm("   in  r28, 0x3D   ");                 \
        asm("   ldi r29, 0      ");

    #define     _OS_MOV_SP_Y()                      \
        asm("   out     0x3D, r28           ");     \

    #define     _OS_STORE_SP()                      \
        asm("   in      r22, 0x3D           ");     \
        asm("   sts     _OS_SP_Temp, r22    ");     \


    #define     _OS_RESTORE_SP()                    \
        asm("   lds     r22, _OS_SP_Temp    ");     \
        asm("   out     0x3D, r22           ");     \

    #define     _OS_CLEAR_Y_HI()    asm("   ldi     r29, 0   ")


//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------

    extern OST_UINT16 _OS_SP_Temp;


    #define _OS_MOV_Y_SP()                          \
        asm("   in  r28, 0x3D   ");                 \
        asm("   in  r29, 0x3E   ");

    #define _OS_MOV_SP_Y()                          \
        asm("   in      r25, 0x3F           ");     \
        asm("   cli                         ");     \
        asm("   out     0x3D, r28           ");     \
        asm("   out     0x3E, r29           ");     \
        asm("   out     0x3F, r25           ");     \

    #define     _OS_STORE_SP()                      \
        asm("   in      r22, 0x3D           ");     \
        asm("   sts     _OS_SP_Temp, r22    ");     \
        asm("   in      r22, 0x3E           ");     \
        asm("   sts     _OS_SP_Temp+1, r22  ");     \


    #define     _OS_RESTORE_SP()                    \
        asm("   lds     r22, _OS_SP_Temp    ");     \
        asm("   lds     r23, _OS_SP_Temp+1  ");     \
        asm("   in      r24, 0x3F           ");     \
        asm("   cli                         ");     \
        asm("   out     0x3D, r22           ");     \
        asm("   out     0x3E, r23           ");     \
        asm("   out     0x3F, r24           ");     \

    #define     _OS_CLEAR_Y_HI()

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------



#define _OS_SAVE_TEMP()   /* Save 4 bytes of stack (dew to WinAVR bug) */               \
    asm("   st      x, r1               "); /* Write zero to NumberOfTemp   */          \
    asm("   lds     r23, _OS_SP_Temp    ");                                             \
    asm("   sub     r23, r28            ");                                             \
    asm("   subi    r23, 2              "); /* Two bytes are used by saved YH:YL */     \
    asm("   breq    SKIP_SAVE_TEMP      ");                                             \
    /* Now R23 = SP - Y */                                                              \
    asm("   cpi     r23, 5              "); /* Check if there are less then */          \
    asm("   brsh    SKIP_SAVE_TEMP      "); /* 4 bytes to save              */          \
    asm("                               ");                                             \
    asm("   st      x+, r23             "); /* Save number of temp bytes    */          \
    asm("   ld      r22, Y+             "); /* Skip unused bytes            */          \
    asm("REPEAT_SAVE_TEMP:              ");                                             \
    asm("   ld      r22, Y+             "); /* Load temp value      */                  \
    asm("   st      x+, r22             "); /* Save temp value      */                  \
    asm("   dec     r23                 ");                                             \
    asm("   brne    REPEAT_SAVE_TEMP    ");                                             \
    asm("SKIP_SAVE_TEMP:                ");                                             \


#define _OS_RESTORE_TEMP()                                                              \
    asm("   ld      r22, x+             "); /* Get number of temp bytes */              \
    asm("   mov     r23, r22            ");                                             \
    /* Copy data */                                                                     \
    asm("   tst     r22                 ");                                             \
    asm("   breq    SKIP_RESTORE_TEMP   ");                                             \
    asm("   ld      r24, Y+             ");                                             \
    asm("REPEAT_RESTORE_TEMP:           ");                                             \
    asm("   ld      r24, X+             ");                                             \
    asm("   st      Y+, r24             ");                                             \
    asm("   dec     r23                 ");                                             \
    asm("   brne    REPEAT_RESTORE_TEMP ");                                             \
    asm("   inc     r22                 ");                                             \
    asm("   sub     r28, r22            ");                                             \
    asm("   sbc     r29, r1             ");                                             \
    asm("SKIP_RESTORE_TEMP:             ");                                             \





//******************************************************************************
//  ROM size depended macros
//******************************************************************************

//------------------------------------------------------------------------------
#if OS_CODE_POINTER_SIZE == 2
//------------------------------------------------------------------------------

    #define _OS_SET_PC()                         \
        asm("   ld      r22, x+         ");     \
        asm("   sts     _OS_State, r22   ");     \
        /* load task entry point */             \
        asm("   ld      r22, x+         ");     \
        asm("   ld      r23, x+         ");     \
        /* load saved Y value               */  \
        asm("   ld      r28, x+         ");     \
        asm("   ld      r29, x+         ");     \
        /* restore SP                       */  \
        asm("   sbrs    r29, 7          ");     \
        asm("   rjmp    SKIP            ");     \
        _OS_MOV_Y_SP();                         \
        asm("SKIP:                      ");     \
        _OS_MOV_SP_Y();                         \
        /* push task entry point into stack */  \
        asm("   push    r22             ");     \
        asm("   push    r23             ");     \


        //asm("   ret                     ");



    #define _OS_SAVE_PC()                       \
        _OS_CLEAR_Y_HI();                       \
        asm("   ld      r22, x+     ");         \
        /* get return address               */  \
        asm("   pop     r22         ");         \
        asm("   pop     r23         ");         \
        /* and save it into TCB             */  \
        asm("   st      x+, r23     ");         \
        asm("   st      x+, r22     ");         \
        /* save Y                           */  \
        asm("   st      x+, r28     ");         \
        asm("   st      x+, r29     ");

    #define _OS_PUSH_BACK()                     \
        /* push return address back to stack */ \
        asm("   push    r23         ");         \
        asm("   push    r22         ");         \

    #define _OS_ADD_X_TEMP()      /*110306*/    \
        asm("   subi    r26, -5     ");         \
        asm("   sbci    r27, -1     ");         \

//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------

    #define _OS_ADD_X_TEMP()      /*110306*/    \
        asm("   subi    r26, -7     ");         \
        asm("   sbci    r27, -1     ");         \
    
    #define _OS_SET_PC()                        \
        asm("   ld      r22, x+         ");     \
        asm("   sts     _OS_State, r22   ");    \
        /* load task entry point */             \
        asm("   ld      r22, x+         ");     \
        asm("   ld      r23, x+         ");     \
        asm("   ld      r24, x+         ");     \
        /* skip highes byte of dwaddress    */  \
        asm("   ld      r25, x+         ");     \
        /* load saved Y value               */  \
        asm("   ld      r28, x+         ");     \
        asm("   ld      r29, x+         ");     \
        /* restore SP                       */  \
        asm("   sbrs    r29, 7          ");     \
        asm("   rjmp    SKIP            ");     \
        _OS_MOV_Y_SP();                         \
        asm("SKIP:                      ");     \
        _OS_MOV_SP_Y();                         \
        /* push task entry point into stack */  \
        asm("   push    r22             ");     \
        asm("   push    r23             ");     \
        asm("   push    r24             ");     \
        asm("   ret                     ");


    #define _OS_SAVE_PC()                        \
        _OS_CLEAR_Y_HI();                       \
        asm("   ld      r22, x+     ");         \
        /* get return address               */  \
        asm("   pop     r22         ");         \
        asm("   pop     r23         ");         \
        asm("   pop     r21         ");         \
        /* and save it into TCB             */  \
        asm("   st      x+, r21     ");         \
        asm("   st      x+, r23     ");         \
        asm("   st      x+, r22     ");         \
        /* write 0 to highest byte of dw addr */\
        asm("   ldi     r22, 0      ");         \
        asm("   st      x+, r22     ");         \
        /* save Y                           */  \
        asm("   st      x+, r28     ");         \
        asm("   st      x+, r29     ");

    #define _OS_PUSH_BACK()                     \
        /* push return address back to stack */ \
        asm("   push    r21         ");         \
        asm("   push    r23         ");         \
        asm("   push    r22         ");


//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------




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

void _OS_JumpToTask (void) __attribute__ ((naked, noinline, returns_twice));



#define _OS_JUMP_TO_TASK()                                              \
        asm("   push  r28     ");                                       \
        asm("   push  r29     ");                                       \
        _OS_JumpToTask();                                               \



#define _OS_BACK_TO_SCEDULER()                                          \
    {                                                                   \
        _OS_SET_FSR_CUR_TASK();                                         \
        asm("   lds     r22, _OS_State   ");                            \
        /* r22 |= (OS_TASK_READY_MASK | OS_TASK_CANCONTINUE_MASK) */    \
        asm("   andi    r22, 0x28       ");                             \
        asm("   in      r24, 0x3F       ");                             \
        asm("   cli                     ");                             \
        asm("   ld      r23, x          ");                             \
        /* r23 &= ~(OS_TASK_READY_MASK | OS_TASK_CANCONTINUE_MASK) */   \
        asm("   andi    r23, 0xD7       ");                             \
        asm("   or      r23, r22        ");                             \
        asm("   st      x, r23          ");                             \
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

void _OS_ReturnSave      (void) __attribute__ ((naked, noinline, returns_twice));
void _OS_ReturnNoSave    (void) __attribute__ ((naked, noinline, returns_twice));
void _OS_EnterWaitMode   (void) __attribute__ ((naked, noinline));
void _OS_EnterWaitModeTO (void) __attribute__ ((naked, noinline));
void _OS_CheckEvent     (OST_UINT bEvent) __attribute__((__naked__, __noinline__, __returns_twice__));
void _OS_SET_FSR_CUR_TASK (void)     __attribute__ ((naked, noinline));


//------------------------------------------------------------------------------
#if FLASHEND < 8196
//------------------------------------------------------------------------------

    #define _OS_JMP(label)         asm("   rjmp    "label)

//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------

    #define _OS_JMP(label)         asm("   jmp     "label)
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------


#define _OS_RETURN_NO_SAVE()        _OS_JMP("_OS_ReturnNoSave")
#define _OS_RETURN_SAVE()           _OS_ReturnSave()
#define _OS_ENTER_WAIT_MODE()       _OS_EnterWaitMode()
#define _OS_ENTER_WAIT_MODE_TO()    _OS_EnterWaitModeTO()
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
        _OS_CurTask->nY_Temp = 0x8000;                                                  \
        _OS_RETURN_NO_SAVE();                                                           \
    }

#define OS_Task_Replace_P(priority, TaskAddr)                                           \
    {                                                                                   \
        *((OS_RAM_NEAR OST_UINT*)&_OS_State) = priority | 0x48; /* bEnable è bReady */  \
        _OS_CurTask->State = _OS_State;                                                 \
        _OS_CurTask->pTaskPointer = (OST_CODE_POINTER)(TaskAddr);                       \
        _OS_CurTask->nY_Temp = 0x8000;                                                  \
        _OS_RETURN_NO_SAVE();                                                           \
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

#define OS_Task_Reserve(TaskName)



/************************************************************************************************
 *                                                                                              *
 *                                     I N T E R R U P T S                                      *
 *                                                                                              *
 ************************************************************************************************/

OST_UINT    OS_DI (void);
void        OS_RI (OST_UINT);

#define     OS_EI()             sei()

extern void __OS_DI_INT (void) __attribute__ ((naked, noinline));
extern void __OS_RI_INT (void) __attribute__ ((naked, noinline));

#define _OS_DI_INT() __OS_DI_INT()
#define _OS_RI_INT() __OS_RI_INT()




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
//  END OF FILE osa_avr_winavr.h
//******************************************************************************

