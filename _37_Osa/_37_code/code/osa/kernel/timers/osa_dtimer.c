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
 *  File:           osa_dtimer.c
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    Functions for work with dynamic timers
 *                  This file directly included in osa.c
 *
 *  History:        10.09.2010 -    File updated
 *
 ************************************************************************************************
 */


/************************************************************************************************
 *                                                                                              *
 *                     W O R K   W I T H   D Y N A M I C   T I M E R S                          *
 *                                                                                              *
 ************************************************************************************************/

//-----------------------------------------------------------------
#ifdef OS_ENABLE_DTIMERS
//-----------------------------------------------------------------




//******************************************************************************
//  Variables forDynamic Timers
//******************************************************************************


#if (OS_BANK_OS == 0) && defined(__OSA_PIC18_MPLABC__)
#pragma udata access os_bsems
#endif

    OS_BANK OST_DTIMER_CB           _OS_DTimers;    // Pointer to list of


    #ifndef _OS_DtimersWork_DEFINED
    OST_DTIMER                    * _OS_DtimerWork; // Temp var for work with list of
                                                    // dynamic timers in ISR
    #endif


#if (OS_BANK_OS == 0) && defined(__OSA_PIC18_MPLABC__)
#pragma udata
#endif







/*
 *******************************************************************************
 *
 *  void _OS_Dtimer_Create (OST_DTIMER *dtimer)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    (Internal function called by system kernel througth
 *                  service OS_Dtimer_Create)
 *
 *                  Add dynamic timer into list of active dynamic timers. This
 *                  timer will be added at first position in list.
 *
 *
 *  parameters:     dtimer - pointer to dynamic timer.
 *                  (for PICC16 can be allocated only in OS_RAM_BANK0 and OS_RAM_BANK1)
 *
 *  on return:      none
 *
 *
 *  Overloaded in:  "osa_pic12_htpicc.c"
 *                  "osa_pic16_htpicc.c"
 *                  "osa_pic18_htpicc.c"
 *                  "osa_pic16_ccs.c"
 *                  "osa_pic18_ccs.c"
 *                  "osa_pic18_mplabc.c"
 *                  "osa_pic24_mplabc.c"
 *                  "osa_stm8_iar.c"
 *
 ********************************************************************************
 */


//-----------------------------------------------------------------
#if !defined(_OS_Dtimer_Create_DEFINED)
//-----------------------------------------------------------------

//-----------------------------------------------------------------
#if defined(__OSA18__)
//-----------------------------------------------------------------

    void _OS_Dtimer_Create (OST_DTIMER *dtimer)
    {
        _OS_Temp = OS_DI();

        _fsr = (OS_FSR_TYPE) dtimer;
        _postinc = *((OS_BANK char*)&_OS_DTimers.Flags);
        // Here: bActive is set,
        //       bTimeout is cleared,
        //       bRun is cleared
        //       bNextEnable is copied

        _postinc = *((OS_BANK char*)&_OS_DTimers.Next + 0);
        _postinc = *((OS_BANK char*)&_OS_DTimers.Next + 1);
        _postinc = 0;
        #if  OS_DTIMER_SIZE >= 2
        _postinc = 0;
        #endif
        #if  OS_DTIMER_SIZE == 4
        _postinc = 0;
        _postinc = 0;
        #endif

        _OS_DTimers.Next = (OST_DTIMER*)dtimer;
        _OS_DTimers.Flags.bNextEnable = 1;

        OS_RI(_OS_Temp);
    }

//-----------------------------------------------------------------
#else   // __OSA18__
//-----------------------------------------------------------------

    void _OS_Dtimer_Create (OST_DTIMER *dtimer)
    {
        _OS_Temp = OS_DI();

        dtimer->Next = _OS_DTimers.Next;
        dtimer->Flags = _OS_DTimers.Flags;
        dtimer->Timer = 0;

        _OS_DTimers.Next = (OST_DTIMER*)dtimer;
        OS_DTIMER_ATOMIC_WRITE_A(_OS_DTimers.Flags.bNextEnable = 1);

        OS_RI(_OS_Temp);
    }

