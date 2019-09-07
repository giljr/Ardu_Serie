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
 *  File:           osa_pic18_ccs.h
 *
 *  Compilers:      CCS
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    CCS for PIC18 specific definition
 *                  This file directly included in osa.h
 *
 *  History:        13.09.2010 -    File updated
 *
 *                  26.12.2010 -    OS_RETURN_NOSAVE corrected (NOP added after call to _OS_ReturnNoSave)
 * 
 ************************************************************************************************
 */


#ifndef __OSACCS18_H__
#define __OSACCS18_H__




/************************************************************************************************
 *                                                                                              *
 *     Integer types definitions                                                                *
 *                                                                                              *
 ************************************************************************************************/

#define OST_UINT8     unsigned int8
#define OST_UINT16    unsigned int16
#define OST_UINT32    unsigned int32
#define OST_BOOL      unsigned int8

#define OST_UINT   OST_UINT8
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


#define _OS_CUR_FLAGS_IN_OS_STATE   // Work with tasks state througth _OS_State variable



/************************************************************************************************
 *                                                                                              *
 *     Processor specified definitions for system flags                                         *
 *     (see OST_SYSTEM_FLAGS and OST_TCB definition in file "OSA.h")                            *
 *                                                                                              *
 ************************************************************************************************/

#define _OS_SYSTEM_FLAGS_PROC_SPEC()                                                       \
                                                                                           \
        /*5*/   OST_UINT   bTimeout    : 1; /* Event waiting timeout                */     \
        /*6*/   OST_UINT   bGIE_CTemp  : 1; /* Saved GIEH while in critical section */     \
        /*7*/   OST_UINT   bGIEL_CTemp : 1; /* Saved GIEL while in critical section */     \

#define _OS_TCB_PROC_SPEC()      // Empty macro for this processor


/************************************************************************************************
 *                                                                                              *
 *     Registers definitions                                                                    *
 *                                                                                              *
 ************************************************************************************************/

#word _fsr      = 0xFE9
#byte _fsrl     = 0xFE9
#byte _fsrh     = 0xFEA
#byte _indf     = 0xFEF
#byte _postinc  = 0xFEE
#byte _postdec  = 0xFED
#byte _preinc   = 0xFEC

#byte _fsr1l    = 0xFE1

#byte _pcl      = 0xFF9
#byte _pclath   = 0xFFA
#byte _pclatu   = 0xFFB
#byte _status   = 0xFD8
#byte _tosl     = 0xFFD
#byte _tosh     = 0xFFE
#byte _tosu     = 0xFFF
#byte _bsr      = 0xFE0
#byte _wreg     = 0xFE8
#byte _intcon   = 0xFF2
#byte _rcon     = 0xFD0
#byte _stkptr   = 0xFFC


#bit  _gie      = _intcon.7
#bit  _gieh     = _intcon.7
#bit  _giel     = _intcon.6
#bit  _ipen     = _rcon.7


/************************************************************************************************
 *                                                                                              *
 *     Constants and types                                                                      *
 *                                                                                              *
 ************************************************************************************************/

#ifndef OS_SMSG_SIZE
#define OS_SMSG_SIZE    0
#endif

//------------------------------------------------------------------------------

#define OS_FSR_TYPE            OST_UINT16

//------------------------------------------------------------------------------

#define __SMALL__

#if defined(__SMALL__)
    #define OST_CODE_POINTER    OST_UINT16
    #define OS_CODE_POINTER_SIZE    2
#else
    #define OST_CODE_POINTER    OST_UINT32
    #define OS_CODE_POINTER_SIZE    4
#endif




/************************************************************************************************
 *                                                                                              *
 *     Platform specific macros                                                                 *
 *                                                                                              *
 ************************************************************************************************/


#define _OS_PIC18_ERRATA_NOP()  #asm nop #endasm
#define _OS_POP()         #asm pop #endasm

#define OS_CLRWDT()   #asm  clrwdt  #endasm
#define OS_SLEEP()    #asm  sleep   #endasm









/************************************************************************************************
 *                                                                                              *
 *     Internal macro for work with state flags                                                 *
 *                                                                                              *
 ************************************************************************************************/

