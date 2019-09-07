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
 *  File:           osa.h
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
 *  Description:    This file contains all prototypes of functions and variables,
 *                  data types definitions, system constants.
 *                  This file must be included in all modules that use OSA services.
 *
 *  History:        10.00.2010 -    Removed all modification marks
 *  History:        10.07.2010 -    Added ports for STM8: IAR and Raisonance
 * 
 *                  25.10.2010  -   Bug fixed: OS_Qtimer_Break did not deleted timer from the list
 *                              -   Bug fixed: interrupt became disables after calling OS_Qtimer_Run for timer already
 *                                  presented in queue
 *                              -   (osa_qtimer.c)
 *                              -   (osa_pic16_htpicc.c)
 *                              -   Definition of OS_Ttimer_Delay fixed (osa_ttimer.h)
 * 
 *                  30.10.2010  -   Another bug in qtimers fixed (osa_qtimer.c, osa_pic16_htpicc.c)
 * 
 *                  22.11.2010  -   IAR for AVR port bugs fixed (osa_avr_iar.h)
 * 
 *                  08.12.2010  -   osa_pic18_htpicc.h
 * 
 *                  26.12.2010  -   Bug fixed for PIC18: _OS_RETURN_NO_SAVE
 * 
 *                  06.03.2011  -   osa_avr_winavr.c
 *                                  osa_avr_winavr.h
 *                                  osa_pic18_mikroc.c 
 *
 *
 ************************************************************************************************
 */

#ifndef __OSA__
#define __OSA__



#define __OSA_VERSION__         110306          /* 6 mar, 2011    */




#define __OSAVERSION__          __OSA_VERSION__ /*old*/

/*
 ************************************************************************************************
 *                                                                                              *
 *  Setting compiler constant. This constant used in "osa.c" to make most                       *
 *  efficient code for used compiler.                                                           *
 *                                                                                              *
 *                                                                                              *
 ************************************************************************************************
 */

//------------------------------------------------------------------------------
#if   defined(__PICC18__)           /* HT-PICC18        */

    #define __OSA_PIC18_HTPICC__
    #define __OSA_PIC18__
    #define __OSA_HTPICC__

    #define __OSAPICC18__   /*old*/
    #define __OSAPICC__     /*old*/
    #define __OSA18__       /*old*/
    #define __OSAPIC__      /*old*/

//------------------------------------------------------------------------------
#elif defined(__18CXX)              /* Microchip C18    */

    #if __EXTENDED18__ == 1
        #define __OSA_PIC18_MPLABC_EXT__     
        #define __OSAMCC18EXT__             /*old*/ 
    #else
        #define __OSA_PIC18_MPLABC__
        #define __OSAMCC18__                /*old*/
    #endif

    #define __OSA_PIC18__
    #define __OSA_MPLABC__

    #define __OSA18__       /*old*/
    #define __OSAPIC__      /*old*/

//------------------------------------------------------------------------------
#elif defined(__C30__)              /* Microchip C30    */

    #define __OSA_PIC24_MPLABC__
    #define __OSA_DSPIC_MPLABC__

    #define __OSA_MPLABC__
    // See osa_pic24_mplabc.h for processor definition

    #define __OSAMCC30__    /*old*/
    #define __OSA30__       /*old*/
    #define __OSAPIC__      /*old*/

//------------------------------------------------------------------------------
#elif defined(__PCH__)              /* CCS for PIC18  */

    #define __OSA_PIC18_CCS__
    #define __OSA_PIC18__
    #define __OSA_CCS__

    #define __OSACCS18__    /*old*/
    #define __OSACCS__      /*old*/
    #define __OSA18__       /*old*/
    #define __OSAPIC__      /*old*/

//------------------------------------------------------------------------------
#elif defined(__PCM__)              /* CCS for PIC16  */

    #define __OSA_PIC16_CCS__
    #define __OSA_PIC16__
    #define __OSA_CCS__

    #define __OSACCS16__    /*old*/
    #define __OSACCS__      /*old*/
    #define __OSA16__       /*old*/
    #define __OSAPIC__      /*old*/

//------------------------------------------------------------------------------
#elif defined(_PIC12)               /* HT-PICC with baserange family  */

    #define __OSA_PIC12_HTPICC__
    #define __OSA_PIC12__
    #define __OSA_HTPICC__

    #define __OSAPICC12__   /*old*/
    #define __OSAPICC__     /*old*/
    #define __OSA12__       /*old*/
    #define __OSAPIC__      /*old*/

//------------------------------------------------------------------------------
#elif defined(__MIKROC_PRO_FOR_PIC__)

    #if   defined(P16)              /* mikroC for PIC16 */
        #define __OSA_PIC16_MIKROC__
        #define __OSA_PIC16__
        #define __OSA_MIKROC__

        #define __OSAMIKROC16__         /*old*/
        #define __OSAMIKROC__           /*old*/
        #define __OSA16__               /*old*/
    #elif defined(P18)              /* mikroC for PIC18 */
        #define __OSA_PIC18_MIKROC__
        #define __OSA_PIC18__
        #define __OSA_MIKROC__

        #define __OSAMIKROC18__         /*old*/
        #define __OSAMIKROC__           /*old*/
        #define __OSA18__               /*old*/
    #endif

    #define __OSAPIC__                  /*old*/
//------------------------------------------------------------------------------
#elif defined(__AVR__)

    #define __OSA_AVR_WINAVR__
    #define __OSA_WINAVR__
    #define __OSA_AVR__

    #define __OSAWINAVR__       /*old*/
    #define __OSAAVR__          /*old*/

    #define OS_PROTECT_MEMORY_ACCESS            // for atomic access protection

//------------------------------------------------------------------------------
#elif defined(__ICCAVR__)

    #define __OSA_AVR_IAR__
    #define __OSA_IAR__
    #define __OSA_AVR__

    #define __OSAIARAVR__       /*old*/
    #define __OSAAVR__          /*old*/
    #define __OSAICCAVR__       /*old*/
 
    #define OS_PROTECT_MEMORY_ACCESS            // for atomic access protection

//------------------------------------------------------------------------------
#elif defined(__CODEVISIONAVR__)

    #define __OSA_AVR__
    #define __OSA_AVR_CODEVISION__
    #define __OSA_CODEVISION__

    #define OS_PROTECT_MEMORY_ACCESS            // for atomic access protection

