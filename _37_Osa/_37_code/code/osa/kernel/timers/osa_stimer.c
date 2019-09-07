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
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    Functions for work with static timers
 *                  This file directly included in osa.c
 *
 *  History:        02.10.2010 -    File updated
 *
 ************************************************************************************************
 */


/************************************************************************************************
 *                                                                                              *
 *                     W O R K   W I T H   S T A T I C   T I M E R S                            *
 *                                                                                              *
 ************************************************************************************************/

//-----------------------------------------------------------------
#if     OS_STIMERS > 0


#if (OS_BANK_STIMERS == 0) && defined(__OSA_PIC18_MPLABC__)
#pragma udata access os_bsems
#endif

    volatile OS_STIMERS_BANK     OS_STIMER_TYPE     _OS_Stimers[OS_STIMERS]      ; 
    
    #ifdef OS_STIMERS_ENABLE_ALLOCATION    
    OS_STIMERS_BANK     OST_WORD   _OS_StimersFree[(OS_STIMERS + _OST_INT_SIZE-1) / _OST_INT_SIZE];
    #endif

#if (OS_BANK_STIMERS == 0) && defined(__OSA_PIC18_MPLABC__)
#pragma udata
#endif




//------------------------------------------------------------------------------
#ifdef  OS_STIMERS_ENABLE_ALLOCATION
//-----------------------------------------------------------------









/*
 ********************************************************************************
 *
 *  _OS_Stimer_GetFree
 *
 *------------------------------------------------------------------------------
 *
 *  description:    (Internal function called by system kernel througth service OS)
 *                  Searches free static timer and returns it's ID
 *
 *
 *  parameters:     OST_UINT bCreate - =0 - do not created timer, just check for
 *                                          aviability
 *                                     =1 - create timer and mark it asm used
 *
 *  on return:      none
 *
 *  Overloaded in:  osa_pic18_htpicc.c
 *                  osa_pic16_htpicc.c
 *                  osa_pic12_htpicc.c
 *                  osa_pic18_mplabc.c
 *
 ********************************************************************************
 */

//-----------------------------------------------------------------
#ifndef _OS_Stimer_GetFree_DEFINED
//-----------------------------------------------------------------

    OST_UINT8   _OS_Stimer_GetFree (OST_UINT bCreate)
    {
        OST_UINT n, i;
        OST_UINT temp, mask;

        _OS_Flags.bError = 0;

        for (n = 0; n <= OS_STIMERS >> 3; n++)
        {
            temp = _OS_StimersFree[n];
            if (temp)           // There is a free timer
            {
                if (!(bCreate & 1)) return 0;

                i = n << _OST_INT_SHIFT;
                mask = 1;
                while (!(temp & mask))
                {
                    mask <<= 1;
                    i++;
                }
                _OS_StimersFree[n] &= ~mask;
                return i;
            }
        }

        _OS_Flags.bError = 1;
        return 0xFF;
    }




    void    _OS_Stimer_Free (OST_UINT8 ID)
    {
        _OS_StimersFree[ID >> _OST_INT_SHIFT] |= (1 << (ID & _OST_INT_MASK));
    }
    /**/

//==============================================================================
#endif  // Compiler type
//==============================================================================

//-----------------------------------------------------------------
#endif  //  OS_STIMERS_ENABLE_ALLOCATION
#endif  //  OS_ENABLE_STIMERS
//-----------------------------------------------------------------



