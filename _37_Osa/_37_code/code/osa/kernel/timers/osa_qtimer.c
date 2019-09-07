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
 *  File:           osa_qtimers.c
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    
 *
 *  History:        10.09.2010 -    File created
 * 
 *                  25.10.2010  -   Bug fixed: OS_Qtimer_Break did not deleted timer from the list
 *                              -   Bug fixed: interrupt became disables after calling OS_Qtimer_Run for timer already
 *                                  presented in queue
 *
 *                  30.10.2010  -   Another bug in qtimers fixed
 * 
 ************************************************************************************************
 */


//******************************************************************************
//  
//******************************************************************************

//------------------------------------------------------------------------------
#ifndef __OSA_PIC12_HTPICC__
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef OS_ENABLE_QTIMERS
//------------------------------------------------------------------------------



#if (OS_BANK_OS == 0) && defined(__OSA_PIC18_MPLABC__)
#pragma udata access os_bsems
#endif

    OS_BANK OST_QTIMER_CB           _OS_Qtimers;    // Pointer to list of fast timers

    #ifndef _OS_QtimersWork_DEFINED
    OST_QTIMER  * OS_BANK         _OS_QtimerWork;   // Temp var for work with list of
                                                    // fast timers in ISR
    #endif

#if (OS_BANK_OS == 0) && defined(__OSA_PIC18_MPLABC__)
#pragma udata
#endif








/*
 ********************************************************************************
 *
 *  _OS_Qtimer_List
 *
 *------------------------------------------------------------------------------
 *
 *  description:    (Internal function called by system kernel througth service
 *                  OS)
 *
 *                  This function works with timer's list. It adds, deletes,
 *                  counts timers according to value of ftimer->Flags.
 * 
 *  parameters:     OS_QTIMER_TYPE time   - new time for added timer (when
 *                                      _OS_Qtimers.Flags.bListFunction =
 *                                      FUNCTION_ADD)
 *                  OST_QTIMER *ftimer - pointer to timer to work on.
 *
 *  on return:      OS_QTIMER_TYPE:
 *                  1.  When bListGetTime == "1", this function returns remaining
                        time for current timer (flag bListFunction does not matter)
                    2.  When bListFunction == OS_QTIMER_LIST_FUNCTION_ADD, this
                        function adds new timer into list (before adding it checks
                        for timer is already in list; if so, then timer deleted first
                        and function executes twice (see "goto RESTART"))
                    3.  When bListFunction == OS_QTIMER_LIST_FUNCTION_DELETE, this
                        function deletes current timer
 
 *  Overloaded in:  osa_pic16_htpicc.c
 * 
 ********************************************************************************
 */


//******************************************************************************
#if !defined(_OS_Qtimer_List_DEFINED)
//******************************************************************************

