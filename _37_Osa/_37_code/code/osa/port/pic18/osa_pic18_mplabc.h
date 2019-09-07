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
 *  File:           osa_pic18_mplabc.h
 *
 *  Compilers:      Mplab C18
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    Mplab C18 specific definition
 *                  This file directly included in osa.h
 *
 *  History:        13.09.2010 -    File updated
 *
 *                  26.12.2010 -    OS_RETURN_NOSAVE corrected (NOP added after call to _OS_ReturnNoSave)
 * 
 ************************************************************************************************
 */

#ifndef __OSAMCC18_H__
#define __OSAMCC18_H__



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

#define OS_RAM_NEAR            near
#define OS_RAM_BANK0           near
#define OS_RAM_BANK1
#define OS_RAM_BANK2
#define OS_RAM_BANK3

#define _OS_CUR_FLAGS_IN_FSR        // Work with tasks state througth FSR



/************************************************************************************************
 *                                                                                              *
 *     Processor specified definitions for system flags                                         *
 *     (see OST_SYSTEM_FLAGS and OST_TCB definition in file "OSA.h")                            *
 *                                                                                              *
 ************************************************************************************************/

#define _OS_SYSTEM_FLAGS_PROC_SPEC()                                                             \
                                                                                                \
        /*5*/   OST_UINT   bTimeout    : 1; /* Event waiting timeout                */     \
        /*6*/   OST_UINT   bGIE_CTemp  : 1; /* Saved GIEH while in critical section */     \
        /*7*/   OST_UINT   bGIEL_CTemp : 1; /* Saved GIEL while in critical section */     \

#define _OS_TCB_PROC_SPEC()                                                                      \
                unsigned char   cTempFSR1;          /* Saved FSR1 (for MCC18) */                \

#define _OS_TASK_CREATE_PROC_SPEC()   Task->cTempFSR1 = 0

/************************************************************************************************
 *                                                                                              *
 *     Registers definitions                                                                    *
 *                                                                                              *
 ************************************************************************************************/

extern          near unsigned char      TOSL;
extern          near unsigned char      TOSH;
extern          near unsigned char      TOSU;
extern volatile near unsigned char      STKPTR;
extern volatile near unsigned char      INTCON;
extern volatile near unsigned char      RCON;


#define _fsr        FSR0
#define _fsrl       FSR0L
#define _indf       INDF0
#define _postinc    POSTINC0
#define _postdec    POSTDEC0
#define _preinc     PREINC0


#define _postinc1   POSTINC1
#define _postdec1   POSTDEC1
#define _indf1      INDF1
#define _fsr1l      FSR1L
#define _fsr2l      FSR2L

#define _pcl        PCL
#define _pclath     PCLATH
#define _pclatu     PCLATU
#define _status     STATUS
#define _tosh       TOSH
#define _tosl       TOSL
#define _tosu       TOSU
#define _bsr        BSR
#define _wreg       WREG
#define _stkptr     STKPTR
#define _intcon     INTCON
#define _rcon       RCON

#define _gie        INTCONbits.GIE
#define _giel       INTCONbits.GIEL
#define _ipen       RCONbits.IPEN
#define _carry      STATUSbits.C



/************************************************************************************************
 *                                                                                              *
 *     Constants and types                                                                      *
 *                                                                                              *
 ************************************************************************************************/

#ifndef OS_SMSG_SIZE
#define OS_SMSG_SIZE    0   // We can not use sizeof() because it does not work with #ifdef directive
#endif


#define _OS_PIC18_ERRATA_NOP()  _asm nop _endasm
#define _OS_POP()               _asm pop _endasm
#define _OS_SET_TASK_IRP()      // Empty macro (used only for PIC16)

typedef unsigned int            OS_FSR_TYPE;

// Code pointer (used in OST_TCB)

//------------------------------------------------------------------------------
#if defined(__SMALL__)
//------------------------------------------------------------------------------

    typedef unsigned int            OST_CODE_POINTER;
    #define OS_CODE_POINTER_SIZE    2

