#line 1 "C:/Users/giljr/Documents/jungletronics/Ardu_Serie/37_Ardu_RTOS/code/osa/osa.c"
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
#line 55 "C:/Users/giljr/Documents/jungletronics/Ardu_Serie/37_Ardu_RTOS/code/osa/osa.c"
 volatile   OST_SYSTEM_FLAGS _OS_Flags;
    OST_UINT  _OS_Temp;
#line 63 "C:/Users/giljr/Documents/jungletronics/Ardu_Serie/37_Ardu_RTOS/code/osa/osa.c"
 volatile    OST_UINT  _OS_TempH;



 volatile   OST_TASK_STATE _OS_State;






   OST_TCB *    volatile _OS_CurTask;
#line 99 "C:/Users/giljr/Documents/jungletronics/Ardu_Serie/37_Ardu_RTOS/code/osa/osa.c"
   OST_TCB *    volatile _OS_BestTask;
   OST_TCB *    volatile _OS_LastTask;

    OST_UINT  _OS_Best_Priority;
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/osa_include.c"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.c"
#line 34 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.c"
volatile unsigned int _fsr absolute 0xFE9;
volatile char _indf absolute 0xFEF;
volatile char _postinc absolute 0xFEE;
volatile char _postdec absolute 0xFED;
volatile char _preinc absolute 0xFEC;

volatile unsigned char _fsr1l absolute 0xFE1;

volatile char _pcl absolute 0xFF9;
volatile char _pclath absolute 0xFFA;
volatile char _pclatu absolute 0xFFB;
volatile char _status absolute 0xFD8;
volatile char _tosl absolute 0xFFD;
volatile char _tosh absolute 0xFFE;
volatile char _tosu absolute 0xFFF;
volatile char _bsr absolute 0xFE0;
volatile char _wreg absolute 0xFE8;
volatile char _intcon absolute 0xFF2;
volatile char _rcon absolute 0xFD0;
volatile char _stkptr absolute 0xFFC;










