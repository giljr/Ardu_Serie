#line 1 "C:/Users/giljr/Documents/jungletronics/Ardu_Serie/37_Ardu_RTOS/code/osa/apps/First_OSA_Project/First_OSA_Project.c"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/osa.h"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/apps/osacfg.h"
#line 1068 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/osa.h"
typedef  unsigned char  _OST_SMSG;
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/osa_include.h"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.h"
#line 41 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.h"
typedef unsigned char OST_UINT8;
typedef unsigned int OST_UINT16;
typedef unsigned long OST_UINT32;
typedef unsigned char OST_BOOL;

typedef OST_UINT8 OST_UINT;
#line 93 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.h"
extern volatile unsigned int _fsr;
extern volatile char _indf;
extern volatile char _postinc;
extern volatile char _postdec;
extern volatile char _preinc;

extern volatile unsigned char _fsr1l;

extern volatile char _pcl;
extern volatile char _pclath;
extern volatile char _pclatu;
extern volatile char _status;
extern volatile char _tosl;
extern volatile char _tosh;
extern volatile char _tosu;
extern volatile char _bsr;
extern volatile char _wreg;
extern volatile char _intcon;
extern volatile char _rcon;
extern volatile char _stkptr;
#line 251 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.h"
void _OS_JumpToTask (void);
#line 288 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.h"
extern void _OS_ReturnSave (void);
extern void _OS_ReturnNoSave (void);
extern void _OS_EnterWaitMode (void);
extern void _OS_EnterWaitModeTO (void);

extern void _OS_SET_FSR_CUR_TASK (void);
#line 397 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.h"
char OS_DI (void);
void OS_RI (char);
#line 419 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.h"
extern void _OS_CheckEvent (OST_UINT);
#line 1092 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/osa.h"
typedef  unsigned char  OST_SMSG;
#line 1110 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/osa.h"
typedef  void *  OST_MSG;







typedef

 volatile

 struct _OST_MSG_CB
{
 OST_UINT status;
 OST_MSG msg;

} OST_MSG_CB;
#line 1139 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/osa.h"
typedef struct
{
 OST_UINT cSize;
 OST_UINT cFilled;
 OST_UINT cBegin;

} OST_QUEUE_CONTROL;




typedef struct
{
 OST_QUEUE_CONTROL Q;
 OST_MSG *pMsg;

} OST_QUEUE;




typedef struct
{
 OST_QUEUE_CONTROL Q;
 OST_SMSG *pSMsg;

} OST_SQUEUE;
#line 1190 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/osa.h"
typedef struct
{
 OST_UINT bEventError : 1;
 OST_UINT bError : 1;

 OST_UINT bInCriticalSection : 1;

 OST_UINT bCheckingTasks : 1;


 OST_UINT bBestTaskFound : 1;





  OST_UINT bTimeout : 1; OST_UINT bGIE_CTemp : 1; OST_UINT bGIEL_CTemp : 1; 
#line 1218 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/osa.h"
} OST_SYSTEM_FLAGS;
#line 1235 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/osa.h"
typedef struct
{
 OST_UINT cPriority : 3;
 OST_UINT bReady : 1;
 OST_UINT bDelay : 1;


 OST_UINT bCanContinue: 1;
 OST_UINT bEnable : 1;
 OST_UINT bPaused : 1;


  


} OST_TASK_STATE;
#line 1270 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/osa.h"
typedef struct
{
 OST_TASK_STATE State;
  unsigned long  pTaskPointer;




  



  OST_UINT16  Timer;


} OST_TCB;
#line 1477 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/osa.h"
typedef OST_UINT8 OST_CSEM;
#line 1506 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/osa.h"
extern volatile   OST_SYSTEM_FLAGS _OS_Flags;
extern   OST_UINT _OS_Temp;


extern volatile   OST_UINT _OS_TempH;




extern volatile   OST_TASK_STATE _OS_State;
#line 1527 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/osa.h"
extern   OST_TCB *    volatile _OS_CurTask;





 extern   OST_UINT _OS_Best_Priority;






 extern   OST_TCB *    volatile _OS_BestTask;
 extern   OST_TCB *    volatile _OS_LastTask;
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/osa_oldnames.h"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/system/osa_system.h"
#line 48 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/system/osa_system.h"
extern void OS_Init (void);



extern void OS_EnterCriticalSection (void);
extern void OS_LeaveCriticalSection (void);
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/system/osa_tasks.h"
#line 37 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/system/osa_tasks.h"
extern   OST_TCB _OS_Tasks[ 3 ];






void _OS_Task_Create(OST_UINT priority,  unsigned long  TaskAddr);
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/events/osa_bsem.h"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/events/osa_csem.h"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/events/osa_flag.h"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/events/osa_msg.h"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/events/osa_queue.h"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/events/osa_smsg.h"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/events/osa_squeue.h"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/timers/osa_stimer.h"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/timers/osa_stimer_old.h"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/timers/osa_dtimer.h"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/timers/osa_qtimer.h"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/timers/osa_ttimer.h"
#line 53 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/timers/osa_ttimer.h"
void _OS_InitDelay( OST_UINT16  Delay);
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/timers/osa_timer.h"
#line 14 "C:/Users/giljr/Documents/jungletronics/Ardu_Serie/37_Ardu_RTOS/code/osa/apps/First_OSA_Project/First_OSA_Project.c"
void InitMCU (void);
void TickTimerIE(void);