//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------

    typedef unsigned long           OST_CODE_POINTER;
    #define OS_CODE_POINTER_SIZE    4

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------








/************************************************************************************************
 *                                                                                              *
 *     Platform macros                                                                          *
 *                                                                                              *
 ************************************************************************************************/

#define OS_CLRWDT()   _asm clrwdt   _endasm
#define OS_ClrWdt()   _asm clrwdt   _endasm
#define OS_SLEEP()    _asm sleep    _endasm
#define OS_Sleep()    _asm sleep    _endasm







/************************************************************************************************
 *                                                                                              *
 *     Context switching macros                                                                 *
 *                                                                                              *
 ************************************************************************************************/

#define _OS_TASK_POINTER_OFFSET         1

//------------------------------------------------------------------------------
// Save/restore PC macros
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#if defined(__SMALL__)
//------------------------------------------------------------------------------

    #define _OS_SET_PC()            _pclath = _postdec;                 \
                                    _asm                                \
                                        movf    _postdec, 0, 0          \
                                        movwf   _pcl, 0                 \
                                    _endasm


    #define _OS_SAVE_PC()           _postdec = _tosh;                   \
                                    _postdec = _tosl;


    #define _OS_SET_INDF_WORD(value)                                    \
        _postinc = (((OST_CODE_POINTER)value)) & 0xFF;                  \
        _postinc = (((OST_CODE_POINTER)value) >> 8) & 0xFF;

//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------

    #define _OS_SET_PC()            _postdec = 0;                       \
                                    _pclatu = _postdec;                 \
                                    _pclath = _postdec;                 \
                                    _asm                                \
                                        movf    _postdec, 0, 0          \
                                        movwf   _pcl, 0                 \
                                    _endasm


    #define _OS_SAVE_PC()           _postdec = 0;                       \
                                    _postdec = _tosu;                   \
                                    _postdec = _tosh;                   \
                                    _postdec = _tosl;

    #define _OS_SET_INDF_WORD(value)                                    \
        _postinc = (((OST_CODE_POINTER)value)) & 0xFF;                  \
        _postinc = (((OST_CODE_POINTER)value) >> 8) & 0xFF;             \
        _postinc = (((OST_CODE_POINTER)value) >> 16) & 0xFF;            \
        _postinc = 0;

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------




/*
 ********************************************************************************
 *                                                                              *
 *  _OS_JUMP_TO_TASK()                                                            *
 *                                                                              *
 *------------------------------------------------------------------------------*
 *                                                                              *
 *  description:        jump indirectly from kernel (OS_Sched) into task        *
 *                                                                              *
 ********************************************************************************
 */

void _OS_JumpToTask         (void);

#define _OS_JUMP_TO_TASK()    _OS_JumpToTask()

#define _OS_BACK_TO_SCEDULER()                                          \
    if (_fsr1l != _OS_TempH)                                            \
    {                                                                   \
        _fsr1l = _OS_TempH;                                             \
        _fsr2l = _indf1;                                                \
    }                                                                   \






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


extern void _OS_ReturnSave (void);
extern void _OS_ReturnNoSave (void);
extern void _OS_EnterWaitMode (void);
extern void _OS_EnterWaitModeTO (void);
extern void _OS_CheckEvent (void);


extern void  __OS_SET_FSR_CUR_TASK_W (void);
extern void  _OS_SET_FSR_CUR_TASK (void);

#define _OS_SET_FSR_CUR_TASK_W(w)               \
    {   _wreg = w;                              \
        __OS_SET_FSR_CUR_TASK_W();              \
    }


//------------------------------------------------------------------------------
#define _OS_RETURN_SAVE()           {                                  \
                                        _OS_ReturnSave();              \
                                        _OS_PIC18_ERRATA_NOP();        \
                                    }

//------------------------------------------------------------------------------
#define _OS_ENTER_WAIT_MODE()           {                              \
                                        _OS_EnterWaitMode();           \
                                        _OS_PIC18_ERRATA_NOP();        \
                                    }

