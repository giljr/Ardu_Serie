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
 *  File:           osa_pic16e_htpicc_pro.c
 *
 *  Compilers:      HT-PICC PRO
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    HTPICC PRO for PIC16F1xxx specific functions definition
 *                  This file directly included in osa.c
 *
 *  History:        13.09.2010 -    File updated
 *
 ************************************************************************************************
 */





#define _OS_INIT_PROC_SPEC()                    \
    if (_pclath & 0x80)                         \
    {                                           \
        _OS_ReturnSave();                       \
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

void _OS_ClrReadySetClrCanContinue (void);

void _OS_JumpToTask (void)
{
//    _OS_SET_FSR_CUR_TASK_W(_OS_TASK_POINTER_OFFSET + 1);
    _OS_SET_FSR_CUR_TASK_W(1 + 1);
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

void _OS_ReturnSave (void)
{
//    _OS_SET_FSR_CUR_TASK_W(_OS_TASK_POINTER_OFFSET + 1);
    _OS_SET_FSR_CUR_TASK_W(1 + 1);
    _OS_SAVE_PC();
    _OS_bTaskReady = 1;
    _PIC16E_POP();
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
    _OS_ClrReadySetClrCanContinue();
    NOP();  // To prevent calling function through GOTO instruction (see previous line)
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
    _OS_ClrReadySetClrCanContinue();
    NOP();  // To prevent calling function through GOTO instruction (see previous line)
}


/*
 ********************************************************************************
 *
 *  void _OS_ClrReadySetClrCanContinue (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Addition function. Called only from:
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
    _PIC16E_POP();

    _OS_Flags.bEventError = 0;

//    _OS_SET_FSR_CUR_TASK_W(_OS_TASK_POINTER_OFFSET + 1);
    _OS_SET_FSR_CUR_TASK_W(1 + 1);
    _OS_SAVE_PC();

    _OS_bTaskReady = 0;

    #if defined(OS_ENABLE_TTIMERS)

        if (!_OS_bTaskDelay) _OS_bTaskCanContinue = 0;
        if (_OS_Temp & 0x01) _OS_bTaskCanContinue = 1;

    #endif
}







/*
 ************************************************************************************************
 *                                                                                              *
 *                         C R I T I C A L   S E C T I O N S                                    *
 *                                                                                              *
 ************************************************************************************************
 */

//------------------------------------------------------------------------------
#ifdef OS_ENABLE_CRITICAL_SECTION
//------------------------------------------------------------------------------


/*
 ********************************************************************************
 *
 *  void OS_EnterCriticalSection (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Enter critical section.
 *                  This function disables interrupts (with saving current
 *                  state) and sets system flag bInCriticalSection
 *
 *  parameters:     none
 *
 *  on return:      none
 *
 *  note:           This function overloads "osa.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define OS_EnterCriticalSection_DEFINED
//------------------------------------------------------------------------------

void OS_EnterCriticalSection (void)
{
    OST_UINT temp;
    temp = OS_DI();
    _OS_Flags.bInCriticalSection = 1;

    _OS_Flags.bGIE_CTemp = 0;
    if (temp & 0x80) _OS_Flags.bGIE_CTemp = 1;

}



/*
 ********************************************************************************
 *
 *   void OS_LeaveCriticalSection (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Leave critical section.
 *                  This function restore interrupt state from OS_Flag temp bits
 *                  and clears system flag bInCriticalSection
 *
 *  parameters:     none
 *
 *  on return:      none
 *
 *  note:           This function overloads "osa.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define OS_LeaveCriticalSection_DEFINED
//------------------------------------------------------------------------------

void OS_LeaveCriticalSection (void)
{
    char temp;
    _OS_Flags.bInCriticalSection = 0;
    temp = 0;
    if (_OS_Flags.bGIE_CTemp) temp |= 0x80;
    OS_RI(temp);
}

//------------------------------------------------------------------------------
#endif      //     #ifdef OS_ENABLE_CRITICAL_SECTION
//------------------------------------------------------------------------------














/************************************************************************************************
 *                                                                                              *
 *                            S Y S T E M   F U N C T I O N S                                   *
 *                                                                                              *
 ************************************************************************************************/







/*
 ********************************************************************************
 *
 *  OST_UINT8 OS_DI (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Disable interrupt with GIE saving
 *
 *  parameters:     none
 *
 *  on return:      char - bit 7 = previous GIE value, bits 0..6 = 0
 *
 ********************************************************************************
 */

//-------------------------------------------------------------------------------
#define OS_DI_DEFINED
//-------------------------------------------------------------------------------

OST_UINT8 OS_DI (void)
{
    char temp;
    temp = _intcon & 0x80;
    _intcon &= 0x7F;
    return temp;
}


//------------------------------------------------------------------------------
#ifdef OS_ENABLE_INT
//------------------------------------------------------------------------------

void _OS_DI_INT (void)
{
    _OS_Temp_I = _intcon & 0x80;
    _intcon &= 0x7F;
}

void _OS_RI_INT (void)
{
    if (_OS_Temp_I & 0x80) _intcon |= 0x80;
}


//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------









/*
 ********************************************************************************
 *
 *  OST_BOOL _OS_CheckEvent (char bEvent)
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

void _OS_CheckEvent (OST_UINT bEvent)
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

        _PIC16E_POP();

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

        _PIC16E_POP();

    //------------------------------------------------------------------------------
    #endif
    //------------------------------------------------------------------------------

}






//******************************************************************************
//  END OF FILE osa_pic16e_htpicc_pro.c
//******************************************************************************