void _OS_JumpToTask (void)
{
 _OS_SET_FSR_CUR_TASK();
 *((  char*)&_OS_State) = _postinc;
 asm {
 push
 movf __postinc, W, 0
 movwf __tosl
 movf __postinc, W, 0
 movwf __tosh
 movf __postinc, W, 0
 movwf __tosu
 return
 }
}
#line 88 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.c"
void _OS_ClrReadySetClrCanContinue (void);
#line 106 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.c"
void _OS_ReturnSave (void)
{
 _OS_SET_FSR_CUR_TASK();
  _preinc = _tosl; _preinc = _tosh; _preinc = _tosu; ;
  _OS_State.bReady  = 1;
  asm{ pop } ;
}
#line 132 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.c"
void _OS_ReturnNoSave (void)
{
  asm{ pop } ;
}
#line 156 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.c"
void _OS_EnterWaitMode (void)
{
 _OS_Temp = 0;
 _OS_ClrReadySetClrCanContinue();
}
#line 181 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.c"
void _OS_EnterWaitModeTO (void)
{
 _OS_Temp = 0xFF;
 _OS_ClrReadySetClrCanContinue();
}
#line 207 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.c"
void _OS_ClrReadySetClrCanContinue (void)
{
  asm{ pop } ;

 _OS_Flags.bEventError = 0;

 _OS_SET_FSR_CUR_TASK();
  _preinc = _tosl; _preinc = _tosh; _preinc = _tosu; ;
  _OS_State.bReady  = 0;


 if (! _OS_State.bDelay )  _OS_State.bCanContinue  = 0;
 if (_OS_Temp & 0x01)  _OS_State.bCanContinue  = 1;


}
#line 242 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.c"
void _OS_SET_FSR_CUR_TASK (void)
{
 _fsr = ( unsigned int )_OS_CurTask;
}
#line 281 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.c"
char OS_DI (void)
{
 char temp;
 temp = _intcon & 0xC0;
  GIE_bit  = 0;
 if ( IPEN_bit )  GIEL_bit  = 0;
 return temp;
}
#line 326 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.c"
void OS_RI (char temp)
{
 if (temp & 0x80)  GIE_bit  = 1;
 if ( IPEN_bit )
 {
 if (temp & 0x40)  GIEL_bit  = 1;
 }
}
#line 383 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.c"
void _OS_CheckEvent (OST_UINT bEvent)
{
#line 421 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.c"
 _OS_Flags.bTimeout = 0;
 _OS_SET_FSR_CUR_TASK();

 if (bEvent)
 {
 if ( _OS_State.bReady ) {



  _OS_State.bDelay  = 0;
 _indf &= ~0x10;
#line 439 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.c"
 return;
 }

  _OS_State.bReady  = 1;

 } else {
  _OS_State.bReady  = 0;
 }



 if ( (!_OS_State.bDelay)  &&  _OS_State.bCanContinue )
 {
  _OS_State.bReady  = 1;
 _OS_Flags.bTimeout = 1;
#line 459 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/port/pic18/osa_pic18_mikroc.c"
 return;
 }



  asm{ pop } ;




}
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/timers/osa_dtimer.c"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/timers/osa_qtimer.c"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/timers/osa_stimer.c"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/timers/osa_ttimer.c"
#line 69 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/timers/osa_ttimer.c"
 void _OS_InitDelay ( OST_UINT16  Delay)
 {

   { ; _OS_State.bCanContinue  = 0; _OS_CurTask->State.bDelay = 0;; ; } ;
#line 77 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/timers/osa_ttimer.c"
 if (Delay)
 {
 Delay ^= ( OST_UINT16 )-1;
 Delay ++;
 _OS_CurTask->Timer = Delay;
   { ; _OS_CurTask->State.bReady = 1; _OS_CurTask->State.bDelay = 1;; ; } ;
#line 87 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/timers/osa_ttimer.c"
  _OS_State.bDelay  = 1;
  _OS_State.bReady  = 1;

 }
 }
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/events/osa_squeue.c"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/events/osa_queue.c"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/events/osa_csem.c"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/events/osa_bsem.c"
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/system/osa_system.c"
#line 67 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/system/osa_system.c"
void OS_Init (void)
{



 *(   OST_UINT *)&_OS_Flags = 0;


  ;





 _OS_LastTask = (  OST_TCB * ) _OS_Tasks + ( 3  - 1);
#line 326 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/system/osa_system.c"
 _OS_Tasks[0].State.bEnable = 0;


 _OS_Tasks[1].State.bEnable = 0;



 _OS_Tasks[2].State.bEnable = 0;
#line 394 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/system/osa_system.c"
}
#line 1 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/system/osa_tasks.c"
#line 36 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/system/osa_tasks.c"
  OST_TCB _OS_Tasks[ 3 ]  ;
#line 75 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/system/osa_tasks.c"
 void _OS_Task_Create( OST_UINT  priority,  unsigned long  TaskAddr)
 {
   OST_TCB *  Task;

 _OS_Flags.bError = 0;
#line 88 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/system/osa_tasks.c"
 Task = (  OST_TCB * )_OS_Tasks;
 _OS_Temp = 0;

 do {

 if (!Task->State.bEnable)
 {
 ((OST_TASK_STATE*)&priority)->bEnable = 1;
 ((OST_TASK_STATE*)&priority)->bReady = 1;

 Task->pTaskPointer = TaskAddr;


 Task->Timer = 0;
#line 108 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/system/osa_tasks.c"
 *((  char*)&Task->State) = priority;


 if (Task == _OS_CurTask) *((  char*)&_OS_State) = priority;
#line 126 "c:/users/giljr/documents/jungletronics/ardu_serie/37_ardu_rtos/code/osa/kernel/system/osa_tasks.c"
 _OS_Flags.bError = 0;

 return ;

 }

 Task ++;

 } while (++_OS_Temp <  3 );


 _OS_Flags.bError = 1;

 return ;
 }
