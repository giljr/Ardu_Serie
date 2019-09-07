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
 *  File:           osa_pic24_mplabc.c
 *
 *  Compilers:      Mplab C30
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    MPLAB C30 for PIC24 and dsPIC specific functions definition
 *                  This file directly included in osa.c
 *
 *  History:        13.09.2010 -    File updated
 *
 ************************************************************************************************
 */




/************************************************************************************************
 *                                                                                              *
 *     Variables                                                                                *
 *                                                                                              *
 ************************************************************************************************/


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
    asm volatile("  bset    __OS_State, #3       "); // bReady

    asm volatile("  btss    __OS_Flags, #10      "); //  bOSSchedRetPoint_LNK
    asm volatile("  bra     OS_RETURN_SKIP_ULNK ");
    asm volatile("  ulnk                        ");
    asm volatile("OS_RETURN_SKIP_ULNK:          ");

    //------------------------------------------------------------------------------
    asm volatile("  mov     __OS_CurTask, w0    ");
    asm volatile("  pop.d   w2                  "); // Saving return address
    asm volatile("  mov     w2, [w0 + #2]       ");
#ifdef __OSA_C30_24BIT__

    asm volatile("  mov.b   w3, [w0 + #1]       ");

#endif
    asm volatile("  goto    OS_RETURN_SAVE_STACK");
}


/*
 ********************************************************************************
 *
 *  void _OS_ReturnNoSave (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Return to scheduler without saving return address
 *
 *  parameters:     none
 *
 *  on return:      none
 *
 ********************************************************************************
 */

void _OS_ReturnNoSave (void)
{
    asm volatile("  mov     #(1<<11), w0        "); //  bOSSchedRetPointNoSave_LNK
    asm volatile("OS_RETURN:                    ");
    asm volatile("  and     __OS_Flags, wreg     ");
    asm volatile("  bra     z, OS_RETURN_NS_SKIP_ULNK");
    asm volatile("  ulnk                        ");
    asm volatile("OS_RETURN_NS_SKIP_ULNK:       ");

    //------------------------------------------------------------------------------
    asm volatile("  mov     __OS_CurTask, w0    ");
    asm volatile("  pop.d   w2                  "); // Get address of task who called this service

    //------------------------------------------------------------------------------
    asm volatile("OS_RETURN_SAVE_STACK:         ");

    /* Save registers (see C30 manual 4.12 Function call conversions)   */

    asm volatile("  add     #4, w0              ");
    asm volatile("  mov.d   w8,  [w0++]         ");
    asm volatile("  mov.d   w10, [w0++]         ");
    asm volatile("  mov.d   w12, [w0++]         ");
    asm volatile("  mov.w   w14, [w0++]         ");

    asm volatile("  mov     w15, w4             "); // Save stack pointer
    asm volatile("  mov     [w0], w15           "); // Restore previously saved stack pointer

    asm volatile("  btsc    __OS_Flags, #9       "); // bTaskReplaced
    asm volatile("  clr     w4                  "); // Clear stack pointer if there was OS_Task_Replace
    asm volatile("  mov     w4, [w0]            "); // Save stack pointer
    asm volatile("  bclr    __OS_Flags, #9       "); // bTaskReplaced
    asm volatile("  return                      ");
}

