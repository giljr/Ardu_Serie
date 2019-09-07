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
 *  File:           osa_pic18_htpicc.h
 *
 *  Compilers:      HT-PICC18 STD
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    HTPICC for PIC18 specific definition
 *                  This file directly included in osa.h
 *
 *  History:        13.09.2010 -    File updated
 * 
 *                  08.12.2010 -    _OS_RETURN_NOSAVE corrected (NOP added after call to _OS_ReturnNoSave)
 *
 ************************************************************************************************
 */


#ifndef __OSAPICC18_H__
#define __OSAPICC18_H__


#if _HTC_EDITION_ == __PRO__
//#define __OSAPICCPRO__
//#warning "PRO!!!!"
#endif



/************************************************************************************************
 *                                                                                              *
 *     Integer types definitions                                                                *
 *                                                                                              *
 ************************************************************************************************/

typedef unsigned char   OST_UINT8;
typedef unsigned int    OST_UINT16;
typedef unsigned long   OST_UINT32;
typedef bit             OST_BOOL;

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
#define OS_RAM_BANK1           bdata
#define OS_RAM_BANK2           bdata
#define OS_RAM_BANK3           bdata


#define _OS_CUR_FLAGS_IN_OS_STATE   // Work with tasks state througth _OS_State variable




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


/************************************************************************************************
 *                                                                                              *
 *     Registers definitions                                                                    *
 *                                                                                              *
 ************************************************************************************************/

static volatile near unsigned int _fsr      @ 0xFE9;
static volatile near char         _indf     @ 0xFEF;
static volatile near char         _postinc  @ 0xFEE;
static volatile near char         _postdec  @ 0xFED;
static volatile near char         _preinc   @ 0xFEC;
static volatile near char         _plusw    @ 0xFEB;

static volatile near unsigned char _fsr1l   @ 0xFE1;

static volatile near char         _pcl      @ 0xFF9;
static volatile near char         _pclath   @ 0xFFA;
static volatile near char         _pclatu   @ 0xFFB;
static volatile near char         _status   @ 0xFD8;
static volatile near char         _tosl     @ 0xFFD;
static volatile near char         _tosh     @ 0xFFE;
static volatile near char         _tosu     @ 0xFFF;
static volatile near char         _bsr      @ 0xFE0;
static volatile near char         _wreg     @ 0xFE8;
static volatile near char         _intcon   @ 0xFF2;
static volatile near char         _rcon     @ 0xFD0;


static          near bit    _giel       @ ((unsigned)&_intcon*8)+6;
static          near bit    _gie        @ ((unsigned)&_intcon*8)+7;
static          near bit    _ipen       @ ((unsigned)&_rcon*8)+7;
static volatile near bit    _carry      @ ((unsigned)&_status*8)+0;
static volatile near bit    _zero       @ ((unsigned)&_status*8)+2;


/************************************************************************************************
 *                                                                                              *
 *     Constants and types                                                                      *
 *                                                                                              *
 ************************************************************************************************/

#undef OS_SMSG_SIZE
#define OS_SMSG_SIZE    sizeof(OS_SMSG_TYPE)


//------------------------------------------------------------------------------

typedef unsigned int                OS_FSR_TYPE;

//------------------------------------------------------------------------------

#if defined(_ROMSIZE) && (_ROMSIZE <= 0x10000)

    typedef unsigned int                OST_CODE_POINTER;
    #define OS_CODE_POINTER_SIZE        2

#else

    typedef unsigned long               OST_CODE_POINTER;
    #define OS_CODE_POINTER_SIZE        4

#endif



//------------------------------------------------------------------------------

/************************************************************************************************
 *                                                                                              *
 *     Platform macros                                                                          *
 *                                                                                              *
 ************************************************************************************************/

#define _OS_POP()            asm("      pop     ");

#if defined(_ROMSIZE) && (_ROMSIZE <= 0x4000)
#define _OS_PIC18_ERRATA_NOP()
#else
#define _OS_PIC18_ERRATA_NOP()     asm("\t    dw 0xFFFF     ")
#endif


