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
 *  File:           osa_queue.c
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    Functions for queues
 *                  This file directly included in osa.c
 *
 *  History:        15.09.2010 -    File updated
 *
 ************************************************************************************************
 */





//------------------------------------------------------------------------------
#if defined(OS_ENABLE_QUEUE)
//------------------------------------------------------------------------------

/*
 ********************************************************************************
 *                                                                              *
 *  void _OS_Queue_Send (OST_QUEUE *pQueue, OST_MSG Msg)                        *
 *                                                                              *
 *------------------------------------------------------------------------------*
 *                                                                              *
 *  description:    (Internal function called by system kernel througth         *
 *                  service _OS_Queue_Send)                                     *
 *                                                                              *
 *                  Adds message into queue of pointers to messages. Deletes    *
 *                  first message if there is no free room to add new message.  *
 *                  Service OS_Queue_Send before adding new message checks for  *
 *                  free room. Thus messages will not deleted accidentally.     *
 *                                                                              *
 *                                                                              *
 *  parameters:     pQueue      - pointer to queue descriptor                   *
 *                  Msg        - pointer to  message to be added                *
 *                                                                              *
 *  on return:      OS_IsEventError() return 1, if first message was pushed out *
 *                                                                              *
 *  Overloaded in:  "osa_pic12_htpicc.c"                                         *
 *                                                                              *
 ********************************************************************************
 */


//------------------------------------------------------------------------------
#if !defined(_OS_Queue_Send_DEFINED)
//------------------------------------------------------------------------------

    void _OS_Queue_Send (OST_QUEUE *pQueue, OST_MSG Msg)
    {

        OST_QUEUE_CONTROL   q;
        OST_UINT16 temp;              

        q = pQueue->Q;
        _OS_Flags.bEventError = 0;

        //------------------------------------------------------
        // If there is no free room in queue, then replace
        // first message in queue by new SMsg

        if (q.cSize == q.cFilled)
        {
            pQueue->pMsg[q.cBegin] = Msg;
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
        pQueue->pMsg[temp] = Msg;
        q.cFilled++;

    EXIT:

        pQueue->Q = q;
    }

//------------------------------------------------------------------------------
#endif  // !defined(_OS_Queue_Send_DEFINED)
//------------------------------------------------------------------------------








/*
 ********************************************************************************
 *                                                                              *
 *  void _OS_Queue_Send_I (OST_QUEUE *pQueue, OST_MSG Msg)                      *
 *                                                                              *
 *------------------------------------------------------------------------------*
 *                                                                              *
 *  description:    Copy of _OS_Queue_Send to be called from interrupt          *
 *                                                                              *
 *  parameters:     pQueue      - pointer to queue descriptor                   *
 *                  Msg        - pointer to message to be added                 *
 *                                                                              *
 *  on return:      OS_IsEventError() return 1, if first message was pushed out *
 *                                                                              *
 *  Overloaded in:  -                                                           *
 *                                                                              *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT_QUEUE) && !defined(_OS_Queue_Send_I_DEFINED)
//------------------------------------------------------------------------------

    void _OS_Queue_Send_I (OST_QUEUE *pQueue, OST_MSG Msg)
    {

        OST_QUEUE_CONTROL   q;
        OST_UINT16 temp;              

        q = pQueue->Q;
        _OS_Flags.bEventError = 0;

        //------------------------------------------------------
        // If there is no free room in queue, then replace
        // first message in queue by new SMsg

        if (q.cSize == q.cFilled)
        {
            pQueue->pMsg[q.cBegin] = Msg;
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
        pQueue->pMsg[temp] = Msg;
        q.cFilled++;

    EXIT:

        pQueue->Q = q;
    }

//------------------------------------------------------------------------------
#endif  // (OS_ENABLE_INT_QUEUE) && !defined(_OS_Queue_Send_I_DEFINED)
//------------------------------------------------------------------------------









/*
 ********************************************************************************
 *                                                                              *
 *  OST_MSG _OS_Queue_Get (OST_QUEUE *pQueue)                                    *
 *                                                                              *
 *------------------------------------------------------------------------------*
 *                                                                              *
 *  description:    (Internal function called by system kernel througth         *
 *                  service OS_AcceptQueue)                                     *
 *                                                                              *
 *                  Get first pointer to message from queue. Before calling this*
 *                  function be sure that queue is not empty (OS_AcceptQueue does*
 *                  it automatically). After execution this function first      *
 *                  message will be deleted from queue.                         *
 *                                                                              *
 *  parameters:     pQueue      - pointer to queue descriptor                   *
 *                                                                              *
 *  on return:      first message from queue                                    *
 *                                                                              *
 *  Overloaded in:  "osa_pic12_htpicc.c"                                         *
 *                                                                              *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#if !defined(_OS_Queue_Get_DEFINED)
//------------------------------------------------------------------------------

    OST_MSG _OS_Queue_Get (OST_QUEUE *pQueue)
    {
        OST_QUEUE_CONTROL   q;
        OST_UINT          temp;
        OST_MSG             msg_temp;


        q = pQueue->Q;
        temp = q.cBegin;
        q.cBegin++;

        if (q.cBegin >= q.cSize)    q.cBegin = 0;

        q.cFilled--;
        pQueue->Q = q;

        msg_temp = pQueue->pMsg[temp];

        return  msg_temp;
    }

//------------------------------------------------------------------------------
#endif  // !defined(_OS_Queue_Get_DEFINED)
//------------------------------------------------------------------------------


/*
 ********************************************************************************
 *                                                                              *
 *  OST_MSG _OS_Queue_Get (OST_QUEUE *pQueue)                                    *
 *                                                                              *
 *------------------------------------------------------------------------------*
 *                                                                              *
 *  description:    (Internal function called by system kernel througth         *
 *                  service OS_Queue_Accept_I)                                   *
 *                                                                              *
 *                  Get first pointer to message from queue. Before calling this*
 *                  function be sure that queue is not empty (OS_AcceptQueue does*
 *                  it automatically). After execution this function first      *
 *                  message will be deleted from queue.                         *
 *                                                                              *
 *  parameters:     pQueue      - pointer to queue descriptor                   *
 *                                                                              *
 *  on return:      first message from queue                                    *
 *                                                                              *
 *  Overloaded in:  -                                                            *
 *                                                                              *
 ********************************************************************************
 */

//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT_QUEUE) && !defined(_OS_Queue_Get_I_DEFINED)
//------------------------------------------------------------------------------

    OST_MSG _OS_Queue_Get_I (OST_QUEUE *pQueue)
    {
        OST_QUEUE_CONTROL   q;
        OST_UINT temp;

        q = pQueue->Q;
        temp = q.cBegin;
        q.cBegin++;

        if (q.cBegin >= q.cSize)    q.cBegin = 0;

        q.cFilled--;
        pQueue->Q = q;

        return  pQueue->pMsg[temp];
    }

//------------------------------------------------------------------------------
#endif  // defined(OS_ENABLE_INT_QUEUE) && !defined(_OS_Queue_Get_DEFINED)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#endif  // OS_ENABLE_QUEUE
//------------------------------------------------------------------------------
//******************************************************************************
//  END OF FILE osa_queue.c
//******************************************************************************