//#define _OS_bTaskReady              ((OST_TASK_STATE)_indf).bReady
//#define _OS_bTaskCanContinue        ((OST_TASK_STATE)_indf).bCanContinue
//#define _OS_bTaskDelay              ((OST_TASK_STATE)_indf).bDelay
//#define _OS_bTaskTimeout            (!(((OST_TASK_STATE)_indf).bDelay))
//#define _OS_bTaskEnable             ((OST_TASK_STATE)_indf).bEnable
//#define _OS_cTaskPriority           ((OST_TASK_STATE)_indf).cPriority
//#define _OS_bTaskPaused             ((OST_TASK_STATE)_indf).bPaused



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
#if OS_CODE_POINTER_SIZE == 2
//------------------------------------------------------------------------------

    #define _OS_SET_PC()                    \
        #asm                                \
            movff      _postinc, _wreg      \
            movff      _postinc, _pclath    \
            movwf     _pcl                  \
        #endasm


    #define _OS_SAVE_PC()                   \
        _postdec = _tosh;                   \
        _postdec = _tosl;


    #define _OS_SET_INDF_WORD(value)                                    \
        _postinc = (((OST_CODE_POINTER)value)) & 0xFF;                  \
        _postinc = (((OST_CODE_POINTER)value) >> 8) & 0xFF;

//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------

    #define _OS_SET_PC()                    \
        #asm                                \
            movff      _postinc, _wreg      \
            movff      _postinc, _pclath    \
            movff      _postinc, _pclatu    \
            movwf     _pcl                  \
        #endasm

    #define _OS_SAVE_PC()                   \
        _postdec = 0;                       \
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



#separate void _OS_JumpToTask (void);
#separate void _OS_ReturnSave (void);
#separate void _OS_ReturnNoSave (void);
#separate void _OS_EnterWaitMode (void);
#separate void _OS_EnterWaitModeTO (void);
#separate void _OS_CheckEvent (OST_UINT);


#define _OS_JUMP_TO_TASK()      _OS_JumpToTask()


#define _OS_BACK_TO_SCEDULER()                                                          \
        _OS_SET_FSR_CUR_TASK();                                                         \
        _indf &= ~(OS_TASK_READY_MASK | OS_TASK_CANCONTINUE_MASK);                      \
        _indf |= ((char)_OS_State) & (OS_TASK_READY_MASK | OS_TASK_CANCONTINUE_MASK);   \



//------------------------------------------------------------------------------
#define _OS_RETURN_SAVE()                       \
    {                                           \
        _OS_ReturnSave();                       \
        _OS_PIC18_ERRATA_NOP();                 \
    }

//------------------------------------------------------------------------------
#define _OS_ENTER_WAIT_MODE()                   \
    {                                           \
        _OS_EnterWaitMode();                    \
        _OS_PIC18_ERRATA_NOP();                 \
    }
//------------------------------------------------------------------------------
#define _OS_ENTER_WAIT_MODE_TO()                \
    {                                           \
        _OS_EnterWaitModeTO();                  \
        _OS_PIC18_ERRATA_NOP();                 \
    }
//------------------------------------------------------------------------------
#define _OS_RETURN_NO_SAVE()                    \
    {                                           \
        _OS_ReturnNoSave();                     \
        _OS_PIC18_ERRATA_NOP();   /*101226*/    \
    }                                           \

//------------------------------------------------------------------------------
#define _OS_CHECK_EVENT(event)      _OS_CheckEvent(event)


#define _OS_SET_FSR_CUR_TASK()      _fsr = (OS_FSR_TYPE)&_OS_CurTask->State
#define _OS_SET_FSR_CUR_TASK_W(w)   _fsr = ((OS_FSR_TYPE)&_OS_CurTask->State) + w







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

#define OS_Task_Replace(priority, TaskName)                                         \
    {                                                                               \
        _OS_SET_FSR_CUR_TASK();                                                     \
        _indf = priority | 0x48; /* bEnable и bReady */                             \
        _OS_CurTask->pTaskPointer = ((OST_CODE_POINTER)TaskName);                   \
        _OS_RETURN_NO_SAVE();                                                       \
    }

#define OS_Task_Replace_P(p,t)   OS_Task_Replace(p,t)

/************************************************************************/
/*                                                                      */
/* Create task                                                          */
/*                                                                      */
/************************************************************************/

#define OS_Task_Create(priority, TaskName)                          \
    {                                                               \
        _OS_Task_Create(priority, (OST_CODE_POINTER)TaskName);      \
    }

#define OS_Task_Create_M(priority, TaskName)                        \
    {                                                               \
        OS_Task_Reserve(TaskName);                                  \
        _OS_Task_Create(priority, (OST_CODE_POINTER)TaskName);      \
     }


/************************************************************************/
/*                                                                      */
/* Create task by indirrect address                                     */
/*                                                                      */
/************************************************************************/

#define OS_Task_Reserve(TaskName)                                   \
        if (_pcl & 1) TaskName();                                   \





/************************************************************************************************
 *                                                                                              *
 *                                     I N T E R R U P T S                                      *
 *                                                                                              *
 ************************************************************************************************/

char    OS_DI (void);
void    OS_RI (char);
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
 * T I M E R S    M A C R O S    D E F I N I T I O N S
 *
 ******************************************************************************************/

