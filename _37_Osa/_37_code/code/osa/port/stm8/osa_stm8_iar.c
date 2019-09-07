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
 *  File:           osa_stm8_iar.c
 *
 *  Compilers:      IAR
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    IAR for STM8 specific functions definition
 *                  This file directly included in osa.c
 *
 *  History:        13.09.2010 -    File updated
 *
 ************************************************************************************************
 */


//#error "OSA error #26: IAR for STM8 is not supported by OSA!"


#define _OS_INIT_PROC_SPEC()      // Empty macro for mikroC


OS_RAM_NEAR OST_UINT16 _OS_SP_Temp;

#pragma optimize=no_inline

char aaa (char a)
{
    return a+a*2;
}


#pragma optimize=no_inline
void _OS_JumpToTask (void)
{
    _OS_STORE_SP();
    _OS_SET_FSR_CUR_TASK();
    _OS_Restore_TaskSP();
    _OS_Restore_Btemp();
    _OS_SET_PC();/**/
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

#pragma optimize=no_inline

void _OS_ReturnSave (void)
{
    _OS_SET_FSR_CUR_TASK();
    _OS_SAVE_PC();
    _OS_Store_Btemp();
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

#pragma optimize=no_inline
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

#pragma optimize=no_inline
void _OS_EnterWaitMode (void)
{
    _OS_Temp = 0;
    //_OS_ClrReadySetClrCanContinue();
    asm(" jp _OS_ClrReadySetClrCanContinue ");
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

#pragma optimize=no_inline
void _OS_EnterWaitModeTO (void)
{
    _OS_Temp = 0xFF;
    //_OS_ClrReadySetClrCanContinue();
    asm(" jp _OS_ClrReadySetClrCanContinue ");
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

#pragma optimize=no_inline
void _OS_ClrReadySetClrCanContinue (void)
{
    _OS_SET_FSR_CUR_TASK();
    _OS_SAVE_PC();
    _OS_PUSH();
    _OS_Store_Btemp();
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
    asm("  push    CC  ");
    asm("  sim         ");
    asm("  pop     A   ");
    asm("  ret         ");
    return 0;
}


#if defined(OS_ENABLE_INT)

void _OS_DI_INT (void)
{
    asm("  push    CC          ");
    asm("  sim                 ");
    asm("  pop     _OS_Temp_I  ");
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
    asm("  push    A       ");
    asm("  pop     CC      ");
}


#if defined(OS_ENABLE_INT)

void _OS_RI_INT (void)
{
    asm("  push    _OS_Temp_I  ");
    asm("  pop     CC          ");
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

#pragma optimize=no_inline
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
                _OS_ATOMIC_WRITE_A(_OS_CurTask->State.bDelay = 0);// Clear bDelay in task descriptor

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
                    _OS_ATOMIC_WRITE_A(_OS_CurTask->State.bDelay = 0);// Clear bDelay in task descriptor

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













/*
 *******************************************************************************
 *
 *  void _OS_Dtimer_Create (OST_DTIMER *dtimer)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    (Internal function called by system kernel througth
 *                  service OS_Dtimer_Create)
 *
 *                  Add dynamic timer into list of active dynamic timers. This
 *                  timer will be added at first position in list.
 *
 *
 *  parameters:     dtimer - pointer to dynamic timer.
 *                  (for PICC16 can be allocated only in OS_RAM_BANK0 and OS_RAM_BANK1)
 *
 *  on return:      none
 * 
 *  This functions is overloads osa_dtimer.c: the reason is that IAR for STM8 can
 *  not perform structure assignement (see commented text)
 * 
 *******************************************************************************/

#ifdef  OS_ENABLE_DTIMERS
//------------------------------------------------------------------------------
#define _OS_Dtimer_Create_DEFINED
//------------------------------------------------------------------------------

void _OS_Dtimer_Create (OST_DTIMER *dtimer)
{
    _OS_Temp = OS_DI();

    dtimer->Next = _OS_DTimers.Next;
    dtimer->Timer = 0;

    // Perform "dtimer->Flags = _OS_DTimers.Flags;":
    *(unsigned char*)dtimer = *(unsigned char*)&_OS_DTimers.Flags;

    _OS_DTimers.Next = (OST_DTIMER*)dtimer;
    OS_DTIMER_ATOMIC_WRITE_A(_OS_DTimers.Flags.bNextEnable = 1);

    OS_RI(_OS_Temp);
}

#endif  // OS_ENABLE_DTIMERS

//******************************************************************************
//  END OF FILE osa_stm8_iar.c
//******************************************************************************

