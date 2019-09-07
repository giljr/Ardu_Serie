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
 *  File:           osa_pic12_htpicc.c
 *
 *  Compilers:      HT-PICC STD
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    HT-PICC for 12-bit PICs specific functions definition
 *                  This file directly included in osa.c
 *
 *  History:        10.09.2010 -    File updated
 *
 ************************************************************************************************
 */




#define _OS_INIT_PROC_SPEC()      // Empty macro for PIC12






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

    if (Delay)
    {

        Delay ^= -1;
        Delay ++;

        #if     OS_TTIMER_SIZE == 1
        asm("   movlw   2               ");
        asm("   addwf   __OS_CurTask, w ");
            _OS_PIC12_AND_1F();
            asm("   movwf   __fsr    ");
            asm("   movf    ?a__OS_InitDelay, w");
            _OS_OS_SET_TASK_IRP();
            asm("   movwf   __indf");
            _OS_OS_SET_TASK_IRP_0();
        #elif   OS_TTIMER_SIZE == 2
            asm("   movlw   2               ");
            asm("   addwf   __OS_CurTask, w ");
            _OS_PIC12_AND_1F();
            asm("   movwf   __fsr    ");
            asm("   movf    ?__OS_InitDelay + 1, w");
            _OS_OS_SET_TASK_IRP();
            asm("   movwf   __indf");

            _OS_OS_SET_TASK_IRP_0();
            asm("   incf    __fsr ");
            asm("   movf    ?__OS_InitDelay + 2, w");
            _OS_OS_SET_TASK_IRP();
            asm("   movwf   __indf");

            _OS_OS_SET_TASK_IRP_0();

        #elif   OS_TTIMER_SIZE == 4
            asm("   movlw   2               ");
            asm("   addwf   __OS_CurTask, w ");
            _OS_PIC12_AND_1F();
            asm("   movwf   __fsr    ");
            asm("   movf    ?__OS_InitDelay + 1, w");
            _OS_OS_SET_TASK_IRP();
            asm("   movwf   __indf");

            _OS_OS_SET_TASK_IRP_0();
            asm("   incf    __fsr ");
            asm("   movf    ?__OS_InitDelay + 2, w");
            _OS_OS_SET_TASK_IRP();
            asm("   movwf   __indf");

            _OS_OS_SET_TASK_IRP_0();
            asm("   incf    __fsr ");
            asm("   movf    ?__OS_InitDelay + 3, w");
            _OS_OS_SET_TASK_IRP();
            asm("   movwf   __indf");

            _OS_OS_SET_TASK_IRP_0();
            asm("   incf    __fsr ");
            asm("   movf    ?__OS_InitDelay + 4, w");
            _OS_OS_SET_TASK_IRP();
            asm("   movwf   __indf");

            _OS_OS_SET_TASK_IRP_0();
        #endif

        _OS_SET_FSR_CUR_TASK();
        _OS_bTaskDelay = 1;
        _OS_bTaskReady = 1;

    }
}


