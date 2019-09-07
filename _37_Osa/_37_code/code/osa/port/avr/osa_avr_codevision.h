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
 *  File:           osa_avr_codevision.h
 *
 *  Compilers:      CodeVision AVR
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    CodeVision AVR specific definition
 *                  This file directly included in osa.h
 *
 *  History:        13.09.2010 -    File updated
 *
 ************************************************************************************************
 */



#ifndef __OSACODEVISION_H__
#define __OSACODEVISION_H__




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


#define OS_CLRWDT()             asm("  wdr   ")
#define OS_SLEEP()              asm("  sleep   ")








/************************************************************************************************
 *                                                                                              *
 *     Context switching macros                                                                 *
 *                                                                                              *
 ************************************************************************************************/

#define _OS_PUSH_Y()        #asm("   push  r28     ");  \
                            #asm("   push  r29     ");

#define _OS_POP_Y()         #asm("   push  r28     ");  \
                            #asm("   push  r29     ");

#define _OS_PUSH()          #asm("   push  r22     ");  \
                            #asm("   push  r23     ");

#define _OS_POP()           #asm("   push  r23     ");  \
                            #asm("   push  r22     ");




//******************************************************************************
//  ROM size depended macros
//******************************************************************************

//------------------------------------------------------------------------------
#if OS_CODE_POINTER_SIZE == 2
//------------------------------------------------------------------------------

    #define _OS_SET_PC()                        \
        /* load task entry point        */      \
        #asm("  ld      r22, x+         ");     \
        #asm("  ld      r23, x+         ");     \
                                                \
        /* load saved Y value           */      \
        #asm("  ld      r28, x+         ");     \
        #asm("  ld      r29, x+         ");     \
                                                \
        _OS_PUSH();                             \
        #asm("  ret                     ");



    #define _OS_SAVE_PC()                       \
        #asm("   ld      r22, x+     ");        \
        /* get return address               */  \
        _OS_POP();                              \
        /* and save it into TCB             */  \
        #asm("   st      x+, r23     ");        \
        #asm("   st      x+, r22     ");        \
        /* save Y                           */  \
        #asm("   st      x+, r28     ");        \
        #asm("   st      x+, r29     ");


//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------

    #define _OS_SET_PC()                        \


    #define _OS_SAVE_PC()                       \

    #define _OS_PUSH_BACK()                     \


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

void    _OS_JumpToTask (void);



#define _OS_JUMP_TO_TASK()      _OS_JumpToTask()

#define _OS_BACK_TO_SCEDULER()                              \
        _OS_State &= 0x28;                                  \
        _OS_Temp = OS_DI();                                 \
        (unsigned char)_OS_CurTask->State &= ~0x28;         \
        (unsigned char)_OS_CurTask->State |= _OS_State;     \
        OS_RI(_OS_Temp);                                    \
        _OS_POP_Y();



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

void _OS_ReturnSave      (void);
void _OS_ReturnNoSave    (void);
void _OS_EnterWaitMode   (void);
void _OS_EnterWaitModeTO (void);
//void _OS_CheckEvent      (OST_UINT bEvent);


#define _OS_RETURN_NO_SAVE()        #asm("   jmp     __OS_ReturnNoSave") 
#define _OS_RETURN_SAVE()           _OS_ReturnSave()
#define _OS_ENTER_WAIT_MODE()       _OS_EnterWaitMode()
#define _OS_ENTER_WAIT_MODE_TO()    _OS_EnterWaitModeTO()
#define _OS_CHECK_EVENT(event)      _OS_CheckEvent(event)


#if defined(_MODEL_TINY_) || defined(_MODEL_MEDIUM_)

    #define _OS_SET_FSR_CUR_TASK()      #asm(" lds r26, __OS_CurTask     "); \
                                        #asm(" clr r27                  ");

#else   // defined(_MODEL_SMALL_) || defined(_MODEL_LARGE_)

    #define _OS_SET_FSR_CUR_TASK()      #asm(" lds r26, __OS_CurTask     "); \
                                        #asm(" lds r27, __OS_CurTask+1   ");

#endif








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

#ifdef OS_ENABLE_INT
    extern void __OS_DI_INT (void);
    extern void __OS_RI_INT (void);

    #define _OS_DI_INT() __OS_DI_INT()
    #define _OS_RI_INT() __OS_RI_INT()
#endif



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
//  END OF FILE osa_avr_codevision.h
//******************************************************************************

