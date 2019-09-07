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
 *  File:           osa_pic12_htpicc.h
 *
 *  Compilers:      HT-PICC STD
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    HTPICC for PIC12 specific definition
 *                  This file directly included in osa.h
 *
 *  History:        13.09.2010 -    File updated
 *
 ************************************************************************************************
 */



#ifndef __OSAPICC12_H__
#define __OSAPICC12_H__





asm("   psect os_pic12_jmp_table,ovrld,global,class=ENTRY,delta=2   ");
asm("_os_pic12_jmp_table:                                           ");



/************************************************************************************************
 *                                                                                              *
 *     Integer types definitions                                                                * 
 *                                                                                              *
 ************************************************************************************************/

typedef unsigned char   OST_UINT8;
typedef unsigned int    OST_UINT16;
typedef unsigned long   OST_UINT32;
typedef OST_UINT8       OST_UINT;
typedef bit             OST_BOOL;
#define _OST_INT_MASK    7
#define _OST_INT_SHIFT   3
#define _OST_INT_SIZE    8



/************************************************************************************************
 *                                                                                              * 
 *     Location keywords                                                                        * 
 *                                                                                              * 
 ************************************************************************************************/

#define OS_RAM_NEAR             near
#define OS_RAM_BANK0            near
#define OS_RAM_BANK1            bank1
#define OS_RAM_BANK2            bank2
#define OS_RAM_BANK3            bank3


#define _OS_CUR_FLAGS_IN_OS_STATE   // Work with tasks state througth _OS_State variable



/************************************************************************************************
 *                                                                                              * 
 *     Processor specified definitions for system flags                                         * 
 *     (see OST_SYSTEM_FLAGS and OST_TCB definition in file "OSA.h")                            * 
 *                                                                                              * 
 ************************************************************************************************/

#define _OS_SYSTEM_FLAGS_PROC_SPEC()                                                       \
                                                                                           \
        /*5*/   OST_UINT   bTimeout    : 1; /* Event waiting timeout               */      \
        /*6*/   OST_UINT   bUser0      : 1; /* For PIC12: Two user purpose bits    */      \
        /*7*/   OST_UINT   bUser1      : 1; /* For PIC12: Two user purpose bits    */      \


#define _OS_TCB_PROC_SPEC()


/************************************************************************************************
 *                                                                                              *
 *     Registers definitions                                                                    *
 *                                                                                              *
 ************************************************************************************************/

static volatile char _indf @ 0x0;
static volatile char _pcl @ 0x02;
static volatile char _fsr @ 0x4;
static volatile char _status @ 0x3;



/************************************************************************************************
 *                                                                                              *
 *     Constants and types                                                                      *
 *                                                                                              *
 ************************************************************************************************/

#undef OS_SMSG_SIZE
#define OS_SMSG_SIZE    sizeof(OS_SMSG_TYPE)

//------------------------------------------------------------------------------

#if defined(OS_ENABLE_QUEUE) || defined(OS_ENABLE_SQUEUE)
    #define OS_ENABLE_QUEUE
    #undef  OS_ENABLE_SQUEUE
    #define OS_QUEUE_SQUEUE_IDENTICAL
#endif

//------------------------------------------------------------------------------

typedef unsigned char              OS_FSR_TYPE;
typedef unsigned char               OST_CODE_POINTER;









/************************************************************************************************
 *                                                                                              *
 *     Platform macros                                                                          *
 *                                                                                              *
 ************************************************************************************************/

#define OS_CLRWDT()   asm(" clrwdt ");
#define OS_ClrWdt()   asm(" clrwdt ");
#define OS_SLEEP()    asm(" sleep  ");









/************************************************************************************************
 *                                                                                              *
 *     Context switching macros                                                                 *
 *                                                                                              *
 ************************************************************************************************/


#if !defined   (_BANKBITS_)
    #define     _BANKBITS_  2
#endif