//------------------------------------------------------------------------------
#define _OS_ENTER_WAIT_MODE_TO()                                       \
                                    {                                  \
                                        _OS_EnterWaitModeTO();         \
                                        _OS_PIC18_ERRATA_NOP();        \
                                    }

//------------------------------------------------------------------------------
#define _OS_RETURN_NO_SAVE()        {                                       \
                                        _OS_ReturnNoSave();                 \
                                        _OS_PIC18_ERRATA_NOP();   /*101226*/\
                                    }

#define _OS_CHECK_EVENT(event) {_OS_Temp = event; _OS_CheckEvent();}
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
    _OS_SET_FSR_CUR_TASK();                                                     \
    _postinc = priority | 0x48; /* bEnable è bReady */                          \
    _OS_SET_INDF_WORD(TaskName);                                                \
    _indf = 0;  /* temp FSR1 */                                                 \
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

extern void _OS_RI (void);
extern char OS_DI (void);
#define OS_RI(cSavedInt) {_wreg = cSavedInt; _OS_RI();}
#define OS_EI()         { _gie = 1; if (_ipen) _giel = 1;}
#define OS_EIH()        { _gie = 1; }
#define OS_EIL()        { _giel = 1; }


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
 *  WORK WITH STATIC TIMERS
 *
 ******************************************************************************************/

//------------------------------------------------------------------------------
#ifdef OS_ENABLE_STIMERS
//------------------------------------------------------------------------------

#if OS_STIMER_SIZE == 4
    #define __OS_StimerBank()   _asm    movlb   _OS_Stimers    _endasm    \
                                _asm    movlw   0            _endasm

#else

    #define __OS_StimerBank()   _asm    movlb   _OS_Stimers    _endasm

#endif



//------------------------------------------------------------------------------
#if     OS_STIMER_SIZE == 1
//------------------------------------------------------------------------------

    #define __OS_StimerWork(STIMER_ID)                          \
        _asm                                                    \
            btfsc   _OS_Stimers + (STIMER_ID*1 + 0), 7, 1       \
            incf    _OS_Stimers + (STIMER_ID*1 + 0), 1, 1       \
        _endasm                                                 \


    #define __OS_StimersWorkSize()                              \
        _fsr = (OS_FSR_TYPE)(_OS_Stimers-1);                    \
        _asm                                                    \
            movlw   OS_STIMERS                                  \
            btfsc   _preinc, 7, 0                               \
            incf    _indf, 1, 0                                 \
            decfsz  WREG, 0, 0                                  \
            bra     -4                                          \
        _endasm                                                 \

//------------------------------------------------------------------------------
#elif   OS_STIMER_SIZE == 2
//------------------------------------------------------------------------------

    #define __OS_StimerWork(STIMER_ID)                          \
        _asm                                                    \
            btfss   _OS_Stimers + (STIMER_ID*2 + 1), 7, 1       \
            bra     +2                                          \
            infsnz  _OS_Stimers + (STIMER_ID*2 + 0), 1, 1       \
            incf    _OS_Stimers + (STIMER_ID*2 + 1), 1, 1       \
        _endasm                                                 \


    #define __OS_StimersWorkSize()                              \
        _fsr = (OS_FSR_TYPE)_OS_Stimers + OS_STIMERS*2;         \
        do {                                                    \
            _fsrl--;                                            \
            if (_postdec & 0x80)                                \
            {                                                   \
                if (!++_postinc) _indf++;                       \
                _fsrl--;                                        \
            }                                                   \
        } while (_fsrl != (char)((OS_FSR_TYPE)_OS_Stimers)&0xFF);\