#define OS_CLRWDT()             asm(" clrwdt ");
#define OS_ClrWdt()             asm(" clrwdt ");
#define OS_Clrwdt()             asm(" clrwdt ");
#define OS_SLEEP()              asm(" sleep  ");
#define OS_Sleep()              asm(" sleep  ");










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

    #define _OS_SET_PC()    asm("   movf    __postinc, w, c     ");         \
                            asm("   movff   __postinc, __pclath ");         \
                            asm("   movwf   __pcl, c            ");


    #define _OS_SAVE_PC()   asm("   movff   __tosl, __preinc    ");         \
                            asm("   movff   __tosh, __preinc    ");

    #define _OS_SET_INDF_WORD(value)                                        \
        _postinc = (((OST_CODE_POINTER)value)) & 0xFF;                      \
        _postinc = (((OST_CODE_POINTER)value) >> 8) & 0xFF;

//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------

    #define _OS_SET_PC()    asm("   movf    __postinc, w, c     ");         \
                            asm("   movff   __postinc, __pclath ");         \
                            asm("   movff   __postinc, __pclatu ");         \
                            asm("   movwf   __pcl, c            ");


    #define _OS_SAVE_PC()   asm("   movff   __tosl, __preinc    ");         \
                            asm("   movff   __tosh, __preinc    ");         \
                            asm("   movff   __tosu, __preinc    ");


    #define _OS_SET_INDF_WORD(value)                                \
        asm("   movlw   (_"#value" >> 0) & 0xFF   ");               \
        asm("   movwf   __postinc,c         ");                     \
        asm("   movlw   (_"#value" >> 8) & 0xFF   ");               \
        asm("   movwf   __postinc,c         ");                     \
        asm("   movlw   (_"#value" >> 16) & 0xFF  ");               \
        asm("   movwf   __postinc,c         ");                     \
        asm("   movlw   (_"#value" >> 24) & 0xFF  ");               \
        asm("   movwf   __postinc,c         ");

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

void _OS_JumpToTask (void);

#define _OS_JUMP_TO_TASK()      _OS_JumpToTask()


#define _OS_BACK_TO_SCEDULER()                                                              \
    _OS_SET_FSR_CUR_TASK();                                                                 \
    asm("   movlw   0xD7                        "); /*  except bReady and bCanContinue*/    \
    asm("   andwf   __indf, f, c                ");                                         \
    asm("   movlw   0x28                        "); /*  bReady and bCanContinue*/           \
    asm("   andwf   __OS_State, w, c            ");                                         \
    asm("   iorwf   __indf, f, c                ");                                         \





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


void _OS_ReturnSave (void);
void _OS_ReturnNoSave (void);
void _OS_EnterWaitMode (void);
void _OS_EnterWaitModeTO (void);
void _OS_CheckEvent (OST_UINT);

void  _OS_SET_FSR_CUR_TASK (void);


//------------------------------------------------------------------------------
#define _OS_RETURN_SAVE()           {                                       \
                                        _OS_ReturnSave();                   \
                                        _OS_PIC18_ERRATA_NOP();             \
                                    }                                       \

//------------------------------------------------------------------------------
#define _OS_ENTER_WAIT_MODE()           {                                   \
                                        _OS_EnterWaitMode();                \
                                        _OS_PIC18_ERRATA_NOP();             \
                                    }                                       \

//------------------------------------------------------------------------------
#define _OS_ENTER_WAIT_MODE_TO()                                            \
                                    {                                       \
                                        _OS_EnterWaitModeTO();              \
                                        _OS_PIC18_ERRATA_NOP();             \
                                    }                                       \

//------------------------------------------------------------------------------
#define _OS_RETURN_NO_SAVE()        {                                       \
                                        asm(" global __OS_ReturnNoSave");   \
                                        _OS_ReturnNoSave();                 \
                                        _OS_PIC18_ERRATA_NOP();   /*101208*/\
                                    }                                       \

//------------------------------------------------------------------------------

#define _OS_CHECK_EVENT(event) _OS_CheckEvent(event)






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
        OS_Task_Reserve(TaskName);                                                      \
        *((OS_RAM_NEAR OST_UINT*)&_OS_State) = priority | 0x48; /* bEnable è bReady */  \
        _OS_SET_FSR_CUR_TASK();                                                         \
        _postinc = priority | 0x48; /* bEnable è bReady */                              \
        _OS_SET_INDF_WORD(TaskName);                                                    \
        _OS_RETURN_NO_SAVE();                                                           \
    }


