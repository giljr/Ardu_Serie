/*
 ************************************************************************************************
 *
 *  OSA cooperative RTOS for microcontrollers PIC (Microchip) and 8-bit AVR (Atmel)
 *
 *  OSA is distributed under BSD license (see license.txt)
 *
 *  URL:            http://wiki.pic24.ru/doku.php/en/osa/ref/intro
 *                  http://picosa.narod.ru
 *
 *----------------------------------------------------------------------------------------------
 *
 *  File:           osa_squeue.c
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    Functions for queues of simple messages
 *
 *  History:        14.03.2010 -    File created
 * 
 *                  20.04.2010 -    2. Type of variable temp in _OS_XXX_Send changed OST_UINT -> __osa_int16
 * 
 *                  31.05.2010 -    2. Buf fixed for MCC18: queues with buffer size greater than 128 elements
 *                                     worked incorrectly (see _OS_Queue_Send)
 * 
 *
 ************************************************************************************************
 */


/************************************************************************************************
 *                                                                                              *
 *     M E S S A G E   Q U E U E S                                                              *
 *                                                                                              *
 ************************************************************************************************/


//------------------------------------------------------------------------------
#if defined(OS_ENABLE_SQUEUE) && !defined(OS_QUEUE_SQUEUE_IDENTICAL)
//------------------------------------------------------------------------------




/*
 ********************************************************************************
 *                                                                              *
 *  void _OS_Squeue_Send (OST_SQUEUE *pSQueue, OST_SMSG SMsg)                   *
 *                                                                              *
 *------------------------------------------------------------------------------*
 *                                                                              *
 *  description:    (Internal function called by system kernel througth         *
 *                  service _OS_SendSQueue)                                     *
 *                                                                              *
 *                  Add message into queue of simple messages. Delete first     *
 *                  message if there is no free room to add new message.        *
 *                  Service OS_SQueue_Send before adding new message checks for *
 *                  free room. Thus messages will not deleted accidentally.     *
 *                                                                              *
 *                                                                              *
 *  parameters:     pSQueue      - pointer to queue descriptor                  *
 *                  SMsg        - simple message to be added                    *
 *                                                                              *
 *  on return:      OS_IsEventError() return 1, if first message was pushed out *
 *                                                                              *
 *  Overloaded in:  "osa_pic12_htpicc.c"                                         *
 *                                                                              *
 ********************************************************************************
 */


//------------------------------------------------------------------------------
#if !defined(_OS_Squeue_Send_DEFINED)
//------------------------------------------------------------------------------

    void _OS_Squeue_Send (OST_SQUEUE * pSQueue, OST_SMSG SMsg)
    {
        OST_QUEUE_CONTROL   q;          // Temp variable to increase speed
        OST_UINT16 temp;              

        q = pSQueue->Q;
        _OS_Flags.bEventError = 0;                   // First we clear error flag


        //------------------------------------------------------
        // If there is no free room in queue, then replace
        // first message in queue by new SMsg

        if (q.cSize == q.cFilled)
        {
            pSQueue->pSMsg[q.cBegin] = SMsg;
            q.cBegin++;                             // Update pointer
            if (q.cBegin == q.cSize) q.cBegin = 0;

            _OS_Flags.bEventError = 1;               // Set error flag

            goto EXIT;                              // Skip adding
        }

        //------------------------------------------------------
        // There is a free room in queue.
        // Add new message at end of queue.

        temp = (OST_UINT16)q.cBegin + q.cFilled;  
        if (temp >= q.cSize) temp -= q.cSize;
        pSQueue->pSMsg[temp] = SMsg;
        q.cFilled++;                                // Update fillness

    EXIT:
        pSQueue->Q = q;

    }

//------------------------------------------------------------------------------
#endif  // !defined(_OS_Squeue_Send_DEFINED)
//------------------------------------------------------------------------------






/*
 ********************************************************************************
 *                                                                              *
 *  void _OS_Squeue_Send_I (OST_SQUEUE *pSQueue, OST_SMSG SMsg)                 *
 *                                                                              *
 *------------------------------------------------------------------------------*
 *                                                                              *
 *  description:    Copy of _OS_SQueue_Send to be called from interrupt         *
 *                                                                              *
 *  parameters:     pSQueue      - pointer to queue descriptor                  *
 *                  SMsg        - simple message to be added                    *
 *                                                                              *
 *  on return:      OS_IsEventError() return 1, if first message was pushed out *
 *                                                                              *
 *  Overloaded in:  -                                                           *
 *                                                                              *
 ********************************************************************************
 */


