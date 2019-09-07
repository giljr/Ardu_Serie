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
 *  File:           osa_avr_winavr.c
 *
 *  Compilers:      WinAVR
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    WinAVR specific functions definition
 *                  This file directly included in osa.c
 *
 *  History:        15.09.2010 -    File updated
 *
 *                  06.03.2011 -    Bug fixed: local variables were not saved correctly when
 *                                  task exited through the OS_CheckEvent function
 * 
 ************************************************************************************************
 */

#define _OS_INIT_PROC_SPEC()      // Empty macro for WinAVR


#if XRAMEND < 0x100
    OST_UINT8  _OS_SP_Temp;
#else
    OST_UINT16 _OS_SP_Temp;
#endif



extern void _OS_ClrReadySetClrCanContinue (void) __attribute__ ((naked, noinline));


/************************************************************************************************
 *                                                                                              *
 *                           K E R N E L   F U N C T I O N S                                    *
 *                                                                                              *
 ************************************************************************************************/

void  _OS_SET_FSR_CUR_TASK (void)
{
    asm(" lds r26, _OS_CurTask ");
    asm(" lds r27, _OS_CurTask+1 ");
    asm("ret");
}

//******************************************************************************

void _OS_JumpToTask (void)
{
    _OS_STORE_SP();
    _OS_SET_FSR_CUR_TASK();
    _OS_SET_PC();
    _OS_RESTORE_TEMP();
    asm(" ret ");
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
    _OS_SET_FSR_CUR_TASK();
    _OS_SAVE_PC();
    _OS_bTaskReady = 1;
    _OS_JMP("_OS_ReturnNoSave");
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
    _OS_SAVE_TEMP();
    _OS_RESTORE_SP();
    asm(" ret ");
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
    asm("   ldi     r24, 0    ");
    _OS_JMP("_OS_ClrReadySetClrCanContinue");
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
    asm("   ldi    r24, 1     ");
    _OS_JMP("_OS_ClrReadySetClrCanContinue");
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
    asm(" ret ");
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
    temp = SREG;
    asm("cli");
    return temp;
}

#if defined(OS_ENABLE_INT)

void __OS_DI_INT (void)
{
    asm("   in      r22, 0x3F       ");
    asm("   sts     _OS_Temp_I, r22 ");
    asm("   cli                     ");
    asm("   ret                     ");
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
    SREG = temp;
}

#if defined(OS_ENABLE_INT)

void __OS_RI_INT (void)
{
    asm("   lds     r22, _OS_Temp_I ");
    asm("   out     0x3F, r22       ");
    asm("   ret                     ");
}

#endif



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
    _OS_SET_FSR_CUR_TASK(); /*110306*/

    //------------------------------------------------------------------------------
    #if OS_PRIORITY_LEVEL == OS_PRIORITY_DISABLED
    //------------------------------------------------------------------------------

        asm("   lds     r22, _OS_Flags              "); // _OS_Flags.bTimeout = 0;
        asm("   andi    r22, 0xDF                   ");
        asm("   sts     _OS_Flags, r22              ");

        asm("   lds     r23, _OS_State              ");

        asm("   tst     r24                         "); //if (bEvent)
        asm("   breq    OS_CE_BAD                   ");
        {
            #ifdef OS_ENABLE_TTIMERS
                                                        // Clear bDelay in temporary variable _OS_State
                asm("   andi    r23, 0xEF           "); //_OS_bTaskDelay = 0;
                asm("   sts     _OS_State, r23      ");

//*110306*                _OS_SET_FSR_CUR_TASK();
                _OS_ATOMIC_WRITE_A
                (
                    asm("   ld      r25, x          "); // Clear bDelay in task descriptor
                    asm("   andi    r25, 0xEF       ");
                    asm("   st      x, r25          ");
                );
            #endif
            asm("   ret                             "); // Exit OK
        }

        asm("OS_CE_BAD:                             ");

        #ifdef OS_ENABLE_TTIMERS


            asm("   sbrs    r23, 4                  "); //if (_OS_bTaskTimeout && _OS_bTaskCanContinue)
            asm("   sbrs    r23, 5                  ");
            asm("   rjmp    OS_CE_NO_TIMEOUT        ");
            {
                asm("   ori     r22, 0x20           "); //_OS_Flags.bTimeout = 1;
                asm("   sts     _OS_Flags, r22      ");
                asm("   ret                         "); // Exit OK
            }
            asm("OS_CE_NO_TIMEOUT:                  ");

        #endif

        _OS_ADD_X_TEMP();   /*110306*/
        _OS_POP();
        _OS_JMP("_OS_ReturnNoSave");

    //------------------------------------------------------------------------------
    #else
    //------------------------------------------------------------------------------

        asm("   lds     r22, _OS_Flags              "); // _OS_Flags.bTimeout = 0;
        asm("   andi    r22, 0xDF                   ");
        asm("   sts     _OS_Flags, r22              ");

        asm("   lds     r23, _OS_State              ");

        asm("   tst     r24                         "); //if (bEvent)
        asm("   breq    OS_CE_BAD                   ");
        {
            asm("   sbrs    r23, 3                  "); // if (_OS_bTaskReady) {
            asm("   rjmp    OS_CE_WAIT_MORE         ");
            {
                #ifdef OS_ENABLE_TTIMERS

                    asm("   andi    r23, 0xEF       "); //_OS_bTaskDelay = 0;
//*110306*          _OS_SET_FSR_CUR_TASK();
                    _OS_ATOMIC_WRITE_A
                    (
                        asm("   ld   r22, x         ");     // Clear bDelay in task descriptor
                        asm("   andi r22, 0xEF      ");
                        asm("   st   x, r22         ");
                    );

                #endif
                asm("OS_CE_EXIT_OK:                 ");

                #if OS_PRIORITY_LEVEL == OS_PRIORITY_EXTENDED
                asm("   push    r23                 ");
                _OS_Flags.bEventOK = 1;
                asm("   pop     r23                 ");
                #endif

                asm("   sts     _OS_State, r23      ");
                asm("   ret                         "); // Exit OK
            }

            asm("OS_CE_WAIT_MORE:                   ");
            asm("   sbr     r23, (1<<3)             "); // _OS_bTaskReady = 1;
            asm("   rjmp    OS_CE_CHECK_TIMEOUT     ");
        }

        asm("OS_CE_BAD:                             ");
        asm("   cbr     r23, (1<<3)                 "); // _OS_bTaskReady = 0;

        asm("OS_CE_CHECK_TIMEOUT:                   ");

        #ifdef OS_ENABLE_TTIMERS

            asm("   sbrs    r23, 4                  "); //if (_OS_bTaskTimeout && _OS_bTaskCanContinue)
            asm("   sbrs    r23, 5                  ");
            asm("   rjmp    OS_CE_NO_TIMEOUT        ");
            {
                asm("   ori     r22, 0x20           "); //_OS_Flags.bTimeout = 1;
                asm("   sts     _OS_Flags, r22      ");
                asm("   sbr     r23, (1<<3)         "); // _OS_bTaskReady = 1;
                asm("   rjmp    OS_CE_EXIT_OK       "); // Exit OK
            }
            asm("OS_CE_NO_TIMEOUT:                  ");
        #endif

        asm("   sts     _OS_State, r23              ");
        _OS_ADD_X_TEMP();   /*110306*/
        _OS_POP();
        _OS_JMP("_OS_ReturnNoSave");

    //------------------------------------------------------------------------------
    #endif
    //------------------------------------------------------------------------------

}




//******************************************************************************
//  END OF FILE osa_avr_winavr.c
//******************************************************************************

