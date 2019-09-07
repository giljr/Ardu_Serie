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
 *  File:           osa_pic16_htpicc.h
 *
 *  Compilers:      HT-PICC STD
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    HTPICC for PIC16 specific definition
 *                  This file directly included in osa.h
 *
 *  History:        13.09.2010 -    File updated
 *
 ************************************************************************************************
 */


#ifndef __OSAPICC16_H__
#define __OSAPICC16_H__


#include <pic.h>

#if _HTC_EDITION_ == __PRO__
//#define __OSAPICCPRO__
#endif


/************************************************************************************************
 *                                                                                              *
 *     Integer types definitions                                                                *
 *                                                                                              *
 ************************************************************************************************/

typedef unsigned char   OST_UINT8;
typedef unsigned int    OST_UINT16;
typedef unsigned long   OST_UINT32;
typedef OST_UINT8     OST_UINT;
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

#define _OS_CUR_FLAGS_IN_FSR        // Work with tasks state througth FSR


/************************************************************************************************
 *                                                                                              *
 *     Processor specified definitions for system flags                                         *
 *     (see OST_SYSTEM_FLAGS and OST_TCB definition in file "OSA.h")                            *
 *                                                                                              *
 ************************************************************************************************/

#define _OS_SYSTEM_FLAGS_PROC_SPEC()                                                       \
                                                                                           \
        /*5*/   OST_UINT   bTimeout    : 1; /* Event waiting timeout               */      \
        /*6*/   OST_UINT   bGIE_CTemp  : 1; /* Saved GIE while in critical section */      \

#define _OS_TCB_PROC_SPEC()


/************************************************************************************************
 *                                                                                              *
 *     Registers definitions                                                                    *
 *                                                                                              *
 ************************************************************************************************/

static volatile char _fsr       @ 0x04;
static volatile char _status    @ 0x03;
static volatile char _indf      @ 0x00;
static volatile char _pcl       @ 0x02;
static volatile char _pclath    @ 0x0A;
static volatile char _intcon    @ 0x0B;

static volatile       bit       _carry      @((unsigned)&_status*8)+0;
static volatile       bit       _zero       @((unsigned)&_status*8)+2;
static                bit       _irp        @((unsigned)&_status*8)+7;
static                bit       _gie        @((unsigned)&_intcon*8)+7;

/*
#ifdef __OSAPICCPRO__
    asm("   __fsr       set     0x04    ");
    asm("   __status    set     0x03    ");
    asm("   __indf      set     0x00    ");
    asm("   __pclath    set     0x0A    ");
    asm("   __pcl       set     0x02    ");
    asm("   __intcon    set     0x0B    ");
#endif
*/

/************************************************************************************************
 *                                                                                              *
 *     Constants and types                                                                      *
 *                                                                                              *
 ************************************************************************************************/

#undef  OS_SMSG_SIZE
#define OS_SMSG_SIZE            sizeof(OS_SMSG_TYPE)

typedef OST_UINT8               OS_FSR_TYPE;

// Type of code pointer (used in TCB)
typedef OST_UINT16              OST_CODE_POINTER;






/************************************************************************************************
 *                                                                                              *
 *     Platform macros                                                                          *
 *                                                                                              *
 ************************************************************************************************/

#define     OS_CLRWDT()         asm(" clrwdt ");
#define     OS_ClrWdt()         asm(" clrwdt ");
#define     OS_SLEEP()          asm(" sleep  ");
#define     OS_Sleep()          asm(" sleep  ");






/************************************************************************************************
 *                                                                                              *
 *     Context switching macros                                                                 *
 *                                                                                              *
 ************************************************************************************************/

#define _OS_TASK_POINTER_OFFSET         1


//------------------------------------------------------------------------------
// Set FSR to point to current task
//------------------------------------------------------------------------------

#if defined(_ROMSIZE) && (_ROMSIZE <= 0x800)

    extern void  _OS_SET_FSR_CUR_TASK_W (char);
    extern void  _OS_SET_FSR_CUR_TASK (void);

//    #define _OS_SET_FSR_CUR_TASK()      {_fsr =  (OS_FSR_TYPE)_OS_CurTask     ; _OS_SET_TASK_IRP();}
//    #define _OS_SET_FSR_CUR_TASK_W(w)   {_fsr = ((OS_FSR_TYPE)_OS_CurTask) + w; _OS_SET_TASK_IRP();}
#else

    #define _OS_SET_FSR_CUR_TASK()      {_fsr =  (OS_FSR_TYPE)_OS_CurTask     ; _OS_SET_TASK_IRP();}
    #define _OS_SET_FSR_CUR_TASK_W(w)   {_fsr = ((OS_FSR_TYPE)_OS_CurTask) + w; _OS_SET_TASK_IRP();}

