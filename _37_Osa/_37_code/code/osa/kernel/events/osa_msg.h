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
 *  File:           osa_msg.h
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    Services for messages
 *                  This file directly included in osa.h
 *
 *  History:        15.09.2010 -    File updated
 *
 ************************************************************************************************
 */



/************************************************************************************************
 *                                                                                              *
 *                               P O I N T E R S   T O   M E S S A G E S                        *
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


// These macro used in services
#if defined(OS_ENABLE_INT_MSG)

    #define __OS_MSG_DI()       _OS_DI_INT()
    #define __OS_MSG_RI()       _OS_RI_INT()

#else

    #define __OS_MSG_DI()
    #define __OS_MSG_RI()

#endif


#ifdef __OSACCS__                   // When OST_MSG = const char*, expressions like:
                                    // (OST_MSG)(message)
                                    // loose high byte of an address
                                    // At this time (91210) it doesn't matter because of
                                    // CCS bugs. Expression pointer1 = pointer2 copies only
                                    // low byte. May be in future this bug will be fixed
    #define OST_CONVERT_TYPE_MSG
#else
    #define OST_CONVERT_TYPE_MSG    (OST_MSG)
#endif

//------------------------------------------------------------------------------
// Check for message exists

#define OS_Msg_Check(msg_cb)          ((msg_cb).status & 1)
#define OS_Msg_Check2(msg_cb)         ((msg_cb).status)

//------------------------------------------------------------------------------
// Create message and zero it.
//------------------------------------------------------------------------------

#define OS_Msg_Create(msg_cb)                       \
    OSM_BEGIN {                                     \
        __OS_MSG_DI();                              \
        (msg_cb).status = 0;                        \
        (msg_cb).msg = OST_CONVERT_TYPE_MSG 0;      \
        __OS_MSG_RI();                              \
    } OSM_END

//------------------------------------------------------------------------------
// Make message free
//------------------------------------------------------------------------------

#define OS_Msg_Clear(msg_cb)          (msg_cb).status = 0


//------------------------------------------------------------------------------
// Send message. If message is already exists then it will be overwritten.
//------------------------------------------------------------------------------

#define OS_Msg_Send_Now(msg_cb, value)                                          \
    OSM_BEGIN {                                                                 \
        _OS_Flags.bEventError = 0;                                              \
        __OS_MSG_DI();                                                          \
        if (OS_Msg_Check(msg_cb)) _OS_Flags.bEventError = 1;                    \
        (msg_cb).status = 1;                                                    \
        (msg_cb).msg = OST_CONVERT_TYPE_MSG (value);                            \
        __OS_MSG_RI();                                                          \
    } OSM_END

//------------------------------------------------------------------------------
// Accept existing message. After accepting message is set free.
//------------------------------------------------------------------------------

#define OS_Msg_Accept(msg_cb, os_msg_type_var)                          \
    OSM_BEGIN {                                                         \
        __OS_MSG_DI();                                                  \
        os_msg_type_var = (msg_cb).msg;                                 \
        OS_Msg_Clear(msg_cb);                                           \
        __OS_MSG_RI();                                                  \
    } OSM_END

//------------------------------------------------------------------------------
// Get pointer to message. Message remains existing.    
//------------------------------------------------------------------------------

#define OS_Msg_Get(msg_cb, os_msg_type_var)                             \
    OSM_BEGIN {                                                         \
        __OS_MSG_DI();                                                  \
        os_msg_type_var = (msg_cb).msg;                                 \
        OS_Msg_Clear(msg_cb);                                           \
        __OS_MSG_RI();                                                  \
    } OSM_END





//------------------------------------------------------------------------------
#ifdef OS_ENABLE_INT_MSG
//------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    // Send message from interrupt. If message is already exists then it will be overwritten.
    //------------------------------------------------------------------------------
    #define OS_Msg_Send_I(msg_cb, value)                                    \
        OSM_BEGIN {                                                         \
            _OS_Flags.bEventError = 0;                                       \
            if (OS_Msg_Check_I(msg_cb)) _OS_Flags.bEventError = 1;           \
            (msg_cb).status = 1;                                            \
            (msg_cb).msg = OST_CONVERT_TYPE_MSG (value);                    \
        } OSM_END

    //------------------------------------------------------------------------------
    // Accept existing message. After accepting message is set free.
    //------------------------------------------------------------------------------
    #define OS_Msg_Accept_I(msg_cb, os_msg_type_var)                        \
        OSM_BEGIN {                                                         \
            os_msg_type_var = (msg_cb).msg;                                 \
            OS_Msg_Clear_I(msg_cb);                                         \
        } OSM_END

    //------------------------------------------------------------------------------
    // Get existing message. Message slill remains existing
    //------------------------------------------------------------------------------
    #define OS_Msg_Get_I(msg_cb, os_msg_type_var)                           \
        OSM_BEGIN {                                                         \
            os_msg_type_var = (msg_cb).msg;                                 \
        } OSM_END


    #define OS_Msg_Check_I(msg_cb)      OS_Msg_Check(msg_cb)
    #define OS_Msg_Clear_I(msg_cb)      OS_Msg_Clear(msg_cb)


    //------------------------------------------------------------------------------
    // Send message. If message is already exists then wait when it became free.
    //------------------------------------------------------------------------------

    #define OS_Msg_Send(msg_cb, value)                                      \
        OSM_BEGIN {                                                         \
            __OS_MSG_DI();                                                  \
            while (OS_Msg_Check(msg_cb))                                    \
            {                                                               \
                __OS_MSG_RI();                                              \
                OS_Wait(!OS_Msg_Check(msg_cb));                      \
                __OS_MSG_DI();                                              \
            };                                                              \
            (msg_cb).status = 1;                                            \
            (msg_cb).msg = OST_CONVERT_TYPE_MSG (value);                    \
            __OS_MSG_RI();                                                  \
        } OSM_END

    //------------------------------------------------------------------------------
    // Send message. If message is already exists then wait when it became free. Exit if timeout expired.
    //------------------------------------------------------------------------------

    #define OS_Msg_Send_TO(msg_cb, value,timeout)                               \
        OSM_BEGIN {                                                             \
            _OS_Flags.bTimeout = 0;                                              \
            __OS_MSG_DI();                                                      \
            while (OS_Msg_Check(msg_cb) && !OS_IsTimeout())                     \
            {                                                                   \
                __OS_MSG_RI();                                                  \
                OS_Wait_TO(!OS_Msg_Check(msg_cb), timeout);         \
                __OS_MSG_DI();                                                  \
            };                                                                  \
            if (!OS_IsTimeout()){                                               \
                (msg_cb).status = 1;                                            \
                (msg_cb).msg = OST_CONVERT_TYPE_MSG (value);                    \
            }                                                                   \
            __OS_MSG_RI();                                                      \
        } OSM_END

    //------------------------------------------------------------------------------
    // Wait for message. After waiting message is set free
    //------------------------------------------------------------------------------

    #define OS_Msg_Wait(msg_cb, os_msg_type_var)                        \
        OSM_BEGIN {                                                     \
            for (;;) {                                                  \
                OS_Wait(OS_Msg_Check(msg_cb));                   \
                __OS_MSG_DI();                                          \
                if (OS_Msg_Check(msg_cb)) break;                        \
                __OS_MSG_RI();                                          \
            }                                                           \
            os_msg_type_var = (msg_cb).msg;                             \
            OS_Msg_Clear(msg_cb);                                       \
            __OS_MSG_RI();                                              \
        } OSM_END

    //------------------------------------------------------------------------------
    // Wait for mesage. Exit if timeout expired.
    //------------------------------------------------------------------------------
    #define OS_Msg_Wait_TO(msg_cb, os_msg_type_var,timeout)             \
        OSM_BEGIN {                                                     \
            for (;;) {                                                  \
                OS_Wait_TO(OS_Msg_Check(msg_cb), timeout);  \
                __OS_MSG_DI();                                          \
                if (OS_Msg_Check(msg_cb) || OS_IsTimeout()) break;      \
                __OS_MSG_RI();                                          \
            }                                                           \
            if (!OS_IsTimeout()) {                                      \
                os_msg_type_var = (msg_cb).msg;                         \
                OS_Msg_Clear(msg_cb);                                   \
            }                                                           \
            __OS_MSG_RI();                                              \
        } OSM_END

//------------------------------------------------------------------------------
#else   // OS_ENABLE_INT_MSG
//------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    // Send message. If message is already exists then wait when it became free.
    //------------------------------------------------------------------------------

    #define OS_Msg_Send(msg_cb, value)                                              \
        OSM_BEGIN {                                                                 \
            if (OS_Msg_Check(msg_cb)) OS_Wait(!OS_Msg_Check(msg_cb));        \
            (msg_cb).status = 1;                                                    \
            (msg_cb).msg = OST_CONVERT_TYPE_MSG (value);                            \
        } OSM_END

    //------------------------------------------------------------------------------
    // Send message. If message is already exists then wait when it became free. Exit if timeout expired.
    //------------------------------------------------------------------------------

    #define OS_Msg_Send_TO(msg_cb, value,timeout)                                   \
        OSM_BEGIN {                                                                 \
            _OS_Flags.bTimeout = 0;                                                  \
            if (OS_Msg_Check(msg_cb))                                               \
            {                                                                       \
                OS_Wait_TO(!OS_Msg_Check(msg_cb), timeout);             \
            }                                                                       \
            if (!OS_IsTimeout()){                                                   \
                (msg_cb).status = 1;                                                \
                (msg_cb).msg = OST_CONVERT_TYPE_MSG (value);                        \
            }                                                                       \
        } OSM_END

    //------------------------------------------------------------------------------
    // Wait for message. After waiting message is set free
    //------------------------------------------------------------------------------

    #define OS_Msg_Wait(msg_cb, os_msg_type_var)                            \
        OSM_BEGIN {                                                         \
            OS_Wait(OS_Msg_Check(msg_cb));                           \
            os_msg_type_var = (msg_cb).msg;                                 \
            OS_Msg_Clear(msg_cb);                                           \
        } OSM_END


    //------------------------------------------------------------------------------
    // Wait for mesage. Exit if timeout expired.
    //------------------------------------------------------------------------------

    #define OS_Msg_Wait_TO(msg_cb, os_msg_type_var,timeout)                 \
        OSM_BEGIN {                                                         \
            os_msg_type_var = OST_CONVERT_TYPE_MSG 0;                       \
            OS_Wait_TO(OS_Msg_Check(msg_cb), timeout);          \
            if (!OS_IsTimeout()) {                                          \
                os_msg_type_var = (msg_cb).msg;                             \
                OS_Msg_Clear(msg_cb);                                       \
            }                                                               \
        } OSM_END


//------------------------------------------------------------------------------
#endif  // OS_ENABLE_INT_MSG
//------------------------------------------------------------------------------






//******************************************************************************
//  END OF FILE osa_msg.h
//******************************************************************************

