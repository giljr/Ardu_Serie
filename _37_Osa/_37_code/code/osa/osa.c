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
 *  File:           osa.c
 *
 *  Compilers:      HT-PICC STD
 *                  HT-PICC PRO
 *                  HT-PICC18 STD
 *                  Mplab C18
 *                  Mplab C30
 *                  MikroC PRO
 *                  CCS
 *                  WinAVR
 *                  IAR
 *                  Cosmic
 *                  Raisonance
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    This file contains all system variables and functions definitions.
 *
 *  History:        10.09.2010 -    File updated
 *
 ************************************************************************************************
 */


#include <OSA.h>


/*
 ************************************************************************************************
 *                                                                                              *
 *  V A R I A B L E S   D E F I N I T I O N                                                     *
 *                                                                                              *
 ************************************************************************************************
 */

//------------------------------------------------------------------------------
#ifdef __OSA_PIC18_MPLABC__
#pragma udata access ACCESS
#endif
//------------------------------------------------------------------------------


    volatile OS_RAM_NEAR    OST_SYSTEM_FLAGS    _OS_Flags;      // System state flags
             OS_RAM_NEAR    OST_WORD            _OS_Temp;       // For internal purpose
    
    #if defined(OS_ENABLE_INT) || defined(OS_PROTECT_MEMORY_ACCESS)
    volatile OS_RAM_NEAR    OST_WORD            _OS_Temp_I;
    #endif
    
    #ifndef __OSA_PIC12_HTPICC__
    volatile OS_RAM_NEAR    OST_WORD            _OS_TempH;      // For internal purpose
    #endif
    
    #if defined(_OS_CUR_FLAGS_IN_OS_STATE)
    volatile OS_RAM_NEAR    OST_TASK_STATE      _OS_State;
    #endif
    
    #ifdef __OSA_CCS__
// The CCS compiler do not allows to declare a volatile pointers
    OST_TASK_POINTER OS_RAM_NEAR           _OS_CurTask;    // Pointer to current task
    #else
    OST_TASK_POINTER OS_RAM_NEAR volatile  _OS_CurTask;    // Pointer to current task
    #endif
                                                                    // descriptor.
//------------------------------------------------------------------------------
#ifdef __OSA_PIC18_MPLABC__
#pragma udata
#endif
//------------------------------------------------------------------------------





//------------------------------------------------------------------------------
#if OS_PRIORITY_LEVEL == OS_PRIORITY_NORMAL
//------------------------------------------------------------------------------
#if (OS_BANK_OS == 0) && defined(__OSA_PIC18_MPLABC__)
#pragma udata access os_bsems
#endif

    #if defined(__OSA_CCS__) || defined(__OSA_IAR__)
    // The CCS compiler do not allows to declare a volatile pointers
    OST_TASK_POINTER    OS_BANK          _OS_BestTask;  // Pointer to ready task with the highest priority
    OST_TASK_POINTER    OS_BANK          _OS_LastTask;  // Pointer to last executed task
    #else
    OST_TASK_POINTER    OS_BANK volatile _OS_BestTask;  // Pointer to ready task with the highest priority
    OST_TASK_POINTER    OS_BANK volatile _OS_LastTask;  // Pointer to last executed task
    #endif
    OS_BANK OST_WORD                     _OS_Best_Priority;

#if (OS_BANK_OS == 0) && defined(__OSA_PIC18_MPLABC__)
#pragma udata
#endif

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#if OS_PRIORITY_LEVEL == OS_PRIORITY_EXTENDED
//------------------------------------------------------------------------------
#if (OS_BANK_OS == 0) && defined(__OSA_PIC18_MPLABC__)
#pragma udata access os_bsems
#endif

    OS_BANK OST_UINT    _OS_Best_Priority;
    OS_BANK OST_UINT    _OS_Worst_Priority;
    OS_BANK OST_UINT    _OS_Best_n;
    OS_BANK OST_UINT    _OS_Cur_Pos;
    OS_BANK OST_UINT    _OS_n;
    OS_BANK OST_UINT8   _OS_TaskQueue[OS_TASKS];    // Queue of lesting in scheduler
    OS_BANK OST_UINT8   _OS_TaskLevel[OS_TASKS];    // Current priority levels

#if (OS_BANK_OS == 0) && defined(__OSA_PIC18_MPLABC__)
#pragma udata
#endif

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------



/*
 ************************************************************************************************
 *  I N C L U D E   C O M P I L E R   S P E C I F I E D   D E F I N I T I O N S                 *
 ************************************************************************************************
 */

#include "port\osa_include.c"





//******************************************************************************
//  INCLUDE INTERNAL SERVICE FUNCTIONS
//******************************************************************************



#include "kernel\timers\osa_dtimer.c"
#include "kernel\timers\osa_qtimer.c"
#include "kernel\timers\osa_stimer.c"
#include "kernel\timers\osa_ttimer.c"

#include "kernel\events\osa_squeue.c"
#include "kernel\events\osa_queue.c"
#include "kernel\events\osa_csem.c"
#include "kernel\events\osa_bsem.c"

#include "kernel\system\osa_system.c"
#include "kernel\system\osa_tasks.c"






//******************************************************************************
//  END OF FILE osa.c
//******************************************************************************


