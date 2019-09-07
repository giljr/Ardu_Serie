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
 *  File:           osa_avr_iar.c
 *
 *  Compilers:      IAR
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    IAR for AVR specific functions definition
 *                  This file directly included in osa.c
 *
 *  History:        13.09.2010 -    File updated
 *
 ************************************************************************************************
 */


#include <ioavr.h>

#define _OS_INIT_PROC_SPEC()      // Empty macro for mikroC


OS_RAM_NEAR OST_UINT16 _OS_SP_Temp;


void _OS_ClrReadySetClrCanContinue (void);  /* see description below    */



/************************************************************************************************
 *                                                                                              *
 *                           K E R N E L   F U N C T I O N S                                    *
 *                                                                                              *
 ************************************************************************************************/



#pragma optimize=no_inline
void  _OS_SET_FSR_CUR_TASK (void)
{
    asm(" lds r30, _OS_CurTask ");
    if (sizeof(OST_TASK_POINTER)==2)      asm(" lds r31, _OS_CurTask+1 ");
    else                                  asm(" ldi r31, 0 ");
}



#pragma optimize=no_inline
void _OS_JumpToTask (void)
{
    _OS_STORE_SP();
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
    _OS_SAVE_Y();
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
    _OS_ClrReadySetClrCanContinue();
    asm(" nop ");
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
    _OS_ClrReadySetClrCanContinue();
    asm(" nop ");
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
    asm("   st  -y, r21 ");
    asm("   st  -y, r22 ");
    asm("   st  -y, r23 ");
    asm("   st  -y, r26 ");
    asm("   st  -y, r27 ");
    asm("   st  -y, r30 ");
    asm("   st  -y, r31 ");

    _OS_POP();
    _OS_Flags.bEventError = 0;

    _OS_SET_FSR_CUR_TASK();
    _OS_SAVE_PC();
    _OS_SAVE_Y_PLUS_7();
    _OS_PUSH();
    asm("   ld  r31, y+ ");
    asm("   ld  r30, y+ ");
    asm("   ld  r27, y+ ");
    asm("   ld  r26, y+ ");
    asm("   ld  r23, y+ ");
    asm("   ld  r22, y+ ");
    asm("   ld  r21, y+ ");

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
 *  on return:      char - bit 7 = previous I-bit value,
 *                         bits 0..6 = 0
 *
 ********************************************************************************
 */

//-------------------------------------------------------------------------------
#define OS_DI_DEFINED
//-------------------------------------------------------------------------------

char OS_DI (void)
{
    char temp;
    temp = SREG & 0x80;
    asm("cli");
    return temp;
}


#if defined(OS_ENABLE_INT)

void _OS_DI_INT (void)
{
    _OS_Temp_I = SREG & 0x80;
    asm("cli");
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
 *  parameters:     char - bit 7 = previos I-value,
 *                         bits 0..6 = 0
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
    if (temp & 0x80) asm("sei");
}


#if defined(OS_ENABLE_INT)

void _OS_RI_INT (void)
{
    if (_OS_Temp_I & 0x80) asm("sei");
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
                _OS_ATOMIC_WRITE_A(_OS_CurTask->State.bDelay = 0);  // Clear bDelay in task descriptor

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



//******************************************************************************
//  END OF FILE osa_avr_iar.c
//******************************************************************************

