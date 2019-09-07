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
 *  File:           osa_stm8_cosmic.c
 *
 *  Compilers:      Cosmic
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    Cosmic for STM8 specific functions definition
 *                  This file directly included in osa.c
 *
 *  History:        13.09.2010 -    File updated
 *
 ************************************************************************************************
 */


#define _OS_INIT_PROC_SPEC()      // Empty for STM8


OS_RAM_NEAR OST_UINT16 _OS_SP_Temp;



void _OS_ClrReadySetClrCanContinue (void);


#define _OS_CURTASK_STATE_DELAY_CLEAR()     \
    _OS_SET_FSR_CUR_TASK();                 \
    _asm("  ld      A, (Y)      ");         \
    _asm("  and     A, #0xEF    ");         \
    _asm("  ld      (Y), A      ");         \

#define _OS_CURTASK_STATE_DELAY_READY_SET() \
    _OS_SET_FSR_CUR_TASK();                 \
    _asm("  ld      A, (Y)      ");         \
    _asm("  or      A, #0x18    ");         \
    _asm("  ld      (Y), A      ");         \




void _OS_JumpToTask (void)
{
    _OS_STORE_SP();
    _OS_SET_FSR_CUR_TASK();
    _OS_Restore_TaskSP();
    _OS_SET_PC();
}


/************************************************************************************************
 *                                                                                              *
 *                           K E R N E L   F U N C T I O N S                                    *
 *                                                                                              *
 ************************************************************************************************/



/*
 ********************************************************************************
 *
 *  void _OS_ReturnSave (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Return to sheduler with saving current task return point
 *
 *  parameters:     none
 *
 *  on return:      none
 *
 ********************************************************************************
 */

void _OS_ReturnSave (void)
{
    _OS_SET_FSR_CUR_TASK();
    _OS_SAVE_PC();
    _OS_Store_TaskSP();
    _OS_bTaskReady = 1;
    _OS_RESTORE_SP();
}



/*
 ********************************************************************************
 *
 *  void _OS_ReturnNoSave (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Return to scheduler without saving context
 *
 *  parameters:     none
 *
 *  on return:      none
 *
 ********************************************************************************
 */

void _OS_ReturnNoSave (void)
{
    _OS_POP();
    _OS_RESTORE_SP();
}


/*
 ********************************************************************************
 *
 *  void __OS_EnterWaitMode (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Clear bReady flag and return to the sheduler with seaving
 *                  task return point
 *
 *  parameters:     none
 *
 *  on return:      none
 *
 *
 ********************************************************************************
 */

void _OS_EnterWaitMode (void)
{
    _OS_Temp = 0;
//    _OS_ClrReadySetClrCanContinue();
    _asm(" jp    __OS_ClrReadySetClrCanContinue   ");
}


/*
 ********************************************************************************
 *
 *  void __OS_EnterWaitModeTO (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Clear bReady flag, set bCanContinue flag and return to the
 *                  sheduler with seaving task return point
 *
 *  parameters:     none
 *
 *  on return:      none
 *
 *
 ********************************************************************************
 */

void _OS_EnterWaitModeTO (void)
{
    _OS_Temp = 0xFF;
//    _OS_ClrReadySetClrCanContinue();
    _asm(" jp    __OS_ClrReadySetClrCanContinue   ");
}


/*
 ********************************************************************************
 *
 *  void _OS_ClrReadySetClrCanContinue (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Addition function. Called only by GOTO from:
 *                      _OS_EnterWaitMode()
 *                      _OS_EnterWaitModeTO()
 *                  Stack has pointer of task who called theese services.
 *
 *  parameters:     none
 *
 *  on return:      none
 *
 ********************************************************************************
 */

void _OS_ClrReadySetClrCanContinue (void)
{
    _OS_SET_FSR_CUR_TASK();
    _OS_SAVE_PC();
    _OS_PUSH();
    _OS_Store_TaskSP2();

    _OS_Flags.bEventError = 0;
    _OS_bTaskReady = 0;

    #if defined(OS_ENABLE_TTIMERS)
        if (!_OS_bTaskDelay) _OS_bTaskCanContinue = 0;
        if (_OS_Temp & 0x01) _OS_bTaskCanContinue = 1;
    #endif

}



















/************************************************************************************************
 *                                                                                              *
 *                            S Y S T E M   F U N C T I O N S                                   *
 *                                                                                              *
 ************************************************************************************************/



//------------------------------------------------------------------------------
#ifdef OS_ENABLE_TTIMERS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#define _OS_InitDelay_DEFINED
//------------------------------------------------------------------------------

void _OS_InitDelay (OS_TTIMER_TYPE Delay)
{

    _OS_TASK_ATOMIC_WRITE_A(
        _OS_bTaskCanContinue = 0;
        _OS_CURTASK_STATE_DELAY_CLEAR();
    );

    if (Delay)
    {
        Delay ^= (OS_TTIMER_TYPE)-1;
        Delay ++;
        _OS_CurTask->Timer = Delay;
        _OS_TASK_ATOMIC_WRITE_A(_OS_CURTASK_STATE_DELAY_READY_SET());
        #if defined(_OS_CUR_FLAGS_IN_OS_STATE)
            _OS_bTaskDelay = 1;
            _OS_bTaskReady = 1;
        #endif
    }
}

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------