//------------------------------------------------------------------------------
// Macros for setting task descriptor's bank
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#if     OS_BANK_TASKS == 0
//------------------------------------------------------------------------------

    #define _OS_OS_SET_TASK_IRP()
    #define _OS_OS_SET_TASK_IRP_0()

//------------------------------------------------------------------------------
#elif   OS_BANK_TASKS == 1
//------------------------------------------------------------------------------

    #define _OS_OS_SET_TASK_IRP()       asm("   bsf     __fsr, 5 ");
    #define _OS_OS_SET_TASK_IRP_0()     asm("   bcf     __fsr, 5 ");

//------------------------------------------------------------------------------
#elif   OS_BANK_TASKS == 2
//------------------------------------------------------------------------------

    #define _OS_OS_SET_TASK_IRP()       asm("   bsf     __fsr, 6 ");
    #define _OS_OS_SET_TASK_IRP_0()     asm("   bcf     __fsr, 6 ");

//------------------------------------------------------------------------------
#elif   OS_BANK_TASKS == 3
//------------------------------------------------------------------------------

    #define _OS_OS_SET_TASK_IRP()       asm("   bsf     __fsr, 5 ");    \
                                        asm("   bsf     __fsr, 6 ");

    #define _OS_OS_SET_TASK_IRP_0()     asm("   bcf     __fsr, 5 ");    \
                                        asm("   bcf     __fsr, 6 ");

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------




//------------------------------------------------------------------------------
// Macros for setting OS variables bank
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#if     OS_BANK_OS == 0
//------------------------------------------------------------------------------

    #define _OS_SET_OS_IRP()
    #define _OS_SET_OS_IRP_0()

//------------------------------------------------------------------------------
#elif   OS_BANK_OS == 1
//------------------------------------------------------------------------------

    #define _OS_SET_OS_IRP()    asm("   bsf     __fsr, 5 ");
    #define _OS_SET_OS_IRP_0()  asm("   bcf     __fsr, 5 ");

//------------------------------------------------------------------------------
#elif   OS_BANK_OS == 2
//------------------------------------------------------------------------------

    #define _OS_SET_OS_IRP()    asm("   bsf     __fsr, 6 ");
    #define _OS_SET_OS_IRP_0()  asm("   bcf     __fsr, 6 ");

//------------------------------------------------------------------------------
#elif   OS_BANK_OS == 3
//------------------------------------------------------------------------------

    #define _OS_SET_OS_IRP()    asm("   bsf     __fsr, 5 ");    \
                                asm("   bsf     __fsr, 6 ");

    #define _OS_SET_OS_IRP_0()  asm("   bcf     __fsr, 5 ");    \
                                asm("   bcf     __fsr, 6 ");

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------







//------------------------------------------------------------------------------
#if _BANKBITS_ == 0
    #define _OS_PIC12_AND_1F()
    #define _OS_PIC12_AND_7F()  asm("   andlw   0x1F    ");
#endif

//------------------------------------------------------------------------------
#if _BANKBITS_ == 1
    #define _OS_PIC12_AND_1F()  asm("   andlw   0x1F    ");
    #define _OS_PIC12_AND_7F()  asm("   andlw   0x3F    ");
#endif

//------------------------------------------------------------------------------
#if _BANKBITS_ == 2
    #define _OS_PIC12_AND_1F()  asm("   andlw   0x1F    ");
    #define _OS_PIC12_AND_7F()  asm("   andlw   0x7F    ");
#endif


//------------------------------------------------------------------------------



#define OS_SET_PAGE_ADDR0()                                     \
    asm("   global _os_pic12_jmp_table      ");                 \
    asm("   dw  0x4A3   | ((_os_pic12_jmp_table & 0x200)>>1)");


#define OS_SET_PAGE_ADDR1()                                     \
    asm("   global _os_pic12_jmp_table      ");                 \
    asm("   dw  0x4C3   | ((_os_pic12_jmp_table & 0x400)>>2)");


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