#endif



//------------------------------------------------------------------------------
// Set PCLATH bits according to label address (used in _OS_RETURN_SAVE)
//------------------------------------------------------------------------------

#if !defined(_ROMSIZE) || (_ROMSIZE > 0x1000)

    #define _OS_SET_PCLATH(label)       /*  bsf or bcf PCLATH, 3    */                      \
                                    asm("dw 0x118A | (("#label" >> 1) & 0x400)  ");     \
                                    /*  bsf or bcf PCLATH, 4    */                      \
                                    asm("dw 0x120A | (("#label" >> 2) & 0x400)  ");

#elif (_ROMSIZE > 0x800)

                                    /*  bsf or bcf PCLATH, 3    */
    #define _OS_SET_PCLATH(label)       asm("dw 0x118A | (("#label" >> 1) & 0x400)  ");

#else

    #define _OS_SET_PCLATH(label)

#endif



//------------------------------------------------------------------------------

#if    !defined(_BANKBITS_)
    #define     _BANKBITS_      2
#endif

//------------------------------------------------------------------------------

    #if OS_BANK_TASKS >= 2
        #define _OS_SET_TASK_IRP()          _status |= 0x80
    #else
        #if _BANKBITS_ >= 2
            #define _OS_SET_TASK_IRP()      _status &= ~0x80
        #else
            #define _OS_SET_TASK_IRP()
        #endif
    #endif

//------------------------------------------------------------------------------

    #if OS_BANK_STIMERS >= 2
        #define _OS_SET_STIMER_IRP()           asm("    bsf _STATUS, 7  ")
    #else
        #if _BANKBITS_ >= 2
            #define _OS_SET_STIMER_IRP()       asm("    bcf _STATUS, 7  ")
        #else
            #define _OS_SET_STIMER_IRP()
        #endif
    #endif

//------------------------------------------------------------------------------




//------------------------------------------------------------------------------
// Shell we work with bCanContinue flag? It is defined by OS_ENABLE_TTIMERS constant
//------------------------------------------------------------------------------
#ifdef OS_ENABLE_TTIMERS

    #define _OS_BSF_PCLATH_0()      asm("   bsf   __pclath, 0     ");               \
                                    asm("   btfss __pclath, 0     ");

    #define _OS_BCF_PCLATH_0()      asm("   bcf   __pclath, 0     ");

    #define _OS_INDF_CANCONTINUE()  if (!_OS_bTaskDelay) _OS_bTaskCanContinue = 0;  \
                                    if (_pclath & 0x01) _OS_bTaskCanContinue = 1;

#else

    #define _OS_BSF_PCLATH_0()
    #define _OS_BCF_PCLATH_0()
    #define _OS_INDF_CANCONTINUE()

#endif
//------------------------------------------------------------------------------




/*
 ********************************************************************************
 *
 *  _OS_SAVE_RP0_RP1()
 *  _OS_RESTORE_RP0_RP1()
 *
 *------------------------------------------------------------------------------
 *
 *  description:        save and restore bank selection on enter/leave task
 *
 ********************************************************************************
 */


#if     _BANKBITS_ == 0

    // Don't save/restore banksel bits when there is only one RAM bank

    #define _OS_SAVE_RP0_RP1()
    #define _OS_RESTORE_RP0_RP1()

#elif   _BANKBITS_ == 1

    // When there are two banks, we save/restore only 1 bit

    #define _OS_SAVE_RP0_RP1()          asm("   btfsc   __status, 5     ");     \
                                        asm("   iorlw   0x20            ");

    #define _OS_RESTORE_RP0_RP1()       asm("   btfsc   __OS_TempH, 5   ");     \
                                        asm("   bsf     __status, 5     ");

#elif   _BANKBITS_ == 2

    // When there are three or four banks, we save/restore 2 bits

    #define _OS_SAVE_RP0_RP1()          asm("   btfsc   __status, 5     ");     \
                                        asm("   iorlw   0x20            ");     \
                                        asm("   btfsc   __status, 6     ");     \
                                        asm("   iorlw   0x40            ");

    #define _OS_RESTORE_RP0_RP1()       asm("   movf    __OS_TempH, w   ");     \
                                        asm("   andlw   0x60            ");     \
                                        asm("   iorwf   __status, f     ");

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


//------------------------------------------------------------------------------

#define _OS_JUMP_TO_TASK()                                                  \
    {                                                                       \
        asm("      clrf    __status  ");                                    \
        _OS_SET_FSR_CUR_TASK_W(_OS_TASK_POINTER_OFFSET+1);                  \
        asm("      movf     __indf, w       ");                             \
        asm("      movwf    __pclath        ");                             \
        asm("      andlw    0x60 ");                                        \
        asm("      iorwf    __status, f     ");                             \
        _fsr--;                                                             \
        asm("      movf     __indf, w       ");                             \
        _fsr--;                                                             \
        asm("      movwf    __pcl     ");                                   \
        /* Now FSR points to OS_CurTask->State    */                        \
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
 *  parameters:     WREG    - address high byte
 *                  FSR     - address low byte
 *
 ********************************************************************************
 */

#define _OS_BACK_TO_SCEDULER()                                              \
    {                                                                       \
        asm("\t global     __OS_Temp        ");                             \
        asm("\t global     __OS_TempH       ");                             \
        asm("global  __OS_ReturnSave   ");                                  \
        asm("global  __OS_ReturnSave3   ");                                 \
        asm("global  __OS_EnterWaitMode  ");                                \
        asm("global  __OS_EnterWaitModeTO  ");                              \
                                                                            \
        asm("__OS_EnterWaitModeTO:  ");                                     \
        _OS_BSF_PCLATH_0();                                                 \
        asm("__OS_EnterWaitMode:  ");                                       \
        _OS_BCF_PCLATH_0();                                                 \
        asm("__OS_ReturnSave:   ");                                         \
            /* First we save RP1 and RP0 */                                 \
        _OS_SAVE_RP0_RP1();                                                 \
        asm("   clrf    __status     ");                                    \
        asm("   movwf   __OS_TempH    ");                                   \
        _OS_Temp = _fsr;                                                    \
            /* Set FSR at _OS_CurTask.pTaskPointer + 1  */                  \
        _OS_SET_FSR_CUR_TASK_W(_OS_TASK_POINTER_OFFSET+1);                  \
        _indf = _OS_TempH;                                                  \
        _fsr--;                                                             \
        _indf = _OS_Temp;                                                   \
        _fsr--;                                                             \
                                                                            \
        _OS_bTaskReady = 1;                                                 \
        asm("   btfss   __OS_TempH, 7");                                    \
        asm("   goto    __OS_ReturnSave3    ");                             \
        _OS_bTaskReady = 0;                                                 \
        _OS_INDF_CANCONTINUE();                                             \
        _OS_RESTORE_RP0_RP1();                                              \
                                                                            \
        asm("   return     ");                                              \
                                                                            \
        asm("__OS_ReturnSave3:      ");                                     \
                                                                            \
        asm(" clrf __status");                                              \
    }
//------------------------------------------------------------------------------









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

OST_BOOL _OS_CheckEvent (char);


#define _OS_RETURN_NO_SAVE()            asm(" _goto__OS_ReturnSave3 ");
#define _OS_ENTER_WAIT_MODE()           asm(" _call__OS_EnterWaitMode ");
#define _OS_ENTER_WAIT_MODE_TO()        asm(" _call__OS_EnterWaitModeTO ");
#define _OS_CHECK_EVENT(event)          if (!_OS_CheckEvent(event)) _OS_RETURN_NO_SAVE()


//------------------------------------------------------------------------------
/************************************************************************/
/* Exit task without saving context                                     */
/************************************************************************/

    asm("_goto__OS_ReturnSave3              macro   ");
    asm("                                           ");
    asm("   global  __OS_ReturnSave3                ");
    asm("   ljmp    __OS_ReturnSave3                ");
    asm("                                           ");
    asm("   endm                                    ");


//------------------------------------------------------------------------------
/************************************************************************/
/* Save context with clearing bReady flag                               */
/************************************************************************/

    asm("_call__OS_EnterWaitMode            macro   ");
    asm("                                           ");
    asm("   global  __OS_EnterWaitMode              ");
    asm("   local   task_ret_point                  ");
    asm("                                           ");
    asm("   movlw   low(task_ret_point)             ");
    asm("   movwf   0x04                            ");
    asm("   movlw   0x80|high(task_ret_point)       ");
    asm("   lcall   __OS_EnterWaitMode              ");
    asm("                                           ");
    asm("task_ret_point:                            ");
    asm("                                           ");
    asm("   endm                                    ");


//------------------------------------------------------------------------------
/************************************************************************/
/* Save context with clearing bReady and setting bCanContinue           */
/************************************************************************/

    asm("_call__OS_EnterWaitModeTO    macro         ");
    asm("                                           ");
    asm("   global  __OS_EnterWaitModeTO            ");
    asm("   local   task_ret_point_                 ");
    asm("                                           ");
    asm("   movlw   low(task_ret_point_)            ");
    asm("   movwf   0x04                            ");
    asm("   movlw   0x80|high(task_ret_point_)      ");
    asm("   lcall   __OS_EnterWaitModeTO            ");
    asm("                                           ");
    asm("task_ret_point_:                           ");
    asm("                                           ");
    asm("   endm                                    ");


//------------------------------------------------------------------------------



#define _OS_RETURN_SAVE()                                                                   \
                                                                                            \
{                                                                                           \
    asm("global __OS_ReturnSave   ");                                                       \
    _OS_SET_PCLATH  (__OS_ReturnSave);                                                      \
    asm("dw (0x3000 | low($+4))");                      /*  movlw   $ + 4               */  \
    asm("dw (0x0084) ");                                /*  movwf   fsr                 */  \
    asm("dw (0x3000 | high($+2)) ");                    /*  movlw   $ + 2               */  \
    asm("dw (0x2800 | (__OS_ReturnSave & 0x7FF))");     /*  goto    __OS_ReturnSave     */  \
}






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

#define OS_Task_Replace(priority, TaskName)                             \
{                                                                       \
    OS_Task_Reserve(TaskName);                                          \
    _OS_SET_FSR_CUR_TASK();                                             \
    _indf = priority | 0x48; /* bEnable è bReady */                     \
    _fsr++;                                                             \
    _indf = ((OST_CODE_POINTER)TaskName) & 0xFF;                        \
    _fsr++;                                                             \
    _indf = (((OST_CODE_POINTER)TaskName) >> 8) & 0xFF;                 \
    _OS_RETURN_NO_SAVE();                                               \
}



#define OS_Task_Replace_P(priority, TaskAddr)                           \
{                                                                       \
    _OS_SET_FSR_CUR_TASK();                                             \
    _indf = priority | 0x48; /* bEnable è bReady */                     \
    _fsr++;                                                             \
    _indf = ((OST_UINT16)(TaskAddr)) & 0xFF;                            \
    _fsr++;                                                             \
    _indf = ((OST_UINT16)(TaskAddr) >> 8) & 0xFF;                       \
    _OS_RETURN_NO_SAVE();                                               \
}


/************************************************************************/
/*                                                                      */
/* Create task                                                          */
/*                                                                      */
/************************************************************************/

//--------------------------------------------------------------------------
#if defined(__OSAPICCPRO__)
//--------------------------------------------------------------------------
    #define OS_Task_Create(priority, TaskName)                          \
        {                                                               \
            _OS_Task_Create(priority, (int*)&TaskName);                 \
            if (!OS_IsError())                                          \
            {                                                           \
                asm("   incf    __fsr, f                ");             \
                asm("   movlw   low(fp__"#TaskName")    ");             \
                asm("   movwf   __indf                  ");             \
                asm("   movlw   high(fp__"#TaskName")   ");             \
                asm("   incf    __fsr, f                ");             \
                asm("   movwf   __indf                  ");             \
                if (_indf & 0x80) TaskName();                           \
            }                                                           \
        }

//--------------------------------------------------------------------------
#else
//--------------------------------------------------------------------------
    #define OS_Task_Create(priority, TaskName)                          \
        {                                                               \
            OS_Task_Reserve(TaskName);                                  \
            _OS_Task_Create(priority, (OST_CODE_POINTER)TaskName);      \
        }
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------


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
    }







/************************************************************************************************
 *                                                                                              *
 *                                     I N T E R R U P T S                                      *
 *                                                                                              *
 ************************************************************************************************/

#if     defined(_14000)  || defined(_16C61)  || defined(_16C62)  ||\
        defined(_16C63)  || defined(_16C63A) || defined(_16C64)  ||\
        defined(_16C65)  || defined(_16C65B) || defined(_16C71)  ||\
        defined(_16C73)  || defined(_16C73B) || defined(_16C74)  ||\
        defined(_16C74B) || defined(_16C84)  || defined(_16C745) ||\
        defined(_16C765) || defined(_16LC74B)

    #define _OS_PIC16_GIE_LOOPED    // When defined, GIE->0: "do {GIE=0;} while (GIE);"
                                    // Else          GIE->0: "GIE = 0;"

    #ifndef     di
        #define di()    do { GIE = 0; } while ( GIE == 1 ) // disable interrupt bit
    #endif
#else
    #ifndef     di
        #define di()    (GIE = 0)       // interrupt enable bit
    #endif
#endif


char    OS_DI (void);
#define OS_RI(cSavedInt)    OSM_BEGIN {    if (cSavedInt & 0x80) _gie = 1; } OSM_END
#define OS_EI()             OSM_BEGIN {    _gie = 1; } OSM_END


#ifdef OS_ENABLE_INT

    #if defined(_ROMSIZE) && (_ROMSIZE <= 0x800)

        extern void _OS_DI_INT (void);
        extern void _OS_RI_INT (void);

    #else

        #define _OS_DI_INT() OSM_BEGIN {    _OS_Temp_I = _intcon; di(); } OSM_END
        #define _OS_RI_INT() OSM_BEGIN {    if (_OS_Temp_I & 0x80) _gie = 1;  } OSM_END

    #endif

#endif


















//____________________________________________________________________________________________________________
//************************************************************************************************************
//
//                    S P E C I F I C      M A C R O S       O V E R L O A D
//
//                    (used ro increasing the speed and reducing the code size)
//
//************************************************************************************************************





//______________________________________________________________________________
/******************************************************************************************
 *
 * MACROS FOR OLD STYLE STATIC TIMERS WORK
 *
 ******************************************************************************************/


#define __OS_Timer8Work(TIMER_ID,L)                                                             \
                                                                                                \
  if ((OS_Timeouts[(TIMER_ID)>>3]&(1<<((TIMER_ID)&7))))                                         \
    if (!++OS_Timers8[TIMER_ID])                                                                \
    {                                                                                           \
      OS_Timeouts[(TIMER_ID)>>3]&=~(1<<((TIMER_ID)&7));                                         \
    }                                                                                           \


#define __OS_Timer16Work(TIMER_ID,L)                                                            \
                                                                                                \
  if ((OS_Timeouts[(TIMER_ID+_OS_TIMER16_POS)>>3]&(1<<((TIMER_ID+_OS_TIMER16_POS)&7))))         \
    if (!++*((OS_TIMERS16_BANK char*)&OS_Timers16[TIMER_ID]+0)) {                               \
      if (!++*((OS_TIMERS16_BANK char*)&OS_Timers16[TIMER_ID]+1)) {                             \
        OS_Timeouts[(TIMER_ID+_OS_TIMER16_POS)>>3]&=~(1<<((TIMER_ID+_OS_TIMER16_POS)&7));       \
      };                                                                                        \
    };                                                                                          \


#define __OS_Timer24Work(TIMER_ID,L)                                                            \
                                                                                                \
  if ((OS_Timeouts[(TIMER_ID+_OS_TIMER24_POS)>>3]&(1<<((TIMER_ID+_OS_TIMER24_POS)&7))))         \
    if (!++*((OS_TIMERS24_BANK char*)&OS_Timers24[TIMER_ID]+0)) {                               \
      if (!++*((OS_TIMERS24_BANK char*)&OS_Timers24[TIMER_ID]+1)) {                             \
        OS_Timeouts[(TIMER_ID+_OS_TIMER24_POS)>>3]&=~(1<<((TIMER_ID+_OS_TIMER24_POS)&7));       \
      };                                                                                        \
    };                                                                                          \



#define __OS_Timer32Work(TIMER_ID,L)                                                            \
                                                                                                \
  if ((OS_Timeouts[(TIMER_ID+_OS_TIMER32_POS)>>3]&(1<<((TIMER_ID+_OS_TIMER32_POS)&7))))         \
    if (!++*((OS_TIMERS32_BANK char*)&OS_Timers32[TIMER_ID]+0)) {                               \
      if (!++*((OS_TIMERS32_BANK char*)&OS_Timers32[TIMER_ID]+1)) {                             \
        if (!++*((OS_TIMERS32_BANK char*)&OS_Timers32[TIMER_ID]+2)) {                           \
          if (!++*((OS_TIMERS32_BANK char*)&OS_Timers32[TIMER_ID]+3)) {                         \
            OS_Timeouts[(TIMER_ID+_OS_TIMER32_POS)>>3]&=~(1<<((TIMER_ID+_OS_TIMER32_POS)&7));   \
          };                                                                                    \
        };                                                                                      \
      };                                                                                        \
    };                                                                                          \







//______________________________________________________________________________
/******************************************************************************************
 *
 * MACROS FOR WORK WITH NEW STYLE STATIC TIMERS in OS_Timer
 *
 ******************************************************************************************/
#ifdef  OS_ENABLE_STIMERS

#if     OS_STIMER_SIZE == 1

    #define __OS_StimerWork(STIMER_ID)                              \
    {                                                               \
        if (_OS_Stimers[STIMER_ID] & 0x80) _OS_Stimers[STIMER_ID]++;\
    }                                                               \


    #define __OS_StimersWorkSize()                                  \
        _fsr = (OS_FSR_TYPE)_OS_Stimers;                            \
        _OS_SET_STIMER_IRP();                                       \
        do {                                                        \
            if (_indf & 0x80) _indf++;                              \
            _fsr++;                                                 \
        } while (_fsr != (OS_FSR_TYPE)&_OS_Stimers[OS_STIMERS]);    \


#elif   OS_STIMER_SIZE == 2

    #define __OS_StimerWork(STIMER_ID)                                                              \
    {                                                                                               \
        if (*((OS_STIMERS_BANK char*)&_OS_Stimers[STIMER_ID]+1) & 0x80) _OS_Stimers[STIMER_ID]++;   \
    }                                                                                               \

    #define __OS_StimersWorkSize()                                  \
        _fsr = (OS_FSR_TYPE)_OS_Stimers;                            \
        _OS_SET_STIMER_IRP();                                       \
        do {                                                        \
            _fsr++;                                                 \
            if (_indf & 0x80)                                       \
            {                                                       \
                _fsr--;                                             \
                if (!++_indf)                                       \
                {                                                   \
                    _fsr++;                                         \
                    _indf++;                                        \
                }else (_fsr++);                                     \
            }                                                       \
            _fsr++;                                                 \
        } while (_fsr != (OS_FSR_TYPE)&_OS_Stimers[OS_STIMERS]);    \


#elif   OS_STIMER_SIZE == 4

    #define __OS_StimerWork(STIMER_ID)                                                              \
    {                                                                                               \
        if (*((OS_STIMERS_BANK char*)&_OS_Stimers[STIMER_ID]+3) & 0x80) _OS_Stimers[STIMER_ID]++;   \
    }                                                                                               \


    #define __OS_StimersWorkSize()                                  \
         _fsr = (OS_FSR_TYPE)&_OS_Stimers[OS_STIMERS];              \
        _OS_SET_STIMER_IRP();                                       \
        do {                                                        \
            _fsr--;                                                 \
            asm("   movlw   3       ");                             \
            if (_indf & 0x80)                                       \
            {                                                       \
                asm("   subwf   __fsr, f    ");                     \
                asm("   clrw                ");                     \
                if (!++_indf)                                       \
                {                                                   \
                    asm("   addlw   1  ");                          \
                    _fsr++;                                         \
                    if (!++_indf)                                   \
                    {                                               \
                        asm("   addlw   1  ");                      \
                        _fsr++;                                     \
                        if (!++_indf)                               \
                        {                                           \
                            asm("   addlw   1  ");                  \
                            _fsr++;                                 \
                            _indf++;                                \
                        }                                           \
                    }                                               \
                }                                                   \
            }                                                       \
            asm("   subwf   __fsr, f    ");                         \
        } while (_fsr != (OS_FSR_TYPE)_OS_Stimers);                 \



#endif

#endif  // OS_ENABLE_STIMERS



//______________________________________________________________________________
/******************************************************************************************
 *
 * MACROS FOR WORK WITH TASK TIMERS in OS_Timer
 *
 ******************************************************************************************/

#ifdef OS_ENABLE_TTIMERS

#define ASM_OST_TCB_SIZE_CONST  asm(" ");
#define ASM_SET_BANK            asm(" ");


#if   OS_TTIMER_SIZE == 1

    #define __OS_TtimerWork(TASK_ID)                                        \
        {                                                                   \
            if (_OS_Tasks[TASK_ID].State.bDelay) {                          \
                if (!++_OS_Tasks[TASK_ID].Timer)                            \
                    _OS_Tasks[TASK_ID].State.bDelay = 0;                    \
            }                                                               \
        }


    #define __OS_TtimersWorkSize()                                          \
        _fsr = (OS_FSR_TYPE)_OS_Tasks;                                      \
        _OS_SET_TASK_IRP();                                                 \
        do {                                                                \
            asm("   movlw   4           ");                                 \
            if (((near OST_TASK_STATE*)(&_indf))->bDelay) {                 \
                asm("   movlw   3           ");                             \
                asm("   addwf   __fsr, f    ");                             \
                if (!++_indf) {                                             \
                    asm("   subwf   __fsr, f    ");                         \
                    ((near OST_TASK_STATE*)(&_indf))->bDelay = 0;           \
                    asm("   movlw   6       ");                             \
                }                                                           \
                asm("   addlw   -2      ");                                 \
            }                                                               \
            asm("   addwf   __fsr, f    ");                                 \
        } while (_fsr != (OS_FSR_TYPE)&_OS_Tasks[OS_TASKS]);                \

#elif OS_TTIMER_SIZE == 2

    #define __OS_TtimerWork(TASK_ID)                                            \
        {                                                                       \
            if (_OS_Tasks[TASK_ID].State.bDelay)                                \
            if (!++*((OS_TASKS_BANK char*)&_OS_Tasks[TASK_ID].Timer+0)) {       \
                if (!++*((OS_TASKS_BANK char*)&_OS_Tasks[TASK_ID].Timer+1)) {   \
                    _OS_Tasks[TASK_ID].State.bDelay = 0;                        \
                }                                                               \
            }                                                                   \
        }

    #define __OS_TtimersWorkSize()                                          \
        _fsr = (OS_FSR_TYPE)_OS_Tasks;                                      \
        _OS_SET_TASK_IRP();                                                 \
        do {                                                                \
            asm("   movlw   5           ");                                 \
            if (((near OST_TASK_STATE*)(&_indf))->bDelay)                   \
            {                                                               \
                asm("   movlw   3           ");                             \
                asm("   addwf   __fsr, f    ");                             \
                asm("   movlw   2   ");                                     \
                if (!++_indf)                                               \
                {                                                           \
                    _fsr++;                                                 \
                    asm("   addlw   -1  ");                                 \
                    if (!++_indf)                                           \
                    {                                                       \
                        _fsr -= 4;                                          \
                        ((near OST_TASK_STATE*)(&_indf))->bDelay = 0;       \
                        asm("   movlw   5   ");                             \
                    }                                                       \
                }                                                           \
            }                                                               \
            asm("   addwf   __fsr, f    ");                                 \
        } while (_fsr != (OS_FSR_TYPE)&_OS_Tasks[OS_TASKS]);                \




#elif OS_TTIMER_SIZE == 4

    #define __OS_TtimerWork(TASK_ID)                                                    \
        {                                                                               \
            if (_OS_Tasks[TASK_ID].State.bDelay)                                        \
            if (!++*((OS_TASKS_BANK char*)&_OS_Tasks[TASK_ID].Timer+0)) {               \
                if (!++*((OS_TASKS_BANK char*)&_OS_Tasks[TASK_ID].Timer+1)) {           \
                    if (!++*((OS_TASKS_BANK char*)&_OS_Tasks[TASK_ID].Timer+2)) {       \
                        if (!++*((OS_TASKS_BANK char*)&_OS_Tasks[TASK_ID].Timer+3)) {   \
                            _OS_Tasks[TASK_ID].State.bDelay = 0;                        \
                        }                                                               \
                    }                                                                   \
                }                                                                       \
            }                                                                           \
        }


    #define __OS_TtimersWorkSize()                                          \
        _fsr = (OS_FSR_TYPE)_OS_Tasks;                                      \
        _OS_SET_TASK_IRP();                                                 \
        do {                                                                \
            asm("   movlw   7           ");                                 \
            if (((near OST_TASK_STATE*)(&_indf))->bDelay) {                 \
                asm("   movlw   3           ");                             \
                asm("   addwf   __fsr, f    ");                             \
                asm("   movlw   4           ");                             \
                if (!++_indf) {                                             \
                    _fsr++;                                                 \
                    asm("   addlw   -1  ");                                 \
                    if (!++_indf) {                                         \
                        _fsr++;                                             \
                        asm("   addlw   -1  ");                             \
                        if (!++_indf) {                                     \
                            _fsr++;                                         \
                            asm("   addlw   -1  ");                         \
                            if (!++_indf) {                                 \
                                _fsr -= 6;                                  \
                                ((near OST_TASK_STATE*)(&_indf))->bDelay = 0;\
                                asm("   movlw   7   ");                     \
                            }                                               \
                        }                                                   \
                    }                                                       \
                }                                                           \
            }                                                               \
            asm("   addwf   __fsr, f    ");                                 \
        } while (_fsr != (OS_FSR_TYPE)&_OS_Tasks[OS_TASKS]);                \


#endif


#endif  //  OS_ENABLE_TTIMERS
/******************************************************************************************/












/******************************************************************************************
 *
 *  WORK WITH DYNAMIC TIMERS in OS_Timer
 *
 ******************************************************************************************/

#define _OS_IndfTimerTimeout       ((OS_RAM_NEAR OST_DTIMER_FLAGS*)(&_indf))->bTimeout
#define _OS_IndfTimerActive        ((OS_RAM_NEAR OST_DTIMER_FLAGS*)(&_indf))->bActive
#define _OS_IndfTimerRun           ((OS_RAM_NEAR OST_DTIMER_FLAGS*)(&_indf))->bRun
#define _OS_IndfTimerNextEnable    ((OS_RAM_NEAR OST_DTIMER_FLAGS*)(&_indf))->bNextEnable

//------------------------------------------------------------------------------
#ifdef OS_ENABLE_DTIMERS
//------------------------------------------------------------------------------

#define _OS_DtimersWork_DEFINED

//------------------------------------------------------------------------------
#if   OS_DTIMER_SIZE == 1
//------------------------------------------------------------------------------

    #define _OS_INC_DTIMER()                                \
        {                                                   \
            /* Now W = fsr */                               \
            asm("   incf    __fsr, f    ");                 \
            asm("   incf    __fsr, f    ");                 \
            asm("   bcf     __status, 2 ");                 \
            asm("   incf    __indf, f   ");                 \
            asm("   movwf   __fsr       ");                 \
            if (_zero) _OS_IndfTimerTimeout = 1;            \
        }

//------------------------------------------------------------------------------
#elif OS_DTIMER_SIZE == 2
//------------------------------------------------------------------------------

    #define _OS_INC_DTIMER()                                \
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
            if (_carry) _OS_IndfTimerTimeout = 1;           \
        }

//------------------------------------------------------------------------------
#elif OS_DTIMER_SIZE == 4
//------------------------------------------------------------------------------

    #define _OS_INC_DTIMER()                                \
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
            if (_carry) _OS_IndfTimerTimeout = 1;           \
        }

//------------------------------------------------------------------------------
#endif  // OS_DTIMER_SIZE
//------------------------------------------------------------------------------


#define __OS_DtimersWork()                                  \
    {                                                       \
        _irp = 0;                                           \
        _fsr = (OS_FSR_TYPE) &_OS_DTimers;                  \
    REPEAT:                                                 \
        if (_OS_IndfTimerNextEnable)                        \
        {                                                   \
            _fsr++;                                         \
            _fsr = _indf;                                   \
            if (!_OS_IndfTimerRun) goto REPEAT;             \
            _OS_INC_DTIMER();                               \
            goto REPEAT;                                    \
        }                                                   \
    }


//------------------------------------------------------------------------------
#endif  // OS_ENABLE_DTIMERS
//------------------------------------------------------------------------------








//------------------------------------------------------------------------------
#if defined(OS_ENABLE_QTIMERS)
//------------------------------------------------------------------------------

#define _OS_IndfFTimerTimeout       ((OS_RAM_NEAR OST_QTIMER_FLAGS*)(&_indf))->bTimeout
#define _OS_IndfFTimerActive        ((OS_RAM_NEAR OST_QTIMER_FLAGS*)(&_indf))->bActive
#define _OS_IndfFTimerNextEnable    ((OS_RAM_NEAR OST_QTIMER_FLAGS*)(&_indf))->bNextEnable

#define _OS_QtimersWork_DEFINED

//------------------------------------------------------------------------------
#if   OS_QTIMER_SIZE == 1
//------------------------------------------------------------------------------

    #define _OS_IF_INC_QTIMER()         if (!++_indf)

    #define _OS_CHECK_QTIMER()          asm("   movf    __indf, f    ");

//------------------------------------------------------------------------------
#elif OS_QTIMER_SIZE == 2
//------------------------------------------------------------------------------

    #define _OS_IF_INC_QTIMER()         if (!++_indf) if ((_fsr++, !++_indf))

    #define _OS_CHECK_QTIMER()                              \
        asm("   movf    __indf, w   ");                     \
        asm("   incf    __fsr, f    ");                     \
        asm("   iorwf   __indf, w   ");


//------------------------------------------------------------------------------
#elif OS_QTIMER_SIZE == 4
//------------------------------------------------------------------------------

    #define _OS_IF_INC_QTIMER()                             \
        if (!++_indf)                                       \
        if ((_fsr++, !++_indf))                             \
        if ((_fsr++, !++_indf))                             \
        if ((_fsr++, !++_indf))

    #define _OS_CHECK_QTIMER()                              \
        asm("   movf    __indf, w   ");                     \
        asm("   incf    __fsr, f    ");                     \
        asm("   iorwf   __indf, w   ");                     \
        asm("   incf    __fsr, f    ");                     \
        asm("   iorwf   __indf, w   ");                     \
        asm("   incf    __fsr, f    ");                     \
        asm("   iorwf   __indf, w   ");



//------------------------------------------------------------------------------
#endif  // OS_QTIMER_SIZE
//------------------------------------------------------------------------------



#define __OS_QtimersWork()                                      \
    if (_OS_Qtimers.Flags.bNextEnable)                          \
    {                                                           \
        _irp = 0;                                               \
        _fsr = ((OS_FSR_TYPE)&_OS_Qtimers) + 1;                 \
        _fsr = _indf + 2;                                       \
        _OS_IF_INC_QTIMER()                                     \
        do  {                                                   \
            _fsr -= (OS_QTIMER_SIZE + 1);                       \
            _OS_IndfFTimerTimeout = 1;                          \
            _OS_IndfFTimerActive  = 0;                          \
            if (!_OS_IndfFTimerNextEnable)                      \
            {                                                   \
                _OS_Qtimers.Flags.bNextEnable = 0;              \
                break;                                          \
            }                                                   \
            _fsr++;                                             \
            _fsr = _indf;                                       \
            _OS_Qtimers.Next = (void*)_fsr;                     \
            _fsr += 2;                                          \
            _OS_CHECK_QTIMER();                                 \
        } while (_zero);                                        \
    }

/**/
//------------------------------------------------------------------------------
#endif  // OS_ENABLE_QTIMERS
//------------------------------------------------------------------------------









#endif

//******************************************************************************
//  END OF FILE osa_pic16_htpicc.h
//******************************************************************************

