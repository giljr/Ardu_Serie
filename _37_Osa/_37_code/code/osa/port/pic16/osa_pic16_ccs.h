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
 *  File:           osa_pic16_ccs.h
 *
 *  Compilers:      CCS
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    CCS for PIC16 specific definition
 *                  This file directly included in osa.h
 *
 *  History:        13.09.2010 -    File updated
 *
 ************************************************************************************************
 */


#ifndef __OSACCS16_H__
#define __OSACCS16_H__



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


//#define _OS_CUR_FLAGS_IN_OS_STATE       // Work with tasks state througth _OS_State variable
#define _OS_CUR_FLAGS_IN_FSR       // Work with tasks state througth FSR



/************************************************************************************************
 *                                                                                              *
 *     Processor specified definitions for system flags                                         *
 *     (see OST_SYSTEM_FLAGS and OST_TCB definition in file "OSA.h")                            *
 *                                                                                              *
 ************************************************************************************************/

#define _OS_SYSTEM_FLAGS_PROC_SPEC()                                                             \
                                                                                                \
        /*5*/   OST_UINT   bTimeout    : 1; /* Event waiting timeout               */      \
        /*6*/   OST_UINT   bGIE_CTemp  : 1; /* Saved GIE while in critical section */      \

#define _OS_TCB_PROC_SPEC()      // Empty macro for this processor


/************************************************************************************************
 *                                                                                              *
 *     Registers definitions                                                                    *
 *                                                                                              *
 ************************************************************************************************/

#byte   _fsr        = 0x04
#byte   _status     = 0x03
#byte   _indf       = 0x00
#byte   _pcl        = 0x02
#byte   _pclath     = 0x0A
#byte   _intcon     = 0x0B

#bit    _carry      = _status.0
#bit    _zero       = _status.2
#bit    _irp        = _status.7
#bit    _gie        = _intcon.7


extern OST_UINT16    _OS_RetAddr_Temp;

/************************************************************************************************
 *                                                                                              *
 *     Constants and types                                                                      *
 *                                                                                              *
 ************************************************************************************************/

#ifndef OS_SMSG_SIZE
#define OS_SMSG_SIZE    0
#endif

//------------------------------------------------------------------------------
#define OS_FSR_TYPE               OST_UINT8

#if sizeof(char*) == 1
    #define OS_RAM_POINTER_SIZE     1
#endif

#if sizeof(char*) == 2
    #define OS_RAM_POINTER_SIZE     2
#endif

//------------------------------------------------------------------------------

#define OS_CODE_POINTER_SIZE        2
#define OST_CODE_POINTER            OST_UINT16



/************************************************************************************************
 *                                                                                              *
 *     Platform specific macros                                                                 *
 *                                                                                              *
 ************************************************************************************************/


#define     OS_CLRWDT()         #asm clrwdt #endasm
#define     OS_SLEEP()          #asm sleep  #endasm


//------------------------------------------------------------------------------
#ifdef _OS_CUR_FLAGS_IN_FSR
//------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    #define OS_CUR_FLAGS_DEFINED
    //------------------------------------------------------------------------------

    #define _OS_bTaskReady              ((OS_RAM_NEAR OST_TASK_STATE)_indf).bReady
    #define _OS_bTaskCanContinue        ((OS_RAM_NEAR OST_TASK_STATE)_indf).bCanContinue
    #define _OS_bTaskDelay              ((OS_RAM_NEAR OST_TASK_STATE)_indf).bDelay
    #define _OS_bTaskTimeout          (!((OS_RAM_NEAR OST_TASK_STATE)_indf).bDelay)
    #define _OS_bTaskEnable             ((OS_RAM_NEAR OST_TASK_STATE)_indf).bEnable
    #define _OS_cTaskPriority           ((OS_RAM_NEAR OST_TASK_STATE)_indf).cPriority
    #define _OS_bTaskPaused             ((OS_RAM_NEAR OST_TASK_STATE)_indf).bPaused