#define _OS_JUMP_TO_TASK()                                                  \
    {                                                                       \
        _fsr = ((OS_FSR_TYPE)_OS_CurTask);                                  \
        asm("   movf    __indf, w    ");                                    \
        _OS_OS_SET_TASK_IRP_0();                                            \
        asm("   movwf   __OS_State   ");                                    \
        _fsr = ((OS_FSR_TYPE)_OS_CurTask) + 1;                              \
        asm("   movf    __indf, w    ");                                    \
        asm("   global  OS_JumpToTask_W    ");                              \
        asm("OS_JumpToTask_W:       ");                                     \
        asm("   clrf    __fsr       ");                                     \
        OS_SET_PAGE_ADDR0();                                                \
        OS_SET_PAGE_ADDR1();                                                \
        asm("   movwf   __pcl        ");                                    \
    }


//------------------------------------------------------------------------------






/*
 ********************************************************************************
 *                                                                              
 *  _OS_BACK_TO_SCEDULER()                                                      
 *                                                                              
 *------------------------------------------------------------------------------
 *                                                                              
 *  description:    Save task return address.                                   
 *                                                                              
 *  parameters:     WREG    - address low byte                                  
 *                                                                                   
 ********************************************************************************
 */

#define _OS_BACK_TO_SCEDULER()                                                          \
{                                                                                       \
    asm("global  _OS_ReturnSave   ");                                                   \
    asm("global  _OS_ReturnSave3   ");                                                  \
    asm("global  _OS_EnterWaitMode  ");                                                 \
    asm("global  _OS_EnterWaitModeTO  ");                                               \
    asm("global  OS_JumpToTask_W    ");                                                 \
                                                                                        \
    asm("_OS_EnterWaitModeTO:  ");                                                      \
    asm("_OS_EnterWaitMode:  ");                                                        \
    asm("   bcf   __OS_State, 3      ");       /* bReady = 0 */                         \
    asm("   bsf   __status, 1        ");      /* =1 - we need to return to task */      \
    asm("   btfss __status, 1        ");                                                \
    asm("_OS_ReturnSave:   ");                                                          \
    asm("   bcf   __status, 1    ");          /* =0 - we don't need to return to task */\
                                                                                        \
    /* Now FSR = 0, W = task return point */                                            \
                                                                                        \
    asm("   movwf   __OS_Temp     ");                                                   \
    asm("   incf    __OS_CurTask, w ");                                                 \
    _OS_PIC12_AND_1F();                                                                 \
    asm("   movwf   __fsr    ");                                                        \
    asm("   movf    __OS_Temp, w");                                                     \
    _OS_OS_SET_TASK_IRP();                                                              \
    asm("   movwf   __indf   ");                                                        \
    _OS_OS_SET_TASK_IRP_0();                                                            \
    asm("   btfsc   __status, 1 ");                                                     \
    asm("   goto    OS_JumpToTask_W ");                                                 \
    asm("_OS_ReturnSave3:      ");                                                      \
                                                                                        \
    /* Save state      */                                                               \
                                                                                        \
    asm("   movf    __OS_CurTask, w");                                                  \
    _OS_PIC12_AND_1F();                                                                 \
    asm("   movwf   __fsr   ");                                                         \
    asm("   movf    __OS_State, w");                                                    \
    asm("   andlw   0x38    ");             /* bReady, bCanContinue and             */  \
                                            /* !!!bDelay!!! (!!! - only for PICC12) */  \
    _OS_OS_SET_TASK_IRP();                                                              \
    asm("   bcf     __indf, 3");            /* bReady */                                \
    asm("   bcf     __indf, 4");            /* bDelay */                                \
    asm("   bcf     __indf, 5");            /* bCanContinue */                          \
    asm("   iorwf   __indf, f");                                                        \
    _OS_OS_SET_TASK_IRP_0();                                                            \
}

//------------------------------------------------------------------------------








/************************************************************************************************
 *                                                                                              *
 *     Return from task to OS_Sched macros                                                      *
 *                                                                                              *
 ************************************************************************************************/