void Task_LED0 (void);
void Task_LED1 (void);
void Task_LED2 (void);
#pragma funcall main Task_LED0
#pragma funcall main Task_LED1
#pragma funcall main Task_LED2
#line 27 "C:/Users/giljr/Documents/jungletronics/Ardu_Serie/37_Ardu_RTOS/code/osa/apps/First_OSA_Project/First_OSA_Project.c"
void INTERRUPT_HIGH() iv 0x0008 ics ICS_AUTO {
 if (TMR2IF_bit)
 {
 TMR2IF_bit = 0;
  { { ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ;  { ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; } } ; { ; ; if (_OS_Tasks[0].State.bDelay) { if(!++_OS_Tasks[0].Timer) _OS_Tasks[0].State.bDelay = 0; } ; if (_OS_Tasks[1].State.bDelay) { if(!++_OS_Tasks[1].Timer) _OS_Tasks[1].State.bDelay = 0; } ; if (_OS_Tasks[2].State.bDelay) { if(!++_OS_Tasks[2].Timer) _OS_Tasks[2].State.bDelay = 0; } ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; } ; ; ; ; } ;
 }
}

void TickTimerIE(void)
{

 T2CON =  0B01001101 ;
 PR2 =  (49) ;

 INTCON.GIEH = 1;
 INTCON.GIEL = 1;
 RCON.IPEN = 1;

 TMR2IE_bit = 1;
 TMR2IP_bit = 1;
 T2CON.TMR2ON = 1;
}

void main (void)
{
 InitMCU();
 TickTimerIE();

 OS_Init();


  { _OS_Task_Create(0, ( unsigned long )&(Task_LED0)); } ;
  { _OS_Task_Create(0, ( unsigned long )&(Task_LED1)); } ;
  { _OS_Task_Create(0, ( unsigned long )&(Task_LED2)); } ;

  { GIE_bit  = 1; if ( IPEN_bit ) GIEL_bit  = 1;} ;


  for(;;) { _OS_Flags.bBestTaskFound = 0; _OS_Flags.bCheckingTasks = 1; _OS_Best_Priority = 7  + 1; ; _OS_CurTask = _OS_LastTask; do { if (_OS_CurTask >= (  OST_TCB * )_OS_Tasks + ( 3 -1)) _OS_CurTask = (  OST_TCB * )_OS_Tasks; else _OS_CurTask++;  _OS_State = _OS_CurTask->State ; if (! _OS_State.bEnable  || _OS_State.bPaused ) goto _OS_SCHED_CONTINUE; ; if (_OS_Best_Priority <= (_OS_Temp = _OS_State.cPriority )) goto _OS_SCHED_CONTINUE; if (! _OS_State.bReady )  { _OS_SCHED_RUN:; _OS_JumpToTask() ; _OS_SET_FSR_CUR_TASK(); _indf &= ~( 0x08  | 0x20 ); _indf |= *((char*)&_OS_State) & ( 0x08  | 0x20 ); ; if (!_OS_Flags.bCheckingTasks) goto SCHED_END; ; } if ( _OS_State.bDelay  && ! _OS_State.bCanContinue ) goto _OS_SCHED_CONTINUE; ; if ( _OS_State.bReady )  { _OS_Flags.bBestTaskFound = 1; _OS_BestTask = _OS_CurTask; _OS_Best_Priority = _OS_State.cPriority ; if (_OS_Best_Priority == 0 ) break; } _OS_SCHED_CONTINUE:;  } while ( 1  && _OS_CurTask != _OS_LastTask); if (_OS_Flags.bBestTaskFound) { _OS_Flags.bCheckingTasks = 0; _OS_LastTask = _OS_CurTask = _OS_BestTask; _OS_State = _OS_CurTask->State ; if ( _OS_State.bEnable  && ! _OS_State.bPaused ) goto _OS_SCHED_RUN; } SCHED_END:; } ;

}


void InitMCU(void)
{







 ADCON1 |= 0X0F;


 TRISD = 0;
 PORTD = 0;

}



void Task_LED0(void)
{
 for(;;)
 {
 LATD.RD0 = ~LATD.RD0;
  { _OS_InitDelay(300 /1 ); { _OS_ReturnSave(); asm{ nop } ; } ; } ;







 }
}

void Task_LED1(void)
{
 for(;;)
 {
 LATD.RD1 = ~LATD.RD1;
  { _OS_InitDelay(300 /1 ); { _OS_ReturnSave(); asm{ nop } ; } ; } ;
 }
}

void Task_LED2(void)
{
 for(;;)
 {
 LATD.RD2 = ~LATD.RD2;
  { _OS_InitDelay(300 /1 ); { _OS_ReturnSave(); asm{ nop } ; } ; } ;
 }
}
