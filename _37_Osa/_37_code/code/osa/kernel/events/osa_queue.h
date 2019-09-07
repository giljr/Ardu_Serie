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
 *  File:           osa_queue.h
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    Services for work with queue of pointers to messages 
 *                  This file directly included in osa.h
 *
 *  History:        15.09.2010 -    File updated
 *
 ************************************************************************************************
 */


/************************************************************************************************
 *                                                                                              *
 *                              Q U E U E   O F   M E S S A G E S                               *
 *                                                                                              *
 ************************************************************************************************/


//******************************************************************************
//  VARIABLES
//******************************************************************************


//******************************************************************************
//  FUNCTION PROTOTYPES
//******************************************************************************


//******************************************************************************
//  MACROS
//******************************************************************************


#if defined(OS_ENABLE_INT_QUEUE)

    #define __OS_QUEUE_DI()       _OS_DI_INT() 
    #define __OS_QUEUE_RI()       _OS_RI_INT() 

    #if defined(OS_PROTECT_MEMORY_ACCESS)           
        #define __OS_QUEUE_DI_A()    _OS_DI_INT()
        #define __OS_QUEUE_RI_A()    _OS_RI_INT()
    #else
        #define __OS_QUEUE_DI_A()
        #define __OS_QUEUE_RI_A()
    #endif

#else

    #define __OS_QUEUE_DI()
    #define __OS_QUEUE_RI()
    #define __OS_QUEUE_DI_A()
    #define __OS_QUEUE_RI_A()

#endif


//------------------------------------------------------------------------------

#define OS_QUEUE_ATOMIC_WRITE_A(expr)                                       \
    OSM_BEGIN {                                                             \
        __OS_QUEUE_DI_A();                                                  \
        expr;                                                               \
        __OS_QUEUE_RI_A();                                                  \
    } OSM_END


#define OS_QUEUE_ATOMIC_READ_A(expr)                                        \
    (__OS_QUEUE_DI_A(), _OS_Temp = (expr), __OS_QUEUE_RI_A(), _OS_Temp)     \




//------------------------------------------------------------------------------
#ifdef OS_ENABLE_QUEUE
//------------------------------------------------------------------------------

extern void     _OS_Queue_Send (OST_QUEUE *pQueue, OST_MSG Msg);
extern OST_MSG  _OS_Queue_Get  (OST_QUEUE *pQueue);


//------------------------------------------------------------------------------
// Create queue

#define OS_Queue_Create(queue, buffer, size)             \
    OSM_BEGIN {                                          \
        __OS_QUEUE_DI();                                 \
        (queue).Q.cSize = size;                          \
        (queue).Q.cBegin = 0;                            \
        (queue).Q.cFilled = 0;                           \
        (queue).pMsg = (OST_MSG*)(buffer);               \
        __OS_QUEUE_RI();                                 \
    } OSM_END


// Internal macros

#define __OS_Queue_IsFull(queue)     ((queue).Q.cFilled == (queue).Q.cSize)
#define __OS_Queue_Check(queue)      ((queue).Q.cFilled)


// Check for any message present in queue
#define OS_Queue_Check(queue)        __OS_Queue_Check(queue)

// Clear queue
#define OS_Queue_Clear(queue)        (queue).Q.cFilled = 0



//------------------------------------------------------------------------------
// Send message via queue. If queue is full then most rearly message will be pushed out.

#define OS_Queue_Send_Now(queue, value)                                 \
    OSM_BEGIN {                                                         \
        __OS_QUEUE_DI();                                                \
        _OS_Queue_Send(&(queue), OST_CONVERT_TYPE_MSG(value));          \
        __OS_QUEUE_RI();                                                \
    } OSM_END


#define __OS_Queue_Accept(queue,os_msg_type_var)                        \
    OSM_BEGIN {                                                         \
        os_msg_type_var = _OS_Queue_Get(&(queue));                      \
    } OSM_END


#define OS_Queue_Accept(queue,os_msg_type_var)                          \
    OSM_BEGIN {                                                         \
        __OS_QUEUE_DI();                                                \
        __OS_Queue_Accept(queue,os_msg_type_var);                       \
        __OS_QUEUE_RI();                                                \
    } OSM_END


#define OS_Queue_Delete(queue)                                          \
    OSM_BEGIN {                                                         \
        __OS_QUEUE_DI();                                                \
        _OS_Queue_Get(&(queue));                                        \
        __OS_QUEUE_RI();                                                \
    } OSM_END


#define OS_Queue_Get(queue,os_msg_type_var)                             \
    OSM_BEGIN {                                                         \
        __OS_QUEUE_DI();                                                \
        os_msg_type_var = (queue).pMsg[(queue).Q.cBegin];               \
        __OS_QUEUE_RI();                                                \
    } OSM_END



