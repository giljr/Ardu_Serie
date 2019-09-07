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
 *  File:           osa_pic18_ccs.c
 *
 *  Compilers:      CCS
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    CCS for PIC18 specific functions definition
 *                  This file directly included in osa.c
 *
 *  History:        13.09.2010 -    File updated
 *
 ************************************************************************************************
 */





//******************************************************************************
//
//  _OS_INIT_PROC_SPEC.
//
//******************************************************************************
//
//  Specific macro for CCS18 compiler:
//
//  The problem is that system functions that operate with stack can be
//  called through 'goto' operator in case when there is only one call of
//  function. To avoid it we call all these functions once to be sure that
//  in case of they will be used in program, compilser will call them by 'call',
//  not 'goto'.
//
//******************************************************************************


#define _OS_INIT_PROC_SPEC()                      \
    _OS_Temp = 0;                               \
    if (_OS_Temp & 1)                           \
    {                                           \
        _OS_ReturnSave();                       \
        _OS_ReturnNoSave();                     \
        _OS_EnterWaitMode();                    \
        _OS_EnterWaitModeTO();                  \
        _OS_CheckEvent(0);                      \
        _OS_JumpToTask();                       \
    }



/************************************************************************************************
 *                                                                                              *
 *                           K E R N E L   F U N C T I O N S                                    *
 *                                                                                              *
 ************************************************************************************************/


#separate void _OS_ClrReadySetClrCanContinue (void);



#separate
void _OS_JumpToTask (void)
{
    _OS_SET_FSR_CUR_TASK();
    (OS_RAM_NEAR char)_OS_State = _postinc;
    _OS_SET_PC();
}

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

#separate void _OS_ReturnSave (void)
{
    _OS_SET_FSR_CUR_TASK_W(OS_CODE_POINTER_SIZE + _OS_TASK_POINTER_OFFSET - 1);
    _OS_SAVE_PC();
    _OS_bTaskReady = 1;
    _OS_POP();
}

/*
 ********************************************************************************
 *
 *  void _OS_ReturnSave (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Return to sheduler without saving current task return point
 *
 *  parameters:     none
 *
 *  on return:      none
 *
 ********************************************************************************
 */

