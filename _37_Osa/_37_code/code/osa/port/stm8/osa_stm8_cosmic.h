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
 *  File:           osa_stm8_cosmic.h
 *
 *  Compilers:      Cosmic
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    Cosmic for STM8 specific definition
 *                  This file directly included in osa.h
 *
 *  History:        13.09.2010 -    File updated
 *
 ************************************************************************************************
 */


#ifndef __OSACOSMICSTM8_H__
#define __OSACOSMICSTM8_H__


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

#define OS_RAM_NEAR     @tiny
#define OS_RAM_BANK0    @tiny
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

#define _OS_SYSTEM_FLAGS_PROC_SPEC()                                                          \
                                                                                              \
        /*5*/   OST_UINT   bTimeout    : 1; /* Event waiting timeout                */        \
        /*6*/   OST_UINT   bI1_CTemp   : 1; /* Saved I1 while in critical section */          \
        /*7*/   OST_UINT   bI0_CTemp   : 1; /* Saved I0 while in critical section */          \

#define _OS_TCB_PROC_SPEC()                                                 \
                OST_UINT16   nSP_Temp;       /* Stack pointer temp   */     \


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

#define OST_CODE_POINTER        OST_UINT16
#define OS_CODE_POINTER_SIZE    2





/************************************************************************************************
 *                                                                                              *
 *     Platform macros                                                                          *
 *                                                                                              *
 ************************************************************************************************/



//#define OS_CLRWDT() 
//#define OS_SLEEP()              





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
        _asm("   ldw     X, Y            ");    \
        _asm("   ldw     X, (1,X)        ");    \
        _asm("   pushw   X               ");    \
        _asm("   ret                     ");    \

    #define _OS_Restore_TaskSP()                \
        _asm("   ldw     X, Y            ");    \
        _asm("   ldw     X, (3,X)        ");    \
        _asm("   tnzw    X               ");    \
        _asm("   jreq    SKIP_SP         ");    \
        _asm("   ldw     SP, X           ");    \
        _asm("SKIP_SP:                   ");    \


    #define _OS_SAVE_PC()                       \
        _asm("   popw    X               ");    \
        _asm("   ldw     (1,Y), X        ");    \


    #define _OS_Store_TaskSP()                  \
        _asm("   ldw     X, SP           ");    \
        _asm("   ldw     (3,Y), X        ");    \

    #define _OS_Store_TaskSP2()                 \
        _asm("   ldw     X, SP           ");    \
        _asm("   addw    X, #2           ");    \
        _asm("   ldw     (3,Y), X        ");    \

//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------

#error "OSA error #25: STM8 with ROM > 64K are not supported yet"

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------




#define     _OS_STORE_SP()                          \
    {                                               \
        _asm("   ldw     X, SP           ");        \
        _asm("   ldw     __OS_SP_Temp, X  ");       \
    }

#define     _OS_RESTORE_SP()                        \
    {                                               \
        _asm("   ldw     X, __OS_SP_Temp  ");       \
        _asm("   ldw     SP, X           ");        \
    }

#define     _OS_POP()       _asm("   popw    X   ")
#define     _OS_PUSH()      _asm("   pushw   X   ")











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




#define _OS_JUMP_TO_TASK()    _OS_JumpToTask();

#define _OS_BACK_TO_SCEDULER()                                          \
    {                                                                   \
        _OS_SET_FSR_CUR_TASK();                                         \
        /* cli() */                                                     \
        _asm("   push    CC              ");                            \
        _asm("   pop     __OS_Temp       ");                            \
        _asm("   sim                     ");                            \
        /* _OS_CurTask->State &= 0xD7; */                               \
        /* 0xD7 = ~(OS_TASK_READY_MASK | OS_TASK_CANCONTINUE_MASK) */   \
        _asm("   ld      A, (Y)          ");                            \
        _asm("   and     A, #0xD7        ");                            \
        _asm("   ld      (Y), A          ");                            \
        /* _OS_CurTask->State |= _OS_State & 0x28; */                   \
        /* 0x28 = (OS_TASK_READY_MASK | OS_TASK_CANCONTINUE_MASK) */    \
        _asm("   ld      A, #0x28        ");                            \
        _asm("   and     A, __OS_State    ");                           \
        _asm("   or      A, (Y)          ");                            \
        _asm("   ld      (Y), A          ");                            \
        /* ri() */                                                      \
        _asm("   push    __OS_Temp       ");                            \
        _asm("   pop     CC              ");                            \
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

void _OS_JumpToTask         (void);
void _OS_ReturnSave         (void);
void _OS_ReturnNoSave       (void);
void _OS_EnterWaitMode      (void);
void _OS_EnterWaitModeTO    (void);
void _OS_CheckEvent         (OST_UINT);




#define _OS_RETURN_SAVE()           _OS_ReturnSave()
#define _OS_ENTER_WAIT_MODE()       _OS_EnterWaitMode()
#define _OS_ENTER_WAIT_MODE_TO()    _OS_EnterWaitModeTO()
#define _OS_RETURN_NO_SAVE()        _OS_ReturnNoSave()
#define _OS_CHECK_EVENT(event)      _OS_CheckEvent(event)



#if     OS_BANK_TASKS == 0

    #define _OS_SET_FSR_CUR_TASK()              \
            _asm("  clrw    Y               "); \
            _asm("  ld      A, __OS_CurTask "); \
            _asm("  ld      YL, A           ");

#else

    #define _OS_SET_FSR_CUR_TASK()      _asm("   ldw     Y, __OS_CurTask     ");

#endif

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
        _OS_CurTask->nSP_Temp = 0x0000;                                                 \
        _OS_RETURN_NO_SAVE();                                                           \
    }


