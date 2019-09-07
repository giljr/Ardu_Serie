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
 *  File:           osa_smsg.h
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    Services for work with simple messages
 *                  This file directly included in osa.h
 *
 *  History:        15.09.2010 -    File updated
 *
 ************************************************************************************************
 */





/************************************************************************************************
 *                                                                                              *
 *                              S I M P L E     M E S S A G E S                                 *
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



#if defined(OS_ENABLE_INT_SMSG) && ((OS_SMSG_SIZE != 1) || defined(OS_PROTECT_MEMORY_ACCESS))

    #define __OS_SMSG_DI()       _OS_DI_INT()
    #define __OS_SMSG_RI()       _OS_RI_INT()

    #if defined(OS_PROTECT_MEMORY_ACCESS)           
        #define __OS_SMSG_DI_A()    _OS_DI_INT()
        #define __OS_SMSG_RI_A()    _OS_RI_INT()
    #else
        #define __OS_SMSG_DI_A()
        #define __OS_SMSG_RI_A()
    #endif

#else

    #define __OS_SMSG_DI()
    #define __OS_SMSG_RI()
    #define __OS_SMSG_DI_A()
    #define __OS_SMSG_RI_A()

#endif


#define OS_SMSG_ATOMIC_WRITE_A(expr)    \
    OSM_BEGIN {                         \
        __OS_SMSG_DI_A();               \
        expr;                           \
        __OS_SMSG_RI_A();               \
    } OSM_END


//------------------------------------------------------------------------------



#define __OS_Smsg_Check(smsg)   ((smsg) != (OST_SMSG)0)
#define __OS_Smsg_IsFree(smsg)  ((smsg) == (OST_SMSG)0)


#if defined(OS_ENABLE_INT_SMSG) && ((OS_SMSG_SIZE != 1) || defined(OS_PROTECT_MEMORY_ACCESS))
    // Check for message exists
    #define OS_Smsg_Check(smsg)       (__OS_SMSG_DI(), _OS_Temp = __OS_Smsg_Check(smsg), __OS_SMSG_RI(), _OS_Temp)

    // Check for message is free
    #define OS_Smsg_IsFree(smsg)      (__OS_SMSG_DI(), _OS_Temp = __OS_Smsg_IsFree(smsg), __OS_SMSG_RI(), _OS_Temp)

#else

    // Check for message exists
    #define OS_Smsg_Check(smsg)       __OS_Smsg_Check(smsg)

    // Check for message is free
    #define OS_Smsg_IsFree(smsg)      __OS_Smsg_IsFree(smsg)

#endif


//------------------------------------------------------------------------------
// Internal macro: Clear message (internal system service)
//------------------------------------------------------------------------------
#define __OS_Smsg_MakeFree(smsg)       smsg = (OST_SMSG)0

//------------------------------------------------------------------------------
// Create simple message
//------------------------------------------------------------------------------

#define OS_Smsg_Create(smsg)                                            \
    OSM_BEGIN {                                                         \
        __OS_SMSG_DI();                                                 \
        __OS_Smsg_MakeFree(smsg);                                       \
        __OS_SMSG_RI();                                                 \
    } OSM_END


//------------------------------------------------------------------------------
// Send simple message. If message already exists then it will be overwritten.
//------------------------------------------------------------------------------

#define OS_Smsg_Send_Now(smsg, message)                                 \
    OSM_BEGIN {                                                         \
        _OS_Flags.bEventError = 0;                                      \
        __OS_SMSG_DI();                                                 \
        if (__OS_Smsg_Check(smsg)) _OS_Flags.bEventError = 1;           \
        smsg = (OST_SMSG) (message);                                    \
        __OS_SMSG_RI();                                                 \
    } OSM_END


//------------------------------------------------------------------------------
// Accept existing simple message. After accepting message is cleared.
//------------------------------------------------------------------------------

#define __OS_Smsg_Accept(smsg, os_smsg_type_var)                        \
    OSM_BEGIN {                                                         \
        os_smsg_type_var = smsg;                                        \
        __OS_Smsg_MakeFree(smsg);                                       \
    } OSM_END


#define OS_Smsg_Accept(smsg, os_smsg_type_var)                          \
    OSM_BEGIN {                                                         \
        __OS_SMSG_DI();                                                 \
        __OS_Smsg_Accept(smsg, os_smsg_type_var);                       \
        __OS_SMSG_RI();                                                 \
    } OSM_END

//------------------------------------------------------------------------------
// Get message. Message remains existing.    
//------------------------------------------------------------------------------

#define OS_Smsg_Get(smsg, os_smsg_type_var)                             \
    OSM_BEGIN {                                                         \
        __OS_SMSG_DI();                                                 \
        os_smsg_type_var = smsg;                                        \
        __OS_SMSG_RI();                                                 \
    } OSM_END



//------------------------------------------------------------------------------
#ifdef OS_ENABLE_INT_SMSG
//------------------------------------------------------------------------------

    // Send simple message from interrupt. If message already exists then it will be overwritten.
    #define OS_Smsg_Send_I(smsg, message)                               \
        OSM_BEGIN {                                                     \
            _OS_Flags.bEventError = 0;                                  \
            if (OS_Smsg_Check_I(smsg)) _OS_Flags.bEventError = 1;       \
            smsg = (OST_SMSG) (message);                                \
        } OSM_END

    #define OS_Smsg_Accept_I(smsg, os_smsg_type_var)    __OS_Smsg_Accept(smsg, os_smsg_type_var)
    #define OS_Smsg_Get_I(smsg, os_smsg_type_var)       os_smsg_type_var = smsg     

    //#define OS_Smsg_MakeFree_I(smsg)                    __OS_Smsg_MakeFree(smsg)
    #define OS_Smsg_Check_I(smsg)                       __OS_Smsg_Check(smsg)
    #define OS_Smsg_IsFree_I(smsg)                      __OS_Smsg_IsFree(smsg)


    //------------------------------------------------------------------------------
    // Send simple message. If message already exists then wait when it became free.
    //------------------------------------------------------------------------------

    #define OS_Smsg_Send(smsg, message)                                 \
        OSM_BEGIN {                                                     \
            __OS_SMSG_DI();                                             \
            while (__OS_Smsg_Check(smsg)) {                             \
                __OS_SMSG_RI();                                         \
                OS_Wait(__OS_Smsg_IsFree(smsg));                        \
                __OS_SMSG_DI();                                         \
            }                                                           \
            smsg = (OST_SMSG) (message);                                \
            __OS_SMSG_RI();                                             \
        } OSM_END

    //------------------------------------------------------------------------------
    // Send simple message. If message already exists then wait when it became free.
    // Exit if timeout expired.
    //------------------------------------------------------------------------------

    #define OS_Smsg_Send_TO(smsg, message, timeout)                     \
        OSM_BEGIN {                                                     \
            _OS_Flags.bTimeout = 0;                                     \
            __OS_SMSG_DI();                                             \
            while (__OS_Smsg_Check(smsg) && !OS_IsTimeout())            \
            {                                                           \
                __OS_SMSG_RI();                                         \
                OS_Wait_TO(__OS_Smsg_IsFree(smsg), timeout);            \
                __OS_SMSG_DI();                                         \
            }                                                           \
            if (!OS_IsTimeout()) {                                      \
                smsg = (OST_SMSG) (message);                            \
            }                                                           \
            __OS_SMSG_RI();                                             \
        } OSM_END


    //------------------------------------------------------------------------------
    // Wait for simple message. After accepting simple message is cleared.
    //------------------------------------------------------------------------------

    #define OS_Smsg_Wait(smsg, os_smsg_type_var)                        \
        OSM_BEGIN {                                                     \
            for (;;) {                                                  \
                OS_Wait(__OS_Smsg_Check(smsg));                         \
                __OS_SMSG_DI();                                         \
                if (__OS_Smsg_Check(smsg)) break;                       \
                __OS_SMSG_RI();                                         \
            }                                                           \
            __OS_Smsg_Accept(smsg, os_smsg_type_var);                   \
            __OS_SMSG_RI();                                             \
        } OSM_END


    //------------------------------------------------------------------------------
    // Wait for simple message. After accepting simple message is cleared.
    // Exit if timeout expired.
    //------------------------------------------------------------------------------

    #define OS_Smsg_Wait_TO(smsg, os_smsg_type_var, timeout)            \
        OSM_BEGIN {                                                     \
            for (;;) {                                                  \
                OS_Wait_TO(__OS_Smsg_Check(smsg), timeout);             \
                __OS_SMSG_DI();                                         \
                if (__OS_Smsg_Check(smsg) || OS_IsTimeout()) break;     \
                __OS_SMSG_RI();                                         \
            }                                                           \
            if (!OS_IsTimeout()) {                                      \
                __OS_Smsg_Accept(smsg, os_smsg_type_var);               \
            }                                                           \
            __OS_SMSG_RI();                                             \
        } OSM_END


//------------------------------------------------------------------------------
#else   // OS_ENABLE_INT_SMSG
//------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    // Send simple message. If message already exists then wait when it became free.
    //------------------------------------------------------------------------------

    #define OS_Smsg_Send(smsg, message)                                 \
        OSM_BEGIN {                                                     \
            if (__OS_Smsg_Check(smsg))                                  \
                OS_Wait(__OS_Smsg_IsFree(smsg));                        \
            smsg = (OST_SMSG) (message);                                \
        } OSM_END

    //------------------------------------------------------------------------------
    // Send simple message. If message already exists then wait when it became free.
    // Exit if timeout expired.
    //------------------------------------------------------------------------------

    #define OS_Smsg_Send_TO(smsg, message, timeout)                     \
        OSM_BEGIN {                                                     \
            _OS_Flags.bTimeout = 0;                                     \
            if (__OS_Smsg_Check(smsg))                                  \
            {                                                           \
                OS_Wait_TO(__OS_Smsg_IsFree(smsg), timeout);            \
            }                                                           \
            if (!OS_IsTimeout()) {                                      \
                smsg = (OST_SMSG) (message);                            \
            }                                                           \
        } OSM_END


    //------------------------------------------------------------------------------
    // Wait for simple message. After accepting simple message is cleared.
    //------------------------------------------------------------------------------

    #define OS_Smsg_Wait(smsg, os_smsg_type_var)                        \
        OSM_BEGIN {                                                     \
            OS_Wait(__OS_Smsg_Check(smsg));                             \
            __OS_Smsg_Accept(smsg, os_smsg_type_var);                   \
        } OSM_END


    //------------------------------------------------------------------------------
    // Wait for simple message. After accepting simple message is cleared.
    // Exit if timeout expired.
    //------------------------------------------------------------------------------

    #define OS_Smsg_Wait_TO(smsg, os_smsg_type_var, timeout)            \
        OSM_BEGIN {                                                     \
            OS_Wait_TO(__OS_Smsg_Check(smsg), timeout);                 \
            if (!OS_IsTimeout()) {                                      \
                __OS_Smsg_Accept(smsg, os_smsg_type_var);               \
            }                                                           \
        } OSM_END



//------------------------------------------------------------------------------
#endif  // OS_ENABLE_INT_SMSG
//------------------------------------------------------------------------------


//******************************************************************************
//  Alternate names
//******************************************************************************

#if !defined(__OSACCS__) && !defined(__OSAMIKROC__)

#define OS_SMsg_Create          OS_Smsg_Create
#define OS_SMsg_Check           OS_Smsg_Check
#define OS_SMsg_Accept          OS_Smsg_Accept
#define OS_SMsg_Send            OS_Smsg_Send
#define OS_SMsg_Send_TO         OS_Smsg_Send_TO
#define OS_SMsg_Send_Now        OS_Smsg_Send_Now
#define OS_SMsg_Send_I          OS_Smsg_Send_I
#define OS_SMsg_Wait            OS_Smsg_Wait
#define OS_SMsg_Wait_TO         OS_Smsg_Wait_TO
#define OS_SMsg_IsFree          OS_Smsg_IsFree


#endif




//******************************************************************************
//  END OF FILE osa_smsg.h
//******************************************************************************

