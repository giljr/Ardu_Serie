/*
 ***********************************************************************************************
 *                                                                                              
 *  OSA cooperative RTOS for Microchip PIC-controllers: PIC10/12/16/18/24/dsPIC                 
 *                                                                                              
 *  OSA cooperative RTOS for microcontrollers PIC (Microchip) and 8-bit AVR (Atmel)
 *                                                                                              
 *  URL:        http://wiki.pic24.ru/doku.php/en/osa/ref/intro                                  
 *              http://picosa.narod.ru                                                          
 *                                                                                              
 *----------------------------------------------------------------------------------------------
 *                                                                                              
 *  File:       osa_csem.h                                                                      
 *                                                                                              
 *  Programmer: Timofeev Victor                                                                 
 *              osa@pic24.ru, testerplus@mail.ru                                                
 *                                                                                              
 *  Definition: Services for work with counting semaphores                                      
 *                                                                                              
 *  History:    21.01.2009                                                                      
 *              21.03.2009 -    Enable/disable interrupt actions on SignalCsem are placed incide
 *                              macro OS_Csem_Signal and deleted from _OS_Csem_Signal           
 *                                                                                              
 *              10.12.2009 -    1. Services for interrupt were corrected.
 *                              2. Service OS_Csem_Signal waits for semaphore is free.
 *                                 Service OS_Csem_Signal_Now added
 * 
 *              31.05.2010 -    3. OS_Csem_Check returns boolean value now
 *                                                                                              
 ************************************************************************************************
 */


/************************************************************************************************
 *                                                                                              *
 *                            C O U N T I N G   S E M A P H O R E S                             *
 *                                                                                              *
 ************************************************************************************************/

//------------------------------------------------------------------------------
#ifdef OS_ENABLE_CSEM
//------------------------------------------------------------------------------


//******************************************************************************
//  VARIABLES
//******************************************************************************


//******************************************************************************
//  FUNCTION PROTOTYPES
//******************************************************************************

extern void _OS_Csem_Signal(OST_CSEM *pCSem);

#ifdef OS_ENABLE_INT_CSEM
extern void _OS_Csem_Signal_I(OST_CSEM *pCSem);
#endif


//******************************************************************************
//  MACROS
//******************************************************************************




// These services are used in countsem functions (see OSA.C)

#if defined(OS_ENABLE_INT_CSEM) && ((OS_CSEM_SIZE > 1) || defined(OS_PROTECT_MEMORY_ACCESS)) 

    #define __OS_CSEM_DI()      _OS_DI_INT() 
    #define __OS_CSEM_RI()      _OS_RI_INT() 

#else

    #define __OS_CSEM_DI()
    #define __OS_CSEM_RI()

#endif



#define __OS_Csem_Accept(countsem)              countsem--
#define __OS_Csem_Check(countsem)              (countsem != 0)      
#define __OS_Csem_SetValue(countsem,value)      countsem = value


// Increase counting semaphore's value by 1

#define OS_Csem_Signal(countsem)                                        \
    OSM_BEGIN {                                                         \
        for (;;) {                                                      \
            __OS_CSEM_DI();                                             \
            _OS_Csem_Signal(&(countsem));                               \
            __OS_CSEM_RI();                                             \
            if (!OS_IsEventError()) break;                              \
            OS_Yield();                                                 \
        }                                                               \
    } OSM_END



// Increase counting semaphore's value by 1

#define OS_Csem_Signal_Now(countsem)                                    \
    OSM_BEGIN {                                                         \
        __OS_CSEM_DI();                                                 \
        _OS_Csem_Signal(&(countsem));                                   \
        __OS_CSEM_RI();                                                 \
    } OSM_END


// Set counting semaphore's value to given value


#define OS_Csem_SetValue(countsem,value)                                \
    OSM_BEGIN {                                                         \
        __OS_CSEM_DI();                                                 \
        __OS_Csem_SetValue(countsem,value);                             \
        __OS_CSEM_RI();                                                 \
    } OSM_END


// Accepting counting semaphore. Decrease it's value by 1.

#define OS_Csem_Accept(countsem)                                        \
    OSM_BEGIN {                                                         \
        __OS_CSEM_DI();                                                 \
        countsem--;                                                     \
        __OS_CSEM_RI();                                                 \
    } OSM_END



