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
 *  File:           osa_pic16e_htpicc_pro.h
 *
 *  Compilers:      HT-PICC PRO
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    HTPICC PRO for PIC16F1xxx specific definition
 *                  This file directly included in osa.h
 *
 *  History:        13.09.2010 -    File updated
 *
 ************************************************************************************************
 */


#ifndef __OSAPICC16E_H__
#define __OSAPICC16E_H__

#include <htc.h>


/************************************************************************************************
 *                                                                                              *
 *     Integer types definitions                                                                *
 *                                                                                              *
 ************************************************************************************************/

typedef unsigned char   OST_UINT8;
typedef unsigned int    OST_UINT16;
typedef unsigned long   OST_UINT32;
typedef unsigned char   OST_UINT;
typedef bit             OST_BOOL;

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

static volatile unsigned int    _fsr    @ 0x04;
static volatile unsigned char   _fsrl   @ 0x04;
static volatile unsigned char   _fsrh   @ 0x05;
static volatile unsigned char _indf      @ 0x00;
static volatile unsigned char _pcl       @ 0x02;
static volatile unsigned char _status    @ 0x03;
static volatile unsigned char _bsr       @ 0x08;
static volatile unsigned char _pclath    @ 0x0A;
static volatile unsigned char _intcon    @ 0x0B;
static volatile unsigned char _stkptr    @ 0xFED;
static volatile unsigned char _tosl      @ 0xFEE;
static volatile unsigned char _tosh      @ 0xFEF;

static volatile       bit       _carry      @((unsigned)&_status*8)+0;
static volatile       bit       _zero       @((unsigned)&_status*8)+2;
static                bit       _irp        @((unsigned)&_status*8)+7;
static                bit       _gie        @((unsigned)&_intcon*8)+7;


#define _PIC16E_POP()   _stkptr--;


/************************************************************************************************
 *                                                                                              *
 *     Constants and types                                                                      *
 *                                                                                              *
 ************************************************************************************************/

#undef  OS_SMSG_SIZE
#define OS_SMSG_SIZE            sizeof(OS_SMSG_TYPE)

typedef unsigned int            OS_FSR_TYPE;

// Type of code pointer (used in TCB)
typedef unsigned int            OST_CODE_POINTER;






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

//#define _OS_TASK_POINTER_OFFSET         1


#define _OS_SET_PC()        asm("   dw      0x0013      ");     /* moviw  INDF0--   */  \
                            asm("   dw      0x008A      ");     /* movwf  PCLATH    */  \
                            asm("   dw      0x0013      ");     /* moviw  INDF0--   */  \
                            asm("   dw      0x0082      ");     /* movwf  PCL       */  \

#define _OS_SAVE_PC()       asm("   movlb   0x1F        ");                             \
                            asm("   dw      0x086F      ");     /* movf   TOSH, w   */  \
                            asm("   dw      0x001B      ");     /* movwi  INDF0--   */  \
                            asm("   dw      0x086E      ");     /* movf   TOSL, w   */  \
                            asm("   dw      0x001B      ");     /* movwi  INDF0--   */  \
                            asm("   movlb   0x00        ");                             \




void _OS_JumpToTask (void);


#define _OS_JUMP_TO_TASK()          _OS_JumpToTask()

#define _OS_BACK_TO_SCEDULER()      asm("   movlb   0   ")



void _OS_ReturnSave (void);
void _OS_EnterWaitMode (void);
void _OS_EnterWaitModeTO (void);


//------------------------------------------------------------------------------

#define _OS_RETURN_SAVE()           _OS_ReturnSave()
#define _OS_ENTER_WAIT_MODE()       _OS_EnterWaitMode()
#define _OS_ENTER_WAIT_MODE_TO()    _OS_EnterWaitModeTO()
#define _OS_RETURN_NO_SAVE()        asm("   return                  ")

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Set FSR to point to current task
//------------------------------------------------------------------------------


#define _OS_SET_FSR_CUR_TASK()      _fsr = (OS_FSR_TYPE)_OS_CurTask
#define _OS_SET_FSR_CUR_TASK_W(w)   {_fsr = (OS_FSR_TYPE)_OS_CurTask; asm(" dw 0x3100 +"#w);}






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
    {                                                                           \
        _OS_SET_FSR_CUR_TASK();                                                 \
        _indf = priority | 0x48; /* bEnable è bReady */                         \
        FSR1 = (OST_UINT16)TaskName;                                            \
        asm("   dw      0x0806          ");      /* movf    FSR1L, w    */      \
        asm("   dw      0x0018          ");      /* movwi   ++FSR0      */      \
        asm("   dw      0x0807          ");      /* movf    FSR1H, w    */      \
        asm("   dw      0x0018          ");      /* movwi   ++FSR0      */      \
        _OS_RETURN_NO_SAVE();                                                   \
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
        if (_pclath & 0x80) TaskName();                             \







/************************************************************************************************
 *                                                                                              *
 *                                     I N T E R R U P T S                                      *
 *                                                                                              *
 ************************************************************************************************/

#ifndef     di
    #define di()    (GIE = 0)       // interrupt enable bit
#endif


OST_UINT8   OS_DI (void);
#define     OS_RI(cSavedInt)    OSM_BEGIN {    if (cSavedInt & 0x80) _gie = 1; } OSM_END
#define     OS_EI()             OSM_BEGIN {    _gie = 1; } OSM_END


#ifdef OS_ENABLE_INT

    void    _OS_DI_INT (void);
    void    _OS_RI_INT (void);

#endif

/************************************************************************************************
 *                                                                                              *
 *                                     C O N D I T I O N S                                      *
 *                                                                                              *
 ************************************************************************************************/

extern void _OS_CheckEvent (OST_UINT);

#define _OS_CHECK_EVENT(event) _OS_CheckEvent(event)






#endif






//******************************************************************************
//  END OF FILE osa_pic16e_htpicc_pro.h
//******************************************************************************