//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------

    #define __OS_StimerWork(STIMER_ID)                          \
        _asm                                                    \
            btfss   _OS_Stimers + (STIMER_ID*4 + 3), 7, 1       \
            bra     +4                                          \
            incf    _OS_Stimers + (STIMER_ID*4 + 0), 1, 1       \
            addwfc  _OS_Stimers + (STIMER_ID*4 + 1), 1, 1       \
            addwfc  _OS_Stimers + (STIMER_ID*4 + 2), 1, 1       \
            addwfc  _OS_Stimers + (STIMER_ID*4 + 3), 1, 1       \
        _endasm                                                 \


    #define __OS_StimersWorkSize()                              \
        _fsr = (OS_FSR_TYPE)_OS_Stimers;                        \
        do {                                                    \
            _asm                                                \
               movlw   3                                        \
               addwf   _fsr, 1, 0                               \
            _endasm                                             \
            if (_indf & 0x80)                                   \
            {                                                   \
                _asm                                            \
                    subwf   _fsr, 1, 0                          \
                    incf    _postinc, 1, 0                      \
                    movlw   0                                   \
                    addwfc  _postinc, 1, 0                      \
                    addwfc  _postinc, 1, 0                      \
                    addwfc  _indf, 1, 0                         \
                _endasm                                         \
            }                                                   \
            _fsrl++;                                            \
        } while (_fsrl != (char)(_OS_Stimers + OS_STIMERS));    \


//------------------------------------------------------------------------------
#endif      // OS_STIMER_SIZE
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
#endif      // OS_ENABLE_TTIMERS
//------------------------------------------------------------------------------











/******************************************************************************************
 *
 *  WORK WITH TASK TIMERS
 *
 ******************************************************************************************/

//------------------------------------------------------------------------------
#ifdef  OS_ENABLE_TTIMERS
//------------------------------------------------------------------------------


#define     ASM_SET_BANK    _asm movlb _OS_Tasks _endasm

//------------------------------------------------------------------------------
#if   OS_TTIMER_SIZE == 1
//------------------------------------------------------------------------------

    #define __OS_TtimerWork(TASK_ID)                                                \
        _asm                                                                        \
            btfss   _OS_Tasks + (TASK_ID+0)*(3+OS_CODE_POINTER_SIZE), 4, 1          \
            bra     +2                                                              \
            infsnz  _OS_Tasks + ((TASK_ID+1)*(3+OS_CODE_POINTER_SIZE)-1), 1, 1      \
            bcf     _OS_Tasks + (TASK_ID+0)*(3+OS_CODE_POINTER_SIZE), 4, 1          \
        _endasm                                                                     \


    #define __OS_TtimersWorkSize()                                                  \
        ASM_SET_BANK;                                                               \
        _fsr = (OS_FSR_TYPE)_OS_Tasks;                                              \
        do {                                                                        \
            _asm                                                                    \
                movlw   (OS_STIMER_SIZE + OS_CODE_POINTER_SIZE + 2)                 \
            _endasm                                                                 \
            if (((near OST_TASK_STATE*)(&_indf))->bDelay)                           \
            {                                                                       \
                _asm                                                                \
                   movlw   (OS_CODE_POINTER_SIZE + 2)                               \
                   addwf   _fsr, 1, 0                                               \
                   incf    _postinc, 1, 0                                           \
                   movlw   0                                                        \
                _endasm                                                             \
                if (_carry)                                                         \
                {                                                                   \
                    _asm                                                            \
                        movlw   (OS_STIMER_SIZE + OS_CODE_POINTER_SIZE + 2)         \
                        subwf   _fsr, 1, 0                                          \
                    _endasm                                                         \
                    ((near OST_TASK_STATE*)(&_indf))->bDelay = 0;                   \
                }                                                                   \
            }                                                                       \
            _asm                                                                    \
                addwf   _fsr, 1, 0                                                  \
            _endasm                                                                 \
        } while (_fsrl != (char)(_OS_Tasks + OS_TASKS));                            \