#separate void _OS_ReturnNoSave (void)
{
    _OS_POP();
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

#separate void _OS_EnterWaitMode (void)
{
    _OS_Temp = 0;
    _OS_ClrReadySetClrCanContinue();
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

#separate void _OS_EnterWaitModeTO (void)
{
    _OS_Temp = 0xFF;
    _OS_ClrReadySetClrCanContinue();
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

#separate void _OS_ClrReadySetClrCanContinue (void)
{
    _OS_POP();

    _OS_Flags.bEventError = 0;

    _OS_SET_FSR_CUR_TASK_W(OS_CODE_POINTER_SIZE + _OS_TASK_POINTER_OFFSET - 1);
    _OS_SAVE_PC();

    _OS_bTaskReady = 0;

    #if defined(OS_ENABLE_TTIMERS)

        if (!_OS_bTaskDelay) _OS_bTaskCanContinue = 0;
        if (_OS_Temp & 0x01) _OS_bTaskCanContinue = 1;

    #endif

    _OS_POP();
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
    char temp;
    temp = _intcon & 0xC0;
    _gie = 0;
    if (_ipen) _giel = 0;
    return temp;
}

//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT)
//------------------------------------------------------------------------------

    void _OS_DI_INT (void)
    {
        _OS_Temp_I = _intcon & 0xC0;
        _gie = 0;
        if (_ipen) _giel = 0;
    }

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------


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
    if (temp & 0x80) _gie = 1;
    if (_ipen)
    {
        if (temp & 0x40) _giel = 1;
    }
}


//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT)
//------------------------------------------------------------------------------

    void _OS_RI_INT (void)
    {
        if (_OS_Temp_I & 0x80) _gie = 1;
        if (_ipen)
        {
            if (_OS_Temp_I & 0x40) _giel = 1;
        }
    }

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------






















/*
 ************************************************************************************************
 *                                                                                              *
 *                                   D E L A Y S   I N   T A S K                                *
 *                                                                                              *
 ************************************************************************************************
 */

//------------------------------------------------------------------------------
#ifdef OS_ENABLE_TTIMERS
//------------------------------------------------------------------------------

/*
 ********************************************************************************
 *
 *   void _OS_InitDelay (OS_TTIMER_TYPE Delay)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    (Internal function called by system kernel)
 *
 *                  Init task timer delay.Set timer variable in task descriptor,
 *                  sets bit bDelay and clears bit bCanContinue.
 *
 *                  _OS_CurTask must point to descriptor.
 *
 *
 *  parameters:     Delay   - time of delay in system ticks
 *
 *  on return:      none
 *
 *  note:           This function overloads "osa.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define _OS_InitDelay_DEFINED
//------------------------------------------------------------------------------

void _OS_InitDelay (OS_TTIMER_TYPE Delay)
{

    _OS_bTaskCanContinue = 0;
    _OS_bTaskDelay = 0;         // Clear bDelay to prevent timer changing in interrupt
    _OS_SET_FSR_CUR_TASK();
    _indf = (OS_RAM_NEAR char)_OS_State;

    if (Delay)
    {

        Delay ^= -1;
        Delay ++;

        _OS_CurTask->Timer = Delay;

        _OS_bTaskDelay = 1;
        _OS_bTaskReady = 1;
        _OS_SET_FSR_CUR_TASK();
        _indf = (OS_RAM_NEAR char)_OS_State;

    }
}

//______________________________________________________________________________
//******************************************************************************

#endif  // #ifdef OS_TIMER











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
 *  on return:      1 - when condition is true, or timeout occured
 *                  0 - condition is false and no timeout
 *
 *  note:           This function overloads "osa.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define _OS_CheckEvent_DEFINED
//------------------------------------------------------------------------------

#separate void _OS_CheckEvent (OST_UINT bEvent)
{
    //------------------------------------------------------------------------------
    #if OS_PRIORITY_LEVEL == OS_PRIORITY_DISABLED
    //------------------------------------------------------------------------------

        _OS_Flags.bTimeout = 0;
        _OS_SET_FSR_CUR_TASK();

        if (bEvent)
        {
            #ifdef OS_ENABLE_TTIMERS

                _OS_bTaskDelay = 0; // Clear bDelay in temporary variable _OS_State
                _indf &= ~0x10;     // Clear bDelay in task descriptor

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

    //------------------------------------------------------------------------------
    #else
    //------------------------------------------------------------------------------

        _OS_Flags.bTimeout = 0;
        _OS_SET_FSR_CUR_TASK();

        if (bEvent)
        {
            if (_OS_bTaskReady)
            {
                #ifdef OS_ENABLE_TTIMERS

                    _OS_bTaskDelay = 0; // Clear bDelay in temporary variable _OS_State
                    _indf &= ~0x10;     // Clear bDelay in task descriptor

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

    //------------------------------------------------------------------------------
    #endif
    //------------------------------------------------------------------------------

}





/************************************************************************************************
 *                                                                                              *
 *                     W O R K   W I T H   D Y N A M I C   T I M E R S                          *
 *                                                                                              *
 ************************************************************************************************/

//-----------------------------------------------------------------
#ifdef OS_ENABLE_DTIMERS
//-----------------------------------------------------------------


/*
 ********************************************************************************
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
 *
 *  on return:      none
 *
 *  note:           This function overloads "osa.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define _OS_Dtimer_Create_DEFINED
//------------------------------------------------------------------------------

void _OS_Dtimer_Create (OST_DTIMER *dtimer)
{
    _OS_Temp = OS_DI();

    _fsr = (OS_FSR_TYPE) dtimer;
    _postinc = (OS_BANK char)_OS_DTimers.Flags;
    // Here: bActive is set,
    //       bTimeout is cleared,
    //       bRun is cleared
    //       bNextEnable is copied

    _postinc = _OS_DTimers.Next & 0xFF;
    _postinc = _OS_DTimers.Next >> 8;

    _postinc = 0;

    #if  OS_DTIMER_SIZE >= 2
    _postinc = 0;
    #endif
    #if  OS_DTIMER_SIZE == 4
    _postinc = 0;
    _postinc = 0;
    #endif

    _OS_DTimers.Next = (OST_DTIMER*)dtimer;
    _OS_DTimers.Flags.bNextEnable = 1;

    OS_RI(_OS_Temp);
}




/*
 ********************************************************************************
 *
 *  void _OS_Dtimer_Delete (OST_DTIMER *dtimer)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    (Internal function called by system kernel througth
 *                  service OS_Dtimer_Delete)
 *
 *                  Delete dynamic timer from list of active timers. It is
 *                  recommended to delete all unused dynamic timers to avoid
 *                  speed down.
 *
 *  parameters:     dtimer - pointer to deleting dynamic timer
 *
 *  on return:      none
 *
 *  note:           This function overloads "osa.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define _OS_Dtimer_Delete_DEFINED
//------------------------------------------------------------------------------

void _OS_Dtimer_Delete (OST_DTIMER *dtimer)
{
    char ptcb0, ptcb1;

    char flags;

    _fsr = (OS_FSR_TYPE)dtimer;

    _OS_IndfTimerActive = 0;

    flags = _postinc;
    ptcb0 = _postinc;
    ptcb1 = _postinc;

    _fsr = (OS_FSR_TYPE)&_OS_DTimers;
REPEAT:
    if (_OS_PostincTimerNextEnable)
    {
        if (_indf == ((char)dtimer))
        {
            if (_preinc == ((int16)dtimer)>>8){
                _OS_Temp = OS_DI();
                _postdec = ptcb1;
                _postdec = ptcb0;

                if (!(flags & 0x8)) _OS_IndfTimerNextEnable = 0;   // Check for bNextEnable
                // If we check in other way (for example: !((OST_DTIMER_FLAGS*)&flags)->bNextEnable)
                // then compiler can use FSR0 (for some compilers: MCC18 e.g.),  as a result
                // current pointer to dtimer will be lost
                OS_RI(_OS_Temp);
                return;
            }
            _fsr--;
        }
        _OS_Temp = _postinc;
        _indf = _postinc;
        _fsrl = _OS_Temp;

        goto REPEAT;
    }

}




#endif  //  OS_ENABLE_DTIMERS








//******************************************************************************
//  END OF FILE osa_pic18_ccs.c
//******************************************************************************