//------------------------------------------------------------------------------
#endif  // #ifdef OS_TIMER
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
    OST_TASK_POINTER Task;

    _OS_Flags.bError = 0;


    Task = (OST_TASK_POINTER)_OS_Tasks;
    _OS_Temp = 0;
    do {
        if (!Task->State.bEnable)
        {
            ((OST_TASK_STATE*)&priority)->bEnable = 1;
            ((OST_TASK_STATE*)&priority)->bReady = 1;

            asm("   incf    __OS_Task_Create$Task, w     ");
            _OS_PIC12_AND_1F();
            asm("   movwf   __fsr                        ");
            asm("   movf    __OS_Task_Create$TaskAddr, w      ");
            _OS_OS_SET_TASK_IRP();
            asm("   movwf    __indf ");
            asm("   decf    __fsr   ");
            _OS_OS_SET_TASK_IRP_0();
            asm("   movf    __OS_Task_Create$priority, w      ");   // priority
            _OS_OS_SET_TASK_IRP();
            asm("   movwf    __indf ");
            _OS_OS_SET_TASK_IRP_0();
            if (Task == _OS_CurTask) _OS_State = *((OST_TASK_STATE*)&priority);
            _OS_Flags.bError = 0;

            return ;

        }
        Task ++;
    } while (++_OS_Temp < OS_TASKS);

    // No free descriptor
    _OS_Flags.bError = 1;
    return ;
}









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
OST_BOOL _OS_CheckEvent (char bEvent)
{
    //------------------------------------------------------------------------------
    #if OS_PRIORITY_LEVEL == OS_PRIORITY_DISABLED
    //------------------------------------------------------------------------------

        _OS_Flags.bTimeout = 0;

        if (bEvent)
        {

            #ifdef OS_ENABLE_TTIMERS
                _OS_bTaskDelay = 0;
            #endif
            return 1;

        }

        #ifdef OS_ENABLE_TTIMERS

            if (_OS_bTaskTimeout && _OS_bTaskCanContinue)
            {
                _OS_Flags.bTimeout = 1;
                return 1;
            }

        #endif

        return 0;

    //------------------------------------------------------------------------------
    #else
    //------------------------------------------------------------------------------

        _OS_Flags.bTimeout = 0;

        if (bEvent)
        {

            if (_OS_bTaskReady)
            {
                #ifdef OS_ENABLE_TTIMERS
                    _OS_bTaskDelay = 0;
                #endif

                #if OS_PRIORITY_LEVEL == OS_PRIORITY_EXTENDED
                _OS_Flags.bEventOK = 1;
                #endif

                return 1;
            }

            _OS_bTaskReady = 1;

        } else _OS_bTaskReady = 0;

        #ifdef OS_ENABLE_TTIMERS

            if (_OS_bTaskTimeout && _OS_bTaskCanContinue)
            {
                _OS_bTaskReady = 1;
                _OS_Flags.bTimeout = 1;
                #if OS_PRIORITY_LEVEL == OS_PRIORITY_EXTENDED
                _OS_Flags.bEventOK = 1;
                #endif
                return 1;
            }

        #endif

        return 0;

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
    //------------------------------------------------------------------------------
    // dtimer->Flags = _OS_DTimers.Flags;
        #if _BANKBITS_ == 2
        asm("   bcf     __status, 0             ");
        asm("   btfsc   __OS_Dtimer_Create$dtimer, 6  ");
        asm("   bsf     __status, 0             ");
        #endif
        asm("   movf    __OS_Dtimer_Create$dtimer, w  ");
        _OS_PIC12_AND_1F();
        asm("   movwf   __fsr                   ");
        //-------
        _OS_SET_OS_IRP();
        asm("   movf    __OS_DTimers & 0x1F, w  ");
        _OS_SET_OS_IRP_0();
        //-------
        #if _BANKBITS_ >= 1
        asm("   btfsc   __OS_Dtimer_Create$dtimer, 5 ");
        asm("   bsf     __fsr, 5            ");
        #endif
        #if _BANKBITS_ == 2
        asm("   btfsc   __status, 0 ");
        asm("   bsf     __fsr, 6            ");
        #endif
        asm("   movwf   __indf              ");
        _fsr = 0;

    //------------------------------------------------------------------------------
    // dtimer->Next = _OS_DTimers.Next;

        /* Now CARRY = __OS_Dtimer_Create$dtimer.6 */

        asm("   movf    __OS_Dtimer_Create$dtimer, w      ");
        _OS_PIC12_AND_1F();
        asm("   movwf   __fsr                       ");
        asm("   incf    __fsr, f                    ");
        //-------
        _OS_SET_OS_IRP();
        asm("   movf    (__OS_DTimers + 1) & 0x1F, w");
        _OS_SET_OS_IRP_0();
        //-------
    #if _BANKBITS_ >= 1
        asm("   btfsc   __OS_Dtimer_Create$dtimer, 5      ");
        asm("   bsf     __fsr, 5                    ");
    #endif
    #if _BANKBITS_ == 2
        asm("   btfsc   __status, 0 ");
        asm("   bsf     __fsr, 6                    ");
    #endif
        asm("   movwf   __indf                      ");
        _fsr = 0;

    //------------------------------------------------------------------------------
    dtimer->Timer = 0;


    _OS_DTimers.Next = (OST_DTIMER*)dtimer;
    _OS_DTimers.Flags.bNextEnable = 1;

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

    OST_DTIMER *ptcb;

    ptcb = (OST_DTIMER*)&_OS_DTimers;
    while (ptcb->Flags.bNextEnable) {
        if (ptcb->Next == dtimer) {
            _OS_Temp = (OST_UINT)dtimer->Next;
            //------------------------------------------------------------------------------
            //ptcb->Next = _OS_Temp;
            #if _BANKBITS_ == 2
                asm("   bcf     __status, 0                 ");
                asm("   btfsc   __OS_Dtimer_Delete$ptcb, 6   ");
                asm("   bsf     __status, 0                 ");
            #endif
            asm("   movf    __OS_Dtimer_Delete$ptcb, w       ");
            _OS_PIC12_AND_1F();
            asm("   movwf   __fsr                   ");
            asm("   incf    __fsr, f                ");
            //-------
            asm("   movf    __OS_Temp, w  ");
            //-------
            #if _BANKBITS_ >= 1
                asm("   btfsc   __OS_Dtimer_Delete$ptcb, 5   ");
                asm("   bsf     __fsr, 5                    ");
            #endif
            #if _BANKBITS_ == 2
                asm("   btfsc   __status, 0 ");
                asm("   bsf     __fsr, 6            ");
            #endif
            asm("   movwf   __indf              ");
            _fsr = 0;

            if (!dtimer->Flags.bNextEnable) ptcb->Flags.bNextEnable = 0;
            dtimer->Flags.bActive = 0;
            return;
        }
        ptcb = ptcb->Next;
    }
}