//------------------------------------------------------------------------------
#elif defined(__ICCSTM8__)

    #define __OSA_STM8_IAR__
    #define __OSA_STM8__
    #define __OSA_IAR__

    #define __OSAIARSTM8__      /*old*/
    #define __OSASTM8__         /*old*/
    #define __OSAICCSTM8__      /*old*/

    #define OS_PROTECT_MEMORY_ACCESS            // for atomic access protection

//------------------------------------------------------------------------------
#elif defined(__CSMC__)

    #define __OSA_STM8_COSMIC__
    #define __OSA_STM8__
    #define __OSA_COSMIC__

    #define __OSACOSMICSTM8__   /*old*/
    #define __OSASTM8__         /*old*/

    #define OS_PROTECT_MEMORY_ACCESS            // for atomic access protection

//------------------------------------------------------------------------------
#elif defined(__RCST7__)            /* Raisonance*/

    #define __OSA_STM8_RAISONANCE__
    #define __OSA_STM8__
    #define __OSA_RAISONANCE__
    #define __OSA_RCST7__

//------------------------------------------------------------------------------
#else                               /* HT-PICC with midrange family */

    #if _HTC_VER_MAJOR_ >= 9 && _HTC_VER_MINOR_ > 60
        #ifdef _PIC14E
            #define __OSA_PIC16E_HTPICC_PRO__
            #define __OSA_PIC16E__
            #define __OSA_HTPICC_PRO__

            #define __OSAPICC16E__              /*old*/
            #define __OSA16E__                  /*old*/
        #else
            #define __OSA_PIC16_HTPICC__
            #define __OSA_PIC16__
            #define __OSA_HTPICC__

            #define __OSAPICC16__               /*old*/
            #define __OSA16__                   /*old*/
        #endif
        #define __OSAPICC__         /*old*/
        #define __OSAPICCPRO__      /*old*/
        #define __OSAPIC__          /*old*/
    #else
        #define __OSA_PIC16_HTPICC__
        #define __OSA_PIC16__
        #define __OSA_HTPICC__

        #define __OSAPICC16__       /*old*/
        #define __OSAPICC__         /*old*/
        #define __OSA16__           /*old*/
        #define __OSAPIC__          /*old*/
    #endif


#endif
//------------------------------------------------------------------------------

#include "OSAcfg.h"         /* This file contains project configuration         */


#if defined(__OSACCS__) || defined(__OSAMIKROC__) || defined(__OSA_AVR_WINAVR__)

    #define OSM_BEGIN
    #define OSM_END

#else

    #define OSM_BEGIN   do
    #define OSM_END     while (0)

#endif




/*
 ************************************************************************************************
 *                                                                                              *
 * OSA CONFIGURATION                                                                            *
 *                                                                                              *
 ************************************************************************************************
 */


//******************************************************************************
//  PRIORITY MODE
//******************************************************************************

#define OS_PRIORITY_NORMAL      0
#define OS_PRIORITY_DISABLED    1
#define OS_PRIORITY_EXTENDED    2


//------------------------------------------------------------------------------
#ifndef OS_PRIORITY_LEVEL
//------------------------------------------------------------------------------

    #if     defined(OS_DISABLE_PRIORITY)
    #define OS_PRIORITY_LEVEL   OS_PRIORITY_DISABLED
    #elif   defined(OS_EXTENDED_PRIORITY)
    #define OS_PRIORITY_LEVEL   OS_PRIORITY_EXTENDED
    #else
    #define OS_PRIORITY_LEVEL   OS_PRIORITY_NORMAL
    #endif

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------




//******************************************************************************
// 
//  CHECK FOR SET OF SERVICES
// 
//******************************************************************************


#if defined(OS_ENABLE_ALL)

    //------------------------------------------------------------------------------
    // undef all config constants

    #ifdef OS_ENABLE_TIMER
    #undef OS_ENABLE_TIMER
    #endif

    #ifdef OS_ENABLE_CSEM
    #undef OS_ENABLE_CSEM
    #endif

    #ifdef OS_ENABLE_QUEUE
    #undef OS_ENABLE_QUEUE
    #endif

    #ifdef OS_ENABLE_SQUEUE
    #undef OS_ENABLE_SQUEUE
    #endif

    #ifdef OS_ENABLE_CRITICAL_SECTION
    #undef OS_ENABLE_CRITICAL_SECTION
    #endif


    //------------------------------------------------------------------------------
    // undef all old style config constants

    #ifdef OS_TIMER_ENABLE
    #undef OS_TIMER_ENABLE
    #endif

    #ifdef OS_CSEM_ENABLE
    #undef OS_CSEM_ENABLE
    #endif

    #ifdef OS_QMSG_ENABLE
    #undef OS_QMSG_ENABLE
    #endif

    #ifdef OS_QSMSG_ENABLE
    #undef OS_QSMSG_ENABLE
    #endif

    #ifdef OS_DTIMERS_ENABLE
    #undef OS_DTIMERS_ENABLE
    #endif

    #ifdef OS_ENABLE_QMSG
    #undef OS_ENABLE_QMSG
    #endif

    #ifdef OS_ENABLE_QSMSG
    #undef OS_ENABLE_QSMSG
    #endif


    //------------------------------------------------------------------------------
    // define all config constants

    #define OS_ENABLE_CSEM
    #define OS_ENABLE_QUEUE
    #define OS_ENABLE_SQUEUE
    #define OS_ENABLE_TTIMERS
    #define OS_ENABLE_CRITICAL_SECTION

#endif

#if defined(OS_ENABLE_QTIMER) && defined(__OSA_PIC12_HTPICC__) 
#error "OSA error #22: Qtimers are not supported under 12-bit controllers (PIC10 and PIC12)"
#endif


/*------------------------------------------*/
/*                                          */
/* (OSA configuration)                      */
/* Redefining old style constants           */
/*                                          */
/*------------------------------------------*/

#ifdef OS_TIMER_ENABLE
#define OS_ENABLE_TIMER     /* Defined when using timers in tasks (OS_Delay     */
                            /* and wait events with timeout)                    */
#endif

#ifdef OS_CSEM_ENABLE
#define OS_ENABLE_CSEM      /* Defined when using counting semaphores           */
#endif

#ifdef OS_QMSG_ENABLE       //^
#define OS_ENABLE_QUEUE      /* Defined when using queue of pointer to messages  */
#endif

#ifdef OS_ENABLE_QMSG       //^
#define OS_ENABLE_QUEUE      /* Defined when using queue of pointer to messages  */
#endif