extern OST_BOOL _OS_CheckEvent (char);

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


#define _OS_RETURN_SAVE()       {                                               \
                                    _OS_State.bReady = 1;                       \
                                    asm(" _goto__OS_ReturnSave ");              \
                                }                                               \

#define _OS_RETURN_NO_SAVE()        asm(" _goto__OS_ReturnSave3 ");

#define _OS_ENTER_WAIT_MODE()   {                                                       \
                                    if (!_OS_State.bDelay) _OS_State.bCanContinue = 0;  \
                                    asm(" _call__OS_EnterWaitMode ");                   \
                                }                                                       \

#define _OS_ENTER_WAIT_MODE_TO()                                                \
                                {                                               \
                                    _OS_State.bCanContinue = 1;                 \
                                    asm(" _call__OS_EnterWaitModeTO ");         \
                                }                                               \

#define _OS_CHECK_EVENT(event) if (!_OS_CheckEvent(event)) _OS_RETURN_NO_SAVE()

//------------------------------------------------------------------------------

#define _OS_SET_FSR_CUR_TASK_W()
#define _OS_SET_FSR_CUR_TASK()

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/************************************************************************/
/* Add task address in jump table                                       */
/************************************************************************/
    asm("_OS_Add_TaskAddr               macro   TaskAddr");
    asm("   local   label2                              ");
    asm("                                               ");
    asm("   movlw   low(label2)                         ");
    asm("   movwf   __OS_Temp                           ");
    asm("   psect   os_pic12_jmp_table                  ");
    asm("                                               ");
    asm("label2:                                        ");
    asm("   ljmp    TaskAddr                            ");
    asm("                                               ");
    asm("   endm                                        ");

//------------------------------------------------------------------------------
/************************************************************************/
/* Exit task and save context                                           */
/************************************************************************/
    asm("_goto__OS_ReturnSave         macro             ");
    asm("                                               ");
    asm("   local   task_ret_point, label2              ");
    asm("   global  _OS_ReturnSave                      ");
    asm("                                               ");
    asm("   dw      0xC00 | low(label2)                 ");
    asm("   ljmp    _OS_ReturnSave                      ");
    asm("                                               ");
    asm("task_ret_point:                                ");
    asm("   psect   os_pic12_jmp_table                  ");
    asm("                                               ");
    asm("label2:                                        ");
    asm("   ljmp    task_ret_point                      ");
    asm("                                               ");
    asm("   endm                                        ");

//------------------------------------------------------------------------------
/************************************************************************/
/* Exit task without saving context                                     */
/************************************************************************/
    asm("_goto__OS_ReturnSave3        macro             ");
    asm("                                               ");
    asm("   global  _OS_ReturnSave3                     ");
    asm("                                               ");
    asm("   ljmp    _OS_ReturnSave3                     ");
    asm("                                               ");
    asm("   endm                                        ");

//------------------------------------------------------------------------------
/************************************************************************/
/* Save context with clearing bReady flag                               */
/************************************************************************/
    asm("_call__OS_EnterWaitMode            macro       ");
    asm("                                               ");
    asm("   local   task_ret_point, label2              ");
    asm("   global  _OS_EnterWaitMode                   ");
    asm("                                               ");
    asm("   dw      0xC00 | low(label2)                 ");
    asm("   ljmp    _OS_EnterWaitMode                   ");
    asm("                                               ");
    asm("task_ret_point:                                ");
    asm("   psect   os_pic12_jmp_table                  ");
    asm("                                               ");
    asm("label2:    ljmp    task_ret_point              ");
    asm("                                               ");
    asm("   endm                                        ");

