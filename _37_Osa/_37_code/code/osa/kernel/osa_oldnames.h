/*
 ***********************************************************************************************
 *                                                                                              
 *  OSA cooperative RTOS for microcontrollers PIC (Microchip) and 8-bit AVR (Atmel)
 *                                                                                              
 *  OSA is distributed under BSD license (see license.txt)                                      
 *                                                                                              
 *  URL:        http://wiki.pic24.ru/doku.php/en/osa/ref/intro                                  
 *              http://picosa.narod.ru                                                          
 *                                                                                              
 *----------------------------------------------------------------------------------------------
 *                                                                                              
 *  File:           osa_oldnames.h                                                              
 *                                                                                              
 *  Programmer:     Timofeev Victor                                                             
 *                  osa@pic24.ru, testerplus@mail.ru                                            
 *                                                                                              
 *  Definition:     This file contains old style names of OSA services. This file included      
 *                  automatically in OSA.h. This file is produced for compability with all      
 *                  programs written with old names.                                            
 *                                                                                              
 *  History:        26.02.2009  Added OS_ReserveTask and OS_TaskCreateP definitions             
 *                  22.07.2009  Added OS_Task_Define                                            
 *                  04.11.2009  All OS_SMsg_xxx renamed to OS_Smsg_xxx                          
 *                                                                                              
 ***********************************************************************************************
 */


#ifndef __OSA_NAMES_H
#define __OSA_NAMES_H


#define OST_PMSG                    OST_MSG_CB
#define OST_QMSG                    OST_QUEUE
#define OST_QSMSG                   OST_SQUEUE



/************************************************************************************************/
/*                                                                                              */
/*               System services                                                                */
/*                                                                                              */
/************************************************************************************************/

//#define OS_Sched                    OS_Sched                    //  
//#define OS_Run                      OS_Run                      //  
//#define OS_Init                     OS_Init                     //  
//#define OS_Timer                    OS_Timer                    //  

        /* Waiting */

//#define OS_Yield                    OS_Yield                    //  
//#define OS_Delay                    OS_Delay                    //  delaytime

#define OS_Cond_Wait(condition)                 OS_Wait(condition)
#define OS_Cond_Wait_TO(condition, timeout)     OS_Wait_TO(condition, timeout)

#define OS_WaitCondition            OS_Cond_Wait                //  condition
#define OS_WaitConditionTimeout     OS_Cond_Wait_TO             //  condition, timeout

        /* Checking */

//#define OS_IsTimeout                OS_IsTimeout                //  
//#define OS_IsError                  OS_IsError                  //  
//#define OS_IsEventError             OS_IsEventError             //  
//#define OS_IsInCriticalSection      OS_IsInCriticalSection      //  

        /* Interrupts */

//#define OS_EnterInt                 OS_EnterInt                 //  
//#define OS_LeaveInt                 OS_LeaveInt                 //  
//#define OS_DI                       OS_DI                       //  
//#define OS_EI                       OS_EI                       //  
//#define OS_RI                       OS_RI                       //  char
//#define OS_EnterCriticalSection     OS_EnterCriticalSection     //  
//#define OS_LeaveCriticalSection     OS_LeaveCriticalSection     //  



/************************************************************************************************/
/*                                                                                              */
/*               Task control services                                                          */
/*                                                                                              */
/************************************************************************************************/

        /* Create, delete, pause */

#define OS_CreateTask               OS_Task_Create              //  priority, TaskName
#define OS_ReplaceTask              OS_Task_Replace             //  priority, TaskName
#define OS_StopTask                 OS_Task_Stop                //  
#define OS_DeleteTask               OS_Task_Delete              //  tp
#define OS_PauseTask                OS_Task_Pause               //  tp
#define OS_ContinueTask             OS_Task_Continue            //  tp
#define OS_ReserveTask              OS_Task_Reserve             //  tp
#define OS_CreateTaskP              OS_Task_CreateP             //  tp
#define OS_Task_Define              OS_Task_Reserve             //  TaskName      

        /* Priority control */

#define OS_GetTaskPriority          OS_Task_GetPriority         //  tp
#define OS_SetTaskPriority          OS_Task_SetPriority         //  tp, priority

        /* Checking */

#define OS_IsTaskPaused             OS_Task_IsPaused            //  tp
#define OS_IsTaskEnable             OS_Task_IsEnable            //  tp

//#define OS_GetCurTask               OS_GetCurTask               //  



