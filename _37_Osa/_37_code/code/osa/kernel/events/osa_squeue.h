/*
 ***********************************************************************************************
 *
 *  OSA cooperative RTOS for microcontrollers PIC (Microchip) and 8-bit AVR (Atmel)
 *
 *  OSA is distributed under BSD license (see license.txt)
 *
 *  URL:        http://wiki.pic24.ru/doku.php/en/osa/ref/intro
 *              http://picosa.narod.ru
 *
 *----------------------------------------------------------------------------------------------
 *
 *  File:       osa_squeue.h
 *
 *  Programmer: Timofeev Victor
 *              osa@pic24.ru, testerplus@mail.ru
 *
 *  Definition: Services for work with queue of simple messages
 *
 *  History:    21.01.2009
 *              21.03.2009 -    Enable/disable interrupt actions on SendSQueue are placed incide
 *                              macro OS_SQueue_Send and deleted from _OS_SQueue_Send
 *              18.06.2009 -    Argument "value" placed in braces
 *                              Alternate names added
 * 
 *              10.12.2009      1. Services for interrupt were corrected.
 *
 *              12.06.2010      1. OS_Squeue_IsFree and OS_Squeue_IsEmpty services are removed.
 * 
 *              01.09.2010      New services added: OS_Squeue_Get, OS_Squeue_Get_I, OS_Squeue_Delete, OS_Squeue_Delete_I
 *
 ***********************************************************************************************
 */



/************************************************************************************************
 *                                                                                              *
 *                    Q U E U E   O F   S I M P L E    M E S S A G E S                          *
 *                                                                                              *
 ************************************************************************************************/

#ifdef OS_ENABLE_SQUEUE


//******************************************************************************
//  VARIABLES
//******************************************************************************


//******************************************************************************
//  FUNCTION PROTOTYPES
//******************************************************************************


//******************************************************************************
//  MACROS
//******************************************************************************





    #if defined(OS_QUEUE_SQUEUE_IDENTICAL)

        // If size of pointer to message eq. to size of simple message then we
        // can use same functions for sending and getting messages.
        //
        #define _OS_Squeue_Send(pSQueue,SMsg)      _OS_Queue_Send((OST_QUEUE*)pSQueue, (OST_MSG)SMsg)
        #define _OS_Squeue_Send_I(pSQueue,SMsg)    _OS_Queue_Send_I((OST_QUEUE*)pSQueue, (OST_MSG)SMsg)
        #define _OS_Squeue_Get(pSQueue)            _OS_Queue_Get((OST_QUEUE*)pSQueue)
        #define _OS_Squeue_Get_I(pSQueue)          _OS_Queue_Get_I((OST_QUEUE*)pSQueue)

    #else

        extern void         _OS_Squeue_Send (OST_SQUEUE * pSQueue, OST_SMSG SMsg);
        extern OST_SMSG     _OS_Squeue_Get (OST_SQUEUE *pSQueue);

    #endif

//------------------------------------------------------------------------------
// Create queue

#define OS_Squeue_Create(squeue, buffer, size)               \
    OSM_BEGIN {                                              \
        __OS_QUEUE_DI();                                     \
        (squeue).Q.cSize = size;                             \
        (squeue).Q.cBegin = 0;                               \
        (squeue).Q.cFilled = 0;                              \
        (squeue).pSMsg = (OST_SMSG*)(buffer);                \
        __OS_QUEUE_RI();                                     \
    } OSM_END

//------------------------------------------------------------------------------
#define __OS_Squeue_Check(squeue)       ((squeue).Q.cFilled)

// Check for queue is full
#define __OS_Squeue_IsFull(squeue)      ((squeue).Q.cFilled == (squeue).Q.cSize)

// Check for any message present in queue
#define OS_Squeue_Check(squeue)         __OS_Squeue_Check(squeue)

//------------------------------------------------------------------------------
// Clear queue

#define OS_Squeue_Clear(squeue)        { (squeue).Q.cFilled = 0; }


//------------------------------------------------------------------------------
// Send message via queue. If queue is full then most rearly message will be pushed out.

#define OS_Squeue_Send_Now(squeue, value)                               \
    OSM_BEGIN {                                                         \
        __OS_QUEUE_DI();                                                \
        _OS_Squeue_Send((OST_SQUEUE*)&(squeue), (OST_SMSG)(value));     \
        __OS_QUEUE_RI();                                                \
    } OSM_END