//------------------------------------------------------------------------------
#endif  //  OS_ENABLE_DTIMERS
//------------------------------------------------------------------------------

































/************************************************************************************************
 *                                                                                              *
 *     M E S S A G E   Q U E U E S                                                              *
 *                                                                                              *
 ************************************************************************************************/


//------------------------------------------------------------------------------
#if defined(OS_ENABLE_QUEUE)
//------------------------------------------------------------------------------


/*
 ********************************************************************************
 *
 *  void _OS_Queue_Send (OST_QUEUE *pQueue, OST_MSG Msg)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    (Internal function called by system kernel througth
 *                  service _OS_Queue_Send)
 *
 *                  Adds message into queue of pointers to messages. Deletes
 *                  first message if there is no free room to add new message.
 *                  Service OS_Queue_Send before adding new message checks for
 *                  free room. Thus messages will not deleted accidentally.
 *
 *
 *  parameters:     pQueue      - pointer to queue descriptor
 *                  Msg        - pointer to  message to be added
 *
 *  on return:      OS_IsEventError() return 1, if first message was pushed out
 *
 *  note:           This function overloads "osa.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define _OS_Queue_Send_DEFINED
#define _OS_Squeue_Send_DEFINED
//------------------------------------------------------------------------------

void _OS_Queue_Send (OST_QUEUE *pQueue, OST_MSG Msg)
{
    _OS_Flags.bEventError = 0;

    // if pQueue->Q.cSize == pQueue->Q.cFilled
    _fsr = (OS_FSR_TYPE) pQueue;
    asm("   movf    __indf, w               ");     // w = pQueue->Q.cSize
    asm("   incf    __fsr, f                ");     // fsr -> pQueue->Q.cFilled
    asm("   xorwf   __indf, w               ");     // Compare cSize и cFilled

    if (_status & 0x04) {   // Check ZERO flag

        _fsr = 0;
        _OS_Flags.bEventError = 1;

        // pQueue->pMsg[pQueue->Q.cBegin] = Msg;
        _fsr = (OS_FSR_TYPE) pQueue;
        asm("   incf    __fsr, f        ");
        asm("   incf    __fsr, f        ");
        asm("   movf    __indf, w       ");     // w = pQueue->Q.cBegin
    PUT_MESSAGE:
        asm("   incf    __fsr, f        ");
        asm("   addwf   __indf, w       ");
        asm("   movwf   __fsr           ");     // fsr -> pQueue->pMsg[pQueue->Q.cBegin]



        #ifdef _16F59
            asm("   bcf     __status, 5     "); // Here we use ROM page bit to store FSR.7
            asm("   btfsc   __fsr, 7        ");
            asm("   bsf     __status, 5     ");
            asm("   bcf     __fsr, 7     ");
        #endif

        #if _BANKBITS_ == 2
            asm("   bcf     __status, 1     ");
            asm("   btfsc   __fsr, 6        ");
            asm("   bsf     __status, 1     ");
            asm("   bcf     __fsr, 6     ");
        #endif
        #if _BANKBITS_ >= 1
            asm("   bcf     __status, 0     ");
            asm("   btfsc   __fsr, 5        ");
            asm("   bsf     __status, 0     ");
            asm("   bcf     __fsr, 5     ");
        #endif

        asm("   movf    __OS_Queue_Send$Msg, w    ");

        #ifdef _16F59
            asm("   btfsc   __status, 5     "); // Here we use ROM page bit to store FSR.7
            asm("   bsf     __fsr, 7        ");
            asm("   dw  0x4A3   | (($ & 0x200) >>1)");
        #endif
        #if _BANKBITS_ == 2
            asm("   btfsc   __status, 1     ");
            asm("   bsf     __fsr, 6        ");
        #endif
        #if _BANKBITS_ >= 1
            asm("   btfsc   __status, 0     ");
            asm("   bsf     __fsr, 5        ");
        #endif
        asm("   movwf   __indf              ");


        // Set status, c = bEventError.
        // This will help to select register to increaze and compare to cSize
        // If there will be an error then cBegin++, else cFilled++

        _fsr = 0;
        _status &= 0xFE;
        if (_OS_Flags.bEventError) _status |= 0x01;

        // pQueue->Q.cBegin++ (or pQueue->Q.cFileld++) and compare it to cSize
        _fsr = (OS_FSR_TYPE) pQueue;
        asm("   movf    __indf, w               ");
        asm("   btfsc   __status, 0             ");     // Skip ink FSR if there was not error
        asm("   incf    __fsr, f                ");
        asm("   incf    __fsr, f                ");     // fsr -> pQueue->Q.cBegin
        asm("   incf    __indf, f               ");     // cBegin++ (или cFilled++)
        if (_status & 1) {
            asm("   xorwf   __indf, w           ");     // Compare cBegin (or cFilled) to cSize
            asm("   btfsc   __status, 2         ");
            asm("   clrf    __indf              ");     // cBegin (or cFilled) = 0
        }
        asm("   clrf    __fsr                   ");
        return;
    }

    //temp = q.cBegin + q.cFilled;
    //if (temp >= q.cSize) temp -= q.cSize;
    asm("   incf    __fsr, f                ");
    asm("   movf    __indf, w               ");     // w = cBegin
    asm("   decf    __fsr, f                ");
    asm("   addwf   __indf, w               ");     // w += cFilled
    asm("   decf    __fsr, f                ");
    asm("   subwf   __indf, w               ");     // w = cSize - (cFileld + cBegin)
    asm("   btfss   __status, 0             ");     // > 0
    asm("   addwf   __indf, w               ");     // Correcting pointer
    asm("   subwf   __indf, w               ");     // w = cSize - (cFileld + cBegin)
    asm("   incf    __fsr, f                ");
    asm("   incf    __fsr, f                ");
    goto    PUT_MESSAGE;

}



/*
 ********************************************************************************
 *
 *  OST_MSG _OS_Queue_Get (OST_QUEUE *pQueue)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    (Internal function called by system kernel througth
 *                  service OS_AcceptQueue)
 *
 *                  Get first pointer to message from queue. Before calling this
 *                  function be sure that queue is not empty (OS_AcceptQueue does
 *                  it automatically). After execution this function first
 *                  message will be deleted from queue.
 *
 *  parameters:     pQueue      - pointer to queue descriptor
 *
 *  on return:      first message from queue
 *
 *  note:           This function overloads "osa.c"
 *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#define _OS_Queue_Get_DEFINED
#define _OS_Squeue_Get_DEFINED
//------------------------------------------------------------------------------

OST_MSG _OS_Queue_Get (OST_QUEUE *pQueue)
{
    char temp;

    _fsr = (OS_FSR_TYPE)pQueue;
    _fsr++;
    _fsr++;
    asm("   movf    __indf, w           "); // w = cBegin
    asm("   incf    __fsr, f            "); // fsr -> pMsg
    asm("   addwf   __indf, w           ");
    asm("   movwf   __fsr               "); // fsr -> pMsg[cBegin]
    asm("   movf    __indf, w           "); // w = pMsg[cBegin]
    asm("   clrf    __fsr               ");
    asm("   movwf   __OS_Queue_Get$temp   ");

    // Here FSR bits 5 and 6 are cleared (due to work with "temp")
    _fsr = (OS_FSR_TYPE)pQueue;
    asm("   movf    __indf, w           ");
    asm("   incf    __fsr, f            ");
    asm("   incf    __fsr, f            ");
    asm("   incf    __indf, f           "); // cBegin++
    asm("   subwf   __indf, w           ");
    asm("   btfsc   __status, 0         "); // if (cBegin >= cSize) cBegin = 0;
    asm("   clrf    __indf              ");
    asm("   decf    __fsr, f            ");
    asm("   decf    __indf, f           "); // cFilled--

    _fsr = 0;
    return (OST_MSG)temp;
}

//------------------------------------------------------------------------------
#endif      // OS_ENABLE_QUEUE
//------------------------------------------------------------------------------












//------------------------------------------------------------------------------
#if (OS_STIMERS > 0 ) && defined(OS_STIMERS_ENABLE_ALLOCATION)
//-----------------------------------------------------------------


/*
 ********************************************************************************
 *
 *  _OS_Stimer_GetFree
 *
 *------------------------------------------------------------------------------
 *
 *  description:    (Internal function called by system kernel througth service OS
 *                  Searches free static timer and returns it's ID
 *
 *
 *  parameters:     OST_UINT bCreate - =0 - do not created timer, just check for
 *                                          aviability
 *                                     =1 - create timer and mark it asm used
 *
 *  on return:      none
 *
 ********************************************************************************
 */