/************************************************************************************************/
/*                                                                                              */
/*               Binary semaphores                                                              */
/*                                                                                              */
/************************************************************************************************/

        /* Setting/resetting/checking */

#define OS_SetBSem                  OS_Bsem_Set                 //  bsem
#define OS_SignalBSem               OS_Bsem_Signal              //  bsem
#define OS_ResetBSem                OS_Bsem_Reset               //  bsem
#define OS_SwitchBSem               OS_Bsem_Switch              //  bsem
#define OS_CheckBSem                OS_Bsem_Check               //  bsem

        /* Waiting */

#define OS_WaitBSem                 OS_Bsem_Wait                //  bsem
#define OS_WaitBSemTimeout          OS_Bsem_Wait_TO             //  bsem, timeout



/************************************************************************************************/
/*                                                                                              */
/*               Counting semaphores                                                            */
/*                                                                                              */
/************************************************************************************************/

        /* Creating */

#define OS_CreateCSem               OS_Csem_Create              //  csem

        /* Setting */

#define OS_SignalCSem               OS_Csem_Signal              //  csem
#define OS_AcceptCSem               OS_Csem_Accept              //  csem
#define OS_SetCSem                  OS_Csem_Set                 //  csem
#define OS_SetCSemValue             OS_Csem_SetValue            //  csem, value
#define OS_ResetCSem                OS_Csem_Reset               //  csem

        /* Interrupt */

#define OS_IntSignalCSem            OS_Csem_Signal_I            //  csem
#define OS_IntAcceptCSem            OS_Csem_Accept_I            //  csem
#define OS_IntSetCSem               OS_Csem_Set_I               //  csem
#define OS_IntSetCSemValue          OS_Csem_SetValue_I          //  csem, value
#define OS_IntResetCSem             OS_Csem_Reset_I             //  csem

        /* Checking */

#define OS_CheckCSem                OS_Csem_Check               //  csem
#define OS_IntCheckCSem             OS_Csem_Check_I             //  csem

        /* Waiting */

#define OS_WaitCSem                 OS_Csem_Wait                //  csem
#define OS_WaitCSemTimeout          OS_Csem_Wait_TO             //  csem, timeout



/************************************************************************************************/
/*                                                                                              */
/*               Pointers to messages                                                           */
/*                                                                                              */
/************************************************************************************************/

        /* Create */

#define OS_CreateMsg                OS_Msg_Create               //  pmsg

        /* Checking */

#define OS_CheckMsg                 OS_Msg_Check                //  pmsg

        /* Accepting */

#define OS_AcceptMsg                OS_Msg_Accept               //  pmsg, os_msg_type_var

        /* Posting */

#define OS_SendMsg                  OS_Msg_Send                 //  pmsg, message
#define OS_SendMsgTimeout           OS_Msg_Send_TO              //  pmsg, message, timeout
#define OS_SendMsgNoWait            OS_Msg_Send_Now             //  pmsg, message
#define OS_IntSendMsgNoWait         OS_Msg_Send_I               //  pmsg, message

        /* Waiting */

#define OS_WaitMsg                  OS_Msg_Wait                 //  pmsg, os_msg_type_var
#define OS_WaitMsgTimeout           OS_Msg_Wait_TO              //  pmsg, os_msg_type_var, timeout



/************************************************************************************************/
/*                                                                                              */
/*               Simple messages                                                                */
/*                                                                                              */
/************************************************************************************************/

        /* Creating */

#define OS_CreateSMsg               OS_Smsg_Create              //  smsg

        /* Checking */

#define OS_CheckSMsg                OS_Smsg_Check               //  smsg

        /* Accepting */

#define OS_AcceptSMsg               OS_Smsg_Accept              //  smsg, os_smsg_type_var

        /* Posting */

#define OS_SendSMsg                 OS_Smsg_Send                //  smsg, smessage
#define OS_SendSMsgTimeout          OS_Smsg_Send_TO             //  smsg, smessage, timeout
#define OS_SendSMsgNoWait           OS_Smsg_Send_Now            //  smsg, smessage
#define OS_IntSendSMsgNoWait        OS_Smsg_Send_I              //  smsg, smessage

        /* Waiting */

#define OS_WaitSMsg                 OS_Smsg_Wait                //  smsg, os_smsg_type_var
#define OS_WaitSMsgTimeout          OS_Smsg_Wait_TO             //  smsg, os_smsg_type_var, timeout