#ifdef OS_QSMSG_ENABLE      //^
#define OS_ENABLE_SQUEUE     /* Defined when using queue of simple messages      */
#endif

#ifdef OS_ENABLE_QSMSG      //^
#define OS_ENABLE_SQUEUE     /* Defined when using queue of simple messages      */
#endif

#ifdef OS_DTIMERS_ENABLE
#define OS_ENABLE_DTIMERS   /* Defined when using dynamic timers                */
#endif


#ifdef OS_QUEUES_IDENTICAL   //^
#define OS_QUEUE_SQUEUE_IDENTICAL  // Defined to reduce code size when both types
                                   // of queues are used
#endif


//------------------------------------------------------------------------------
// (OSA configuration)
// Set constants for interrupt usage

#ifdef  OS_ENABLE_INT_SERVICE       // Redefining old style constant
#define OS_ENABLE_INT_ALL
#endif

#ifdef OS_ENABLE_INT_ALL

    #define OS_ENABLE_INT_BSEM      /* Enables interrupt services for binary    */
                                    /* semaphores                               */

    #define OS_ENABLE_INT_CSEM      /* Enables interrupt services for counting  */
                                    /* semaphores                               */

    #define OS_ENABLE_INT_MSG       /* Enables interrupt services for pointers  */
                                    /* to messages                              */

    #define OS_ENABLE_INT_SMSG      /* Enables interrupt services for simple    */
                                    /* messages                                 */

    #define OS_ENABLE_INT_QUEUE     /* Enables interrupt services for queues of */
                                    /* pointers to messages and simple messages */

    #define OS_ENABLE_INT_FLAG      /* Enables interrupt services for flags     */
                                    
#endif


#if defined(OS_ENABLE_INT_CSEM)  ||                     \
    defined(OS_ENABLE_INT_BSEM)  ||                     \
    defined(OS_ENABLE_INT_MSG)   ||                     \
    defined(OS_ENABLE_INT_SMSG)  ||                     \
    defined(OS_ENABLE_INT_QUEUE) ||                     \
    defined(OS_ENABLE_INT_FLAG)  ||                     \
    defined(OS_PROTECT_MEMORY_ACCESS)


    #define OS_ENABLE_INT

#endif

/*
 *------------------------------------------------------------------------------*
 *                                                                              *
 * (OSA configuration)                                                          *
 * Set system constants for timers                                              *
 *                                                                              *
 *------------------------------------------------------------------------------*
 */

#if defined(OS_ENABLE_TIMER)
#define OS_ENABLE_TTIMERS       /* Enables task timers                          */
#endif


#ifdef OS_ENABLE_PTIMERS        /* Redefining old name (ptimer) to new (dtimer) */
    #define OS_ENABLE_DTIMERS 
#endif

#if !defined(OS_STIMERS)
    #define OS_STIMERS  0       /* Number of STimers                            */
#endif


/*--------------------------------------------------*/
/*                                                  */
/* Counting number of old style static timers       */
/*                                                  */
/* note:    it is not recommended to use this       */
/*          type of timers. Use STimers instead.    */
/*                                                  */
/*--------------------------------------------------*/

#ifndef OS_TIMERS8                  /* not recommended to use       */
    #define OS_TIMERS8 0
#endif

#ifndef OS_TIMERS16                 /* not recommended to use       */
    #define OS_TIMERS16 0
#endif

#ifndef OS_TIMERS24                 /* not recommended to use       */
    #define OS_TIMERS24 0
#endif

#ifndef OS_TIMERS32                 /* not recommended to use       */
    #define OS_TIMERS32 0
#endif


#define _OS_TIMER8_POS      (0)
#define _OS_TIMER16_POS     (_OS_TIMER8_POS  + OS_TIMERS8)
#define _OS_TIMER24_POS     (_OS_TIMER16_POS + OS_TIMERS16)
#define _OS_TIMER32_POS     (_OS_TIMER24_POS + OS_TIMERS24)


#define OS_TIMERS           (OS_TIMERS8 + OS_TIMERS16 + OS_TIMERS24 + OS_TIMERS32)

#if OS_STIMERS > 0
    #define OS_ENABLE_STIMERS   1
#endif



/*--------------------------------------------------*/
/* Enable system timer if any type of timers used   */
/*--------------------------------------------------*/

#if defined(OS_ENABLE_DTIMERS)  ||              \
    defined(OS_ENABLE_TTIMERS)  ||              \
    defined(OS_ENABLE_STIMERS)  ||              \
    defined(OS_ENABLE_QTIMERS)  ||              \
    (OS_TIMERS > 0)

    #define OS_ENABLE_OS_TIMER
#endif

//------------------------------------------------------------------------------

/*
 ************************************************************************************************
 *                                                                                              *
 *  DEFINE BANKS TO ALLOCATE DATA                                                               *
 *                                                                                              *
 ************************************************************************************************
 */

/*------------------------------------------*/
/* Check for old style defitions            */
/*------------------------------------------*/
#if     defined(OS_BANK)            ||      \
        defined(OS_TASKS_BANK)      ||      \
        defined(OS_TIMERS_BANK)     ||      \
        defined(OS_TIMERS8_BANK)    ||      \
        defined(OS_TIMERS16_BANK)   ||      \
        defined(OS_TIMERS24_BANK)   ||      \
        defined(OS_TIMERS32_BANK)   ||      \
        defined(OS_TIMEOUTS_BANK)   ||      \
        defined(OS_BSEM_BANK)

#error "OSA error #2: Constants OS_xxx_BANK are not allowed now (use OS_BANK_xxx instead)"
        /* See manual section "Appendix/Error codes" for more information*/

#endif



/*------------------------------------------*/
/*                                          */
/* Set RAM bank constants                   */
/*                                          */
/*------------------------------------------*/

#ifndef OS_BANK_OS
#define OS_BANK_OS      0               /* RAM bank for OSA internal variables  */
#endif


#if defined(OS_BANK_TASK) && !defined(OS_BANK_TASKS)
#define OS_BANK_TASKS   OS_BANK_TASK
#endif

//------------------------------------------------------------------------------

#if defined(OS_ADDRESS_TASKS) && defined(__OSA_PIC16_MIKROC__)   // For direct allocation of
    #define OS_ALLOCATION_TASKS absolute OS_ADDRESS_TASKS   // task descriptors in
                                                            // mikroC PRO for PIC16
#else
    #define OS_ALLOCATION_TASKS                             // Empty for others
#endif

//------------------------------------------------------------------------------