#define __OS_Squeue_Accept(squeue,os_smsg_type_var)                     \
    OSM_BEGIN {                                                         \
        os_smsg_type_var = _OS_Squeue_Get((OST_SQUEUE*)&(squeue));      \
    } OSM_END


#define OS_Squeue_Accept(squeue,os_smsg_type_var)                       \
    OSM_BEGIN {                                                         \
        __OS_QUEUE_DI();                                                \
        __OS_Squeue_Accept(squeue,os_smsg_type_var);                    \
        __OS_QUEUE_RI();                                                \
    } OSM_END


#define OS_Squeue_Delete(squeue)                                        \
    OSM_BEGIN {                                                         \
        __OS_QUEUE_DI();                                                \
        _OS_Squeue_Get((OST_SQUEUE*)&(squeue));                         \
        __OS_QUEUE_RI();                                                \
    } OSM_END


#define OS_Squeue_Get(squeue,os_smsg_type_var)                          \
    OSM_BEGIN {                                                         \
        __OS_QUEUE_DI();                                                \
        os_smsg_type_var = (squeue).pSMsg[(squeue).Q.cBegin];           \
        __OS_QUEUE_RI();                                                \
    } OSM_END



//------------------------------------------------------------------------------
// Send message via queue from interrupt. If queue is full then most rearly message will be pushed out.
#if defined(OS_ENABLE_INT_QUEUE)

    extern void     _OS_Squeue_Send_I (OST_SQUEUE *pSQueue, OST_SMSG SMsg);
    extern OST_SMSG _OS_Squeue_Get_I  (OST_SQUEUE *pSQueue);


    #define OS_Squeue_Send_I(squeue, value)   _OS_Squeue_Send_I((OST_SQUEUE*)&(squeue), (OST_SMSG)(value))
    #define OS_Squeue_Accept_I(squeue,os_smsg_type_var) os_smsg_type_var = _OS_Squeue_Get_I((OST_SQUEUE*)&(squeue))

    #define OS_Squeue_Check_I(squeue)         __OS_Squeue_Check(squeue)
    #define OS_Squeue_Clear_I(squeue)         OS_Squeue_Clear(squeue)
    #define OS_Squeue_IsFull_I(squeue)        __OS_Squeue_IsFull(squeue)


    #define OS_Squeue_Delete_I(squeue)                  _OS_Squeue_Get_I((OST_SQUEUE*)&(squeue))    
    #define OS_Squeue_Get_I(squeue,os_smsg_type_var)    os_smsg_type_var = (squeue).pSMsg[(squeue).Q.cBegin] 

    //------------------------------------------------------------------------------
    // Send message via queue. If queue full then wait for free place

    #define OS_Squeue_Send(squeue, value)                                   \
        OSM_BEGIN {                                                         \
            __OS_QUEUE_DI();                                                \
            while (__OS_Squeue_IsFull(squeue))                              \
            {                                                               \
                __OS_QUEUE_RI();                                            \
                OS_Wait(!__OS_Squeue_IsFull(squeue));                \
                __OS_QUEUE_DI();                                            \
            }                                                               \
            _OS_Squeue_Send((OST_SQUEUE*)&(squeue), (OST_SMSG)(value));     \
            __OS_QUEUE_RI();                                                \
        } OSM_END

    //------------------------------------------------------------------------------
    // Send message via queue. If queue full then wait for free place. Exit if timeout expired.

    #define OS_Squeue_Send_TO(squeue, value, timeout)                       \
        OSM_BEGIN {                                                         \
            _OS_Flags.bTimeout = 0;                                          \
            __OS_QUEUE_DI();                                                \
            while (__OS_Squeue_IsFull(squeue) && !OS_IsTimeout())           \
            {                                                               \
                __OS_QUEUE_RI();                                            \
                OS_Wait_TO(!__OS_Squeue_IsFull(squeue), timeout);\
                __OS_QUEUE_DI();                                            \
            }                                                               \
            if (!OS_IsTimeout()) {                                          \
                _OS_Squeue_Send((OST_SQUEUE*)&(squeue), (OST_SMSG)(value)); \
            }                                                               \
            __OS_QUEUE_RI();                                                \
        } OSM_END

    // Wait message from queue. After accepting message will be deleted from queue.

    #define OS_Squeue_Wait(squeue, os_smsg_type_var)                        \
        OSM_BEGIN {                                                         \
            for (;;) {                                                      \
                OS_Wait(__OS_Squeue_Check(squeue));                  \
                __OS_QUEUE_DI();                                            \
                if (__OS_Squeue_Check(squeue)) break;                       \
                __OS_QUEUE_RI();                                            \
            }                                                               \
            __OS_Squeue_Accept(squeue,os_smsg_type_var);                    \
            __OS_QUEUE_RI();                                                \
        } OSM_END

    //------------------------------------------------------------------------------
    // Wait message from queue. After accepting message will be deleted from queue. Exit if timeout expired.

    #define OS_Squeue_Wait_TO(squeue, os_smsg_type_var, timeout)            \
        OSM_BEGIN {                                                         \
            for (;;) {                                                      \
                OS_Wait_TO(__OS_Squeue_Check(squeue), timeout); \
                __OS_QUEUE_DI();                                            \
                if (__OS_Squeue_Check(squeue) || OS_IsTimeout()) break;     \
                __OS_QUEUE_RI();                                            \
            }                                                               \
            if (!OS_IsTimeout())                                            \
            {                                                               \
                __OS_Squeue_Accept(squeue,os_smsg_type_var);                \
            }                                                               \
            __OS_QUEUE_RI();                                                \
        } OSM_END


    // Check for queue is full
    #define OS_Squeue_IsFull(squeue)    (__OS_QUEUE_DI(),_OS_Temp=__OS_Squeue_IsFull(squeue),__OS_QUEUE_RI(),_OS_Temp)