/************************************************************************************************/
/*                                                                                              */
/*               Queues of pointers to messages                                                 */
/*                                                                                              */
/************************************************************************************************/

        /* Create */

#define OS_CreateQMsg               OS_Queue_Create             //  qmsg, buffer, size

        /* Checking */

#define OS_CheckQMsg                OS_Queue_Check              //  qmsg
#define OS_IsQMsgFull               OS_Queue_IsFull             //  qmsg
#define OS_IsQMsgFree               OS_Queue_IsFree             //  qsmsg

        /* Posting */

#define OS_SendQMsg                 OS_Queue_Send               //  qmsg, message
#define OS_SendQMsgTimeout          OS_Queue_Send_TO            //  qmsg, message, timeout
#define OS_SendQMsgNoWait           OS_Queue_Send_Now           //  qmsg, message
#define OS_IntSendQMsgNoWait        OS_Queue_Send_I             //  qmsg, message

        /* Waiting */

#define OS_WaitQMsg                 OS_Queue_Wait               //  qmsg, os_msg_type_var
#define OS_WaitQMsgTimeout          OS_Queue_Wait_TO            //  qmsg, os_msg_type_var, timeout



/************************************************************************************************/
/*                                                                                              */
/*               Queues of simple messages                                                      */
/*                                                                                              */
/************************************************************************************************/

        /* Create */

#define OS_CreateQSMsg              OS_SQueue_Create            //  qsmsg, buffer, size

        /* Checking */

#define OS_CheckQSMsg               OS_SQueue_Check             //  qsmsg
#define OS_IsQSMsgFull              OS_SQueue_IsFull            //  qsmsg
#define OS_IsQSMsgFree              OS_SQueue_IsFree            //  qsmsg

        /* Posting */

#define OS_SendQSMsg                OS_SQueue_Send              //  qsmsg, smessage
#define OS_SendQSMsgTimeout         OS_SQueue_Send_TO           //  qsmsg, smessage, timeout
#define OS_SendQSMsgNoWait          OS_SQueue_Send_Now          //  qsmsg, smessage
#define OS_IntSendQSMsgNoWait       OS_SQueue_Send_I            //  qsmsg, smessage

        /* Waiting */

#define OS_WaitQSMsg                OS_SQueue_Wait              //  qsmsg, os_smsg_type_var
#define OS_WaitQSMsgTimeout         OS_SQueue_Wait_TO           //  qsmsg, os_smsg_type_var, timeout



/************************************************************************************************/
/*                                                                                              */
/*               Flags                                                                          */
/*                                                                                              */
/************************************************************************************************/

        /* Create */

#define OS_CreateFlag               OS_Flag_Create              //  flags

        /* Setting */

#define OS_InitFlag                 OS_Flag_Init                //  flags, value
#define OS_SetFlag                  OS_Flag_Set_1               //  flags, mask
#define OS_ClearFlag                OS_Flag_Set_0               //  flags, mask

        /* Checking */

#define OS_CheckAllFlag             OS_Flag_Check_11            //  flags, mask
#define OS_CheckAnyFlag             OS_Flag_Check_1x            //  flags, mask
#define OS_CheckAllFlagClear        OS_Flag_Check_00            //  flags, mask
#define OS_CheckAnyFlagClear        OS_Flag_Check_0x            //  flags, mask

        /* Waiting */

#define OS_WaitAllFlag              OS_Flag_Wait_11             //  flags, mask
#define OS_WaitAnyFlag              OS_Flag_Wait_1x             //  flags, mask
#define OS_WaitAllFlagClear         OS_Flag_Wait_00             //  flags, mask
#define OS_WaitAnyFlagClear         OS_Flag_Wait_0x             //  flags, mask
#define OS_WaitAllFlagTimeout       OS_Flag_Wait_11_TO          //  flags, mask, timeout
#define OS_WaitAnyFlagTimeout       OS_Flag_Wait_1x_TO          //  flags, mask, timeout
#define OS_WaitAllFlagClearTimeout  OS_Flag_Wait_00_TO          //  flags, mask, timeout
#define OS_WaitAnyFlagClearTimeout  OS_Flag_Wait_0x_TO          //  flags, mask, timeout



/************************************************************************************************/
/*                                                                                              */
/*               Dynamic timers                                                                 */
/*                                                                                              */
/************************************************************************************************/

        /* Create/Delete */