//-----------------------------------------------------------------
#endif  // __OSA18__
//-----------------------------------------------------------------


//-----------------------------------------------------------------
#endif  // !defined(_OS_Dtimer_Create_DEFINED)
//-----------------------------------------------------------------



/*
 ********************************************************************************
 *
 *  void _OS_Dtimer_Delete (OST_DTIMER *dtimer)
 *
 *------------------------------------------------------------------------------
 *
 *  description:    (Internal function called by system kernel througth
 *                  service OS_Dtimer_Delete)
 *
 *                  Delete dynamic timer from list of active timers. It is
 *                  recommended to delete all unused dynamic timers to avoid
 *                  speed down.
 *
 *  parameters:     dtimer - pointer to deleting dynamic timer
 *
 *  on return:      none
 *
 *  Overloaded in:  "osa_pic12_htpicc.c"
 *                  "osa_pic16_htpicc.c"
 *                  "osa_pic18_htpicc.c"
 *                  "osa_pic18_mplabc.c"
 *                  "osa_pic24_mplabc.c"
 *
 ********************************************************************************
 */


//-----------------------------------------------------------------
#if !defined(_OS_Dtimer_Delete_DEFINED)
//-----------------------------------------------------------------

//-----------------------------------------------------------------
#if defined(__OSA18__)
//-----------------------------------------------------------------

    void _OS_Dtimer_Delete (OST_DTIMER *dtimer)
    {
        OST_DTIMER *ptcb;
        char flags;

        _fsr = (OS_FSR_TYPE)dtimer;
        _OS_IndfTimerActive = 0;
        flags = _postinc;
        *((char*)&ptcb + 0) = _postinc;
        *((char*)&ptcb + 1) = _postinc;
        _fsr = (OS_FSR_TYPE)&_OS_DTimers;
    REPEAT:
        if (_OS_PostincTimerNextEnable)
        {
            if (_indf == *((char*)&dtimer + 0))
            {
                if (_preinc == *((char*)&dtimer + 1))
                {
                    _OS_Temp = OS_DI();
                    _postdec = *((char*)&ptcb + 1);
                    _postdec = *((char*)&ptcb + 0);

                    if (!(flags & 0x8)) _OS_IndfTimerNextEnable = 0;   // Check for bNextEnable
                    // If we check in other way (for example: !((OST_DTIMER_FLAGS*)&flags)->bNextEnable)
                    // then compiler can use FSR0 (for some compilers: MCC18 e.g.),  as a result
                    // current pointer to dtimer will be lost
                    OS_RI(_OS_Temp);
                    return;
                }
                _fsr--;
            }
            _OS_Temp = _postinc;
            _indf = _postinc;
            *(char*)&_fsr = _OS_Temp;

            goto REPEAT;
        }

    }

//-----------------------------------------------------------------
#else   // __OSA18__
//-----------------------------------------------------------------

    void _OS_Dtimer_Delete (OST_DTIMER *dtimer)
    {
        OST_DTIMER *ptcb;

        // Search current timer in list

        ptcb = (OST_DTIMER*)&_OS_DTimers;

        while (ptcb->Flags.bNextEnable)
        {
            if (ptcb->Next == dtimer)               // Current timer found
            {
                ptcb->Next = dtimer->Next;

                if (!dtimer->Flags.bNextEnable)
                    OS_DTIMER_ATOMIC_WRITE_A(ptcb->Flags.bNextEnable = 0;);

                OS_DTIMER_ATOMIC_WRITE_A(dtimer->Flags.bActive = 0;);

                return;

            }
            ptcb = (OST_DTIMER*)ptcb->Next;
        }
    }
//-----------------------------------------------------------------
#endif  // __OSA18__
//-----------------------------------------------------------------

//-----------------------------------------------------------------
#endif  // !defined(_OS_Dtimer_Delete_DEFINED)
//-----------------------------------------------------------------




//-----------------------------------------------------------------
#endif  //  OS_ENABLE_DTIMERS
//-----------------------------------------------------------------


