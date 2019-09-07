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
 *  File:           osa_pic16_mikroc.h
 *
 *  Compilers:      MikroC PRO
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    MikorC PRO for PIC16 specific definition
 *                  This file directly included in osa.h
 *
 *  History:        13.09.2010 -    File updated
 *
 ************************************************************************************************
 */


#ifndef __OSAMIKROC16_H__
#define __OSAMIKROC16_H__



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


#define _OS_CUR_FLAGS_IN_FSR       // Work with tasks state througth _OS_State variable



/************************************************************************************************
 *                                                                                              *
 *     Processor specified definitions for system flags                                         *
 *     (see OST_SYSTEM_FLAGS and OST_TCB definition in file "OSA.h")                            *
 *                                                                                              *
 ************************************************************************************************/

#define _OS_SYSTEM_FLAGS_PROC_SPEC()                                                      \
                                                                                          \
        /*5*/   OST_UINT   bTimeout    : 1; /* Event waiting timeout               */     \
        /*6*/   OST_UINT   bGIE_CTemp  : 1; /* Saved GIE while in critical section */     \


#define _OS_TCB_PROC_SPEC()      // Empty macro for this processor


/************************************************************************************************
 *                                                                                              *
 *     Registers definitions                                                                    *
 *                                                                                              *
 ************************************************************************************************/


extern volatile char    _fsr;
extern volatile char    _status;
extern volatile char    _indf;
extern volatile char    _pcl;
extern volatile char    _pclath;
extern volatile char    _intcon;

#define     _carry      CARRY_bit
#define     _zero       ZERO_bit
#define     _irp        IRP_bit
#define     _gie        GIE_bit


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

//------------------------------------------------------------------------------

#define OS_CODE_POINTER_SIZE        2
#define OST_CODE_POINTER            OST_UINT16



/************************************************************************************************
 *                                                                                              *
 *     Platform specific macros                                                                 *
 *                                                                                              *
 ************************************************************************************************/


#define     OS_CLRWDT()         asm {clrwdt }
#define     OS_SLEEP()          asm {sleep  }


//------------------------------------------------------------------------------
#ifdef _OS_CUR_FLAGS_IN_FSR
//------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    #define OS_CUR_FLAGS_DEFINED
    //------------------------------------------------------------------------------

    #define _OS_bTaskReady              ((OS_RAM_NEAR OST_TASK_STATE*)&_indf)->bReady
    #define _OS_bTaskCanContinue        ((OS_RAM_NEAR OST_TASK_STATE*)&_indf)->bCanContinue
    #define _OS_bTaskDelay              ((OS_RAM_NEAR OST_TASK_STATE*)&_indf)->bDelay
    #define _OS_bTaskTimeout          (!((OS_RAM_NEAR OST_TASK_STATE*)&_indf)->bDelay)
    #define _OS_bTaskEnable             ((OS_RAM_NEAR OST_TASK_STATE*)&_indf)->bEnable
    #define _OS_cTaskPriority           ((OS_RAM_NEAR OST_TASK_STATE*)&_indf)->cPriority
    #define _OS_bTaskPaused             ((OS_RAM_NEAR OST_TASK_STATE*)&_indf)->bPaused

//------------------------------------------------------------------------------
#endif      // _OS_CUR_FLAGS_IN_FSR
//------------------------------------------------------------------------------




//******************************************************************************
//  Function prototypes
//******************************************************************************


OST_UINT8 _OS_Task_GetPriority (OST_TASK_POINTER);// pTask);
OST_UINT8 _OS_Task_IsPaused    (OST_TASK_POINTER);// pTask);
OST_UINT8 _OS_Task_IsEnable    (OST_TASK_POINTER);// pTask);

void _OS_JumpToTask (void);
void _OS_ReturnSave (void);
void __OS_SET_FSR_CUR_TASK (void);





/************************************************************************************************
 *                                                                                              *
 *     Context switching macros                                                                 *
 *                                                                                              *
 ************************************************************************************************/

#define _OS_TASK_POINTER_OFFSET             1


#define _OS_JUMP_TO_TASK()    _OS_JumpToTask()


#define _OS_BACK_TO_SCEDULER()                          \
    asm {   clrf    __status    };                      \
    asm {   movlw   hi_addr($)  };                      \
    asm {   movwf   __pclath    };                      \



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

