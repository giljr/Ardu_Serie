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
 *  File:           osa_tasks.c
 *
 *  Programmer:     Timofeev Victor
 *                  osa@pic24.ru, testerplus@mail.ru
 *
 *  Description:    Functions and variables for Tasks.
 *                  This file directly included in osa.c
 *
 *  History:        12.09.2010 -    File updated
 *
 ************************************************************************************************
 */




//******************************************************************************
//  Task descriptors
//******************************************************************************

#if (OS_BANK_TASKS == 0) && defined(__OSA_PIC18_MPLABC__)
#pragma udata access os_bsems
#endif

OS_TASKS_BANK  OST_TCB  _OS_Tasks[OS_TASKS] OS_ALLOCATION_TASKS;

#if (OS_BANK_TASKS == 0) && defined(__OSA_PIC18_MPLABC__)
#pragma udata
#endif



/*
 ********************************************************************************
 *
 *   void             _OS_Task_Create(OST_WORD    priority,
 *                                    OST_CODE_POINTER TaskAddr)
 *
 *------------------------------------------------------------------------------
 *
 *   description:   (Internal function called by system kernel from service
 *                  OS_Task_Create)
 *                  Create task in free descriptor.
 *  parameters:     priority - value from 0 (highest) to 7 (lowest)
 *                  TaskAddr - pointer to C-function that contains task
 *
 *  on return:      check OS_IsError
 *                  OST_TASK_POINTER - pointer to task descriptor
 *                  _OS_Temp - task position in _OS_Tasks array 
 *
 *  Overloaded in:  "osa_pic12_htpicc.c"
 *                  "osa_pic16_mikroc.c"
 *                  "osa_pic24_mplabc.c"
 *                  "port\osa_ccc16.c"
 *
 ********************************************************************************
 */


//------------------------------------------------------------------------------
#if !defined(_OS_Task_Create_DEFINED)
//------------------------------------------------------------------------------

    void _OS_Task_Create(OST_WORD priority, OST_CODE_POINTER TaskAddr)
    {
        OST_TASK_POINTER Task;

        _OS_Flags.bError = 0;


        /*--------------------------------------*
         *                                      *
         *  Start search from first task in     *
         *  OS_TASKS descriptors.               *
         *                                      *
         *--------------------------------------*/
        Task = (OST_TASK_POINTER)_OS_Tasks;
        _OS_Temp = 0;   

        do {

            if (!Task->State.bEnable)               // Is descriptor free?
            {                                       // Yes
                ((OST_TASK_STATE*)&priority)->bEnable = 1;
                ((OST_TASK_STATE*)&priority)->bReady = 1;

                Task->pTaskPointer = TaskAddr;

                #ifdef OS_ENABLE_TTIMERS
                    Task->Timer = 0;
                #endif

                #ifdef _OS_TASK_CREATE_PROC_SPEC
                _OS_TASK_CREATE_PROC_SPEC();
                #endif

                *((OS_TASKS_BANK char*)&Task->State) = priority;

                #if defined(_OS_CUR_FLAGS_IN_OS_STATE)
                if (Task == _OS_CurTask) *((OS_RAM_NEAR char*)&_OS_State) = priority;
                #endif

                #if defined(__OSA_AVR_WINAVR__) || defined(__OSA_AVR_IAR__)
                Task->nY_Temp = 0x8000;
                #endif

                #if defined(__OSA_AVR_WINAVR__)
                Task->c_NumOfTemp = 0;  
                #endif

                #if defined(__OSA_STM8__)
                Task->nSP_Temp = 0;
                #endif

                _OS_Flags.bError = 0;

                return ;

            }

            Task ++;

        } while (++_OS_Temp < OS_TASKS);    

        // There is no free descriptor. Task was not created.
        _OS_Flags.bError = 1;

        return ;
    }

//------------------------------------------------------------------------------
#endif  // !defined(_OS_Task_Create_DEFINED)
//------------------------------------------------------------------------------



//******************************************************************************
//  END OF FILE osa_tasks.c
//******************************************************************************