#define OS_Task_Replace_P(priority, TaskAddr)                                           \
    {                                                                                   \
        *((OS_RAM_NEAR OST_UINT*)&_OS_State) = priority | 0x48; /* bEnable è bReady */  \
        _OS_CurTask->State = _OS_State;                                                 \
        _OS_CurTask->pTaskPointer = (OST_CODE_POINTER)(TaskAddr);                       \
        _OS_CurTask->nSP_Temp = 0x0000;                                                 \
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

#define OS_Task_Reserve(TaskName)       // Empty macro for this compiler



/************************************************************************************************
 *                                                                                              *
 *                                     I N T E R R U P T S                                      *
 *                                                                                              *
 ************************************************************************************************/

extern char OS_DI (void);
extern void OS_RI (char);
#define     OS_EI()         _asm(" rim ")

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


//******************************************************************************
//
// At this time the Cosmic compiler does not allow to set bits included structure
// by the pointer, like "_OS_QtimerWork->Flags.bTimeout |= 1".
//
// By this reason here we redefine all macros that use bit fields in pointers
// (see the *bug resolving* markers)
//
//******************************************************************************

//------------------------------------------------------------------------------
#ifdef OS_ENABLE_DTIMERS
//------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    #define _OS_DtimersWork_DEFINED
    //------------------------------------------------------------------------------

        // At this time the Cosmic compiler does not allow to set bits included structure
        // by the pointer that defined as a global variable,
        // like "_OS_QtimerWork->Flags.bTimeout |= 1".
        //
        // By this reason here we redefine then macro for QTimers working
        // See *bug resolving*

        #define __OS_DtimersWork()                                              \
            {                                                                   \
                OST_DTIMER *_OS_DtimerWork;     /*bug resolving*/               \
                _OS_DtimerWork = (OST_DTIMER*)&_OS_DTimers;                     \
                while (_OS_DtimerWork->Flags.bNextEnable)                       \
                {                                                               \
                  _OS_DtimerWork = (OST_DTIMER*)_OS_DtimerWork->Next;           \
                  if (!(++_OS_DtimerWork->Timer))                               \
                    _OS_DtimerWork->Flags.bTimeout = 1;                         \
                }                                                               \
            }

//------------------------------------------------------------------------------
#endif  // OS_ENABLE_DTIMERS
//------------------------------------------------------------------------------




//------------------------------------------------------------------------------
#ifdef OS_ENABLE_QTIMERS
//------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    #define _OS_QtimersWork_DEFINED
    //------------------------------------------------------------------------------

    #define __OS_QtimersWork()                                              \
        if (_OS_Qtimers.Flags.bNextEnable)                                  \
        {                                                                   \
            OST_QTIMER *_OS_QtimerWork;        /*bug resolving*/            \
            _OS_QtimerWork = (OST_QTIMER*)_OS_Qtimers.Next;                 \
            _OS_QtimerWork->Timer++;                                        \
            while (!_OS_QtimerWork->Timer)                                  \
            {                                                               \
                _OS_QtimerWork->Flags.bTimeout = 1;                         \
                _OS_QtimerWork->Flags.bActive  = 0;                         \
                if (!_OS_QtimerWork->Flags.bNextEnable)                     \
                {                                                           \
                    _OS_Qtimers.Flags.bNextEnable = 0;                      \
                    break;                                                  \
                }                                                           \
                _OS_Qtimers.Next = _OS_QtimerWork->Next;                    \
                _OS_QtimerWork = (OST_QTIMER*)_OS_Qtimers.Next;             \
            }                                                               \
        }                                                                   \

//------------------------------------------------------------------------------
#endif  // OS_ENABLE_QTIMER
//------------------------------------------------------------------------------






#endif  // _OSA_STM8_COSMIC_H

//******************************************************************************
//  END OF FILE osa_stm8_cosmic.h
//******************************************************************************