#define OS_Task_Replace_P(priority, TaskAddr)                                           \
    {                                                                                   \
        *((OS_RAM_NEAR OST_UINT*)&_OS_State) = priority | 0x48; /* bEnable è bReady */  \
        _OS_SET_FSR_CUR_TASK();                                                         \
        _postinc = priority | 0x48; /* bEnable è bReady */                              \
        _OS_SET_INDF_WORD((OST_CODE_POINTER)(TaskAddr));                                \
        _OS_RETURN_NO_SAVE();                                                           \
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
            _OS_Task_Create(priority, &TaskName);                       \
            asm("   incf    __fsr, f                ");                 \
            asm("   movlw   low(fp__"#TaskName")    ");                 \
            asm("   movwf   __indf                  ");                 \
            asm("   movlw   high(fp__"#TaskName")   ");                 \
            asm("   incf    __fsr, f                ");                 \
            asm("   movwf   __indf                  ");                 \
            if (_indf & 0x80) TaskName();                               \
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
 * MACROS FOR OLD STYLE STATIC TIMERS WORK
 *
 ******************************************************************************************/

#define __OS_Timer8Work(TIMER_ID)                                                               \
    {                                                                                           \
        if ((OS_Timeouts[(TIMER_ID)>>3]&(1<<((TIMER_ID)&7))))                                   \
        {                                                                                       \
            asm("\t    global   _OS_Timers8   ");                                               \
            asm("\t    movlb    (_OS_Timers8 + (" #TIMER_ID")) >> 8 ");                         \
            asm("\t    incfsz   (_OS_Timers8 + (" #TIMER_ID")) & 0xFF, f    ");                 \
            asm("\t    bra      _TIMER_NEXT_8_"#TIMER_ID"_     ");                              \
             OS_Timeouts[(TIMER_ID)>>3]&=~(1<<((TIMER_ID)&7));                                  \
            asm("_TIMER_NEXT_8_"#TIMER_ID"_: ");                                                \
        }                                                                                       \
    }


#define __OS_Timer16Work(TIMER_ID)                                                              \
    {                                                                                           \
        if ((OS_Timeouts[(TIMER_ID+_OS_TIMER16_POS)>>3]&(1<<((TIMER_ID+_OS_TIMER16_POS)&7))))   \
        {                                                                                       \
            asm("\t    global   _OS_Timers16  ");                                               \
            asm("\t    movlb    (_OS_Timers16 + (" #TIMER_ID"*2)) >> 8 ");                      \
            asm("\t    infsnz   (_OS_Timers16 + (" #TIMER_ID"*2)+0) & 0xFF, f    ");            \
            asm("\t    incfsz   (_OS_Timers16 + (" #TIMER_ID"*2)+1) & 0xFF, f    ");            \
            asm("\t    bra      _TIMER_NEXT_16_"#TIMER_ID"_     ");                             \
             OS_Timeouts[(TIMER_ID+_OS_TIMER16_POS)>>3]&=~(1<<((TIMER_ID+_OS_TIMER16_POS)&7));  \
            asm("_TIMER_NEXT_16_"#TIMER_ID"_: ");                                               \
        }                                                                                       \
    }


#define __OS_Timer24Work(TIMER_ID)                                                              \
    {                                                                                           \
        if ((OS_Timeouts[(TIMER_ID+_OS_TIMER24_POS)>>3]&(1<<((TIMER_ID+_OS_TIMER24_POS)&7))))   \
        {                                                                                       \
            asm("\t    global   _OS_Timers24  ");                                               \
            asm("\t    movlb    (_OS_Timers24 + (" #TIMER_ID"*2)) >> 8 ");                      \
            asm("\t    infsnz   (_OS_Timers24 + (" #TIMER_ID"*2)+0) & 0xFF, f    ");            \
            asm("\t    incfsz   (_OS_Timers24 + (" #TIMER_ID"*2)+1) & 0xFF, f    ");            \
            asm("\t    bra      _TIMER_NEXT_24_"#TIMER_ID"_     ");                             \
             OS_Timeouts[(TIMER_ID+_OS_TIMER24_POS)>>3]&=~(1<<((TIMER_ID+_OS_TIMER24_POS)&7));  \
            asm("_TIMER_NEXT_24_"#TIMER_ID"_: ");                                               \
        }                                                                                       \
    }


#define __OS_Timer32Work(TIMER_ID)                                                              \
    {                                                                                           \
        if ((OS_Timeouts[(TIMER_ID+_OS_TIMER32_POS)>>3]&(1<<((TIMER_ID+_OS_TIMER32_POS)&7))))   \
        {                                                                                       \
            asm("\t    global   _OS_Timers32  ");                                               \
            asm("\t    movlb    (_OS_Timers32 + (" #TIMER_ID"*4)) >> 8 ");                      \
            asm("\t    incf     (_OS_Timers32 + (" #TIMER_ID"*4)+0) & 0xFF, f    ");            \
            asm("\t    bnc      _TIMER_NEXT_32_"#TIMER_ID"_     ");                             \
            asm("\t    movlw    0     ");                                                       \
            asm("\t    addwfc   (_OS_Timers32 + (" #TIMER_ID"*4)+1) & 0xFF, f    ");            \
            asm("\t    addwfc   (_OS_Timers32 + (" #TIMER_ID"*4)+2) & 0xFF, f    ");            \
            asm("\t    addwfc   (_OS_Timers32 + (" #TIMER_ID"*4)+3) & 0xFF, f    ");            \
            asm("\t    bnc      _TIMER_NEXT_32_"#TIMER_ID"_     ");                             \
             OS_Timeouts[(TIMER_ID+_OS_TIMER32_POS)>>3]&=~(1<<((TIMER_ID+_OS_TIMER32_POS)&7));  \
            asm("_TIMER_NEXT_32_"#TIMER_ID"_: ");                                               \
        }                                                                                       \
    }


/******************************************************************************************
 *
 * MACROS FOR WORK WITH STATIC TIMERS
 *
 ******************************************************************************************/


/**/
//------------------------------------------------------------------------------
#ifdef OS_ENABLE_STIMERS
//------------------------------------------------------------------------------

#if OS_STIMER_SIZE == 4
    #define __OS_StimerBank()                                 \
            asm("global __OS_Stimers");                       \
            asm("movlb __OS_Stimers>>8");                     \
            asm("OS_S0 set low __OS_Stimers");                \
            asm("OS_S1 set low __OS_Stimers+1");              \
            asm("OS_S2 set low __OS_Stimers+2");              \
            asm("OS_S3 set low __OS_Stimers+3");              \
            asm("movlw 0");
#else
    #define __OS_StimerBank()                                 \
            asm("global __OS_Stimers");                       \
            asm("movlb __OS_Stimers>>8");                     \
            asm("OS_S0 set low __OS_Stimers");                \
            asm("OS_S1 set low __OS_Stimers+1");
#endif


//------------------------------------------------------------------------------
#if     OS_STIMER_SIZE == 1
//------------------------------------------------------------------------------

    #define __OS_StimerWork(STIMER_ID)                  \
        asm("   btfsc OS_S0+"#STIMER_ID",7");           \
        asm("   incf OS_S0+"#STIMER_ID"");              \


    #define __OS_StimersWorkSize()                      \
        _fsr = (OS_FSR_TYPE)_OS_Stimers - 1;            \
        _wreg = OS_STIMERS;                             \
        do {                                            \
            if (_preinc & 0x80) _indf++;                \
        } while (--_wreg);                              \

//------------------------------------------------------------------------------
#elif   OS_STIMER_SIZE == 2
//------------------------------------------------------------------------------

    #define __OS_StimerWork(STIMER_ID)                  \
        asm("btfss OS_S1+("#STIMER_ID"*2),7");          \
        asm("bra $+6");                                 \
        asm("infsnz OS_S0+("#STIMER_ID"*2)");           \
        asm("incf OS_S1+("#STIMER_ID"*2)");             \

    #define __OS_StimersWorkSize()                      \
        _fsr = (OS_FSR_TYPE)_OS_Stimers + OS_STIMERS*2; \
        do {                                            \
            _postdec;                                   \
            if (_postdec & 0x80)                        \
            {                                           \
                if (!++_postinc) _indf++;               \
                _postdec;                               \
            }                                           \
            asm("   movlw   __OS_Stimers  ");           \
            asm("   xorwf   __fsr, w, c ");             \
        } while (!ZERO);                                \

//------------------------------------------------------------------------------
#elif   OS_STIMER_SIZE == 4
//------------------------------------------------------------------------------

    #define __OS_StimerWork(STIMER_ID)                  \
        asm("btfss OS_S3+("#STIMER_ID"*4),7");          \
        asm("bra $+10");                                \
        asm("incf OS_S0+("#STIMER_ID"*4)");             \
        asm("addwfc OS_S1+("#STIMER_ID"*4)");           \
        asm("addwfc OS_S2+("#STIMER_ID"*4)");           \
        asm("addwfc OS_S3+("#STIMER_ID"*4)");           \


    #define __OS_StimersWorkSize()                          \
        _fsr = (OS_FSR_TYPE)_OS_Stimers;                    \
        do {                                                \
            asm("   movlw   3           ");                 \
            asm("   addwf   __fsr, f, c ");                 \
            if (_indf & 0x80)                               \
            {                                               \
                asm("   subwf   __fsr, f, c     ");         \
                asm("   incf    __postinc, f, c");          \
                asm("   movlw   0             ");           \
                asm("   addwfc  __postinc, f, c");          \
                asm("   addwfc  __postinc, f, c");          \
                asm("   addwfc  __indf, f, c");             \
            }                                               \
            _postinc;                                       \
            _wreg = OS_STIMERS * 4 + (char)&_OS_Stimers;    \
            asm("   xorwf   __fsr, w, c     ");             \
        } while (!ZERO);


//------------------------------------------------------------------------------
#endif  // OS_STIMER_SIZE
//------------------------------------------------------------------------------
/**/

//------------------------------------------------------------------------------
#endif  // OS_ENABLE_STIMERS
//------------------------------------------------------------------------------











/******************************************************************************************
 *
 * MACROS FOR WORK WITH TASK TIMERS
 *
 ******************************************************************************************/


//------------------------------------------------------------------------------
#if   OS_CODE_POINTER_SIZE == 2
//------------------------------------------------------------------------------

    #if     OS_TTIMER_SIZE == 1
        #define ASM_OST_TCB_SIZE_CONST      asm("TCB_SIZE set 4");
    #elif   OS_TTIMER_SIZE == 2
        #define ASM_OST_TCB_SIZE_CONST      asm("TCB_SIZE set 5");
    #elif   OS_TTIMER_SIZE == 4
        #define ASM_OST_TCB_SIZE_CONST      asm("TCB_SIZE set 7");
    #endif

//------------------------------------------------------------------------------
#elif OS_CODE_POINTER_SIZE == 4
//------------------------------------------------------------------------------

    #if     OS_TTIMER_SIZE == 1
        #define ASM_OST_TCB_SIZE_CONST      asm("TCB_SIZE set 6");
    #elif   OS_TTIMER_SIZE == 2
        #define ASM_OST_TCB_SIZE_CONST      asm("TCB_SIZE set 7");
    #elif   OS_TTIMER_SIZE == 4
        #define ASM_OST_TCB_SIZE_CONST      asm("TCB_SIZE set 9");
    #endif


//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#ifdef OS_ENABLE_TTIMERS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Set BSR for work with task timers
#define     ASM_SET_BANK    asm(" movlb high(__OS_Tasks)");       \
                            asm("DELAY_BIT set 4");                 \
                            asm("OS_TSKVAR set low __OS_Tasks");  \


//------------------------------------------------------------------------------
// Expression compiles correctly in this case only:
//        asm("   movwf   ("#TASK_ID" +1 )* TCB_SIZE + low(__OS_Tasks) - 3 ");
//------------------------------------------------------------------------------
#if   OS_TTIMER_SIZE == 1
//------------------------------------------------------------------------------

    #define __OS_TtimerWork(TASK_ID)                                \
        asm("btfss "#TASK_ID"*TCB_SIZE+OS_TSKVAR,DELAY_BIT");       \
        asm("bra $+6");                                             \
        asm("infsnz ("#TASK_ID"+1)*TCB_SIZE+OS_TSKVAR-1");          \
        asm("bcf "#TASK_ID"*TCB_SIZE+OS_TSKVAR,DELAY_BIT");         \

    #define __OS_TtimersWorkSize()                                  \
        ASM_OST_TCB_SIZE_CONST;                                     \
        ASM_SET_BANK;                                               \
        _fsr = (OS_FSR_TYPE)_OS_Tasks;                              \
        do {                                                        \
            asm("   movlw   TCB_SIZE");                             \
            if (((near OST_TASK_STATE*)(&_indf))->bDelay)           \
            {                                                       \
                asm("   movlw   TCB_SIZE - 1");                     \
                asm("   addwf   __fsr, f, c     ");                 \
                asm("   incf    __postinc, f, c ");                 \
                asm("   movlw   0               ");                 \
                if (CARRY)                                          \
                {                                                   \
                    asm("   movlw   TCB_SIZE    ");                 \
                    asm("   subwf   __fsr, f, c ");                 \
                    ((near OST_TASK_STATE*)(&_indf))->bDelay = 0;   \
                }                                                   \
            }                                                       \
            asm("   addwf   __fsr, f, c ");                         \
            _wreg = OS_TASKS*(OS_TTIMER_SIZE + OS_CODE_POINTER_SIZE + 1) + (char)&_OS_Tasks;  \
            asm("   xorwf   __fsr, w, c         ");                 \
        } while (!ZERO);                                            \

//------------------------------------------------------------------------------
#elif OS_TTIMER_SIZE == 2
//------------------------------------------------------------------------------

    #define __OS_TtimerWork(TASK_ID)                                \
        asm("btfsc "#TASK_ID"*TCB_SIZE+OS_TSKVAR,DELAY_BIT");       \
        asm("incfsz ("#TASK_ID"+1)*TCB_SIZE+OS_TSKVAR-2");          \
        asm("bra $+6");                                             \
        asm("infsnz ("#TASK_ID"+1)*TCB_SIZE+OS_TSKVAR-1");          \
        asm("bcf "#TASK_ID"*TCB_SIZE+OS_TSKVAR,DELAY_BIT");         \

    #define __OS_TtimersWorkSize()                                  \
        ASM_OST_TCB_SIZE_CONST;                                     \
        ASM_SET_BANK;                                               \
        _fsr = (OS_FSR_TYPE)_OS_Tasks;                              \
        do {                                                        \
            asm("   movlw   TCB_SIZE");                             \
            if (((near OST_TASK_STATE*)(&_indf))->bDelay)           \
            {                                                       \
                asm("   movlw   TCB_SIZE - 2");                     \
                asm("   addwf   __fsr, f, c     ");                 \
                asm("   incf    __postinc, f, c ");                 \
                asm("   movlw   0               ");                 \
                asm("   addwfc  __postinc, f, c ");                 \
                if (CARRY)                                          \
                {                                                   \
                    asm("   movlw   TCB_SIZE    ");                 \
                    asm("   subwf   __fsr, f, c ");                 \
                    ((near OST_TASK_STATE*)(&_indf))->bDelay = 0;   \
                }                                                   \
            }                                                       \
            asm("   addwf   __fsr, f, c ");                         \
            _wreg = OS_TASKS*(OS_TTIMER_SIZE + OS_CODE_POINTER_SIZE + 1) + (char)&_OS_Tasks;  \
            asm("   xorwf   __fsr, w, c         ");                 \
        } while (!ZERO);                                            \


//------------------------------------------------------------------------------
#elif OS_TTIMER_SIZE == 4
//------------------------------------------------------------------------------

    #define __OS_TtimerWork(TASK_ID)                                \
        asm("btfsc "#TASK_ID"*TCB_SIZE+OS_TSKVAR,DELAY_BIT");       \
        asm("incfsz ("#TASK_ID"+1)*TCB_SIZE+OS_TSKVAR-4");          \
        asm("bra $+12");                                            \
        asm("infsnz ("#TASK_ID"+1)*TCB_SIZE+OS_TSKVAR-3");          \
        asm("incfsz ("#TASK_ID"+1)*TCB_SIZE+OS_TSKVAR-2");          \
        asm("bra $+6");                                             \
        asm("infsnz ("#TASK_ID" + 1)*TCB_SIZE+OS_TSKVAR-1");        \
        asm("bcf "#TASK_ID"*TCB_SIZE+OS_TSKVAR,DELAY_BIT");


    #define __OS_TtimersWorkSize()                                  \
        ASM_OST_TCB_SIZE_CONST;                                     \
        ASM_SET_BANK;                                               \
        _fsr = (OS_FSR_TYPE)_OS_Tasks;                              \
        do {                                                        \
            asm("   movlw   TCB_SIZE");                             \
            if (((near OST_TASK_STATE*)(&_indf))->bDelay)           \
            {                                                       \
                asm("   movlw   TCB_SIZE - 4");                     \
                asm("   addwf   __fsr, f, c     ");                 \
                asm("   incf    __postinc, f, c ");                 \
                asm("   movlw   0               ");                 \
                asm("   addwfc  __postinc, f, c ");                 \
                asm("   addwfc  __postinc, f, c ");                 \
                asm("   addwfc  __postinc, f, c ");                 \
                if (CARRY)                                          \
                {                                                   \
                    asm("   movlw   TCB_SIZE    ");                 \
                    asm("   subwf   __fsr, f, c ");                 \
                    ((near OST_TASK_STATE*)(&_indf))->bDelay = 0;   \
                }                                                   \
            }                                                       \
            asm("   addwf   __fsr, f, c ");                         \
            _wreg = OS_TASKS*(OS_TTIMER_SIZE + OS_CODE_POINTER_SIZE + 1) + (char)&_OS_Tasks;\
            asm("   xorwf   __fsr, w, c         ");                 \
        } while (!ZERO);                                            \


//------------------------------------------------------------------------------
#endif      // if sizeof(OS_TIMER_TYPE)
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#endif      // OS_ENABLE_TTIMERS
//------------------------------------------------------------------------------

/******************************************************************************************/









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

    #define _OS_INC_DTIMER()                                                             \
        {                                                                               \
            asm("   movlw   3                   ");                                     \
            asm("   addwf   __fsr, f, c         ");  /* FSR0L can't overflow here */    \
            asm("   incf    __postdec, f, c     ");                                     \
            asm("   movf    __postdec, w, c     ");                                     \
            asm("   movf    __postdec, w, c     ");                                     \
            if (_carry) _OS_IndfTimerTimeout = 1;                                       \
        }                                                                               \


//------------------------------------------------------------------------------
#elif OS_DTIMER_SIZE == 2
//------------------------------------------------------------------------------

    #define _OS_INC_DTIMER()                                                             \
        {                                                                               \
            _OS_DTimers.Flags.bTimeout = 0;                                             \
            asm("   movlw   3                   ");                                     \
            asm("   addwf   __fsr, f, c         ");  /* FSR0L can't overflow here */    \
            asm("   movlw   0 ");                                                       \
            asm("   incf    __postinc, f, c     ");                                     \
            asm("   addwfc  __postdec, f, c     ");                                     \
            if (_carry) _OS_DTimers.Flags.bTimeout = 1;                                 \
            asm("   movlw   3                   ");                                     \
            asm("   subwf   __fsr, f, c         "); /* FSR0L can't overflow here */     \
            if (_OS_DTimers.Flags.bTimeout)                                             \
                _OS_IndfTimerTimeout = 1;                                               \
        }                                                                               \

//------------------------------------------------------------------------------
#elif OS_DTIMER_SIZE == 4
//------------------------------------------------------------------------------

    #define _OS_INC_DTIMER()                                                             \
        {                                                                               \
            _OS_DTimers.Flags.bTimeout = 0;                                             \
            asm("   movlw   3                   ");                                     \
            asm("   addwf   __fsr, f, c         ");  /* FSR0L can't overflow here */    \
            asm("   movlw   0                   ");                                     \
            asm("   incf    __postinc, f, c     ");                                     \
            asm("   addwfc  __postinc, f, c     ");                                     \
            asm("   addwfc  __postinc, f, c     ");                                     \
            asm("   addwfc  __postdec, f, c     ");                                     \
            if (_carry) _OS_DTimers.Flags.bTimeout = 1;                                 \
            asm("   movlw   5                   ");                                     \
            asm("   subwf   __fsr, f, c         "); /* FSR0L can't overflow here */     \
            if (_OS_DTimers.Flags.bTimeout)                                             \
                _OS_IndfTimerTimeout = 1;                                               \
        }                                                                               \

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------


#define __OS_DtimersWork()                              \
    {                                                   \
        _fsr = (OS_FSR_TYPE) &_OS_DTimers.Flags;        \
                                                        \
    REPEAT:                                             \
                                                        \
        if (_OS_PostincTimerNextEnable)                 \
        {                                               \
                                                        \
            /* Point FSR to next timer in list   */     \
                                                        \
            asm("   movf    __postinc, w, c        ");  \
            asm("   movff   __postdec, __fsr + 1   ");  \
            asm("   movwf   __fsr + 0, c            "); \
            if (!_OS_IndfTimerRun) goto REPEAT;         \
            _OS_INC_DTIMER();                           \
                                                        \
            goto REPEAT;                                \
        }                                               \
    }



//------------------------------------------------------------------------------
#endif  // OS_ENABLE_DTIMER
//------------------------------------------------------------------------------









//------------------------------------------------------------------------------
#if defined(OS_ENABLE_QTIMERS)  /*100210.1*/
//------------------------------------------------------------------------------

#define _OS_IndfFTimerTimeout       ((OS_RAM_NEAR OST_QTIMER_FLAGS*)(&_indf))->bTimeout
#define _OS_IndfFTimerActive        ((OS_RAM_NEAR OST_QTIMER_FLAGS*)(&_indf))->bActive
#define _OS_IndfFTimerNextEnable    ((OS_RAM_NEAR OST_QTIMER_FLAGS*)(&_indf))->bNextEnable

#define _OS_QtimersWork_DEFINED

//------------------------------------------------------------------------------
#if   OS_QTIMER_SIZE == 1
//------------------------------------------------------------------------------

    #define _OS_IF_INC_QTIMER()         if (!++_indf)

    #define _OS_CHECK_QTIMER()          asm("   movf    __indf, f, c    ")

//------------------------------------------------------------------------------
#elif OS_QTIMER_SIZE == 2
//------------------------------------------------------------------------------

    #define _OS_IF_INC_QTIMER()                             \
        if (!++_postinc)                                    \
        if (!++_indf   )                                    \


    #define _OS_CHECK_QTIMER()                              \
        asm("   movf    __postinc, w, c   ");               \
        asm("   iorwf   __indf, w, c   ");

//------------------------------------------------------------------------------
#elif OS_QTIMER_SIZE == 4
//------------------------------------------------------------------------------

    #define _OS_IF_INC_QTIMER()                             \
        if (!++_postinc)                                    \
        if (!++_postinc)                                    \
        if (!++_postinc)                                    \
        if (!++_indf   )                                    \


    #define _OS_CHECK_QTIMER()                              \
        asm("   movf    __postinc, w, c   ");               \
        asm("   iorwf   __postinc, w, c   ");               \
        asm("   iorwf   __postinc, w, c   ");               \
        asm("   iorwf   __indf   , w, c   ");               \



//------------------------------------------------------------------------------
#endif  // OS_QTIMER_SIZE
//------------------------------------------------------------------------------

#define _OS_QtimersWork_DEFINED


#define __OS_QtimersWork()                                      \
    if (_OS_Qtimers.Flags.bNextEnable)                          \
    {                                                           \
        _fsr = ((OS_FSR_TYPE)&_OS_Qtimers) + 1;                 \
        asm("   movf    __postinc, w, c        ");              \
        asm("   addlw   3                      ");              \
        asm("   movff   __indf, __fsr + 1      ");              \
        asm("   movwf   __fsr + 0, c           ");              \
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
            asm("   movf    __preinc, w, c        ");           \
            asm("   movff   __preinc, __fsr + 1      ");        \
            asm("   movwf   __fsr + 0, c           ");          \
            _OS_Qtimers.Next = (void*)_fsr;                     \
            _fsr += 3;                                          \
            _OS_CHECK_QTIMER();                                 \
        } while (_zero);                                        \
    }

/**/
//------------------------------------------------------------------------------
#endif  // OS_ENABLE_QTIMERS
//------------------------------------------------------------------------------





#endif

//******************************************************************************
//  END OF FILE osa_pic18_htpicc.h
//******************************************************************************