//-----------------------------------------------------------------
#define _OS_Stimer_GetFree_DEFINED
//-----------------------------------------------------------------

//------------------------------------------------------------------------------
// Macros for setting OS variables bank
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#if     OS_BANK_STIMERS == 0
//------------------------------------------------------------------------------

    #define _OS_SET_STIMER_IRP()
    #define _OS_SET_STIMER_IRP_0()

//------------------------------------------------------------------------------
#elif   OS_BANK_STIMERS == 1
//------------------------------------------------------------------------------

    #define _OS_SET_STIMER_IRP()    asm("   bsf     __fsr, 5 ");
    #define _OS_SET_STIMER_IRP_0()  asm("   bcf     __fsr, 5 ");

//------------------------------------------------------------------------------
#elif   OS_BANK_STIMERS == 2
//------------------------------------------------------------------------------

    #define _OS_SET_STIMER_IRP()    asm("   bsf     __fsr, 6 ");
    #define _OS_SET_STIMER_IRP_0()  asm("   bcf     __fsr, 6 ");

//------------------------------------------------------------------------------
#elif   OS_BANK_STIMERS == 3
//------------------------------------------------------------------------------

    #define _OS_SET_STIMER_IRP()    asm("   bsf     __fsr, 5 ");    \
                                    asm("   bsf     __fsr, 6 ");

    #define _OS_SET_STIMER_IRP_0()  asm("   bcf     __fsr, 5 ");    \
                                    asm("   bcf     __fsr, 6 ");

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------