#ifndef OS_BANK_TASKS
#define OS_BANK_TASKS       OS_BANK_OS  /* RAM bank for task descriptors        */
#endif

#ifndef OS_BANK_TIMERS
#define OS_BANK_TIMERS      OS_BANK_OS  /* RAM bank for old style static timers */
#endif

#ifndef OS_BANK_TIMERS8
#define OS_BANK_TIMERS8     OS_BANK_TIMERS
#endif

#ifndef OS_BANK_TIMERS16
#define OS_BANK_TIMERS16    OS_BANK_TIMERS
#endif

#ifndef OS_BANK_TIMERS24
#define OS_BANK_TIMERS24    OS_BANK_TIMERS
#endif

#ifndef OS_BANK_TIMERS32
#define OS_BANK_TIMERS32    OS_BANK_TIMERS
#endif

#ifndef OS_BANK_TIMEOUTS
#define OS_BANK_TIMEOUTS    OS_BANK_TIMERS
#endif

#ifndef OS_BANK_STIMERS
#define OS_BANK_STIMERS     OS_BANK_TIMERS  /* RAM bank for static timers       */
#endif

#if defined(OS_BANK_BSEMS) && !defined(OS_BANK_BSEM)
#define OS_BANK_BSEM   OS_BANK_BSEMS
#endif

#ifndef OS_BANK_BSEM
#define OS_BANK_BSEM      OS_BANK_OS /* RAM bank for array of binary semaphores */
#endif






//******************************************************************************
//  
//  Checking validness of bank settings for each type of OSA data
// 
//******************************************************************************


#if     OS_BANK_OS == 0
#define OS_BANK         OS_RAM_BANK0

#elif   OS_BANK_OS == 1
#define OS_BANK         OS_RAM_BANK1

#elif   OS_BANK_OS == 2
#define OS_BANK         OS_RAM_BANK2

#elif   OS_BANK_OS == 3
#define OS_BANK         OS_RAM_BANK3

#else
#error  "OSA error #3: Incorrect OS_BANK_OS value (should be 0, 1, 2 or 3)"
            /* See manual section "Appendix/Error codes" for more information*/
#endif

//******************************************************************************

#if     OS_BANK_TASKS == 0
#define OS_TASKS_BANK       OS_RAM_BANK0

#elif   OS_BANK_TASKS == 1
#define OS_TASKS_BANK       OS_RAM_BANK1

#elif   OS_BANK_TASKS == 2
#define OS_TASKS_BANK       OS_RAM_BANK2

#elif   OS_BANK_TASKS == 3
#define OS_TASKS_BANK       OS_RAM_BANK3

#else
#error  "OSA error #4: Incorrect OS_BANK_TASKS value (should be 0, 1, 2 or 3)"
        /* See manual section "Appendix/Error codes" for more information*/
#endif

//******************************************************************************

#if     OS_BANK_TIMERS == 0
#define OS_TIMERS_BANK      OS_RAM_BANK0

#elif   OS_BANK_TIMERS == 1
#define OS_TIMERS_BANK      OS_RAM_BANK1

#elif   OS_BANK_TIMERS == 2
#define OS_TIMERS_BANK      OS_RAM_BANK2

#elif   OS_BANK_TIMERS == 3
#define OS_TIMERS_BANK      OS_RAM_BANK3

#else
#error  "OSA error #5: Incorrect OS_BANK_TIMERS value (should be 0, 1, 2 or 3)"
        /* See manual section "Appendix/Error codes" for more information*/
#endif

//******************************************************************************

#if     OS_BANK_STIMERS == 0
#define OS_STIMERS_BANK     OS_RAM_BANK0

#elif   OS_BANK_STIMERS == 1
#define OS_STIMERS_BANK     OS_RAM_BANK1

#elif   OS_BANK_STIMERS == 2
#define OS_STIMERS_BANK     OS_RAM_BANK2

#elif   OS_BANK_STIMERS == 3
#define OS_STIMERS_BANK     OS_RAM_BANK3

#else
#error  "OSA error #6: Incorrect OS_BANK_STIMERS value (should be 0, 1, 2 or 3)"
        /* See manual section "Appendix/Error codes" for more information*/
#endif

//******************************************************************************

#if     OS_BANK_TIMERS8 == 0
#define OS_TIMERS8_BANK     OS_RAM_BANK0

#elif   OS_BANK_TIMERS8 == 1
#define OS_TIMERS8_BANK     OS_RAM_BANK1

#elif   OS_BANK_TIMERS8 == 2
#define OS_TIMERS8_BANK     OS_RAM_BANK2

#elif   OS_BANK_TIMERS8 == 3
#define OS_TIMERS8_BANK     OS_RAM_BANK3

#else
#error  "OSA error #7: Incorrect OS_BANK_TIMERS8 value (should be 0, 1, 2 or 3)"
        /* See manual section "Appendix/Error codes" for more information*/
#endif

//******************************************************************************

#if     OS_BANK_TIMERS16 == 0
#define OS_TIMERS16_BANK    OS_RAM_BANK0

#elif   OS_BANK_TIMERS16 == 1
#define OS_TIMERS16_BANK    OS_RAM_BANK1

#elif   OS_BANK_TIMERS16 == 2
#define OS_TIMERS16_BANK    OS_RAM_BANK2

#elif   OS_BANK_TIMERS16 == 3
#define OS_TIMERS16_BANK    OS_RAM_BANK3

#else
#error  "OSA error #8: Incorrect OS_BANK_TIMERS16 value (should be 0, 1, 2 or 3)"
        /* See manual section "Appendix/Error codes" for more information*/
#endif

//******************************************************************************

#if     OS_BANK_TIMERS24 == 0
#define OS_TIMERS24_BANK    OS_RAM_BANK0

#elif   OS_BANK_TIMERS24 == 1
#define OS_TIMERS24_BANK    OS_RAM_BANK1

#elif   OS_BANK_TIMERS24 == 2
#define OS_TIMERS24_BANK    OS_RAM_BANK2

#elif   OS_BANK_TIMERS24 == 3
#define OS_TIMERS24_BANK    OS_RAM_BANK3

#else
#error  "OSA error #9: Incorrect OS_BANK_TIMERS24 value (should be 0, 1, 2 or 3)"
        /* See manual section "Appendix/Error codes" for more information*/
#endif

//******************************************************************************

#if     OS_BANK_TIMERS32 == 0
#define OS_TIMERS32_BANK    OS_RAM_BANK0