//------------------------------------------------------------------------------
#elif OS_TTIMER_SIZE == 2
//------------------------------------------------------------------------------

    #define __OS_TtimerWork(TASK_ID)                                                \
        _asm                                                                        \
            btfsc   _OS_Tasks + (TASK_ID+0)*(4+OS_CODE_POINTER_SIZE), 4, 1          \
            incfsz  _OS_Tasks + ((TASK_ID+1)*(4+OS_CODE_POINTER_SIZE)-2), 1, 1      \
            bra     +2                                                              \
            infsnz  _OS_Tasks + ((TASK_ID+1)*(4+OS_CODE_POINTER_SIZE)-1), 1, 1      \
            bcf     _OS_Tasks + (TASK_ID+0)*(4+OS_CODE_POINTER_SIZE), 4, 1          \
        _endasm                                                                     \


    #define __OS_TtimersWorkSize()                                                  \
        ASM_SET_BANK;                                                               \
        _fsr = (OS_FSR_TYPE)_OS_Tasks;                                              \
        do {                                                                        \
            _asm                                                                    \
                movlw   (OS_STIMER_SIZE + OS_CODE_POINTER_SIZE + 2)                 \
            _endasm                                                                 \
            if (((near OST_TASK_STATE*)(&_indf))->bDelay)                           \
            {                                                                       \
                _asm                                                                \
                   movlw   (OS_CODE_POINTER_SIZE + 2)                               \
                   addwf   _fsr, 1, 0                                               \
                   incf    _postinc, 1, 0                                           \
                   movlw   0                                                        \
                   addwfc  _postinc, 1, 0                                           \
                _endasm                                                             \
                if (_carry)                                                         \
                {                                                                   \
                    _asm                                                            \
                        movlw   (OS_STIMER_SIZE + OS_CODE_POINTER_SIZE + 2)         \
                        subwf   _fsr, 1, 0                                          \
                    _endasm                                                         \
                    ((near OST_TASK_STATE*)(&_indf))->bDelay = 0;                   \
                }                                                                   \
            }                                                                       \
            _asm                                                                    \
                addwf   _fsr, 1, 0                                                  \
            _endasm                                                                 \
        } while (_fsrl != (char)(_OS_Tasks + OS_TASKS));                            \


//------------------------------------------------------------------------------
#elif OS_TTIMER_SIZE == 4
//------------------------------------------------------------------------------

    #define __OS_TtimerWork(TASK_ID)                                                \
        _asm                                                                        \
            btfsc   _OS_Tasks + (TASK_ID+0)*(6+OS_CODE_POINTER_SIZE), 4, 1          \
            incfsz  _OS_Tasks + ((TASK_ID+1)*(6+OS_CODE_POINTER_SIZE)-4), 1, 1      \
            bra     +5                                                              \
            infsnz  _OS_Tasks + ((TASK_ID+1)*(6+OS_CODE_POINTER_SIZE)-3), 1, 1      \
            incfsz  _OS_Tasks + ((TASK_ID+1)*(6+OS_CODE_POINTER_SIZE)-2), 1, 1      \
            bra     +2                                                              \
            infsnz  _OS_Tasks + ((TASK_ID+1)*(6+OS_CODE_POINTER_SIZE)-1), 1, 1      \
            bcf     _OS_Tasks + (TASK_ID+0)*(6+OS_CODE_POINTER_SIZE), 4, 1          \
        _endasm                                                                     \


    #define __OS_TtimersWorkSize()                                                  \
        ASM_SET_BANK;                                                               \
        _fsr = (OS_FSR_TYPE)_OS_Tasks;                                              \
        do {                                                                        \
            _asm                                                                    \
                movlw   (OS_STIMER_SIZE + OS_CODE_POINTER_SIZE + 2)                 \
            _endasm                                                                 \
            if (((near OST_TASK_STATE*)(&_indf))->bDelay)                           \
            {                                                                       \
                _asm                                                                \
                   movlw   (OS_CODE_POINTER_SIZE + 2)                               \
                   addwf   _fsr, 1, 0                                               \
                   incf    _postinc, 1, 0                                           \
                   movlw   0                                                        \
                   addwfc  _postinc, 1, 0                                           \
                   addwfc  _postinc, 1, 0                                           \
                   addwfc  _postinc, 1, 0                                           \
                _endasm                                                             \
                if (_carry)                                                         \
                {                                                                   \
                    _asm                                                            \
                        movlw   (OS_STIMER_SIZE + OS_CODE_POINTER_SIZE + 2)         \
                        subwf   _fsr, 1, 0                                          \
                    _endasm                                                         \
                    ((near OST_TASK_STATE*)(&_indf))->bDelay = 0;                   \
                }                                                                   \
            }                                                                       \
            _asm                                                                    \
                addwf   _fsr, 1, 0                                                  \
            _endasm                                                                 \
        } while (_fsrl != (char)(_OS_Tasks + OS_TASKS));                            \