OS_QTIMER_TYPE _OS_Qtimer_List (OS_QTIMER_TYPE time, OST_QTIMER *ftimer)
{
    OST_QTIMER          *ftcb;
    OST_QTIMER_FLAGS    flags;
    OS_QTIMER_TYPE      temp, remain;
    OST_UINT            itemp;
    #ifdef __OSA_PIC18_MIKROC__
    OST_QTIMER          *ftcb_temp; // Due to compiler bug (id=4711) can't copy pointed value
                                    // (*Note:4711*) (version mikroC PRO 3.20)
    #endif

RESTART:
    remain = 0;                                 // Count remaining time

    #ifdef __OSA_STM8_IAR__
    *(unsigned char*)&flags = *(unsigned char*)ftimer;
    #else
    flags = ftimer->Flags;
    #endif

// *101030*    if (time == 0 && _OS_Qtimers.Flags.bListFunction == OS_QTIMER_LIST_FUNCTION_ADD)    /*101025*/
// *101030*    {                                   
// *101030*        ftimer->Flags.bTimeout = 1;     
// *101030*        ftimer->Flags.bActive  = 0;     
// *101030*        goto EXIT;
// *101030*    }                                   



    if (_OS_Qtimers.Flags.bListGetTime) 
    {    
        //--------------------------------------------
        // Clear ADD function if GET function present
        //--------------------------------------------
        _OS_Qtimers.Flags.bListFunction = !OS_QTIMER_LIST_FUNCTION_ADD;
    }
    else 
    {
        //---------------------------------------
        // Skip deleting if timer is not in list
        //---------------------------------------
        if (_OS_Qtimers.Flags.bListFunction == OS_QTIMER_LIST_FUNCTION_DELETE && !flags.bActive) goto EXIT;
    }

    _OS_Qtimers.Flags.bAddAfterDelete = 0;

    if (_OS_Qtimers.Flags.bListFunction == OS_QTIMER_LIST_FUNCTION_ADD)
    {
        if (flags.bActive)              // If timer is already in list, it should be deleted first
        {
            OS_QTIMER_ATOMIC_WRITE_A(_OS_Qtimers.Flags.bAddAfterDelete = 1;);  // Set flag to execute ADD after deleting
            _OS_Qtimers.Flags.bListFunction = OS_QTIMER_LIST_FUNCTION_DELETE;
        }
        else
        {
            //---------------------------------------
            // Prepare timer for adding
            //---------------------------------------
            
            if (time == 0)                  // *101030*
            {                               // *101030*
                ftimer->Flags.bTimeout = 1; // *101030*
                ftimer->Flags.bActive  = 0; // *101030*
                goto EXIT;                  // *101030*
            }                               // *101030*

            // *101030* OS_QTIMER_ATOMIC_WRITE_A(ftimer->Flags.bNextEnable = 0;);
            ftimer->Flags.bNextEnable = 0;  // *101030*
            time ^= (OS_QTIMER_TYPE)-1; // time = -time
            time++;
        }
    }

    #if defined(__OSA_PIC24_MPLABC__)
    ftcb = (OST_QTIMER*)(void*)&_OS_Qtimers;
    #else
    ftcb = (OST_QTIMER*)&_OS_Qtimers;           // Start search from header
    #endif

    itemp = OS_DI();

    if (_OS_Qtimers.Flags.bNextEnable)
    {    
        do
        {
            #ifdef __OSA_PIC18_MIKROC__         // (*Note:4711*)
            ftcb_temp = ftcb->Next;             // (*Note:4711*)
            #endif                              // (*Note:4711*)

            if (_OS_Qtimers.Flags.bListFunction == OS_QTIMER_LIST_FUNCTION_DELETE && !_OS_Qtimers.Flags.bListGetTime)
            {
                if (ftcb->Next == ftimer)               // ********* Deleting ***********
                {
                    ftcb->Next = ftimer->Next;
                    OS_QTIMER_ATOMIC_WRITE_A(
                        ftcb->Flags.bNextEnable = flags.bNextEnable;
                        ftimer->Flags.bActive   = 0;
                    );

                    if (flags.bNextEnable) ftcb->Next->Timer += ftimer->Timer;

                    if (_OS_Qtimers.Flags.bAddAfterDelete)
                    {
                        //-----------------------------------------------------------
                        // It was deleting before adding. 
                        // Set ADD flag and restart searching
                        //-----------------------------------------------------------

                        _OS_Qtimers.Flags.bListFunction = OS_QTIMER_LIST_FUNCTION_ADD;
                        OS_RI(itemp);   /*101025*/
                        goto RESTART;
                    }
                    goto EXIT_RI;
                }
            }
            else                                        // ********* Adding or Reading ***********
            {
                #ifdef __OSA_PIC18_MIKROC__     // (*Note:4711*)
                temp = ftcb_temp->Timer;        // (*Note:4711*)
                #else                           // (*Note:4711*)
                temp = ftcb->Next->Timer;       // Copy time value into temp variable
                #endif                          // (*Note:4711*)

                if (_OS_Qtimers.Flags.bListGetTime)
                {
                    if (ftcb == ftimer) goto EXIT_RI;
                    remain -= temp;
                } 
                else
                {
                    if (temp && temp <= time)
                    {
                        OS_QTIMER_ATOMIC_WRITE_A(ftimer->Flags.bNextEnable = 1);
                        break;
                    } 
                }
                time -= temp;
            }

            OS_RI(itemp);
            ftcb = ftcb->Next;
            OS_DI();

        } while (ftcb->Flags.bNextEnable);
    }

    //**************************************
    //               Adding
    //**************************************
    if (_OS_Qtimers.Flags.bListFunction == OS_QTIMER_LIST_FUNCTION_ADD)
    {
        OS_QTIMER_ATOMIC_WRITE_A(
            ftimer->Flags.bTimeout = 0;
            ftimer->Flags.bActive  = 1;
        );
        ftimer->Next = ftcb->Next;
        ftimer->Timer = time;
    
        if (ftcb->Flags.bTimeout)
        {
            #if defined(__OSA_PIC24_MPLABC__)
            ftcb = (OST_QTIMER*)(void*)&_OS_Qtimers;    // 
            #else
            ftcb = (OST_QTIMER*)&_OS_Qtimers;           // Start search from header
            #endif
        }
        ftcb->Next = ftimer;
        OS_QTIMER_ATOMIC_WRITE_A(ftcb->Flags.bNextEnable = 1);
    
        if (ftimer->Flags.bNextEnable)
        {
            ftimer = ftimer->Next;
            ftimer->Timer -= time;
        }
    }

EXIT_RI:
    OS_RI(itemp);
EXIT:
    _OS_Qtimers.Flags.bListFunction = OS_QTIMER_LIST_FUNCTION_ADD;
    _OS_Qtimers.Flags.bListGetTime  = 0;
    return remain;
}

//******************************************************************************
#endif      // _OS_Qtimer_List_DEFINED
//******************************************************************************


//------------------------------------------------------------------------------
#endif      // OS_ENABLE_QTIMERS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#endif      //  __OSA_PIC12_HTPICC__
//------------------------------------------------------------------------------


