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
 *  File:           osa_flag.h
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    Services for flags 
 *                  This file directly included in osa.h
 *
 *  History:        15.09.2010 -    File updated
 *
 ************************************************************************************************
 */





/************************************************************************************************/
/*                                                                                              */
/*                     F L A G S                                                                */
/*                                                                                              */
/************************************************************************************************/

//******************************************************************************
//  VARIABLES
//******************************************************************************


//******************************************************************************
//  FUNCTION PROTOTYPES
//******************************************************************************


//******************************************************************************
//  MACROS
//******************************************************************************



#if defined(OS_ENABLE_INT_FLAG)                     

    #define __OS_FLAG_DI()      _OS_DI_INT()
    #define __OS_FLAG_RI()      _OS_RI_INT()

#else

    #define __OS_FLAG_DI()
    #define __OS_FLAG_RI()

#endif

//------------------------------------------------------------------------------


#define OS_FLAG_ATOMIC_WRITE_A(expr)    \
    OSM_BEGIN {                         \
        __OS_FLAG_DI();                 \
        expr;                           \
        __OS_FLAG_RI();                 \
    } OSM_END


//------------------------------------------------------------------------------


// Create flags and clear all bits

#define OS_Flag_Create(flags)               flags = 0


// Set flags to given value

#define OS_Flag_Init(flags, value)          flags = value


// Set bits in flag by given mask

#define OS_Flag_Set(flags, mask)            OS_FLAG_ATOMIC_WRITE_A(flags |= mask)      

// Clear bits in flag by given mask

#define OS_Flag_Clear(flags, mask)          OS_FLAG_ATOMIC_WRITE_A(flags &= ~(mask))   


// Check for all bits in flag are set by given mask

#define OS_Flag_Check_AllOn(flags, mask)     (((flags) & (mask))==(mask))


// Check for any bit in flag is set by given mask

#define OS_Flag_Check_On(flags, mask)        ((flags) & (mask))


// Check for bits in flag are cleared by given mask

#define OS_Flag_Check_AllOff(flags, mask)    (((flags) & (mask)) == 0)


// Check for any bit in flag is cleared by given mask

#define OS_Flag_Check_Off(flags, mask)       (((flags) & (mask)) != (mask))


// Wait for all bits in flags are set by given mask

#define OS_Flag_Wait_AllOn(flags, mask)                                 \
    {                                                                   \
        OS_Wait(OS_Flag_Check_AllOn(flags, mask));                      \
    }


// Wait for any bit in flags is set by given mask

#define OS_Flag_Wait_On(flags, mask)                                    \
    {                                                                   \
        OS_Wait(OS_Flag_Check_On(flags, mask));                         \
    }


// Wait for all bits in flags are set by given mask. Exit if timeout expired.

#define OS_Flag_Wait_AllOn_TO(flags, mask, timeout)                     \
    {                                                                   \
        OS_Wait_TO(OS_Flag_Check_AllOn(flags, mask), timeout);          \
    }


// Wait for any bit in flags is set by given mask. Exit if timeout expired.

#define OS_Flag_Wait_On_TO(flags, mask, timeout)                        \
    {                                                                   \
        OS_Wait_TO(OS_Flag_Check_On(flags, mask), timeout);             \
    }


// Wait for all bits in flag are cleared by given mask

#define OS_Flag_Wait_AllOff(flags, mask)                                \
    {                                                                   \
        OS_Wait(OS_Flag_Check_AllOff(flags, mask));                     \
    }


// Wait for any bit in flag is cleared by given mask

#define OS_Flag_Wait_Off(flags, mask)                                   \
    {                                                                   \
        OS_Wait(OS_Flag_Check_Off(flags, mask));                        \
    }


// Wait for all bits in flag are cleared by given mask. Exit if timeout expired.

#define OS_Flag_Wait_AllOff_TO(flags, mask, timeout)                    \
    {                                                                   \
        OS_Wait_TO(OS_Flag_Check_AllOff(flags, mask), timeout);         \
    }

// Wait for any bit in flag is cleared by given mask. Exit if timeout expired.

#define OS_Flag_Wait_Off_TO(flags, mask, timeout)                       \
    {                                                                   \
        OS_Wait_TO(OS_Flag_Check_Off(flags, mask), timeout);            \
    }



#if defined(OS_ENABLE_INT_FLAG)                     

    #define OS_Flag_Set_I(flags, mask)          flags |= mask                              
    #define OS_Flag_Clear_I(flags, mask)        flags &= ~(mask)                           
    #define OS_Flag_Check_AllOn_I(flags, mask)   (((flags) & (mask))==(mask))
    #define OS_Flag_Check_On_I(flags, mask)      ((flags) & (mask))
    #define OS_Flag_Check_AllOff_I(flags, mask)  (((flags) & (mask)) == 0)
    #define OS_Flag_Check_Off_I(flags, mask)     (((flags) & (mask)) != (mask))
    #define OS_Flag_Init_I(flags, value)        flags = value

#endif

//******************************************************************************
// Old names 
//******************************************************************************


#define OS_Flag_Set_1(flags, mask)          OS_FLAG_ATOMIC_WRITE_A(flags |= mask)     
#define OS_Flag_Set_0(flags, mask)          OS_FLAG_ATOMIC_WRITE_A(flags &= ~(mask))  
#define OS_Flag_Check_11(flags, mask)       (((flags) & (mask))==(mask))
#define OS_Flag_Check_1x(flags, mask)       ((flags) & (mask))
#define OS_Flag_Check_00(flags, mask)       (((flags) & (mask)) == 0)
#define OS_Flag_Check_0x(flags, mask)       (((flags) & (mask)) != (mask))

#define OS_Flag_Set_1_I(flags, mask)        OS_Flag_Set_1(flags, mask)      
#define OS_Flag_Set_0_I(flags, mask)        OS_Flag_Set_0(flags, mask)      
#define OS_Flag_Check_11_I(flags, mask)     OS_Flag_Check_11(flags, mask)   
#define OS_Flag_Check_1x_I(flags, mask)     OS_Flag_Check_1x(flags, mask)   
#define OS_Flag_Check_00_I(flags, mask)     OS_Flag_Check_00(flags, mask)   
#define OS_Flag_Check_0x_I(flags, mask)     OS_Flag_Check_0x(flags, mask)   



#define OS_Flag_Wait_11(flags, mask)                OS_Flag_Wait_AllOn(flags, mask)
#define OS_Flag_Wait_00(flags, mask)                OS_Flag_Wait_AllOff(flags, mask)
#define OS_Flag_Wait_1x(flags, mask)                OS_Flag_Wait_On(flags, mask)
#define OS_Flag_Wait_0x(flags, mask)                OS_Flag_Wait_Off(flags, mask)
#define OS_Flag_Wait_11_TO(flags, mask, timeout)    OS_Flag_Wait_AllOn_TO(flags, mask, timeout)
#define OS_Flag_Wait_00_TO(flags, mask, timeout)    OS_Flag_Wait_AllOff_TO(flags, mask, timeout)
#define OS_Flag_Wait_1x_TO(flags, mask, timeout)    OS_Flag_Wait_On_TO(flags, mask, timeout)
#define OS_Flag_Wait_0x_TO(flags, mask, timeout)    OS_Flag_Wait_Off_TO(flags, mask, timeout)





//******************************************************************************
//  END OF FILE osa_flag.h
//******************************************************************************