OST_UINT _OS_CheckEvent (OST_UINT bEvent);


//------------------------------------------------------------------------------
#define _OS_RETURN_SAVE()                               \
    {                                                   \
        asm{    movlw   hi_addr(__OS_ReturnSave)    };  \
        asm{    movwf   __pclath                    };  \
        asm{    movlw   $+4           };                \
        asm{    movwf   __fsr         };                \
        asm{    movlw   hi_addr($+0x2)};                \
        asm{    goto    __OS_ReturnSave   };            \
    }

//------------------------------------------------------------------------------
#define _OS_ENTER_WAIT_MODE()                           \
    {                                                   \
        asm{    movlw   hi_addr(__OS_ReturnSave)    };  \
        asm{    movwf   __pclath                    };  \
        asm{    bcf     __pclath, 0         };          \
        asm{    movlw   $+4                 };          \
        asm{    movwf   __fsr               };          \
        asm{    movlw   hi_addr($+0x8002)   };          \
        asm{    goto    __OS_ReturnSave   };            \
    }

//------------------------------------------------------------------------------
#define _OS_ENTER_WAIT_MODE_TO()                        \
    {                                                   \
        asm{    movlw   hi_addr(__OS_ReturnSave)    };  \
        asm{    movwf   __pclath                    };  \
        asm{    bsf     __pclath, 0         };          \
        asm{    movlw   $+4                 };          \
        asm{    movwf   __fsr               };          \
        asm{    movlw   hi_addr($+0x8002)   };          \
        asm{    goto    __OS_ReturnSave   };            \
    }

//------------------------------------------------------------------------------
#define _OS_RETURN_NO_SAVE()                            \
        asm{ return}                                    \

//------------------------------------------------------------------------------


#define _OS_SET_FSR_CUR_TASK()      __OS_SET_FSR_CUR_TASK()
#define _OS_SET_FSR_CUR_TASK_W(w)   {__OS_SET_FSR_CUR_TASK();_fsr+=w;}


//------------------------------------------------------------------------------
#ifdef OS_ADDRESS_TASKS
//------------------------------------------------------------------------------

    #if OS_ADDRESS_TASKS >= 0x100
        #define _OS_SET_IRP_CUR_TASK()  _irp = 1
        #define _OS_CLR_IRP()           _irp = 0
    #else
        #define _OS_SET_IRP_CUR_TASK()  _irp = 0
        #define _OS_CLR_IRP()
    #endif

//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------
    #define _OS_SET_IRP_CUR_TASK()                      \
             asm{    movlw    hi_addr(__OS_Tasks)} ;    \
             asm{    iorlw    0x00                 } ;  \
             asm{    bcf      __status, irp        } ;  \
             asm{    btfss    __status, z          } ;  \
             asm{    bsf      __status, irp        } ;  \

    #define _OS_CLR_IRP()   _irp = 0
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------


#define _OS_CHECK_EVENT(event) if (!_OS_CheckEvent(event)) _OS_RETURN_NO_SAVE()













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
        _indf = priority | 0x48; /* bEnable è bReady */                             \
        _OS_CurTask->pTaskPointer = ((OST_CODE_POINTER)&(TaskName));                \
        _OS_RETURN_NO_SAVE();                                                       \
    }

#define OS_Task_Replace_P(priority, TaskAddr)                                       \
    {                                                                               \
        _OS_SET_FSR_CUR_TASK();                                                     \
        _indf = priority | 0x48; /* bEnable è bReady */                             \
        _OS_CurTask->pTaskPointer = ((OST_CODE_POINTER)TaskAddr);                   \
        _OS_RETURN_NO_SAVE();                                                       \
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
#define OS_RI(cSavedInt)    OSM_BEGIN {    if (cSavedInt & 0x80) _gie = 1;} OSM_END
#define OS_EI()             OSM_BEGIN {    _gie = 1;} OSM_END

//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT)
//------------------------------------------------------------------------------
    void    _OS_DI_INT (void);
    #define _OS_RI_INT()    OSM_BEGIN {    if (_OS_Temp_I & 0x80) _gie = 1;} OSM_END
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------








#endif

