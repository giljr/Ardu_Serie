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
 *  File:           osa_pic18_mplabc_ext.h
 *
 *  Compilers:      Mplab C18
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    MPLABC18 for extended mode specific definition
 *                  This file directly included in osa.h
 *
 *  History:        13.09.2010 -    File updated
 *
 ************************************************************************************************
 */

#ifndef __OSAMCC18_EXT_H__
#define __OSAMCC18_EXT_H__



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

#define OST_BOOL             OST_UINT


#define _OS_CUR_FLAGS_IN_FSR        // Work with tasks state througth FSR



/************************************************************************************************
 *                                                                                              *
 *     Processor specified definitions for system flags                                         *
 *     (see OST_SYSTEM_FLAGS and OST_TCB definition in file "OSA.h")                            *
 *                                                                                              *
 ************************************************************************************************/

#define _OS_SYSTEM_FLAGS_PROC_SPEC()                                                        \
                                                                                            \
        /*5*/   OST_UINT   bTimeout    : 1; /* Event waiting timeout                */      \
        /*6*/   OST_UINT   bGIE_CTemp  : 1; /* Saved GIEH while in critical section */      \
        /*7*/   OST_UINT   bGIEL_CTemp : 1; /* Saved GIEL while in critical section */      \

#define _OS_TCB_PROC_SPEC()                                                                 \
                unsigned char   cTempFSR1;          /* Saved FSR1 (for MCC18) */            \

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


    #define _OS_SET_INDF_WORD(value)                                \
        _postinc = (((OST_CODE_POINTER)value)) & 0xFF;              \
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

    #define _OS_SET_INDF_WORD(value)                                \
        _postinc = (((OST_CODE_POINTER)value)) & 0xFF;              \
        _postinc = (((OST_CODE_POINTER)value) >> 8) & 0xFF;         \
        _postinc = (((OST_CODE_POINTER)value) >> 16) & 0xFF;        \
        _postinc = 0;

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

void _OS_JumpToTask         (void);

#define _OS_JUMP_TO_TASK()    _OS_JumpToTask()

#define _OS_BACK_TO_SCEDULER()                                              \
        if (_fsr1l != _OS_TempH) {                                          \
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

extern void __OS_SET_FSR_CUR_TASK_W (OST_UINT w);

#define _OS_SET_FSR_CUR_TASK(w)     __OS_SET_FSR_CUR_TASK_W(0)
#define _OS_SET_FSR_CUR_TASK_W(w)   __OS_SET_FSR_CUR_TASK_W(w)


//------------------------------------------------------------------------------
#define _OS_RETURN_SAVE()           {                                   \
                                        _OS_ReturnSave();               \
                                        _OS_PIC18_ERRATA_NOP();         \
                                    }

//------------------------------------------------------------------------------
#define _OS_ENTER_WAIT_MODE()       {                                   \
                                        _OS_EnterWaitMode();            \
                                        _OS_PIC18_ERRATA_NOP();         \
                                    }

//------------------------------------------------------------------------------
#define _OS_ENTER_WAIT_MODE_TO()                                        \
                                    {                                   \
                                        _OS_EnterWaitModeTO();          \
                                        _OS_PIC18_ERRATA_NOP();         \
                                    }

//------------------------------------------------------------------------------
#define _OS_RETURN_NO_SAVE()        _OS_ReturnNoSave()

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

/************************************************************************************************
 *                                                                                              *
 *                                     C O N D I T I O N S                                      *
 *                                                                                              *
 ************************************************************************************************/

extern void _OS_CheckEvent (void);

#define _OS_CHECK_EVENT(event) {_OS_Temp = event; _OS_CheckEvent();}









#endif

//******************************************************************************
//  END OF FILE osa_pic18_mplabc_ext.h
//******************************************************************************