#elif   OS_BANK_TIMERS32 == 1
#define OS_TIMERS32_BANK    OS_RAM_BANK1

#elif   OS_BANK_TIMERS32 == 2
#define OS_TIMERS32_BANK    OS_RAM_BANK2

#elif   OS_BANK_TIMERS32 == 3
#define OS_TIMERS32_BANK    OS_RAM_BANK3

#else
#error  "OSA error #10: Incorrect OS_BANK_TIMERS32 value (should be 0, 1, 2 or 3)"
        /* See manual section "Appendix/Error codes" for more information*/
#endif

//******************************************************************************

#if     OS_BANK_TIMEOUTS == 0
#define OS_TIMEOUTS_BANK    OS_RAM_BANK0

#elif   OS_BANK_TIMEOUTS == 1
#define OS_TIMEOUTS_BANK    OS_RAM_BANK1

#elif   OS_BANK_TIMEOUTS == 2
#define OS_TIMEOUTS_BANK    OS_RAM_BANK2

#elif   OS_BANK_TIMEOUTS == 3
#define OS_TIMEOUTS_BANK    OS_RAM_BANK3

#else
#error  "OSA error #11: Incorrect OS_BANK_TIMEOUTS value (should be 0, 1, 2 or 3)"
        /* See manual section "Appendix/Error codes" for more information*/
#endif

//******************************************************************************

#if     OS_BANK_BSEM == 0
#define OS_BSEM_BANK        OS_RAM_BANK0

#elif   OS_BANK_BSEM == 1
#define OS_BSEM_BANK        OS_RAM_BANK1

#elif   OS_BANK_BSEM == 2
#define OS_BSEM_BANK        OS_RAM_BANK2

#elif   OS_BANK_BSEM == 3
#define OS_BSEM_BANK        OS_RAM_BANK3

#else
#error  "OSA error #12: Incorrect OS_BANK_BSEM value (should be 0, 1, 2 or 3)"
        /* See manual section "Appendix/Error codes" for more information*/
#endif

//******************************************************************************











/*
 ************************************************************************************************
 *                                                                                              *
 *  Definition of data sizes for each type of OSA data                                          *
 *                                                                                              *
 ************************************************************************************************
 */

//******************************************************************************
//  Number of task descriptors (maximum active tasks at one time
//******************************************************************************

#if defined (OS_TASKS) && (OS_TASKS == 0)
#undef OS_TASKS
#endif


#ifndef OS_TASKS
#define OS_TASKS    1       // We need at least 1 task
#endif


//******************************************************************************
//  Types of timers
//******************************************************************************

#if defined(OST_TIMER)
#error "OSA error #13: OST_TlMER is not supported! Use OS_TIMER_SIZE instead (see manual)"
        /* See manual section "Appendix/Error codes" for more information*/
#endif

#if !defined(OS_TIMER_SIZE)
    #define OS_TIMER_SIZE   2           /* Default timer size   */
#endif

#if     OS_TIMER_SIZE == 1
#define OS_TIMER_TYPE       OST_UINT8

#elif   OS_TIMER_SIZE == 2
#define OS_TIMER_TYPE       OST_UINT16

#elif   OS_TIMER_SIZE == 4
#define OS_TIMER_TYPE       OST_UINT32

#else
    #error "OSA error #17: Bad TIMER size (must be 1, 2 or 4)"
            /* See manual section "Appendix/Error codes" for more information*/
#endif



//******************************************************************************
//  Size of static timer's counter
//******************************************************************************

#if !defined(OS_STIMER_SIZE)
#define OS_STIMER_SIZE  OS_TIMER_SIZE
#endif

#if     OS_STIMER_SIZE == 1
#define OS_STIMER_TYPE      OST_UINT8

#elif   OS_STIMER_SIZE == 2
#define OS_STIMER_TYPE      OST_UINT16

#elif   OS_STIMER_SIZE == 4
#define OS_STIMER_TYPE      OST_UINT32

#else
#error "OSA error #18: Bad STIMER size (must be 1, 2 or 4)"
        /* See manual section "Appendix/Error codes" for more information*/
#endif


//******************************************************************************
//  Size of dynamic timer's counter
//******************************************************************************

#ifdef OS_PTIMER_SIZE        // Redefine old name (ptimer) to new (dtimer)
#define OS_DTIMER_SIZE OS_PTIMER_SIZE
#endif

#if !defined(OS_DTIMER_SIZE)
#define OS_DTIMER_SIZE  OS_TIMER_SIZE
#endif

#if     OS_DTIMER_SIZE == 1
#define OS_DTIMER_TYPE      OST_UINT8

#elif   OS_DTIMER_SIZE == 2
#define OS_DTIMER_TYPE      OST_UINT16

#elif   OS_DTIMER_SIZE == 4
#define OS_DTIMER_TYPE      OST_UINT32

#else
#error "OSA error #19: Bad DTIMER size (must be 1, 2 or 4)"
        /* See manual section "Appendix/Error codes" for more information*/
#endif


//******************************************************************************
//  Size of fast timer's counter
//******************************************************************************

#if !defined(OS_QTIMER_SIZE)
#define OS_QTIMER_SIZE  OS_TIMER_SIZE
#endif

#if     OS_QTIMER_SIZE == 1
#define OS_QTIMER_TYPE      OST_UINT8

#elif   OS_QTIMER_SIZE == 2
#define OS_QTIMER_TYPE      OST_UINT16

#elif   OS_QTIMER_SIZE == 4
#define OS_QTIMER_TYPE      OST_UINT32

#else
#error "OSA error #21: Bad QTIMER size (must be 1, 2 or 4)" 
        /* See manual section "Appendix/Error codes" for more information*/
#endif


//******************************************************************************
//  Size of task timer's counter
//******************************************************************************

#if !defined(OS_TTIMER_SIZE)
#define OS_TTIMER_SIZE  OS_TIMER_SIZE
#endif

#if     OS_TTIMER_SIZE == 1
#define OS_TTIMER_TYPE      OST_UINT8

#elif   OS_TTIMER_SIZE == 2
#define OS_TTIMER_TYPE      OST_UINT16

#elif   OS_TTIMER_SIZE == 4
#define OS_TTIMER_TYPE      OST_UINT32

#else
#error "OSA error #20: Bad TTIMER size (must be 1, 2 or 4)"
        /* See manual section "Appendix/Error codes" for more information*/
#endif









/*
 ************************************************************************************************
 *                                                                                              *
 *  S Y S T E M   D A T A   T Y P E S                                                           *
 *                                                                                              *
 ************************************************************************************************
 */