//------------------------------------------------------------------------------
#endif      // _OS_CUR_FLAGS_IN_FSR
//------------------------------------------------------------------------------




/************************************************************************************************
 *                                                                                              *
 *     Context switching macros                                                                 *
 *                                                                                              *
 ************************************************************************************************/

#define _OS_TASK_POINTER_OFFSET         1



#separate void _OS_JumpToTask (void);
#separate void _OS_ReturnSave (char mode);
#separate void _OS_EnterWaitMode (void);
#separate void _OS_EnterWaitModeTO (void);


//------------------------------------------------------------------------------

#define _OS_JUMP_TO_TASK()      _OS_JumpToTask()
#define _OS_BACK_TO_SCEDULER()  #asm   clrf   _status   #endasm


//------------------------------------------------------------------------------

#define _OS_RETURN_SAVE()                               \
    {                                                   \
        _OS_RetAddr_Temp = label_address(label1);       \
        _OS_ReturnSave(0);                              \
        #asm return #endasm                             \
        ;                                               \
    label1:;                                            \
        #undef label1                                   \
    }

//------------------------------------------------------------------------------

#define _OS_ENTER_WAIT_MODE()                           \
    {                                                   \
        _OS_RetAddr_Temp = label_address(label);        \
        _OS_ReturnSave(1);                              \
    label:;                                             \
        #undef label                                    \
    }

//------------------------------------------------------------------------------

#define _OS_ENTER_WAIT_MODE_TO()                        \
    {                                                   \
        _OS_RetAddr_Temp = label_address(label);        \
        _OS_ReturnSave(2);                              \
    label:;                                             \
        #undef label                                    \
    }

//------------------------------------------------------------------------------

#define _OS_RETURN_NO_SAVE()     #asm return #endasm

//------------------------------------------------------------------------------

#define _OS_CHECK_EVENT(event) if (!_OS_CheckEvent(event)) _OS_RETURN_NO_SAVE()

//------------------------------------------------------------------------------


void __OS_SET_FSR_CUR_TASK (void);

#define _OS_SET_FSR_CUR_TASK()      __OS_SET_FSR_CUR_TASK()
#define _OS_SET_FSR_CUR_TASK_W(w)   {__OS_SET_FSR_CUR_TASK();_fsr+=w;}





/************************************************************************************************
 *                                                                                              *
 *     Create, replace, reserve task specific services                                          *
 *                                                                                              *
 ************************************************************************************************/


//******************************************************************************
//  Replace current task with new (current task will be deleted)
//******************************************************************************

#define OS_Task_Replace(priority, TaskName)                                         \
    {                                                                               \
        _OS_SET_FSR_CUR_TASK();                                                     \
        _indf = priority | 0x48; /* bEnable è bReady */                             \
        _OS_CurTask->pTaskPointer = ((OST_CODE_POINTER)TaskName);                   \
        _OS_RETURN_NO_SAVE();                                                       \
    }

#define OS_Task_Replace_P(p,t)   OS_Task_Replace(p,t)

//******************************************************************************
//  Create task
//******************************************************************************

#define OS_Task_Create(priority, TaskName)                          \
    {                                                               \
        _OS_Task_Create(priority, (OST_CODE_POINTER)TaskName);      \
    }

#define OS_Task_Create_M(priority, TaskName)                        \
    {                                                               \
        OS_Task_Reserve(TaskName);                                  \
        _OS_Task_Create(priority, (OST_CODE_POINTER)TaskName);      \
    }


//******************************************************************************
//  Create task by indirrect address
//******************************************************************************

#define OS_Task_Reserve(TaskName)                                   \
        if (_pclath & 0x80) TaskName();                             \





/************************************************************************************************
 *                                                                                              *
 *                                     I N T E R R U P T S                                      *
 *                                                                                              *
 ************************************************************************************************/


char    OS_DI (void);
#define OS_RI(cSavedInt)    OSM_BEGIN {    if (cSavedInt & 0x80) _gie = 1; } OSM_END
#define OS_EI()             OSM_BEGIN {    _gie = 1;} OSM_END

