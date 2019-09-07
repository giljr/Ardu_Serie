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
 *  File:           osa_pic18_mikroc.h
 *
 *  Compilers:      MikroC PRO
 *                  
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    MikorC PRO for PIC18  specific definition
 *                  This file directly included in osa.h
 *
 *  History:        13.09.2010 -    File updated
 *
 *                  26.12.2010 -    OS_RETURN_NOSAVE corrected (NOP added after call to _OS_ReturnNoSave)
 * 
 ************************************************************************************************
 */


#ifndef __OSAMIKROC18_H__
#define __OSAMIKROC18_H__



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

#define _OS_SYSTEM_FLAGS_PROC_SPEC()                                                       \
                                                                                           \
        /*5*/   OST_UINT   bTimeout    : 1; /* Event waiting timeout                */     \
        /*6*/   OST_UINT   bGIE_CTemp  : 1; /* Saved GIEH while in critical section */     \
        /*7*/   OST_UINT   bGIEL_CTemp : 1; /* Saved GIEL while in critical section */     \

#define _OS_TCB_PROC_SPEC()      // Empty macro for this processor


#define OST_TASK_POINTER   OS_TASKS_BANK OST_TCB *

/************************************************************************************************
 *                                                                                              *
 *     Registers definitions                                                                    *
 *                                                                                              *
 ************************************************************************************************/

extern volatile unsigned int _fsr;
extern volatile char         _indf;
extern volatile char         _postinc;
extern volatile char         _postdec;
extern volatile char         _preinc;

extern volatile unsigned char _fsr1l;

extern volatile char         _pcl;
extern volatile char         _pclath;
extern volatile char         _pclatu;
extern volatile char         _status;
extern volatile char         _tosl;
extern volatile char         _tosh;
extern volatile char         _tosu;
extern volatile char         _bsr;
extern volatile char         _wreg;
extern volatile char         _intcon;
extern volatile char         _rcon;
extern volatile char         _stkptr;

#define _gie                GIE_bit
#define _giel               GIEL_bit
#define _ipen               IPEN_bit



/************************************************************************************************
 *                                                                                              *
 *     Constants and types                                                                      *
 *                                                                                              *
 ************************************************************************************************/

#ifndef OS_SMSG_SIZE
#define OS_SMSG_SIZE    0   // We can not use sizeof() because it does not work with #ifdef directive
#endif

//------------------------------------------------------------------------------

#define OS_FSR_TYPE            unsigned int

//------------------------------------------------------------------------------

#if defined(_ROMSIZE) && (_ROMSIZE <= 0x10000)

    #define OST_CODE_POINTER        unsigned int
    #define OS_CODE_POINTER_SIZE    2

#else

    #define OST_CODE_POINTER        unsigned long
    #define OS_CODE_POINTER_SIZE    4

#endif





/************************************************************************************************
 *                                                                                              *
 *     Platform macros                                                                          *
 *                                                                                              *
 ************************************************************************************************/

#define _OS_POP()             asm{ pop }


#if defined(_ROMSIZE) && (_ROMSIZE <= 0x4000)
#define _OS_PIC18_ERRATA_NOP()
#else
#define _OS_PIC18_ERRATA_NOP()     asm{    nop     }
#endif


#define OS_CLRWDT()             asm{ clrwdt }
#define OS_ClrWdt()             asm{ clrwdt }
#define OS_Clrwdt()             asm{ clrwdt }
#define OS_SLEEP()              asm{ sleep  }
#define OS_Sleep()              asm{ sleep  }








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

    #define _OS_SET_PC()                            \
        _OS_Temp = _postinc;                        \
        _pclath =  _postinc;


    #define _OS_SAVE_PC()                           \
        _preinc = _tosl;                            \
        _preinc = _tosh;                            \


    #define _OS_SET_INDF_WORD(value)                                    \
        _postinc = (((OST_CODE_POINTER)value)) & 0xFF;                  \
        _postinc = (((OST_CODE_POINTER)value) >> 8) & 0xFF;