/*
 ********************************************************************************
 *
 *  void __OS_EnterWaitMode (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Clear bReady flag and return to the sheduler with saving
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
    #ifdef OS_ENABLE_TTIMERS
        asm volatile("  btss    __OS_State, #4   "); // if (!Delay)
        asm volatile("  bclr    __OS_State, #5   "); // bCanContinue
    #endif

    asm volatile("  mov     #(1<<12), w0        "); //  bOSClearReady_LNK
    asm volatile("OS_CLEAR_READY:               ");
    asm volatile("  and     __OS_Flags, wreg     ");
    asm volatile("  bra     z, OS_CLEAR_READY_SKIP_ULNK");
    asm volatile("  ulnk                        ");
    asm volatile("OS_CLEAR_READY_SKIP_ULNK:");

    asm volatile("  bclr    __OS_Flags, #0       "); // bEventError
    asm volatile("  bclr    __OS_State, #3       "); // bReady

    asm volatile("  mov     __OS_CurTask, w0    ");
    asm volatile("  mov     [w15 - #4], w2      "); // Save task return address
    asm volatile("  mov     w2, [w0 + #2]       ");

#ifdef __OSA_C30_24BIT__
    asm volatile("  mov     [w15 - #2], w3      "); // Save task return address
    asm volatile("  mov.b   w3, [w0 + #1]       ");   // v3

#endif

    asm volatile("  return                      ");
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
    #ifdef OS_ENABLE_TTIMERS
        asm volatile("  bset    __OS_State, #5   "); // bCanContinue
    #endif
    asm volatile("  mov     #(1<<13), w0        "); // bOSClearReadySetCanConinue_LNK
    asm volatile("  goto    OS_CLEAR_READY      ");
}
//------------------------------------------------------------------------------













/*
 ********************************************************************************
 *
 *  void OS_CheckLnk (OST_CODE_POINTER sub)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Check for "lnk" instruction in the begin of function
 *                  (Now works only for SMALL model , because of in LARGE model
 *                  table of jumps (.handle) is used
 *
 *  parameters:     sub - code pointer to checked function
 *
 *  on return:      modified __OS_Flags
 *
 ********************************************************************************
 */

void OS_CheckLnk (OST_CODE_POINTER sub)
{

    asm volatile("  clr     TBLPAG                  ");
    asm volatile("  tblrdh  [w0], w1                ");
    asm volatile("  xor     #0xFA, w1               "); // is it LNK?
    asm volatile("  bra     nz, 0f                  "); // no
    asm volatile("  mov     w3, w0                  ");
    asm volatile("  ior     __OS_Flags               ");

    asm volatile("0:                                ");

    asm volatile("  xor     #0x00FA ^ 0x0004, w1    "); // is it GOTO?
    asm volatile("  bra     nz, 9f                  "); // no

    asm volatile("  tblrdl  [w0++], w2              "); // Low 16 bits of address
    asm volatile("  tblrdh  [w0  ], w1              "); //
    asm volatile("  xor     #0x0000, w1             "); // check for NOP (second word of GOTO operation)
    asm volatile("  bra     nz, 9f                  "); // error

    asm volatile("  tblrdl  [w0], w0                "); // read page
    asm volatile("  mov     w0, TBLPAG              ");
    asm volatile("  tblrdh  [w2], w1                ");
    asm volatile("  xor     #0xFA, w1               "); // is it LNK?
    asm volatile("  bra     nz, 9f                  "); // no
    asm volatile("  mov     w3, w0                  ");
    asm volatile("  ior     __OS_Flags               ");


    asm volatile("9:                                ");
    asm volatile("  rlnc    w3, w3                  ");

}

/*
 ********************************************************************************
 *
 *  void _OS_INIT_PROC_SPEC (void)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    Set bOSxxx_LNK bits for functions which return to kernel
 *
 *  parameters:     none
 *
 *  on return:      none
 *
 ********************************************************************************
 */

void _OS_INIT_PROC_SPEC (void)
{
    asm volatile("  mov     #(1<<10), w3    ");
    OS_CheckLnk((OST_CODE_POINTER)_OS_ReturnSave);
    OS_CheckLnk((OST_CODE_POINTER)_OS_ReturnNoSave);
    OS_CheckLnk((OST_CODE_POINTER)_OS_EnterWaitMode);
    OS_CheckLnk((OST_CODE_POINTER)_OS_EnterWaitModeTO);
    OS_CheckLnk((OST_CODE_POINTER)_OS_CheckEvent);

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

    _OS_Flags.cIPL_Temp = (temp >> 5);

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
    _OS_Flags.bInCriticalSection = 0;
    OS_RI(_OS_Flags.cIPL_Temp << 5);

}