//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT)
//------------------------------------------------------------------------------
    void    _OS_DI_INT (void);
    #define _OS_RI_INT()     OSM_BEGIN {    if (_OS_Temp_I & 0x80) _gie = 1;} OSM_END
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
 *  WORK WITH DYNAMIC TIMERS in OS_Timer
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



#if OS_RAM_POINTER_SIZE == 1

    #define _OS_SER_FSR_DTIMER_NEXT()     _fsr = _indf
    #define _OS_SET_FSR_DTIMER_TIMER()

#endif

#if OS_RAM_POINTER_SIZE == 2

    #define _OS_SER_FSR_DTIMER_NEXT()                       \
            #asm                                            \
                movf    _indf, w                            \
                incf    _fsr, f                             \
                /* here status,z can't be = '1'   */        \
                btfsc   _indf, 0                            \
                bsf     _status, 2                          \
                movwf   _fsr                                \
                bcf     _status, 7                          \
                btfsc   _status, 2                          \
                bsf     _status, 7                          \
            #endasm                                         \

    #define _OS_SET_FSR_DTIMER_TIMER()    _fsr++

#endif



//------------------------------------------------------------------------------
#if   OS_DTIMER_SIZE == 1
//------------------------------------------------------------------------------

    #define _OS_INC_DTIMER()                                \
        {                                                   \
            /* Now W = fsr */                               \
            _OS_SET_FSR_DTIMER_TIMER();                     \
            #asm                                            \
                incf    _fsr, f                             \
                incf    _fsr, f                             \
                bcf     _status, 2                          \
                incf    _indf, f                            \
                movwf   _fsr                                \
            #endasm                                         \
            if (_zero) _OS_IndfTimerTimeout = 1;            \
        }

//------------------------------------------------------------------------------
#elif OS_DTIMER_SIZE == 2
//------------------------------------------------------------------------------

    #define _OS_INC_DTIMER()                                \
        {                                                   \
            /* Now W = fsr */                               \
            _OS_SET_FSR_DTIMER_TIMER();                     \
            #asm                                            \
                bcf     _status, 0                          \
                incf    _fsr, f                             \
                incf    _fsr, f                             \
                incfsz  _indf, f                            \
                goto    label                               \
                incf    _fsr, f                             \
                incfsz  _indf, f                            \
                goto    label                               \
                bsf     _status, 0                          \
        label:                                              \
                movwf   _fsr                                \
            #endasm                                         \
            if (_carry) _OS_IndfTimerTimeout = 1;           \
        }

//------------------------------------------------------------------------------
#elif OS_DTIMER_SIZE == 4
//------------------------------------------------------------------------------

    #define _OS_INC_DTIMER()                                \
        {                                                   \
            /* Now W = fsr */                               \
            _OS_SET_FSR_DTIMER_TIMER();                     \
            #asm                                            \
                bcf     _status, 0                          \
                incf    _fsr, f                             \
                incf    _fsr, f                             \
                incfsz  _indf, f                            \
                goto    label                               \
                incf    _fsr, f                             \
                incfsz  _indf, f                            \
                goto    label                               \
                incf    _fsr, f                             \
                incfsz  _indf, f                            \
                goto    label                               \
                incf    _fsr, f                             \
                incfsz  _indf, f                            \
                goto    label                               \
                bsf     _status, 0                          \
            label:                                          \
                movwf   _fsr                                \
            #endasm                                         \
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
            _OS_SER_FSR_DTIMER_NEXT();                      \
            if (!_OS_IndfTimerRun) goto REPEAT;             \
            _OS_INC_DTIMER();                               \
            goto REPEAT;                                    \
        }                                                   \
    }


//------------------------------------------------------------------------------
#endif  // OS_ENABLE_DTIMER
//------------------------------------------------------------------------------



#endif

//******************************************************************************
//  END OF FILE osa_pic16_ccs.h
//******************************************************************************

