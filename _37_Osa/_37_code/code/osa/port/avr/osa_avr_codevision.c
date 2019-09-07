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
 *  File:           osa_avr_codevision.c
 *
 *  Compilers:      CodeVision AVR
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    CodeVision AVR specific functions definition
 *                  This file directly included in osa.c
 *
 *  History:        08.10.2010 -    File updated
 *
 ************************************************************************************************
 */

#define _OS_INIT_PROC_SPEC()                \
    _OS_Temp = 0;                           \
    if (_OS_Temp)                           \
    {                                       \
        _OS_JumpToTask();                   \
        _OS_ReturnNoSave();                 \
        _OS_ReturnNoSave();                 \
        _OS_EnterWaitMode();                \
        _OS_EnterWaitModeTO();              \
        _OS_ClrReadySetClrCanContinue();    \
    }
    



void _OS_ClrReadySetClrCanContinue (void);


/************************************************************************************************
 *                                                                                              *
 *                           K E R N E L   F U N C T I O N S                                    *
 *                                                                                              *
 ************************************************************************************************/


//******************************************************************************

void _OS_JumpToTask (void)
{
    _OS_PUSH_Y();
    _OS_SET_FSR_CUR_TASK();
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
 *                  Called througth "jmp" or "rjmp"
 *
 *  parameters:     none
 *
 *  on return:      none
 *
 ********************************************************************************
 */

void _OS_ReturnSave (void)
{
    _OS_bTaskReady = 1;
    _OS_SET_FSR_CUR_TASK();
    _OS_SAVE_PC();
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
    //asm("   ldi     r24, 0    ");
    //_OS_JMP("_OS_ClrReadySetClrCanContinue");
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
    //asm("   ldi    r24, 1     ");
    //_OS_JMP("_OS_ClrReadySetClrCanContinue");
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
    /*
    asm("   lds     r22, _OS_State       ");
    asm("   cbr     r22, (1<<3)         ");     // _OS_bTaskReady = 0

    #if defined(OS_ENABLE_TTIMERS)
        asm("   sbrs    r22, 4          ");     // if (!_OS_bTaskDelay) _OS_bTaskCanContinue = 0;
        asm("   cbr     r22, (1<<5)     ");
        asm("   sbrc    r24, 0          ");     // if (_OS_Temp & 0x01) _OS_bTaskCanContinue = 1;
        asm("   sbr     r22, (1<<5)     ");
    #endif

    asm("   sts     _OS_State, r22       ");

    _OS_Flags.bEventError = 0;

    _OS_SET_FSR_CUR_TASK();
    _OS_SAVE_PC();
    _OS_PUSH_BACK();
    asm(" ret ");/***/
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
 *  on return:      OST_UINT - current SREG value
 *
 ********************************************************************************
 */

//-------------------------------------------------------------------------------
#define OS_DI_DEFINED
//-------------------------------------------------------------------------------

OST_UINT OS_DI (void)
{
    char temp;
//    temp = SREG;
//    #asm("cli");
//    return temp;
}

#if defined(OS_ENABLE_INT)

void __OS_DI_INT (void)
{
//    #asm("   in      r22, 0x3F       ");
//    #asm("   sts     _OS_Temp_I, r22 ");
//    #asm("   cli                     ");
//    #asm("   ret                     ");
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

void OS_RI (OST_UINT temp)
{
//    SREG = temp;
}

#if defined(OS_ENABLE_INT)

void __OS_RI_INT (void)
{
//    #asm("   lds     r22, _OS_Temp_I ");
//    #asm("   out     0x3F, r22       ");
//    #asm("   ret                     ");
}

#endif






//******************************************************************************
//  END OF FILE osa_avr_codevision.c
//******************************************************************************