//------------------------------------------------------------------------------
#ifdef OS_ENABLE_TTIMERS
//------------------------------------------------------------------------------


    #if OS_TTIMER_SIZE == 2

        // CCS18 has and error when working with ++(int*).

        #define __OS_TtimersWorkSize()                                                      \
        {                                                                                   \
            OST_UINT _os_i;                                                                 \
            ASM_OST_TCB_SIZE_CONST;                                                         \
            ASM_SET_BANK;                                                                   \
            for (_os_i = 0; _os_i < OS_TASKS; _os_i++)                                      \
            {                                                                               \
                if (_OS_Tasks[_os_i].State.bDelay){                                         \
                    _OS_Tasks[_os_i].Timer++;                                               \
                    if (!_OS_Tasks[_os_i].Timer) _OS_Tasks[_os_i].State.bDelay = 0;         \
                }                                                                           \
            }                                                                               \
        }

    #endif

//------------------------------------------------------------------------------
#endif
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

#define _OS_IndfTimerTimeout       ((OS_RAM_NEAR OST_DTIMER_FLAGS)(_indf)).bTimeout
#define _OS_IndfTimerActive        ((OS_RAM_NEAR OST_DTIMER_FLAGS)(_indf)).bActive
#define _OS_IndfTimerRun           ((OS_RAM_NEAR OST_DTIMER_FLAGS)(_indf)).bRun
#define _OS_IndfTimerNextEnable    ((OS_RAM_NEAR OST_DTIMER_FLAGS)(_indf)).bNextEnable
#define _OS_PostincTimerNextEnable ((OS_RAM_NEAR OST_DTIMER_FLAGS)(_postinc)).bNextEnable





#if   OS_DTIMER_SIZE == 1
    #define _OS_INC_DTIMER()                                                            \
        {                                                                               \
            #asm                                                                        \
                movlw   3                                                               \
                addwf   _fsr, f                                                         \
                incf    _postdec, f     /* Здесь не может быть переполнения FSR0L */    \
                movf    _postdec, f                                                     \
                movf    _postdec, f                                                     \
            #endasm                                                                     \
            if (_status & 0x01) _OS_IndfTimerTimeout = 1;                               \
        }


#elif OS_DTIMER_SIZE == 2
    #define _OS_INC_DTIMER()                                                            \
        {                                                                               \
            _OS_DTimers.Flags.bTimeout = 0;                                             \
            #asm                                                                        \
                movlw   3                                                               \
                addwf   _fsr, f                                                         \
                movlw   0                                                               \
                incf    _postinc, f                                                     \
                addwfc  _postdec, f                                                     \
            #endasm                                                                     \
            if (_status & 0x01) _OS_DTimers.Flags.bTimeout = 1;                         \
            #asm                                                                        \
                movlw   3                                                               \
                subwf   _fsr, f                                                         \
            #endasm                                                                     \
            if (_OS_DTimers.Flags.bTimeout)                                             \
                _OS_IndfTimerTimeout = 1;                                               \
        }


#elif OS_DTIMER_SIZE == 4
    #define _OS_INC_DTIMER()                                                            \
        {                                                                               \
            _OS_DTimers.Flags.bTimeout = 0;                                             \
            #asm                                                                        \
                movlw   3                                                               \
                addwf   _fsr, f                                                         \
                movlw   0                                                               \
                incf    _postinc, f                                                     \
                addwfc  _postinc, f                                                     \
                addwfc  _postinc, f                                                     \
                addwfc  _postdec, f                                                     \
            #endasm                                                                     \
            if (_status & 0x01) _OS_DTimers.Flags.bTimeout = 1;                         \
            #asm                                                                        \
                movlw   5                                                               \
                subwf   _fsr, f                                                         \
            #endasm                                                                     \
            if (_OS_DTimers.Flags.bTimeout)                                             \
                _OS_IndfTimerTimeout = 1;                                               \
        }

#endif


#define __OS_DtimersWork()                              \
    {                                                   \
        _fsr = (OS_FSR_TYPE) &_OS_DTimers.Flags;        \
    REPEAT:                                             \
        if (_OS_PostincTimerNextEnable)                 \
        {                                               \
            /* Устанавливаем FSR на след. таймер */     \
            #asm                                        \
                movf    _postinc, w                     \
                movff   _postdec, _fsrh                 \
                movwf   _fsrl                           \
            #endasm                                     \
            if (!_OS_IndfTimerRun) goto REPEAT;         \
            _OS_INC_DTIMER();                           \
            goto REPEAT;                                \
        }                                               \
    }


//------------------------------------------------------------------------------
#endif  // OS_ENABLE_DTIMER
//------------------------------------------------------------------------------







#endif

//******************************************************************************
//  END OF FILE osa_pic18_ccs.h
//******************************************************************************