//------------------------------------------------------------------------------
#endif      // if sizeof(OS_TIMER_TYPE)
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#endif      // OS_ENABLE_TTIMERS
//------------------------------------------------------------------------------













/******************************************************************************************
 *
 *  WORK WITH DYNAMIC TIMERS
 *
 ******************************************************************************************/

//------------------------------------------------------------------------------
#ifdef OS_ENABLE_DTIMERS
//------------------------------------------------------------------------------

#define _OS_DtimersWork_DEFINED

#define _OS_IndfTimerTimeout       ((OS_RAM_NEAR OST_DTIMER_FLAGS*)(&_indf))->bTimeout
#define _OS_IndfTimerActive        ((OS_RAM_NEAR OST_DTIMER_FLAGS*)(&_indf))->bActive
#define _OS_IndfTimerRun           ((OS_RAM_NEAR OST_DTIMER_FLAGS*)(&_indf))->bRun
#define _OS_IndfTimerNextEnable    ((OS_RAM_NEAR OST_DTIMER_FLAGS*)(&_indf))->bNextEnable
#define _OS_PostincTimerNextEnable ((OS_RAM_NEAR OST_DTIMER_FLAGS*)(&_postinc))->bNextEnable


//------------------------------------------------------------------------------
#if   OS_DTIMER_SIZE == 1
//------------------------------------------------------------------------------

    #define _OS_INC_DTIMER()                                                            \
        {                                                                               \
            _asm                                                                        \
                movlw   3                                                               \
                addwf   _fsr, 1, 0                                                      \
                incf    _postdec, 1, 0  /* FSR0L can't overflow here */                 \
                movf    _postdec, 0, 0                                                  \
                movf    _postdec, 0, 0                                                  \
            _endasm                                                                     \
            if (_status & 0x01) _OS_IndfTimerTimeout = 1;                               \
        }


//------------------------------------------------------------------------------
#elif OS_DTIMER_SIZE == 2
//------------------------------------------------------------------------------

    #define _OS_INC_DTIMER()                                                            \
        {                                                                               \
            _OS_DTimers.Flags.bTimeout = 0;                                             \
            _asm                                                                        \
                movlw   3                                                               \
                addwf   _fsr, 1, 0                                                      \
                movlw   0                                                               \
                incf    _postinc, 1, 0                                                  \
                addwfc  _postdec, 1, 0                                                  \
            _endasm                                                                     \
            if (_status & 0x01) _OS_DTimers.Flags.bTimeout = 1;                         \
            _asm                                                                        \
                movlw   3                                                               \
                subwf   _fsr, 1, 0                                                      \
            _endasm                                                                     \
            if (_OS_DTimers.Flags.bTimeout)                                             \
                _OS_IndfTimerTimeout = 1;                                               \
        }

//------------------------------------------------------------------------------
#elif OS_DTIMER_SIZE == 4
//------------------------------------------------------------------------------

    #define _OS_INC_DTIMER()                                                            \
        {                                                                               \
            _OS_DTimers.Flags.bTimeout = 0;                                             \
            _asm                                                                        \
                movlw   3                                                               \
                addwf   _fsr, 1, 0                                                      \
                movlw   0                                                               \
                incf    _postinc, 1, 0                                                  \
                addwfc  _postinc, 1, 0                                                  \
                addwfc  _postinc, 1, 0                                                  \
                addwfc  _postdec, 1, 0                                                  \
            _endasm                                                                     \
            if (_status & 0x01) _OS_DTimers.Flags.bTimeout = 1;                         \
            _asm                                                                        \
                movlw   5                                                               \
                subwf   _fsr, 1, 0                                                      \
            _endasm                                                                     \
            if (_OS_DTimers.Flags.bTimeout)                                             \
                _OS_IndfTimerTimeout = 1;                                               \
        }

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------


