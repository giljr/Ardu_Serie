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
 *  File:           osa_pic18_mikroc.c
 *
 *  Compilers:      MikroC PRO
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    MikroC PRO for PIC18 specific functions definition
 *                  This file directly included in osa.c
 *
 *  History:        13.09.2010 -    File updated
 * 
 *                  06.03.2011 -    _OS_JumpToTask: case-changed 'w'->'W'
 *
 ************************************************************************************************
 */



#define _OS_INIT_PROC_SPEC()      // Empty macro for mikroC


volatile unsigned int _fsr      absolute 0xFE9;
volatile char         _indf     absolute 0xFEF;
volatile char         _postinc  absolute 0xFEE;
volatile char         _postdec  absolute 0xFED;
volatile char         _preinc   absolute 0xFEC;

volatile unsigned char _fsr1l   absolute 0xFE1;

volatile char         _pcl      absolute 0xFF9;
volatile char         _pclath   absolute 0xFFA;
volatile char         _pclatu   absolute 0xFFB;
volatile char         _status   absolute 0xFD8;
volatile char         _tosl     absolute 0xFFD;
volatile char         _tosh     absolute 0xFFE;
volatile char         _tosu     absolute 0xFFF;
volatile char         _bsr      absolute 0xFE0;
volatile char         _wreg     absolute 0xFE8;
volatile char         _intcon   absolute 0xFF2;
volatile char         _rcon     absolute 0xFD0;
volatile char         _stkptr   absolute 0xFFC;










void _OS_JumpToTask (void)
{
    _OS_SET_FSR_CUR_TASK();
    *((OS_RAM_NEAR char*)&_OS_State) = _postinc;
    asm {
        push
        movf    __postinc, W, 0     /*110306.2*/
        movwf   __tosl
        movf    __postinc, W, 0     /*110306.2*/
        movwf   __tosh
        movf    __postinc, W, 0     /*110306.2*/
        movwf   __tosu
        return
    }
}


/************************************************************************************************
 *                                                                                              *
 *                           K E R N E L   F U N C T I O N S                                    *
 *                                                                                              *
 ************************************************************************************************/


void _OS_ClrReadySetClrCanContinue (void);  /* see description below    */

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
    _OS_bTaskReady = 1;
    _OS_POP();
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

void _OS_EnterWaitModeTO (void)
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

void _OS_ClrReadySetClrCanContinue (void)
{
    _OS_POP();

    _OS_Flags.bEventError = 0;

    _OS_SET_FSR_CUR_TASK();
    _OS_SAVE_PC();
    _OS_bTaskReady = 0;

    #if defined(OS_ENABLE_TTIMERS)
        if (!_OS_bTaskDelay) _OS_bTaskCanContinue = 0;
        if (_OS_Temp & 0x01) _OS_bTaskCanContinue = 1;
    #endif

}











/************************************************************************************************
 *                                                                                              *
 *  MAKE FSR POINTER TO OS_CUR_TASK                                                             *
 *                                                                                              *
 *  (for code reducing)                                                                         *
 *                                                                                              *
 ************************************************************************************************/

void _OS_SET_FSR_CUR_TASK (void)
{
    _fsr = (OS_FSR_TYPE)_OS_CurTask;
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
            if (_OS_bTaskReady) {

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







//******************************************************************************
//  END OF FILE osa_pic18_mikroc.c
//******************************************************************************