//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT_QUEUE) && !defined(_OS_Squeue_Send_I_DEFINED)
//------------------------------------------------------------------------------

    void _OS_Squeue_Send_I (OST_SQUEUE *pSQueue, OST_SMSG SMsg)
    {
        OST_QUEUE_CONTROL   q;
        OST_UINT16 temp;              

        q = pSQueue->Q;
        _OS_Flags.bEventError = 0;

        //------------------------------------------------------
        // If there is no free room in queue, then replace
        // first message in queue by new SMsg

        if (q.cSize == q.cFilled)
        {
            pSQueue->pSMsg[q.cBegin] = SMsg;
            q.cBegin++;
            if (q.cBegin == q.cSize) q.cBegin = 0;

            _OS_Flags.bEventError = 1;
            goto EXIT;
        }

        //------------------------------------------------------
        // There is a free room in queue.
        // Add new message at end of queue.

        temp = (OST_UINT16)q.cBegin + q.cFilled;  
        if (temp >= q.cSize) temp -= q.cSize;
        pSQueue->pSMsg[temp] = SMsg;
        q.cFilled++;

    EXIT:
        pSQueue->Q = q;
    }

//------------------------------------------------------------------------------
#endif  // (OS_ENABLE_INT_QUEUE) && !defined(_OS_Squeue_Send_I_DEFINED)
//------------------------------------------------------------------------------









/*
 ********************************************************************************
 *                                                                              *
 *  OST_SMSG _OS_Squeue_Get (OST_SQUEUE *pSQueue)                               *
 *                                                                              *
 *------------------------------------------------------------------------------*
 *                                                                              *
 *  description:    (Internal function called by system kernel througth         *
 *                  service OS_AcceptSQueue)                                    *
 *                                                                              *
 *                  Get first simple message from queue. Before calling this    *
 *                  function be sure that queue is not empty (OS_AcceptSQueue   *
 *                  does it automatically). After execution this function first *
 *                  message will be deleted from queue.                         *
 *                                                                              *
 *  parameters:     pSQueue      - pointer to queue descriptor                  *
 *                                                                              *
 *  on return:      first message from queue                                    *
 *                                                                              *
 *  Overloaded in:  "osa_pic12_htpicc.c"                                         *
 *                                                                              *
 ********************************************************************************
 */


//------------------------------------------------------------------------------
#if !defined(_OS_Squeue_Get_DEFINED)
//------------------------------------------------------------------------------

    OST_SMSG _OS_Squeue_Get (OST_SQUEUE *pSQueue)
    {
        OST_QUEUE_CONTROL   q;
        OST_UINT       temp;
        OST_SMSG            smsg_temp;

        q = pSQueue->Q;
        temp = q.cBegin;
        q.cBegin++;

        if (q.cBegin >= q.cSize)    q.cBegin = 0;

        q.cFilled--;
        pSQueue->Q = q;

        smsg_temp = pSQueue->pSMsg[temp];

        return  smsg_temp;
    }

//------------------------------------------------------------------------------
#endif  // !defined(_OS_Queue_Get_DEFINED)
//------------------------------------------------------------------------------




/*
 ********************************************************************************
 *                                                                              *
 *  OST_SMSG _OS_Squeue_Get_I (OST_SQUEUE *pSQueue)                             *
 *                                                                              *
 *------------------------------------------------------------------------------*
 *                                                                              *
 *  description:    (Internal function called by system kernel througth         *
 *                  service OS_Squeue_Accept_I)                                 *
 *                                                                              *
 *                  Get first simple message from queue. Before calling this    *
 *                  function be sure that queue is not empty (OS_AcceptSQueue   *
 *                  does it automatically). After execution this function first *
 *                  message will be deleted from queue.                         *
 *                                                                              *
 *  parameters:     pSQueue      - pointer to queue descriptor                  *
 *                                                                              *
 *  on return:      first message from queue                                    *
 *                                                                              *
 *  Overloaded in:  -                                                           *
 *                                                                              *
 ********************************************************************************
 */


//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT_QUEUE) && !defined(_OS_Squeue_Get_I_DEFINED)
//------------------------------------------------------------------------------

    OST_SMSG _OS_Squeue_Get_I (OST_SQUEUE *pSQueue)
    {
        OST_QUEUE_CONTROL   q;
        OST_UINT temp;

        q = pSQueue->Q;
        temp = q.cBegin;
        q.cBegin++;

        if (q.cBegin >= q.cSize)    q.cBegin = 0;

        q.cFilled--;
        pSQueue->Q = q;

        return  pSQueue->pSMsg[temp];
    }

//------------------------------------------------------------------------------
#endif  // defined(OS_ENABLE_INT_QUEUE) && !defined(_OS_Queue_Get_DEFINED)
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#endif  // defined(OS_ENABLE_SQUEUE)
//------------------------------------------------------------------------------