OST_UINT8   _OS_Stimer_GetFree (OST_UINT bCreate)
{
    char i;

    _OS_Flags.bError = 0;
    i = 0;
    _OS_Temp = (OS_STIMERS >> 3) + 1;
    _fsr = (OS_FSR_TYPE)_OS_StimersFree;

    do
    {
        _OS_SET_STIMER_IRP();
        if (_indf)
        {
            asm("   movf    __indf, w                   ");
            _OS_SET_STIMER_IRP_0();
            asm("   movwf   __OS_Temp                   ");

            asm("   btfss    ?a__OS_Stimer_GetFree, 0   ");
            asm("   goto    OS_ST_GF_RETURN             ");

            asm("   movf    __fsr, w                    ");
            asm("   clrf    __fsr                       "); // fsr used as a counter of bit position
            asm("   clrf    __OS_Stimer_GetFree$i       ");
            asm("   incf    __OS_Stimer_GetFree$i, f    ");
            asm("OS_ST_GF_REPEAT:                       ");
            asm("   rrf     __OS_Temp, f                ");
            asm("   btfsc   __status, 0                 ");
            asm("   goto    OS_ST_GF_OK                 ");
            asm("   rlf     __OS_Stimer_GetFree$i, f    ");
            asm("   incf    __fsr, f                    ");
            asm("   goto     OS_ST_GF_REPEAT            ");
            asm("OS_ST_GF_OK:                           ");

            // Exchange WREG with FSR (saved fsr <-> bits counter)
            asm("   xorwf   __fsr, w                    ");
            asm("   xorwf   __fsr, f                    ");
            asm("   xorwf   __fsr, w                    ");
            _OS_PIC12_AND_7F();
            asm("   movwf   __OS_Temp                   ");

            // Now i contains bit position, j contains bit mask
            asm("   movf    __OS_Stimer_GetFree$i, w    ");
            _OS_SET_STIMER_IRP();
            asm("   xorwf   __indf, f                   "); // Mark timer as "in use"

            asm("   movlw   __OS_StimersFree            ");
            asm("   subwf   __fsr, f                    ");
            asm("   rlf     __fsr, f                    ");
            asm("   rlf     __fsr, f                    ");
            asm("   rlf     __fsr, w                    ");
            asm("   andlw   0xF8                        ");

            asm("   clrf    __fsr                       ");

            asm("   addwf   __OS_Temp, w                ");
            asm("OS_ST_GF_RETURN:                       ");
            return _OS_Temp;
        }
        _fsr++;
        _OS_SET_STIMER_IRP_0();
    } while (--_OS_Temp);

    _OS_Flags.bError = 1;
    return 0xFF;
}