//------------------------------------------------------------------------------
/************************************************************************/
/* Save context with clearing bReady and setting bCanContinue           */
/************************************************************************/
    asm("_call__OS_EnterWaitModeTO      macro           ");
    asm("                                               ");
    asm("   local   task_ret_point, label2              ");
    asm("   global  _OS_EnterWaitModeTO                 ");
    asm("                                               ");
    asm("   dw      0xC00 | low(label2)                 ");
    asm("   ljmp    _OS_EnterWaitModeTO                 ");
    asm("task_ret_point:                                ");
    asm("   psect   os_pic12_jmp_table                  ");
    asm("label2:    ljmp    task_ret_point              ");
    asm("                                               ");
    asm("   endm                                        ");

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

#define OS_Task_Replace(priority, TaskName)                                     \
{                                                                               \
    OS_Task_Reserve(TaskName);                                                  \
    *((OST_UINT*)&_OS_State) = priority | 0x48; /* bEnable è bReady */          \
    _fsr = (OS_FSR_TYPE*)_OS_CurTask;                                           \
    _indf = priority | 0x48; /* bEnable è bReady */                             \
    _fsr++;                                                                     \
    _indf = ((OST_CODE_POINTER)TaskName) & 0xFF;                                \
    _fsr = 0;                                                                   \
    _OS_ReturnNoSave();                                                         \
}

#define OS_Task_Replace_P(priority, TaskAddr)                                   \
{                                                                               \
    *((OST_UINT*)&_OS_State) = priority | 0x48; /* bEnable è bReady */          \
    _fsr = (OS_FSR_TYPE*)_OS_CurTask;                                           \
    _indf = priority | 0x48; /* bEnable è bReady */                             \
    _fsr++;                                                                     \
    _indf = ((OST_UINT8)TaskAddr) & 0xFF;                                       \
    _fsr = 0;                                                                   \
    _OS_ReturnNoSave();                                                         \
}




/************************************************************************/
/*                                                                      */
/* Create task                                                          */
/*                                                                      */
/************************************************************************/

#define OS_Task_Create(priority, TaskName)                              \
    {                                                                   \
        OS_Task_Reserve(TaskName);                                      \
        _OS_Task_Create(priority, _OS_Temp);                            \
    }


/************************************************************************/
/*                                                                      */
/* Create task by indirrect address                                     */
/*                                                                      */
/************************************************************************/