#define __OS_DtimersWork()                              \
    {                                                   \
        _fsr = (OS_FSR_TYPE) &_OS_DTimers.Flags;        \
    REPEAT:                                             \
        if (_OS_PostincTimerNextEnable)                 \
        {                                               \
            /* Point FSR to next timer           */     \
            _asm                                        \
                movf    _postinc, 0, 0                  \
                movff   _postdec, _fsr + 1              \
                movwf   _fsr + 0, 0                     \
            _endasm                                     \
            if (!_OS_IndfTimerRun) goto REPEAT;         \
            _OS_INC_DTIMER();                           \
            goto REPEAT;                                \
        }                                               \
    }



//------------------------------------------------------------------------------
#endif  // OS_ENABLE_DTIMER
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

    #define _OS_IF_INC_QTIMER()                              \
        if (!++_indf)

    #define _OS_CHECK_QTIMER()                               \
        _asm    movf    _indf, 1, 0   _endasm

//------------------------------------------------------------------------------
#elif OS_QTIMER_SIZE == 2
//------------------------------------------------------------------------------

    #define _OS_IF_INC_QTIMER()                              \
        if (!++_postinc)                                    \
        if (!++_indf   )                                    \

    #define _OS_CHECK_QTIMER()                              \
        _asm                                                \
            movf    _postinc, 0, 0                          \
            iorwf   _indf, 0, 0                             \
        _endasm

//------------------------------------------------------------------------------
#elif OS_QTIMER_SIZE == 4
//------------------------------------------------------------------------------

    #define _OS_IF_INC_QTIMER()                             \
        if (!++_postinc)                                    \
        if (!++_postinc)                                    \
        if (!++_postinc)                                    \
        if (!++_indf   )                                    \

    #define _OS_CHECK_QTIMER()                              \
        _asm                                                \
            movf    _postinc, 0, 0                          \
            iorwf   _postinc, 0, 0                          \
            iorwf   _postinc, 0, 0                          \
            iorwf   _indf   , 0, 0                          \
        _endasm



//------------------------------------------------------------------------------
#endif  // OS_QTIMER_SIZE
//------------------------------------------------------------------------------

#define _OS_QtimersWork_DEFINED


#define __OS_QtimersWork()                                      \
    if (_OS_Qtimers.Flags.bNextEnable)                          \
    {                                                           \
        _fsr = ((OS_FSR_TYPE)&_OS_Qtimers) + 1;                 \
        _asm                                                    \
            movf    _postinc, 0, 0                              \
            addlw   3                                           \
            movff   _indf, _fsr + 1                             \
            movwf   _fsr + 0, 0                                 \
        _endasm                                                 \
        _OS_IF_INC_QTIMER()                                     \
        do  {                                                   \
            _fsr -= (OS_QTIMER_SIZE + 2);                       \
            _OS_IndfFTimerTimeout = 1;                          \
            _OS_IndfFTimerActive  = 0;                          \
            if (!_OS_IndfFTimerNextEnable)                      \
            {                                                   \
                _OS_Qtimers.Flags.bNextEnable = 0;              \
                break;                                          \
            }                                                   \
            _asm                                                \
                movf    _preinc, 0, 0                           \
                movff   _preinc, _fsr + 1                       \
                movwf   _fsr + 0, 0                             \
            _endasm                                             \
            _OS_Qtimers.Next = (void*)_fsr;                     \
            _fsr += 3;                                          \
            _OS_CHECK_QTIMER();                                 \
        } while (_status & 0x04);                               \
    }

/**/
//------------------------------------------------------------------------------
#endif  // OS_ENABLE_QTIMERS
//------------------------------------------------------------------------------











#endif

//******************************************************************************
//  END OF FILE osa_pic18_mplabc.h
//******************************************************************************