void    _OS_Stimer_Free (OST_UINT8 ID)
{
    asm("   clrf    __OS_Temp                   ");     // W = 1 << (IS & 7)
    asm("   incf    __OS_Temp, f                ");
    asm("   btfsc   ?a__OS_Stimer_Free, 2       ");
    asm("   swapf   __OS_Temp, f                ");

    if (ID & 1) _OS_Temp <<= 1;
    asm("   btfsc   ?a__OS_Stimer_Free, 1       ");
    asm("   rlf     __OS_Temp, f                ");     // CARRY = 0 now
    asm("   btfsc   ?a__OS_Stimer_Free, 1       ");
    asm("   rlf     __OS_Temp, f                ");     // CARRY = 0 now

    _fsr = (OS_FSR_TYPE)_OS_StimersFree;
    _OS_SET_STIMER_IRP_0();

    asm("   rrf    ?a__OS_Stimer_Free, f        ");
    asm("   rrf    ?a__OS_Stimer_Free, f        ");
    asm("   rrf    ?a__OS_Stimer_Free, w        ");
    asm("   andlw   0x1F                        ");
    asm("   addwf   __fsr, f                    ");

    asm("   movf    __OS_Temp, w                ");
    _OS_SET_STIMER_IRP();
    asm("   iorwf   __indf, f                   ");
    _fsr = 0;
}

//------------------------------------------------------------------------------
#endif          //  OS_STIMERS_ENABLE_ALLOCATION
//-----------------------------------------------------------------



//******************************************************************************
//  END OF FILE osa_pic12_htpicc.c
//******************************************************************************