//------------------------------------------------------------------------------
#endif      //     #ifdef OS_ENABLE_CRITICAL_SECTION
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
    _OS_bTaskDelay = 0;

    _OS_CurTask->State = _OS_State;  // Clear bDelay to prevent timer changing in interrupt
    if (Delay)
    {

        Delay = -Delay;
        _OS_CurTask->Timer = Delay;
        _OS_SET_FSR_CUR_TASK();
        _OS_bTaskDelay = 1;
        _OS_bTaskReady = 1;

    }
    _OS_CurTask->State = _OS_State;  // Set flags Ready è Delay
}

//------------------------------------------------------------------------------
#endif  // #ifdef OS_ENABLE_TTIMERS
//------------------------------------------------------------------------------










/************************************************************************************************
 *                                                                                              *
 *                            S Y S T E M   F U N C T I O N S                                   *
 *                                                                                              *
 ************************************************************************************************/



/*
 ********************************************************************************
 *
 *   void _OS_Task_Create(OST_UINT priority, OST_CODE_POINTER TaskAddr)
 *
 *------------------------------------------------------------------------------
 *
 *   description:   (Internal function called by system kernel from service
 *                  OS_Task_Create)
 *                  Create task in free descriptor.
 *
 *  parameters:     priority - value from 0 (highest) to 7 (lowest)
 *                  TaskAddr - pointer to C-function that contains task
 *
 *  on return:      check OS_IsError
 *                  _OS_Temp - task position in _OS_Tasks array
 *
 *  note:           This function overloads "osa.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define _OS_Task_Create_DEFINED
//------------------------------------------------------------------------------

void _OS_Task_Create(OST_UINT priority, OST_CODE_POINTER TaskAddr)
{
    //------------------------------------------------------------------------------
    // according to manual "4.12.1 Function parameters":
    // w0 - priority
    // w1 - TaskAddr
    //------------------------------------------------------------------------------
    // my vars:
    // w0 - state of new task
    // w2 - state of current task
    // w7 - pointer to task descriptor
    // w6 - loop counter
    // w3, w4 - work reg

    asm volatile("  and     #0x0007, w0         ");     //
                                                        //
    asm volatile("  bclr    __OS_Flags, #1      ");    // bError

    asm volatile("  mov     #%0, w7 "::"i"(_OS_Tasks)); // w7 -> _OS_Tasks
    asm volatile("  mov     #0, w6");                   // w6 - counter

    asm volatile("0:                            ");     // CREATE_TASK_REPEAT
    asm volatile("  mov     [w7], w2            ");
    asm volatile("  btsc    w2, #6              ");     // bEnable
    asm volatile("  bra     1f                  ");     // CREATE_TASK_NEXT

    // Free descriptor found
    asm volatile(";CREATE_TASK_FOUND:           ");
    asm volatile("  bset    w0, #6              ");     // bEanble
    asm volatile("  bset    w0, #3              ");     // bReady
    asm volatile("  mov     w0, [w7]            ");     // Write state

    // save task address
    asm volatile("  mov     w1, [w7 + #2]       ");     // Write TaskAddr
    asm volatile("  clr     w4                  ");
    asm volatile("  mov     w4, [w7 + #18]      ");     // Write stack pointer (first time = 0)

    asm volatile("  bra     2f                  ");     // CREATE_TASK_RETURN

    asm volatile("1:                            ");     // CREATE_TASK_NEXT
    asm volatile("  add     #%0, w7 "::"i"(sizeof(OST_TCB)));
    asm volatile("  inc     w6, w6  ");                 //
    asm volatile("  mov     #%0, w3 "::"i"(OS_TASKS));  //
    asm volatile("  cpseq   w3, w6              ");     //
    asm volatile("  bra     nz, 0b              ");     // CREATE_TASK_REPEAT


    asm volatile(";CREATE_TASK_BAD:             ");
    asm volatile("  bset    __OS_Flags, #1      ");     // bError
    asm volatile("2:                            ");     // CREATE_TASK_RETURN

}





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
 *  on return:      int  - bits 5..7 = previous IPL value,
 *                         bits 0..4 = 0
 *
 ********************************************************************************
 */