//------------------------------------------------------------------------------
#else   // OS_ENABLE_INT_QUEUE
//------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    // Send message via queue. If queue full then wait for free place

    #define OS_Squeue_Send(squeue, value)                                   \
        OSM_BEGIN {                                                         \
            if (__OS_Squeue_IsFull(squeue))                                 \
            {                                                               \
                OS_Wait(!__OS_Squeue_IsFull(squeue));                \
            }                                                               \
            _OS_Squeue_Send((OST_SQUEUE*)&(squeue), (OST_SMSG)(value));     \
        } OSM_END

    //------------------------------------------------------------------------------
    // Send message via queue. If queue full then wait for free place. Exit if timeout expired.

    #define OS_Squeue_Send_TO(squeue, value, timeout)                       \
        OSM_BEGIN {                                                         \
            _OS_Flags.bTimeout = 0;                                          \
            if (__OS_Squeue_IsFull(squeue))                                 \
            {                                                               \
                OS_Wait_TO(!__OS_Squeue_IsFull(squeue), timeout);\
            }                                                               \
            if (!OS_IsTimeout()) {                                          \
                _OS_Squeue_Send((OST_SQUEUE*)&(squeue), (OST_SMSG)(value)); \
            }                                                               \
        } OSM_END

    // Wait message from queue. After accepting message will be deleted from queue.

    #define OS_Squeue_Wait(squeue, os_smsg_type_var)                        \
        OSM_BEGIN {                                                         \
            OS_Wait(__OS_Squeue_Check(squeue));                      \
            __OS_Squeue_Accept(squeue,os_smsg_type_var);                    \
        } OSM_END

    //------------------------------------------------------------------------------
    // Wait message from queue. After accepting message will be deleted from queue. Exit if timeout expired.

    #define OS_Squeue_Wait_TO(squeue, os_smsg_type_var, timeout)            \
        OSM_BEGIN {                                                         \
            OS_Wait_TO(__OS_Squeue_Check(squeue), timeout);     \
            if (!OS_IsTimeout())                                            \
            {                                                               \
                __OS_Squeue_Accept(squeue,os_smsg_type_var);                \
            }                                                               \
        } OSM_END


    // Check for queue is full
    #define OS_Squeue_IsFull(squeue)       __OS_Squeue_IsFull(squeue)


//------------------------------------------------------------------------------
#endif  // OS_ENABLE_INT_QUEUE
//------------------------------------------------------------------------------



//******************************************************************************
//  Alternate names
//******************************************************************************

#ifndef __OSACCS__  // To avoid "Warning: dublicate define"

#define OS_SQueue_Create        OS_Squeue_Create
#define OS_SQueue_Check         OS_Squeue_Check
#define OS_SQueue_IsFull        OS_Squeue_IsFull
#define OS_SQueue_Send          OS_Squeue_Send
#define OS_SQueue_Send_TO       OS_Squeue_Send_TO
#define OS_SQueue_Send_Now      OS_Squeue_Send_Now
#define OS_SQueue_Send_I        OS_Squeue_Send_I
#define OS_SQueue_Wait          OS_Squeue_Wait
#define OS_SQueue_Wait_TO       OS_Squeue_Wait_TO

#endif  // __OSACCS__


#endif




