//******************************************************************************
//  Simple byte message
// (this type should be defined BEFORE including port\osa_xxx.h)
//******************************************************************************

#ifdef  OST_SMSG
#error "OSA error #14: Can't redefine name OST SMSG in 'OSAcfg.h'. Use OST_SMSG instead."
        /* See manual section "Appendix/Error codes" for more information*/
#endif


#if !defined(OS_SMSG_TYPE)
#define     OS_SMSG_TYPE    unsigned char
#endif

typedef OS_SMSG_TYPE    _OST_SMSG;      //  See port\pic16\osa_pic16_htpicc.h for example







/************************************************************************************************
 *                                                                                              *
 *     INCLUDE PROCESSOR SPECIFIC DEFINITIONS                                                   *
 *                                                                                              *
 ************************************************************************************************/

#include "port\osa_include.h"

#define OST_WORD        OST_UINT
#define _OST_INT_TYPE   OST_WORD        // Old types redefining (don't use it)
#define __osa_word      OST_WORD        // Old types redefining (don't use it)
#define __osa_uint8     OST_UINT8       // Old types redefining (don't use it)
#define __osa_uint16    OST_UINT16      // Old types redefining (don't use it)
#define __osa_uint32    OST_UINT32      // Old types redefining (don't use it)


typedef OS_SMSG_TYPE    OST_SMSG;



//******************************************************************************
//  Pointer to message
//******************************************************************************

#ifdef  OST_MSG
#error "OSA error #15: Can't redefine name OST MSG in 'OSAcfg.h'. Use OST_MSG instead."
        /* See manual section "Appendix/Error codes" for more information*/
#endif

#if !defined(OS_MSG_TYPE)
#define OS_MSG_TYPE     void *
#endif


typedef OS_MSG_TYPE     OST_MSG;



//******************************************************************************
//  Pointer to message descriptor
//******************************************************************************

typedef 
#ifndef __OSA_AVR_CODEVISION__
        volatile 
#endif
                    struct _OST_MSG_CB
{
    OST_UINT        status;     // State (=1 - busy / =0 - free)
    OST_MSG         msg;        // Pointer to message body

} OST_MSG_CB;





//******************************************************************************
//  Message's queues
//******************************************************************************

/*--- Queue's control block                         ---*/

typedef struct
{
    OST_UINT   cSize;      // Queue size
    OST_UINT   cFilled;    // Number of filled elements
    OST_UINT   cBegin;     // First element in buffer
                           // 
} OST_QUEUE_CONTROL;


/*--- Descriptor of queue of pointers to messages   ---*/

typedef struct
{
    OST_QUEUE_CONTROL Q;
    OST_MSG *pMsg;              // Pointer to queue buffer

} OST_QUEUE;


/*--- Descriptor of queue of simple messages        ---*/

typedef struct
{
    OST_QUEUE_CONTROL Q;
    OST_SMSG *pSMsg;            // Pointer to queue buffer

} OST_SQUEUE;



//******************************************************************************
//  Flags
//******************************************************************************

#define OST_FLAG    OST_UINT8
#define OST_FLAG8   OST_UINT8
#define OST_FLAG16  OST_UINT16
#define OST_FLAG32  OST_UINT32





//******************************************************************************
//  
//  System state flags
// 
//  (!!!Do not change positions of theese bits. For MCC30 bits
//  cIPL_Temp must occupy 5, 6 and 7 positions)
//******************************************************************************

typedef struct
{
    /*0*/   OST_UINT   bEventError        : 1; // Event error
    /*1*/   OST_UINT   bError             : 1; // Task create error
                                               // Stimer Create error
    /*2*/   OST_UINT   bInCriticalSection : 1; // One of task is in critical
                                               // section
    /*3*/   OST_UINT   bCheckingTasks     : 1; // OS_Sched:
                                               //   =1 - best task searching
                                               //   =0 - best task executed
    /*4*/   OST_UINT   bBestTaskFound     : 1; // OS_Sched: Best task found


    //------------------------------------------------------------------------------
    // Processor specified bits
    //------------------------------------------------------------------------------
            _OS_SYSTEM_FLAGS_PROC_SPEC()       // see definition in h-files for
                                               // each processor type in "port" folder

            #if OS_PRIORITY_LEVEL == OS_PRIORITY_EXTENDED
            OST_UINT   bEventOK           : 1; // Goes "1" when task was run after OS_Wait
                                               // See OS_Sched definition
                                               // 
                                               // Have #15 for PIC24 and dsPIC !!!!
                                               // 
            #endif


} OST_SYSTEM_FLAGS;




//******************************************************************************
//  
//  Task state flags
// 
//  !Do not change placement of theese bits
//******************************************************************************


#ifndef OSA_TASK_STATE_PROC_SPEC
#define OSA_TASK_STATE_PROC_SPEC()
#endif

typedef          struct
{
    /*0..2*/OST_UINT   cPriority   : 3;     // Task priority (0..7)
    /*3*/   OST_UINT   bReady      : 1;     // Ready to execute
    /*4*/   OST_UINT   bDelay      : 1;     // Timer active: OS_Delay (bCanContinue=0)
                                            // or event waiting with timeout (bCanContinue=1)

    /*5*/   OST_UINT   bCanContinue: 1;     // Can continue, while bDelay flag is set
    /*6*/   OST_UINT   bEnable     : 1;     // Descriptor is busy by task
    /*7*/   OST_UINT   bPaused     : 1;     // Task paused (it is still active, but can
                                            // became ready only from extenal task)

            OSA_TASK_STATE_PROC_SPEC()      // see definition in h-files for
                                            // each processor type in "port" folder

} OST_TASK_STATE;

//------------------------------------------------------------------------------

#define OS_TASK_PRIORITY_MASK       0x07
#define OS_TASK_READY_MASK          0x08
#define OS_TASK_DELAY_MASK          0x10
#define OS_TASK_CANCONTINUE_MASK    0x20
#define OS_TASK_ENABLE_MASK         0x40
#define OS_TASK_PAUSED_MASK         0x80

//------------------------------------------------------------------------------




//******************************************************************************
//  Task control block (descriptor)
//******************************************************************************