/*
 ********************************************************************************
 *
 *  char OS_DI (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Disable interrupt with GIE saving
 *
 *  parameters:     none
 *
 *  on return:      char - bit 7 = previous GIEH value,
 *                         bit 6 = previous GIEL value
 *                         bits 0..5 = 0
 *
 ********************************************************************************
 */

//-------------------------------------------------------------------------------
#define OS_DI_DEFINED
//-------------------------------------------------------------------------------

char OS_DI (void)
{
    _asm("  push    CC  ");
    _asm("  sim         ");
    _asm("  pop     A   ");
}


#if defined(OS_ENABLE_INT)

void _OS_DI_INT (void)
{
    _asm("  push    CC          ");
    _asm("  sim                 ");
    _asm("  pop     __OS_Temp_I ");
}

#endif


/*
 ********************************************************************************
 *
 *  char OS_RI (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Restore saved by OS_DI() GIEx states
 *
 *  parameters:     char - bit 7 = previos GIE value, bits 0..6 = 0
 *
 *  on return:      none
 *
 ********************************************************************************
 */

//-------------------------------------------------------------------------------
#define OS_RI_DEFINED
//-------------------------------------------------------------------------------

void OS_RI (char temp)
{
    _asm("  push    A       ");
    _asm("  pop     CC      ");
}


#if defined(OS_ENABLE_INT)

void _OS_RI_INT (void)
{
    _asm("  push    __OS_Temp_I ");
    _asm("  pop     CC          ");
}

#endif






//------------------------------------------------------------------------------
#define OS_EnterCriticalSection_DEFINED
//------------------------------------------------------------------------------

void OS_EnterCriticalSection (void)
{
    OST_UINT temp;

    temp = OS_DI();
    _OS_Flags.bInCriticalSection = 1;

    _OS_Flags.bI0_CTemp = 0;
    if (temp & 0x08) _OS_Flags.bI0_CTemp = 1;

    _OS_Flags.bI1_CTemp = 0;
    if (temp & 0x20) _OS_Flags.bI1_CTemp = 1;
}



//------------------------------------------------------------------------------
#define OS_LeaveCriticalSection_DEFINED
//------------------------------------------------------------------------------

void OS_LeaveCriticalSection (void)
{
    char temp;
    _OS_Flags.bInCriticalSection = 0;
    temp = 0;
    if (_OS_Flags.bI0_CTemp) temp |= 0x20;
    if (_OS_Flags.bI1_CTemp) temp |= 0x08;
    OS_RI(temp);
}




/*
 ********************************************************************************
 *
 *  void _OS_CheckEvent (char bEvent)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    (Internal function called by system kernel througth
 *                  waiting services)
 *
 *                  Check condition of bEvent. Accordint to bEvent value and
 *                  current state of bReady, bDelay and bCanContinue flags,
 *                  task becames ready to execute or still remains in waiting
 *                  mode.
 *
 *  parameters:     bEvent - zero, or non-zero condition
 *
 *  on return:      none
 *
 *  note:           This function overloads "osa.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define _OS_CheckEvent_DEFINED
//------------------------------------------------------------------------------

void _OS_CheckEvent (OST_UINT bEvent)
{
    //------------------------------------------------------------------------------
    #if OS_PRIORITY_LEVEL == OS_PRIORITY_DISABLED
    //------------------------------------------------------------------------------

        _OS_Flags.bTimeout = 0;

        if (bEvent)
        {
            #ifdef OS_ENABLE_TTIMERS

                _OS_bTaskDelay = 0;             // Clear bDelay in temporary variable _OS_State
                _OS_ATOMIC_WRITE_A(_OS_CURTASK_STATE_DELAY_CLEAR());

            #endif

            return;

        }

        #ifdef OS_ENABLE_TTIMERS

            if (_OS_bTaskTimeout && _OS_bTaskCanContinue)
            {
                _OS_Flags.bTimeout = 1;

                return;
            }

        #endif

        _OS_POP();
        _OS_RESTORE_SP();

    //------------------------------------------------------------------------------
    #else
    //------------------------------------------------------------------------------

        _OS_Flags.bTimeout = 0;

        if (bEvent)
        {
            if (_OS_bTaskReady) {

                #ifdef OS_ENABLE_TTIMERS

                    _OS_bTaskDelay = 0; // Clear bDelay in temporary variable _OS_State
                    _OS_ATOMIC_WRITE_A(_OS_CURTASK_STATE_DELAY_CLEAR());

                #endif

                #if OS_PRIORITY_LEVEL == OS_PRIORITY_EXTENDED
                _OS_Flags.bEventOK = 1;
                #endif

                return;
            }

            _OS_bTaskReady = 1;

        } else {
            _OS_bTaskReady = 0;
        }

        #ifdef OS_ENABLE_TTIMERS

            if (_OS_bTaskTimeout && _OS_bTaskCanContinue)
            {
                _OS_bTaskReady = 1;
                _OS_Flags.bTimeout = 1;

                #if OS_PRIORITY_LEVEL == OS_PRIORITY_EXTENDED
                _OS_Flags.bEventOK = 1;
                #endif

                return;
            }

        #endif

        _OS_POP();
        _OS_RESTORE_SP();

    //------------------------------------------------------------------------------
    #endif
    //------------------------------------------------------------------------------

}



//******************************************************************************
//  END OF FILE osa_stm8_cosmic.c
//******************************************************************************

