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
 *  File:           osa_bsem.c
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    Definition the array for binary semaphores
 *                  This file directly included in osa.c
 *
 *  History:        12.09.2010 -    File updated
 *
 ************************************************************************************************
 */


#if OS_BSEMS > 0

    #if (OS_BANK_BSEM == 0) && defined(__OSA_PIC18_MPLABC__)
    #pragma udata access os_bsems
    #endif

    volatile OS_BSEM_BANK    OST_WORD   _OS_Bsems[(OS_BSEMS + _OST_INT_SIZE-1) / _OST_INT_SIZE] ;

    #if (OS_BANK_BSEM == 0) && defined(__OSA_PIC18_MPLABC__)
    #pragma udata
    #endif

#endif




//******************************************************************************
//  END OF FILE osa_bsem.c
//******************************************************************************

