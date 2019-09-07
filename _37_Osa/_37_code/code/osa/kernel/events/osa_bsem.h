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
 *  File:       osa_bsem.h
 *
 *  Programmer: Timofeev Victor
 *              osa@pic24.ru, testerplus@mail.ru
 *
 *  Definition: Services for work with binary semaphores
 *
 *  History:    21.01.2009
 * 
 *              10.12.2009      1. Services for interrupt were corrected.
 *                                  Modified:
 *                                      OS_Bsem_Wait
 *                                      OS_Bsem_Wait_TO
 *                                  Added:
 *                                      OS_Bsem_Check_I
 *                                      OS_Bsem_Reset_I
 *                                      OS_Bsem_Set_I
 *                                      OS_Bsem_Switch_I
 *                                      OS_Bsem_Signal_I
 * 
 *
 ************************************************************************************************
 */

/************************************************************************************************
 *                                                                                              *
 *                          B I N A R Y    S E M A P H O R E S                                  *
 *                                                                                              *
 ************************************************************************************************/

#ifndef OS_BSEMS
    #define OS_BSEMS    0
#endif

//------------------------------------------------------------------------------
#if OS_BSEMS > 0
//------------------------------------------------------------------------------


//******************************************************************************
//  VARIABLES
//******************************************************************************

extern  volatile OS_BSEM_BANK OST_UINT  _OS_Bsems[(OS_BSEMS+_OST_INT_SIZE-1)/_OST_INT_SIZE];


//******************************************************************************
//  FUNCTION PROTOTYPES
//******************************************************************************


//******************************************************************************
//  MACROS
//******************************************************************************



#if defined(OS_ENABLE_INT_BSEM)

    #define __OS_BSEM_DI()          _OS_DI_INT()
    #define __OS_BSEM_RI()          _OS_RI_INT()

    #if defined(OS_PROTECT_MEMORY_ACCESS)           
        #define __OS_BSEM_DI_A()    _OS_DI_INT()
        #define __OS_BSEM_RI_A()    _OS_RI_INT()
    #else
        #define __OS_BSEM_DI_A()
        #define __OS_BSEM_RI_A()
    #endif

#else

    #define __OS_BSEM_DI()
    #define __OS_BSEM_RI()
    #define __OS_BSEM_DI_A()                        
    #define __OS_BSEM_RI_A()

#endif


#define OS_BSEM_ATOMIC_WRITE_A(expr)    \
    OSM_BEGIN {                         \
        __OS_BSEM_DI_A();               \
        expr;                           \
        __OS_BSEM_RI_A();               \
    } OSM_END


#define OS_BSEM_ATOMIC_READ_A(expr)                                     \
    (__OS_BSEM_DI_A(), _OS_Temp = (expr), __OS_BSEM_RI_A(), _OS_Temp)   \

//------------------------------------------------------------------------------


#define _OS_BSEM_POS(binsem)        ((binsem) >> _OST_INT_SHIFT)
#define _OS_BSEM_MASK(binsem)       (1 << ((binsem) & _OST_INT_MASK))



#define __OS_Bsem_Set(binsem)       _OS_Bsems[_OS_BSEM_POS(binsem)] |=  _OS_BSEM_MASK(binsem) 
#define __OS_Bsem_Reset(binsem)     _OS_Bsems[_OS_BSEM_POS(binsem)] &= ~_OS_BSEM_MASK(binsem) 
#define __OS_Bsem_Switch(binsem)    _OS_Bsems[_OS_BSEM_POS(binsem)] ^=  _OS_BSEM_MASK(binsem) 
#define __OS_Bsem_Check(binsem)    (_OS_Bsems[_OS_BSEM_POS(binsem)] &   _OS_BSEM_MASK(binsem))


// Set/reset/switch  binary semaphore

#define OS_Bsem_Set(binsem)           OS_BSEM_ATOMIC_WRITE_A(__OS_Bsem_Set(binsem))       
#define OS_Bsem_Reset(binsem)         OS_BSEM_ATOMIC_WRITE_A(__OS_Bsem_Reset(binsem))     
#define OS_Bsem_Signal(binsem)        OS_BSEM_ATOMIC_WRITE_A(__OS_Bsem_Set(binsem))       
#define OS_Bsem_Switch(binsem)        OS_BSEM_ATOMIC_WRITE_A(__OS_Bsem_Switch(binsem))    

// Check for binary semaphore is set

#define OS_Bsem_Check(binsem)   __OS_Bsem_Check(binsem)


// Inverce semaphore's value



//------------------------------------------------------------------------------
#if defined(OS_ENABLE_INT_BSEM)
//------------------------------------------------------------------------------

    // Wait binary semaphore

    #define OS_Bsem_Wait(binsem)                                            \
        OSM_BEGIN {                                                         \
            for (;;) {                                                      \
                OS_Wait(OS_Bsem_Check(binsem));                      \
                __OS_BSEM_DI();                                             \
                if (OS_Bsem_Check(binsem)) break;                           \
                __OS_BSEM_DI();                                             \
            }                                                               \
            __OS_Bsem_Reset(binsem);                                        \
            __OS_BSEM_RI();                                                 \
        } OSM_END



    // Wait binary semaphore. Exit if timeout expired

    #define OS_Bsem_Wait_TO(binsem, timeout)                                \
        OSM_BEGIN {                                                         \
            for (;;) {                                                      \
                OS_Wait_TO(OS_Bsem_Check(binsem), timeout);     \
                __OS_BSEM_DI();                                             \
                if (OS_Bsem_Check(binsem) || OS_IsTimeout()) break;         \
                __OS_BSEM_RI();                                             \
            }                                                               \
            __OS_Bsem_Reset(binsem);                                        \
            __OS_BSEM_RI();                                                 \
        } OSM_END


    #define OS_Bsem_Set_I(binsem)           __OS_Bsem_Set(binsem)       
    #define OS_Bsem_Reset_I(binsem)         __OS_Bsem_Reset(binsem)     
    #define OS_Bsem_Signal_I(binsem)        __OS_Bsem_Set(binsem)       
    #define OS_Bsem_Switch_I(binsem)        __OS_Bsem_Switch(binsem)    
    #define OS_Bsem_Check_I(binsem)         __OS_Bsem_Check(binsem)     


//------------------------------------------------------------------------------
#else   // OS_ENABLE_INT_BSEM
//------------------------------------------------------------------------------

    // Wait binary semaphore

    #define OS_Bsem_Wait(binsem)                                            \
        OSM_BEGIN {                                                         \
            OS_Wait(OS_Bsem_Check(binsem));                                 \
            __OS_Bsem_Reset(binsem);                                        \
        } OSM_END



    // Wait binary semaphore. Exit if timeout expired

    #define OS_Bsem_Wait_TO(binsem, timeout)                                \
        OSM_BEGIN {                                                         \
            OS_Wait_TO(OS_Bsem_Check(binsem), timeout);                     \
            __OS_Bsem_Reset(binsem);                                        \
        } OSM_END

//------------------------------------------------------------------------------
#endif  // OS_ENABLE_INT_BSEM
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#endif          // OS_BSEMS > 0
//------------------------------------------------------------------------------


//******************************************************************************
//  END OF FILE osa_bsem.h
//******************************************************************************