#define OS_Task_Reserve(TaskName)                                       \
    {                                                                   \
        asm("\t global _main       ");                                  \
        asm("\t global _" #TaskName);                                   \
        asm("\t fncall _main,_" #TaskName);                             \
        asm("_OS_Add_TaskAddr   _"#TaskName);                           \
    }




/************************************************************************************************
 *                                                                                              *
 *                                    I N T E R R U P T S                                       *
 *                                                                                              *
 ************************************************************************************************/

#define OS_DI()     0
#define OS_RI(temp)
#define OS_EI()

//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT)
//------------------------------------------------------------------------------
    #define _OS_DI_INT()
    #define _OS_RI_INT()
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#define OS_EnterCriticalSection_DEFINED
#define OS_LeaveCriticalSection_DEFINED
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
 * MACROS FOR OLD STYLE STATIC TIMERS WORK
 *
 ******************************************************************************************/

#define __OS_Timer8Work(TIMER_ID,L)                                                             \
{                                                                                               \
    if ((OS_Timeouts[(TIMER_ID)>>3]&(1<<((TIMER_ID)&7)))) {                                     \
        if (!++OS_Timers8[TIMER_ID])                                                            \
            OS_Timeouts[(TIMER_ID)>>3]&=~(1<<((TIMER_ID)&7));                                   \
    }                                                                                           \
}


#define __OS_Timer16Work(TIMER_ID,L)                                                            \
{                                                                                               \
    if ((OS_Timeouts[(TIMER_ID+_OS_TIMER16_POS)>>3]&(1<<((TIMER_ID+_OS_TIMER16_POS)&7))))       \
    if (!++*((OS_TIMERS16_BANK char*)&OS_Timers16[TIMER_ID]+0)) {                               \
        if (!++*((OS_TIMERS16_BANK char*)&OS_Timers16[TIMER_ID]+1)) {                           \
            OS_Timeouts[(TIMER_ID+_OS_TIMER16_POS)>>3]&=~(1<<((TIMER_ID+_OS_TIMER16_POS)&7));   \
        };                                                                                      \
    };                                                                                          \
}


#define __OS_Timer24Work(TIMER_ID,L)                                                            \
{                                                                                               \
    if ((OS_Timeouts[(TIMER_ID+_OS_TIMER24_POS)>>3]&(1<<((TIMER_ID+_OS_TIMER24_POS)&7))))       \
    if (!++*((OS_TIMERS24_BANK char*)&OS_Timers24[TIMER_ID]+0)) {                               \
        if (!++*((OS_TIMERS24_BANK char*)&OS_Timers24[TIMER_ID]+1)) {                           \
            OS_Timeouts[(TIMER_ID+_OS_TIMER24_POS)>>3]&=~(1<<((TIMER_ID+_OS_TIMER24_POS)&7));   \
        };                                                                                      \
    };                                                                                          \
}



#define __OS_Timer32Work(TIMER_ID,L)                                                            \
    if ((OS_Timeouts[(TIMER_ID+_OS_TIMER32_POS)>>3]&(1<<((TIMER_ID+_OS_TIMER32_POS)&7))))       \
    if (!++*((OS_TIMERS32_BANK char*)&OS_Timers32[TIMER_ID]+0)) {                               \
        if (!++*((OS_TIMERS32_BANK char*)&OS_Timers32[TIMER_ID]+1)) {                           \
            if (!++*((OS_TIMERS32_BANK char*)&OS_Timers32[TIMER_ID]+2)) {                       \
                if (!++*((OS_TIMERS32_BANK char*)&OS_Timers32[TIMER_ID]+3)) {                   \
                    OS_Timeouts[(TIMER_ID+_OS_TIMER32_POS)>>3] &=                               \
                        ~(1<<((TIMER_ID+_OS_TIMER32_POS)&7));                                   \
                };                                                                              \
            };                                                                                  \
        };                                                                                      \
    };                                                                                          \





/******************************************************************************************
 *
 * MACROS FOR WORK WITH TASK TIMERS
 *
 ******************************************************************************************/


#define ASM_OST_TCB_SIZE_CONST  asm(" ");
#define ASM_SET_BANK            asm(" ");


//------------------------------------------------------------------------------
#if   OS_TTIMER_SIZE == 1
//------------------------------------------------------------------------------

    #define __OS_TtimerWork(TASK_ID)                                                            \
        {                                                                                       \
            if (_OS_Tasks[TASK_ID].State.bDelay) {                                              \
                if (!++_OS_Tasks[TASK_ID].Timer)                                                \
                    _OS_Tasks[TASK_ID].State.bDelay = 0;                                        \
            }                                                                                   \
        }

//------------------------------------------------------------------------------
#elif OS_TTIMER_SIZE == 2
//------------------------------------------------------------------------------

    #define __OS_TtimerWork(TASK_ID)                                                            \
        {                                                                                       \
            if (_OS_Tasks[TASK_ID].State.bDelay)                                                \
            if (!++*((OS_TASKS_BANK char*)&_OS_Tasks[TASK_ID].Timer+0)) {                       \
                if (!++*((OS_TASKS_BANK char*)&_OS_Tasks[TASK_ID].Timer+1)) {                   \
                    _OS_Tasks[TASK_ID].State.bDelay = 0;                                        \
                }                                                                               \
            }                                                                                   \
        }

//------------------------------------------------------------------------------
#elif OS_TTIMER_SIZE == 4
//------------------------------------------------------------------------------

    #define __OS_TtimerWork(TASK_ID)                                                            \
        {                                                                                       \
            if (_OS_Tasks[TASK_ID].State.bDelay)                                                \
            if (!++*((OS_TASKS_BANK char*)&_OS_Tasks[TASK_ID].Timer+0)) {                       \
                if (!++*((OS_TASKS_BANK char*)&_OS_Tasks[TASK_ID].Timer+1)) {                   \
                    if (!++*((OS_TASKS_BANK char*)&_OS_Tasks[TASK_ID].Timer+2)) {               \
                        if (!++*((OS_TASKS_BANK char*)&_OS_Tasks[TASK_ID].Timer+3)) {           \
                            _OS_Tasks[TASK_ID].State.bDelay = 0;                                \
                        }                                                                       \
                    }                                                                           \
                }                                                                               \
            }                                                                                   \
        }

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------


#define __OS_TtimersWorkSize    __OS_TtimersWorkSpeed
#define __OS_StimersWorkSize    __OS_StimersWorkSpeed

//------------------------------------------------------------------------------
/******************************************************************************************/












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

    #define _OS_INC_DTIMER()                                 \
        {                                                   \
            /* Now W = fsr */                               \
            asm("   incf    __fsr, f    ");                 \
            asm("   incf    __fsr, f    ");                 \
            asm("   bcf     __status, 2 ");                 \
            asm("   incf    __indf, f   ");                 \
            asm("   movwf   __fsr       ");                 \
            if (_status & 4) _indf |= 1;                    \
        }

//------------------------------------------------------------------------------
#elif OS_DTIMER_SIZE == 2
//------------------------------------------------------------------------------

    #define _OS_INC_DTIMER()                                 \
        {                                                   \
            asm("   bcf     __status, 0 ");                 \
            /* Now W = fsr */                               \
            asm("   incf    __fsr, f    ");                 \
            asm("   incf    __fsr, f    ");                 \
            asm("   incfsz  __indf, f   ");                 \
            asm("   goto    $ + 5       ");                 \
            asm("   incf    __fsr, f    ");                 \
            asm("   incfsz  __indf, f   ");                 \
            asm("   goto    $ + 2       ");                 \
            asm("   bsf     __status, 0 ");                 \
            asm("   movwf   __fsr       ");                 \
            if (_status & 1) _indf |= 1;                    \
        }

//------------------------------------------------------------------------------
#elif OS_DTIMER_SIZE == 4
//------------------------------------------------------------------------------

    #define _OS_INC_DTIMER()                                 \
        {                                                   \
            asm("   bcf     __status, 0 ");                 \
            /* Now W = fsr */                               \
            asm("   incf    __fsr, f    ");                 \
            asm("   incf    __fsr, f    ");                 \
            asm("   incfsz  __indf, f   ");                 \
            asm("   goto    $ + 11      ");                 \
            asm("   incf    __fsr, f    ");                 \
            asm("   incfsz  __indf, f   ");                 \
            asm("   goto    $ + 8       ");                 \
            asm("   incf    __fsr, f    ");                 \
            asm("   incfsz  __indf, f   ");                 \
            asm("   goto    $ + 5       ");                 \
            asm("   incf    __fsr, f    ");                 \
            asm("   incfsz  __indf, f   ");                 \
            asm("   goto    $ + 2       ");                 \
            asm("   bsf     __status, 0 ");                 \
            asm("   movwf   __fsr       ");                 \
            if (_status & 1) _indf |= 1;                    \
        }

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------


#define __OS_DtimersWork()                                  \
    {                                                       \
        _fsr = (OS_FSR_TYPE) &_OS_DTimers;                  \
                                                            \
        /* Avoid to use CheckIndfReady because of FSR */    \
        /* can be cleared                             */    \
                                                            \
        while (_indf & 8)               /* bEnable */       \
        {                                                   \
            _fsr++;                                         \
            _fsr = _indf;                                   \
            if (!(_indf & 4)) continue; /* Run      */      \
            _OS_INC_DTIMER();                                \
        }                                                   \
    }


//------------------------------------------------------------------------------
#endif  // OS_ENABLE_DTIMER
//------------------------------------------------------------------------------








#endif  //  __OSAPICC12_H__





//******************************************************************************
//  END OF FILE osa_pic12_htpicc.h
//******************************************************************************