//-------------------------------------------------------------------------------
#define OS_DI_DEFINED
//-------------------------------------------------------------------------------

OST_UINT OS_DI (void)
{
    OST_UINT temp;
    temp = SR;
    SR |= 0x00E0;
    return temp & 0x00E0;
}

/* Here way to get most compact code, but here we have warning: "function should return value"
{
    asm volatile("   mov     _SR, w1         ");
    asm volatile("   mov     #0x00E0, w0     ");
    asm volatile("   ior     _SR             ");
    asm volatile("   and     w0, w1, w0      ");
}
*/


//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT)
//------------------------------------------------------------------------------

    void _OS_DI_INT (void)
    {
        _OS_Temp_I = SR;
        SR |= 0x00E0;
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
 *  parameters:     int  - bits 5..7 = previous IPL value,
 *                         bits 0..4 = 0
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
    asm volatile("   mov     #0xFF1F, w1     ");
    asm volatile("   and     w0, w1, w1      ");
    asm volatile("   mov     #0x00E0, w0     ");
    asm volatile("   ior     SR, WREG        ");
    asm volatile("   and     w0, w1, w0      ");
    asm volatile("   mov     w0, SR          ");
}

//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT)
//------------------------------------------------------------------------------

    void _OS_RI_INT (void)
    {
        asm volatile("   mov     #0xFF1F, w1     ");
        asm volatile("   mov     __OS_Temp_I, w0 ");
        asm volatile("   and     w0, w1, w1      ");
        asm volatile("   mov     #0x00E0, w0     ");
        asm volatile("   ior     SR, WREG        ");
        asm volatile("   and     w0, w1, w0      ");
        asm volatile("   mov     w0, SR          ");
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

        asm volatile("  bclr    __OS_Flags, #8              "); // bTimeout
        asm volatile("  cp0     w0                          "); //  if (bEvent)
        asm volatile("  bra     Z, 0f                       "); // _OS_CHECK_EVENT_notEVENT
        _OS_CurTask->State.bDelay = 0;
        asm volatile("  bclr    __OS_State, #4              "); // Clear Delay
        asm volatile("  bra     2f                          "); // _OS_CHECK_EVENT_EXIT_GOOD
        asm volatile("0:                                    "); // _OS_CHECK_EVENT_notEVENT
        #ifdef OS_ENABLE_TTIMERS
            asm volatile("  btss    __OS_State, #4          "); // bDelay
            asm volatile("  btss    __OS_State, #5          "); // bCanContinue
            asm volatile("  bra     4f                      "); // _OS_CHECK_EVENT_EXIT_BAD
            asm volatile("  bset    __OS_Flags, #8          "); // bTimeout
            asm volatile("  bra     2f                      "); // _OS_CHECK_EVENT_EXIT_GOOD
        #endif
        asm volatile("4:                                    "); // _OS_CHECK_EVENT_EXIT_BAD
        asm volatile("  mov     #(1<<14), w0                "); //  bOSCheckEvent_LNK
        asm volatile("  goto    OS_RETURN                   ");

        asm volatile("2:                                    "); // _OS_CHECK_EVENT_EXIT_GOOD

    //------------------------------------------------------------------------------
    #else
    //------------------------------------------------------------------------------

        asm volatile("  bclr    __OS_Flags, #8              "); // bTimeout
        asm volatile("  cp0     w0                          "); //  if (bEvent)
        asm volatile("  bra     Z, 0f                       "); // _OS_CHECK_EVENT_notEVENT
        asm volatile("  btss    __OS_State, #3              "); // Check ready
        asm volatile("  bra     1f                          "); // _OS_CHECK_EVENT_SetReady
        _OS_CurTask->State.bDelay = 0;
        asm volatile("  bclr    __OS_State, #4              "); // Clear Delay
        asm volatile("  bra     2f                          "); // _OS_CHECK_EVENT_EXIT_GOOD
        asm volatile("1:                                    "); // _OS_CHECK_EVENT_SetReady
        asm volatile("  bset    __OS_State, #3              "); // Ready = 1
        asm volatile("  bra     3f                          "); // _OS_CHECK_EVENT_CHECK_END
        asm volatile("0:                                    "); // _OS_CHECK_EVENT_notEVENT
        asm volatile("  bclr    __OS_State, #3              "); // Ready = 1
        asm volatile("3:                                    "); // _OS_CHECK_EVENT_CHECK_END
        #ifdef OS_ENABLE_TTIMERS
            asm volatile("  btss    __OS_State, #4          "); // bDelay
            asm volatile("  btss    __OS_State, #5          "); // bCanContinue
            asm volatile("  bra     4f                      "); // _OS_CHECK_EVENT_EXIT_BAD
            asm volatile("  bset    __OS_Flags, #8          "); // bTimeout
            asm volatile("  bset    __OS_State, #3          "); // Ready = 1
            asm volatile("  bra     2f                      "); // _OS_CHECK_EVENT_EXIT_GOOD
        #endif
        asm volatile("4:                                    "); // _OS_CHECK_EVENT_EXIT_BAD
        asm volatile("  mov     #(1<<14), w0                "); //  bOSCheckEvent_LNK
        asm volatile("  goto    OS_RETURN                   ");

        asm volatile("2:                                    "); // _OS_CHECK_EVENT_EXIT_GOOD
        #if OS_PRIORITY_LEVEL == OS_PRIORITY_EXTENDED
        asm volatile("  bset    __OS_Flags, #15             "); // bEventOK
        #endif

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
 *                  (for PICC16 can be allocated only in OS_RAM_BANK0 and OS_RAM_BANK1)
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

    dtimer->Next = _OS_DTimers.Next;
    dtimer->Flags = _OS_DTimers.Flags;
    dtimer->Timer = 0;

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

    asm volatile("   mov     #__OS_DTimers, w1       ");
    asm volatile("0:                                 "); // _OS_DELETE_DTIMER_NEXT
    asm volatile("   mov     w1, w5                  ");
    asm volatile("   mov     [w5], w2                ");
    asm volatile("   btss    w2, #3                  "); // bNextEnable
    asm volatile("   bra     1f                      "); // _OS_DELETE_DTIMER_EXIT
    asm volatile("   mov     [w5 + #2], w1           ");
    asm volatile("   cpseq   w0, w1                  ");
    asm volatile("   bra     0b                      "); // _OS_DELETE_DTIMER_NEXT
    asm volatile("   mov.d   [w0], w2                "); // w2 = flags, w3 = next
    asm volatile("   mov     w3, [w5 + #2]           "); // w1.next = w0.next
    asm volatile("   mov     [w5], w3                "); // flags
    asm volatile("   btss    w2, #3                  "); // skip if bNextEnable
    asm volatile("   bclr    w3, #3                  "); // clear bNextEnable
    asm volatile("   mov     w3, [w5]                ");
    asm volatile("   bclr    w2, #1                  "); // clear bActive
    asm volatile("   mov     w2, [w0]                "); // save flags
    asm volatile("1:                                 "); // _OS_DELETE_DTIMER_EXIT
}

//------------------------------------------------------------------------------
#endif  //  OS_ENABLE_DTIMERS
//------------------------------------------------------------------------------



//******************************************************************************
//  END OF FILE osa_pic24_mplabc.c
//******************************************************************************