typedef struct
{
    OST_TASK_STATE      State;              // Current task state
    OST_CODE_POINTER    pTaskPointer;       // ROM pointer

    //------------------------------------------------------------------------------
    // Processor specified bits
    //------------------------------------------------------------------------------
    _OS_TCB_PROC_SPEC()                     // see definition in h-files for
                                            // each processor type in "port" folder

    #ifdef OS_ENABLE_TTIMERS
    OS_TTIMER_TYPE     Timer;               // Timer counter for delays
    #endif

} OST_TCB;   // Task Control Block


#ifndef OST_TASK_POINTER
typedef OS_TASKS_BANK OST_TCB *     OST_TASK_POINTER;
#endif


#define OS_WORST_PRIORITY       7       // Lowest priority
#define OS_BEST_PRIORITY        0       // Highest priority




/*
 ************************************************************************************************
 *                                                                                              *
 *  T I M E R S   D A T A   T Y P E S                                                           *
 *                                                                                              *
 ************************************************************************************************
 */

//******************************************************************************
//  Dynamic timers
//******************************************************************************

//---------------------------------------------------------------
#ifdef OS_ENABLE_DTIMERS
//---------------------------------------------------------------

    /*
     *------------------------------------------*
     *                                          *
     *  Dynamic timer's state flags             *
     *                                          *
     *  !Do not change placement of variables   *
     *                                          *
     *------------------------------------------*
     */

    typedef 
    #ifndef __OSA_AVR_CODEVISION__
            volatile 
    #endif
                        struct
    {
        OST_UINT   bTimeout    : 1;    // Overflow occured
        OST_UINT   bActive     : 1;    // Timer is active (was created by OS_Dtimer_Create)
                                       // (For system variable _OS_DTimers.Flags allways = 1)

        OST_UINT   bRun        : 1;    // = 1 - Timer is counnting
                                       // = 0 - Timer is stopped

        OST_UINT   bNextEnable : 1;    // = 0 - last timer in list,
                                       // = 1 - there is next timer
    } OST_DTIMER_FLAGS;


    /*
     *------------------------------------------*
     *                                          *
     *  Dynamic timer's descriptor              *
     *                                          *
     *------------------------------------------*
     */

    typedef struct S_OST_DTIMER
    {
        OST_DTIMER_FLAGS        Flags;       // Timer's state flags
        struct S_OST_DTIMER    *Next;        // Pointer to next timer in list
        OS_DTIMER_TYPE          Timer;       // Counter
                                             // 
    } OST_DTIMER;

    /*
     *------------------------------------------*
     *                                          *
     *  Header of list of dynamic timers        *
     *                                          *
     *------------------------------------------*
     */

    typedef struct
    {
        OST_DTIMER_FLAGS        Flags;
        struct S_OST_DTIMER    *Next;          // Pointer to first timer in list

    } OST_DTIMER_CB;

//---------------------------------------------------------------
#endif  // OS_ENABLE_DTIMERS
//---------------------------------------------------------------





//******************************************************************************
//  Queue of timers
//******************************************************************************

//---------------------------------------------------------------
#ifdef OS_ENABLE_QTIMERS
//---------------------------------------------------------------

    /*
     *------------------------------------------*
     *                                          *
     *  Fast timer's state flags                *
     *                                          *
     *  !Do not change placement of variables   *
     *                                          *
     *------------------------------------------*
     */

    typedef 
    #ifndef __OSA_AVR_CODEVISION__
            volatile 
    #endif
                        struct
    {
        OST_UINT   bTimeout      : 1;  // Overflow occured
        OST_UINT   bActive       : 1;  // Timer is active (was created by OS_Dtimer_Create)
                                       // (For system variable _OS_DTimers.Flags allways = 1)

        OST_UINT   bRun          : 1;  // = 1 - Timer is counnting
                                       // = 0 - Timer is stopped

        OST_UINT   bNextEnable   : 1;  // = 0 - last timer in list,
                                       // = 1 - there is next timer

       //------------------------------// Functions for OS_Qtimer_List:
                                       //
        OST_UINT   bListFunction : 1;  // = 00 - add timer into the list
                                       // = 01 - delete timer from the list
        OST_UINT   bListGetTime  : 1;  // = 1x - get remaining time

        //-----------------------------// Internal OS_Qtimer_List bits:
        OST_UINT   bAddAfterDelete:1;  // = 1 - when adding already existing timer it
                                       //       has to be deleted first


    } OST_QTIMER_FLAGS;

    #define OS_QTIMER_LIST_FUNCTION_ADD     0
    #define OS_QTIMER_LIST_FUNCTION_DELETE  1

    /*
     *------------------------------------------*
     *                                          *
     *  Fast timer's control block              *
     *                                          *
     *------------------------------------------*
     */

    typedef struct S_OST_QTIMER
    {
        OST_QTIMER_FLAGS        Flags;         // Timer's state flags
        struct S_OST_QTIMER    *Next;          // Pointer to next timer in list
        OS_QTIMER_TYPE          Timer;         // Counter
                                               // 
    } OST_QTIMER;

    /*
     *------------------------------------------*
     *                                          *
     *  Header of list of fast timers           *
     *                                          *
     *------------------------------------------*
     */

    typedef struct
    {
        OST_QTIMER_FLAGS        Flags;
        struct S_OST_QTIMER    *Next;          // Pointer to first timer in list
    } OST_QTIMER_CB;

//---------------------------------------------------------------
#endif  // OS_ENABLE_QTIMERS
//---------------------------------------------------------------



//******************************************************************************
//  Counting semaphores
//******************************************************************************

#if !defined(OS_CSEM_SIZE)
#define OS_CSEM_SIZE    1
#endif

#if     OS_CSEM_SIZE == 1
typedef OST_UINT8   OST_CSEM;
#elif   OS_CSEM_SIZE == 2
typedef OST_UINT16  OST_CSEM;
#elif   OS_CSEM_SIZE == 4
typedef OST_UINT32  OST_CSEM;
#else
    #error "OSA error #16: Incorrect OS_CSEM_SIZE value (m.b. only 1, 2 or 4)! (see OSAcfg.h)"
            /* See manual section "Appendix/Error codes" for more information*/
#endif










/*
 ************************************************************************************************
 *                                                                                              *
 *  OSA system variables prototypes                                                             *
 *                                                                                              *
 ************************************************************************************************
 */



extern  volatile OS_RAM_NEAR    OST_SYSTEM_FLAGS    _OS_Flags;
extern           OS_RAM_NEAR    OST_UINT            _OS_Temp;

#ifndef __OSA12__
extern  volatile OS_RAM_NEAR    OST_UINT            _OS_TempH;
#endif