#define OS_CreateDTimer             OS_Dtimer_Create            //  ptimer
#define OS_DeleteDTimer             OS_Dtimer_Delete            //  ptimer

        /* Run/stop/pause */

#define OS_RunDTimer                OS_Dtimer_Run               //  ptimer, newtime
#define OS_StopDTimer               OS_Dtimer_Stop              //  ptimer
#define OS_PauseDTimer              OS_Dtimer_Pause             //  ptimer
#define OS_ContinueDTimer           OS_Dtimer_Continue          //  ptimer
#define OS_BreakDTimer              OS_Dtimer_Break             //  ptimer

        /* Updating */

#define OS_UpdateDTimer             OS_Dtimer_Update            //  ptimer, time
#define OS_AddDTimer                OS_Dtimer_Add               //  ptimer, addtime

        /* Checking */

#define OS_GetDTimer                OS_Dtimer_Get               //  ptimer
#define OS_CheckDTimer              OS_Dtimer_Check             //  ptimer
#define OS_IsDTimerActive           OS_Dtimer_IsActive          //  ptimer
#define OS_IsDTimerStopped          OS_Dtimer_IsStopped         //  ptimer
#define OS_IsDTimerRun              OS_Dtimer_IsRun             //  ptimer

        /* Waiting */

#define OS_WaitDTimer               OS_Dtimer_Wait              //  ptimer
#define OS_DelayDTimer              OS_Dtimer_Delay             //  ptimer, delay




#define OS_CheckPTimer      OS_Dtimer_Check
#define OS_IsPTimerActive   OS_Dtimer_IsActive
#define OS_IsPTimerStopped  OS_Dtimer_IsStopped
#define OS_IsPTimerRun      OS_Dtimer_IsRun
#define OS_GetPTimer        OS_Dtimer_Get
#define OS_StopPTimer       OS_Dtimer_Stop
#define OS_PausePTimer      OS_Dtimer_Pause
#define OS_ContinuePTimer   OS_Dtimer_Continue
#define OS_BreakPTimer      OS_Dtimer_Break
#define OS_RunPTimer        OS_Dtimer_Run
#define OS_UpdatePTimer     OS_Dtimer_Update
#define OS_AddPTimer        OS_Dtimer_Add
#define OS_WaitPTimer       OS_Dtimer_Wait
#define OS_DelayPTimer      OS_Dtimer_Delay
#define OS_CreatePTimer     OS_Dtimer_Create
#define OS_DeletePTimer     OS_Dtimer_Delete
#define OST_PTIMER          OST_DTIMER


/************************************************************************************************/
/*                                                                                              */
/*               Static timers                                                                  */
/*                                                                                              */
/************************************************************************************************/

        /* Run/stop/pause */

#define OS_RunSTimer                OS_Stimer_Run               //  stimer_id, time
#define OS_StopSTimer               OS_Stimer_Stop              //  stimer_id
#define OS_PauseSTimer              OS_Stimer_Pause             //  stimer_id
#define OS_ContinueSTimer           OS_Stimer_Continue          //  stimer_id

        /* Checking */

#define OS_GetSTimer                OS_Stimer_Get               //  stimer_id
#define OS_CheckSTimer              OS_Stimer_Check             //  stimer_id
#define OS_IsSTimerRun              OS_Stimer_IsRun             //  stimer_id

        /* Waiting  */

#define OS_WaitSTimer               OS_Stimer_Wait              //  stimer_id
#define OS_DelaySTimer              OS_Stimer_Delay             //  stimer_id, delay



/************************************************************************************************/
/*                                                                                              */
/*               Old style static timers                                                        */
/*                                                                                              */
/************************************************************************************************/

        /* Run */

#define OS_RunTimer8                OS_Oldtimer_Run8            //  timer8_id, time
#define OS_RunTimer16               OS_Oldtimer_Run16           //  timer16_id, time
#define OS_RunTimer24               OS_Oldtimer_Run24           //  timer24_id, time
#define OS_RunTimer32               OS_Oldtimer_Run32           //  timer32_id, time

        /* Stop */

#define OS_StopTimer                OS_Oldtimer_Pause           //  timer_id

        /* Checking */

#define OS_CheckTimer               OS_Oldtimer_Check           //  timer_id

        /* Waiting */

#define OS_WaitTimer                OS_Oldtimer_Wait            //  timer_id



#endif

//******************************************************************************
//  END OF FILE osa_oldnames.h
//******************************************************************************