//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------

    #define _OS_SET_PC()                            \
        _OS_Temp = _postinc;                        \
        _pclath =  _postinc;                        \
        _pclatu =  _postinc;


    #define _OS_SAVE_PC()                           \
        _preinc = _tosl;                            \
        _preinc = _tosh;                            \
        _preinc = _tosu;                            \

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
 *                                                                              *
 *  _OS_JUMP_TO_TASK()                                                            *
 *                                                                              *
 *------------------------------------------------------------------------------*
 *                                                                              *
 *  description:        jump indirectly from kernel (OS_Sched) into task        *
 *                                                                              *
 ********************************************************************************
 */

void _OS_JumpToTask (void);

#define _OS_JUMP_TO_TASK()      _OS_JumpToTask()

#define _OS_BACK_TO_SCEDULER()                                                              \
        _OS_SET_FSR_CUR_TASK();                                                             \
        _indf &= ~(OS_TASK_READY_MASK | OS_TASK_CANCONTINUE_MASK);                          \
        _indf |= *((char*)&_OS_State) & (OS_TASK_READY_MASK | OS_TASK_CANCONTINUE_MASK);    \






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

extern void  _OS_SET_FSR_CUR_TASK (void);


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
        _OS_SET_FSR_CUR_TASK();                                                         \
        _postinc = priority | 0x48; /* bEnable è bReady */                              \
        _OS_SET_INDF_WORD(&TaskName);                                                   \
        _OS_RETURN_NO_SAVE();                                                           \
    }

#define OS_Task_Replace_P(priority, TaskAddr)                                           \
    {                                                                                   \
        *((OS_RAM_NEAR OST_UINT*)&_OS_State) = priority | 0x48; /* bEnable è bReady */  \
        _OS_SET_FSR_CUR_TASK();                                                         \
        _indf = priority | 0x48; /* bEnable è bReady */                                 \
        _OS_SET_INDF_WORD(TaskAddr);                                                    \
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

char    OS_DI (void);
void    OS_RI (char);
#define OS_EI()         { _gie = 1; if (_ipen) _giel = 1;}
#define OS_EIH()        { _gie = 1; }
#define OS_EIL()        { _giel = 1; }

//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT)
//------------------------------------------------------------------------------
    void _OS_DI_INT (void);
    void _OS_RI_INT (void);
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------


/************************************************************************************************
 *                                                                                              *
 *                                     C O N D I T I O N S                                      *
 *                                                                                              *
 ************************************************************************************************/

extern void _OS_CheckEvent (OST_UINT);

#define _OS_CHECK_EVENT(event) _OS_CheckEvent(event)




/******************************************************************************************
 *
 *  WORK WITH DYNAMIC TIMERS
 *
 ******************************************************************************************/

//------------------------------------------------------------------------------
#ifdef OS_ENABLE_DTIMERS
//------------------------------------------------------------------------------

#define _OS_IndfTimerTimeout       ((OS_RAM_NEAR OST_DTIMER_FLAGS*)(&_indf))->bTimeout
#define _OS_IndfTimerActive        ((OS_RAM_NEAR OST_DTIMER_FLAGS*)(&_indf))->bActive
#define _OS_IndfTimerRun           ((OS_RAM_NEAR OST_DTIMER_FLAGS*)(&_indf))->bRun
#define _OS_IndfTimerNextEnable    ((OS_RAM_NEAR OST_DTIMER_FLAGS*)(&_indf))->bNextEnable
#define _OS_PostincTimerNextEnable ((OS_RAM_NEAR OST_DTIMER_FLAGS*)(&_postinc))->bNextEnable

//------------------------------------------------------------------------------
#endif  // OS_ENABLE_DTIMERS
//------------------------------------------------------------------------------



#endif




//******************************************************************************
//  END OF FILE osa_pic18_mikroc.h
//******************************************************************************