//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT_QUEUE)
//------------------------------------------------------------------------------


    extern void     _OS_Queue_Send_I (OST_QUEUE *pQueue, OST_MSG Msg);
    extern OST_MSG  _OS_Queue_Get_I  (OST_QUEUE *pQueue);

    #define OS_Queue_Send_I(queue, value)   _OS_Queue_Send_I(&(queue), OST_CONVERT_TYPE_MSG(value))
    #define OS_Queue_Accept_I(queue,os_msg_type_var)    os_msg_type_var = _OS_Queue_Get_I(&(queue))
    #define OS_Queue_Check_I(queue)         __OS_Queue_Check(queue)
    #define OS_Queue_Clear_I(queue)         OS_Queue_Clear(queue)
    #define OS_Queue_IsFull_I(queue)        __OS_Queue_IsFull(queue)

    #define OS_Queue_Delete_I(queue,os_msg_type_var)    _OS_Queue_Get_I(&(queue))   
    #define OS_Queue_Get_I(queue,os_msg_type_var)       os_msg_type_var = (queue).pMsg[(queue).Q.cBegin] 

    //------------------------------------------------------------------------------
    // Wait message from queue. After accepting message will be deleted from queue.
    
    #define OS_Queue_Wait(queue, os_msg_type_var)                           \
        OSM_BEGIN {                                                         \
            for (;;) {                                                      \
                OS_Wait(__OS_Queue_Check(queue));                           \
                __OS_QUEUE_DI();                                            \
                if (__OS_Queue_Check(queue)) break;                         \
                __OS_QUEUE_RI();                                            \
            }                                                               \
            __OS_Queue_Accept(queue,os_msg_type_var);                       \
            __OS_QUEUE_RI();                                                \
        } OSM_END
    
    //------------------------------------------------------------------------------
    // Wait message from queue. After accepting message will be deleted from queue. Exit if timeout expired.
    
    #define OS_Queue_Wait_TO(queue, os_msg_type_var, timeout)               \
        OSM_BEGIN {                                                         \
            for (;;) {                                                      \
                OS_Wait_TO(__OS_Queue_Check(queue), timeout);               \
                __OS_QUEUE_DI();                                            \
                if (__OS_Queue_Check(queue) || OS_IsTimeout()) break;       \
                __OS_QUEUE_RI();                                            \
            }                                                               \
            if (!OS_IsTimeout()) __OS_Queue_Accept(queue,os_msg_type_var);  \
            __OS_QUEUE_RI();                                                \
        } OSM_END

    // Send message via queue. If queue full then wait for free place
    
    #define OS_Queue_Send(queue, value)                                     \
        OSM_BEGIN {                                                         \
            __OS_QUEUE_DI();                                                \
            while (__OS_Queue_IsFull(queue))                                \
            {                                                               \
                __OS_QUEUE_RI();                                            \
                OS_Wait(!__OS_Queue_IsFull(queue));                         \
                __OS_QUEUE_DI();                                            \
            }                                                               \
            _OS_Queue_Send(&(queue), OST_CONVERT_TYPE_MSG(value));          \
            __OS_QUEUE_RI();                                                \
        } OSM_END
    
    
    //------------------------------------------------------------------------------
    // Send message via queue. If queue full then wait for free place. Exit if timeout expired.
    
    #define OS_Queue_Send_TO(queue, value, timeout)                         \
        OSM_BEGIN {                                                         \
            _OS_Flags.bTimeout = 0;                                         \
            __OS_QUEUE_DI();                                                \
            while (__OS_Queue_IsFull(queue) && !OS_IsTimeout())             \
            {                                                               \
                __OS_QUEUE_RI();                                            \
                OS_Wait_TO(!__OS_Queue_IsFull(queue), timeout);             \
                __OS_QUEUE_DI();                                            \
            }                                                               \
            if (!OS_IsTimeout()) {                                          \
                _OS_Queue_Send(&(queue), OST_CONVERT_TYPE_MSG(value));      \
            }                                                               \
            __OS_QUEUE_RI();                                                \
        } OSM_END


    // Check for queue is full
    
    #define OS_Queue_IsFull(queue)  (__OS_QUEUE_DI(),_OS_Temp = __OS_Queue_IsFull(queue),__OS_QUEUE_RI(),_OS_Temp)
    
    

//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    // Wait message from queue. After accepting message will be deleted from queue.
    
    #define OS_Queue_Wait(queue, os_msg_type_var)                           \
        OSM_BEGIN {                                                         \
            OS_Wait(__OS_Queue_Check(queue));                               \
            __OS_Queue_Accept(queue,os_msg_type_var);                       \
        } OSM_END
    
    //------------------------------------------------------------------------------
    // Wait message from queue. After accepting message will be deleted from queue. Exit if timeout expired.
    
    #define OS_Queue_Wait_TO(queue, os_msg_type_var, timeout)               \
        OSM_BEGIN {                                                         \
            OS_Wait_TO(__OS_Queue_Check(queue), timeout);                   \
            if (!OS_IsTimeout()) __OS_Queue_Accept(queue,os_msg_type_var);  \
        } OSM_END
    
    // Send message via queue. If queue full then wait for free place
    
    #define OS_Queue_Send(queue, value)                                     \
        OSM_BEGIN {                                                         \
            if (__OS_Queue_IsFull(queue))                                   \
            {                                                               \
                OS_Wait(!__OS_Queue_IsFull(queue));                         \
            }                                                               \
            _OS_Queue_Send(&(queue), OST_CONVERT_TYPE_MSG(value));          \
        } OSM_END
    
    
    //------------------------------------------------------------------------------
    // Send message via queue. If queue full then wait for free place. Exit if timeout expired.
    
    #define OS_Queue_Send_TO(queue, value, timeout)                         \
        OSM_BEGIN {                                                         \
            _OS_Flags.bTimeout = 0;                                         \
            if (__OS_Queue_IsFull(queue))                                   \
            {                                                               \
                OS_Wait_TO(!__OS_Queue_IsFull(queue), timeout);             \
            }                                                               \
            if (!OS_IsTimeout()) {                                          \
                _OS_Queue_Send(&(queue), OST_CONVERT_TYPE_MSG(value));      \
            }                                                               \
        } OSM_END

    // Check for queue is full
    #define OS_Queue_IsFull(queue)  __OS_Queue_IsFull(queue)
    
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------

    
//------------------------------------------------------------------------------
//



//------------------------------------------------------------------------------
#endif      // OS_ENABLE_QUEUE
//------------------------------------------------------------------------------


//******************************************************************************
//  END OF FILE osa_queue.h
//******************************************************************************