#if defined(_OS_CUR_FLAGS_IN_OS_STATE)
extern  volatile OS_RAM_NEAR    OST_TASK_STATE      _OS_State;
#endif


//******************************************************************************
//  Pointer to current task
//******************************************************************************

#ifdef __OSA_CCS__
// The CCS compiler do not allows to declare a volatile pointers
extern OST_TASK_POINTER OS_RAM_NEAR             _OS_CurTask;
#else
extern OST_TASK_POINTER OS_RAM_NEAR volatile    _OS_CurTask;
#endif


#if OS_PRIORITY_LEVEL == OS_PRIORITY_NORMAL

    extern  OS_BANK          OST_UINT               _OS_Best_Priority;
    
    #if defined(__OSA_CCS__) || defined(__OSA_IAR__)
    // The CCS compiler do not allows to declare a volatile pointers
    extern OST_TASK_POINTER OS_BANK                 _OS_BestTask;
    extern OST_TASK_POINTER OS_BANK                 _OS_LastTask;
    #else
    extern OST_TASK_POINTER OS_BANK     volatile    _OS_BestTask;
    extern OST_TASK_POINTER OS_BANK     volatile    _OS_LastTask;
    #endif

#endif


#if OS_PRIORITY_LEVEL == OS_PRIORITY_EXTENDED

/* 
    typedef union
    {   
        struct 
        {
            OST_UINT8   cPriority;
            OST_UINT8   cTaskPos;
        } Bytes;

        OST_UINT16  Word;

    } OST_TASK_QUEUE;
*/

    extern  OS_BANK OST_UINT    _OS_Best_Priority;
    extern  OS_BANK OST_UINT    _OS_Worst_Priority;
    extern  OS_BANK OST_UINT    _OS_Best_n;
    extern  OS_BANK OST_UINT    _OS_Cur_Pos;
    extern  OS_BANK OST_UINT    _OS_n;
    extern  OS_BANK OST_UINT8   _OS_TaskQueue[OS_TASKS];    // Queue of lesting in scheduler
    extern  OS_BANK OST_UINT8   _OS_TaskLevel[OS_TASKS];    // Current priority levels

#endif








/*
 ************************************************************************************************
 *                                                                                              *
 *  Internal macro for work with state flags                                                    *
 *                                                                                              *
 ************************************************************************************************
*/

//------------------------------------------------------------------------------
#if !defined(OS_CUR_FLAGS_DEFINED)
//------------------------------------------------------------------------------


#if defined(_OS_CUR_FLAGS_IN_OS_STATE)

    #define _OS_bTaskReady              _OS_State.bReady
    #define _OS_bTaskCanContinue        _OS_State.bCanContinue
    #define _OS_bTaskDelay              _OS_State.bDelay
    #define _OS_bTaskTimeout            (!_OS_State.bDelay)
    #define _OS_bTaskEnable             _OS_State.bEnable
    #define _OS_cTaskPriority           _OS_State.cPriority
    #define _OS_bTaskPaused             _OS_State.bPaused

#endif

#if defined(_OS_CUR_FLAGS_IN_FSR)

    #define _OS_bTaskReady              ((OS_RAM_NEAR OST_TASK_STATE*)(&_indf))->bReady
    #define _OS_bTaskCanContinue        ((OS_RAM_NEAR OST_TASK_STATE*)(&_indf))->bCanContinue
    #define _OS_bTaskDelay              ((OS_RAM_NEAR OST_TASK_STATE*)(&_indf))->bDelay
    #define _OS_bTaskTimeout            (!((OS_RAM_NEAR OST_TASK_STATE*)(&_indf))->bDelay)
    #define _OS_bTaskEnable             ((OS_RAM_NEAR OST_TASK_STATE*)(&_indf))->bEnable
    #define _OS_cTaskPriority           ((OS_RAM_NEAR OST_TASK_STATE*)(&_indf))->cPriority
    #define _OS_bTaskPaused             ((OS_RAM_NEAR OST_TASK_STATE*)(&_indf))->bPaused

#endif

//------------------------------------------------------------------------------
#endif      // OS_CUR_FLAGS_DEFINED
//------------------------------------------------------------------------------






/*
 ************************************************************************************************
 *                                                                                              *
 *  Include definitions of services:                                                            *
 *                                                                                              *
 ************************************************************************************************
 */

#include "kernel\osa_oldnames.h"            // Table of alternate names

#include "kernel\system\osa_system.h"       // System services
#include "kernel\system\osa_tasks.h"        // Tasks

#include "kernel\events\osa_bsem.h"         // Binary semaphores
#include "kernel\events\osa_csem.h"         // Counting semaphores
#include "kernel\events\osa_flag.h"         // Flags
#include "kernel\events\osa_msg.h"          // Pointer to messages
#include "kernel\events\osa_queue.h"        // Queue of pointers to messages
#include "kernel\events\osa_smsg.h"         // Simple messages
#include "kernel\events\osa_squeue.h"       // Queue of simple messages

#include "kernel\timers\osa_stimer.h"       // Static timers
#include "kernel\timers\osa_stimer_old.h"   // Old style static timers
#include "kernel\timers\osa_dtimer.h"       // Dynamic timers
#include "kernel\timers\osa_qtimer.h"       // Fast timers
#include "kernel\timers\osa_ttimer.h"       // Task timers
#include "kernel\timers\osa_timer.h"        // System timer



/* This constant is defined in service's files */

#if defined(OS_ENABLE_INT)

    extern  volatile OS_RAM_NEAR OST_UINT _OS_Temp_I;

    // _OS_DI_INT() and _OS_RI_INT() are defined in port\osa_xxx.h files for each platform

#else

    #define _OS_DI_INT()
    #define _OS_RI_INT()

#endif

#define _OS_ATOMIC_WRITE_A(expr)        \
    OSM_BEGIN {                         \
        _OS_DI_INT();                   \
        expr;                           \
        _OS_RI_INT();                   \
    } OSM_END




//******************************************************************************
// These two macros are used in mikroC for PIC16. For all others they are 
// defined as empty macros
// -----------------------------------------------------------------------------
#ifndef _OS_SET_IRP_CUR_TASK
#define _OS_SET_IRP_CUR_TASK()
#endif

#ifndef _OS_CLR_IRP
#define _OS_CLR_IRP()
#endif
//------------------------------------------------------------------------------




#ifdef __OSA_CCS__
#include <osa.c>
#endif



//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------

//******************************************************************************
//  END OF FILE osa.h
//******************************************************************************
    