// Set counting semaphore's value to 1.
#define OS_Csem_Set(countsem)    OS_Csem_SetValue(countsem, 1)

// Clear counting semaphore
#define OS_Csem_Reset(countsem)  OS_Csem_SetValue(countsem, 0)

#define OS_Csem_Create(countsem) OS_Csem_SetValue(countsem, 0)


//------------------------------------------------------------------------------
#if (OS_CSEM_SIZE == 1) || (!defined(OS_ENABLE_INT_CSEM))

    // Check counting semaphore for non zero value
    #define OS_Csem_Check(countsem)     __OS_Csem_Check(countsem)

#else

    // Check counting semaphore for non zero value

    #if defined(OS_ENABLE_INT_CSEM)
        #define OS_Csem_Check(countsem) (__OS_CSEM_DI(),_OS_Temp = __OS_Csem_Check(countsem),__OS_CSEM_RI(),_OS_Temp)
    #else
        #define OS_Csem_Check(countsem) __OS_Csem_Check(countsem)
    #endif

#endif
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#ifdef OS_ENABLE_INT_CSEM

    // Set counting semaphore's value to given value
    #define OS_Csem_SetValue_I(countsem,value)          countsem = value

    // Set counting semaphore's value to 1
    #define OS_Csem_Set_I(countsem)                     __OS_Csem_SetValue(countsem, 1)

    // Clear counting semaphore
    #define OS_Csem_Reset_I(countsem)                   __OS_Csem_SetValue(countsem, 0)

    // Check counting semaphore for non zero value
    #define OS_Csem_Check_I(countsem)                   __OS_Csem_Check(countsem)

    // Accept counting semaphore. Decrease it's value by 1
    #define OS_Csem_Accept_I(countsem)                  __OS_Csem_Accept(countsem)

    // Increase counting semaphore's value by 1
    #define OS_Csem_Signal_I(countsem)                  _OS_Csem_Signal_I(&countsem)             

#endif


//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT_CSEM)
//------------------------------------------------------------------------------

    // Wait counting semaphore
    #define OS_Csem_Wait(countsem)                                          \
        OSM_BEGIN {                                                         \
            for (;;) {                                                      \
                OS_Wait(__OS_Csem_Check(countsem));                  \
                __OS_CSEM_DI();                                             \
                if (__OS_Csem_Check(countsem)) break;                       \
                __OS_CSEM_RI();                                             \
            }                                                               \
            __OS_Csem_Accept(countsem);                                     \
            __OS_CSEM_RI();                                                 \
        } OSM_END

    // Wait counting semaphore. Exit if timeout expired.
    #define OS_Csem_Wait_TO(countsem, timeout)                              \
        OSM_BEGIN {                                                         \
            for (;;) {                                                      \
                OS_Wait_TO(__OS_Csem_Check(countsem), timeout); \
                __OS_CSEM_DI();                                             \
                if (__OS_Csem_Check(countsem) || OS_IsTimeout()) break;     \
                __OS_CSEM_RI();                                             \
            }                                                               \
            if (!OS_IsTimeout()) {                                          \
                __OS_Csem_Accept(countsem);                                 \
            }                                                               \
            __OS_CSEM_RI();                                                 \
        } OSM_END

//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------

    // Wait counting semaphore
    #define OS_Csem_Wait(countsem)                                          \
        OSM_BEGIN {                                                         \
            OS_Wait(__OS_Csem_Check(countsem));                      \
            __OS_Csem_Accept(countsem);                                     \
        } OSM_END

    // Wait counting semaphore. Exit if timeout expired.
    #define OS_Csem_Wait_TO(countsem, timeout)                              \
        OSM_BEGIN {                                                         \
            OS_Wait_TO(__OS_Csem_Check(countsem), timeout);     \
            if (!OS_IsTimeout()) {                                          \
                __OS_Csem_Accept(countsem);                                 \
            }                                                               \
        } OSM_END

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
#endif  // OS_ENABLE_CSEM
//------------------------------------------------------------------------------


//******************************************************************************
//  END OF FILE osa_csem.h
//******************************************************************************



