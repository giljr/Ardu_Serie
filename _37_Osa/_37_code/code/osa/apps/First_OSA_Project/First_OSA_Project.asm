
_INTERRUPT_HIGH:

;First_OSA_Project.c,27 :: 		void INTERRUPT_HIGH() iv 0x0008 ics ICS_AUTO {
;First_OSA_Project.c,28 :: 		if (TMR2IF_bit)
	BTFSS       TMR2IF_bit+0, BitPos(TMR2IF_bit+0) 
	GOTO        L_INTERRUPT_HIGH0
;First_OSA_Project.c,30 :: 		TMR2IF_bit = 0;
	BCF         TMR2IF_bit+0, BitPos(TMR2IF_bit+0) 
;First_OSA_Project.c,31 :: 		OS_Timer();
	BTFSS       __OS_Tasks+0, 4 
	GOTO        L_INTERRUPT_HIGH1
	MOVLW       1
	ADDWF       __OS_Tasks+5, 0 
	MOVWF       R0 
	MOVLW       0
	ADDWFC      __OS_Tasks+6, 0 
	MOVWF       R1 
	MOVF        R0, 0 
	MOVWF       __OS_Tasks+5 
	MOVF        R1, 0 
	MOVWF       __OS_Tasks+6 
	MOVF        __OS_Tasks+5, 0 
	IORWF       __OS_Tasks+6, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L_INTERRUPT_HIGH2
	BCF         __OS_Tasks+0, 4 
L_INTERRUPT_HIGH2:
L_INTERRUPT_HIGH1:
	BTFSS       __OS_Tasks+7, 4 
	GOTO        L_INTERRUPT_HIGH3
	MOVLW       1
	ADDWF       __OS_Tasks+12, 0 
	MOVWF       R0 
	MOVLW       0
	ADDWFC      __OS_Tasks+13, 0 
	MOVWF       R1 
	MOVF        R0, 0 
	MOVWF       __OS_Tasks+12 
	MOVF        R1, 0 
	MOVWF       __OS_Tasks+13 
	MOVF        __OS_Tasks+12, 0 
	IORWF       __OS_Tasks+13, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L_INTERRUPT_HIGH4
	BCF         __OS_Tasks+7, 4 
L_INTERRUPT_HIGH4:
L_INTERRUPT_HIGH3:
	BTFSS       __OS_Tasks+14, 4 
	GOTO        L_INTERRUPT_HIGH5
	MOVLW       1
	ADDWF       __OS_Tasks+19, 0 
	MOVWF       R0 
	MOVLW       0
	ADDWFC      __OS_Tasks+20, 0 
	MOVWF       R1 
	MOVF        R0, 0 
	MOVWF       __OS_Tasks+19 
	MOVF        R1, 0 
	MOVWF       __OS_Tasks+20 
	MOVF        __OS_Tasks+19, 0 
	IORWF       __OS_Tasks+20, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L_INTERRUPT_HIGH6
	BCF         __OS_Tasks+14, 4 
L_INTERRUPT_HIGH6:
L_INTERRUPT_HIGH5:
;First_OSA_Project.c,32 :: 		}
L_INTERRUPT_HIGH0:
;First_OSA_Project.c,33 :: 		}
L_end_INTERRUPT_HIGH:
L__INTERRUPT_HIGH49:
	RETFIE      1
; end of _INTERRUPT_HIGH

_TickTimerIE:

;First_OSA_Project.c,35 :: 		void TickTimerIE(void)
;First_OSA_Project.c,38 :: 		T2CON = T2CON_CONST;
	MOVLW       77
	MOVWF       T2CON+0 
;First_OSA_Project.c,39 :: 		PR2 = PR2_CONST;
	MOVLW       49
	MOVWF       PR2+0 
;First_OSA_Project.c,41 :: 		INTCON.GIEH = 1;
	BSF         INTCON+0, 7 
;First_OSA_Project.c,42 :: 		INTCON.GIEL = 1;
	BSF         INTCON+0, 6 
;First_OSA_Project.c,43 :: 		RCON.IPEN = 1;
	BSF         RCON+0, 7 
;First_OSA_Project.c,45 :: 		TMR2IE_bit = 1;
	BSF         TMR2IE_bit+0, BitPos(TMR2IE_bit+0) 
;First_OSA_Project.c,46 :: 		TMR2IP_bit = 1;
	BSF         TMR2IP_bit+0, BitPos(TMR2IP_bit+0) 
;First_OSA_Project.c,47 :: 		T2CON.TMR2ON = 1;
	BSF         T2CON+0, 2 
;First_OSA_Project.c,48 :: 		}
L_end_TickTimerIE:
	RETURN      0
; end of _TickTimerIE

_main:

;First_OSA_Project.c,50 :: 		void main (void)
;First_OSA_Project.c,52 :: 		InitMCU();                         // Inicialize MCU's peripheral
	CALL        _InitMCU+0, 0
;First_OSA_Project.c,53 :: 		TickTimerIE();                     // Config TickTimer interruptions
	CALL        _TickTimerIE+0, 0
;First_OSA_Project.c,55 :: 		OS_Init();                         // Inicialize RTO's
	CALL        _OS_Init+0, 0
;First_OSA_Project.c,58 :: 		OS_Task_Create(0, Task_LED0);      //Criate task LE0 (Max priority)
	CLRF        FARG__OS_Task_Create_priority+0 
	MOVLW       _Task_LED0+0
	MOVWF       FARG__OS_Task_Create_TaskAddr+0 
	MOVLW       hi_addr(_Task_LED0+0)
	MOVWF       FARG__OS_Task_Create_TaskAddr+1 
	MOVLW       0
	MOVWF       FARG__OS_Task_Create_TaskAddr+2 
	MOVLW       0
	MOVWF       FARG__OS_Task_Create_TaskAddr+3 
	CALL        __OS_Task_Create+0, 0
;First_OSA_Project.c,59 :: 		OS_Task_Create(0, Task_LED1);      //Criate task LE1 (Max priority)
	CLRF        FARG__OS_Task_Create_priority+0 
	MOVLW       _Task_LED1+0
	MOVWF       FARG__OS_Task_Create_TaskAddr+0 
	MOVLW       hi_addr(_Task_LED1+0)
	MOVWF       FARG__OS_Task_Create_TaskAddr+1 
	MOVLW       0
	MOVWF       FARG__OS_Task_Create_TaskAddr+2 
	MOVLW       0
	MOVWF       FARG__OS_Task_Create_TaskAddr+3 
	CALL        __OS_Task_Create+0, 0
;First_OSA_Project.c,60 :: 		OS_Task_Create(0, Task_LED2);      //Criate task LE2 (Max priority)
	CLRF        FARG__OS_Task_Create_priority+0 
	MOVLW       _Task_LED2+0
	MOVWF       FARG__OS_Task_Create_TaskAddr+0 
	MOVLW       hi_addr(_Task_LED2+0)
	MOVWF       FARG__OS_Task_Create_TaskAddr+1 
	MOVLW       0
	MOVWF       FARG__OS_Task_Create_TaskAddr+2 
	MOVLW       0
	MOVWF       FARG__OS_Task_Create_TaskAddr+3 
	CALL        __OS_Task_Create+0, 0
;First_OSA_Project.c,62 :: 		OS_EI();                           // Enable interrupts
	BSF         GIE_bit+0, BitPos(GIE_bit+0) 
	BTFSS       IPEN_bit+0, BitPos(IPEN_bit+0) 
	GOTO        L_main7
	BSF         GIEL_bit+0, BitPos(GIEL_bit+0) 
L_main7:
;First_OSA_Project.c,65 :: 		OS_Run();                          // Run the scheduler
L_main8:
	BCF         __OS_Flags+0, 4 
	BSF         __OS_Flags+0, 3 
	MOVLW       8
	MOVWF       __OS_Best_Priority+0 
	MOVF        __OS_LastTask+0, 0 
	MOVWF       __OS_CurTask+0 
	MOVF        __OS_LastTask+1, 0 
	MOVWF       __OS_CurTask+1 
L_main11:
	MOVLW       hi_addr(__OS_Tasks+14)
	SUBWF       __OS_CurTask+1, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__main52
	MOVLW       __OS_Tasks+14
	SUBWF       __OS_CurTask+0, 0 
L__main52:
	BTFSS       STATUS+0, 0 
	GOTO        L_main14
	MOVLW       __OS_Tasks+0
	MOVWF       __OS_CurTask+0 
	MOVLW       hi_addr(__OS_Tasks+0)
	MOVWF       __OS_CurTask+1 
	GOTO        L_main15
L_main14:
	MOVLW       7
	ADDWF       __OS_CurTask+0, 0 
	MOVWF       R0 
	MOVLW       0
	ADDWFC      __OS_CurTask+1, 0 
	MOVWF       R1 
	MOVF        R0, 0 
	MOVWF       __OS_CurTask+0 
	MOVF        R1, 0 
	MOVWF       __OS_CurTask+1 
L_main15:
	MOVF        __OS_CurTask+0, 0 
	MOVWF       FSR0 
	MOVF        __OS_CurTask+1, 0 
	MOVWF       FSR0H 
	MOVLW       1
	MOVWF       R0 
	MOVLW       __OS_State+0
	MOVWF       FSR1 
	MOVLW       hi_addr(__OS_State+0)
	MOVWF       FSR1H 
L_main16:
	MOVF        POSTINC0+0, 0 
	MOVWF       POSTINC1+0 
	DECF        R0, 1 
	BTFSS       STATUS+0, 2 
	GOTO        L_main16
	BTFSS       __OS_State+0, 6 
	GOTO        L__main47
	BTFSC       __OS_State+0, 7 
	GOTO        L__main47
	GOTO        L_main19
L__main47:
	GOTO        ___main__OS_SCHED_CONTINUE
L_main19:
	MOVF        __OS_State+0, 0 
	MOVWF       R0 
	MOVLW       7
	ANDWF       R0, 1 
	MOVF        R0, 0 
	MOVWF       __OS_Temp+0 
	MOVF        __OS_Best_Priority+0, 0 
	SUBWF       __OS_Temp+0, 0 
	BTFSS       STATUS+0, 0 
	GOTO        L_main20
	GOTO        ___main__OS_SCHED_CONTINUE
L_main20:
	BTFSC       __OS_State+0, 3 
	GOTO        L_main21
___main__OS_SCHED_RUN:
	CALL        __OS_JumpToTask+0, 0
	CALL        __OS_SET_FSR_CUR_TASK+0, 0
	MOVLW       215
	ANDWF       __indf+0, 1 
	MOVLW       40
	ANDWF       __OS_State+0, 0 
	MOVWF       R0 
	MOVF        R0, 0 
	IORWF       __indf+0, 1 
	BTFSC       __OS_Flags+0, 3 
	GOTO        L_main22
	GOTO        ___main_SCHED_END
L_main22:
L_main21:
	BTFSS       __OS_State+0, 4 
	GOTO        L_main25
	BTFSC       __OS_State+0, 5 
	GOTO        L_main25
L__main46:
	GOTO        ___main__OS_SCHED_CONTINUE
L_main25:
	BTFSS       __OS_State+0, 3 
	GOTO        L_main26
	BSF         __OS_Flags+0, 4 
	MOVF        __OS_CurTask+0, 0 
	MOVWF       __OS_BestTask+0 
	MOVF        __OS_CurTask+1, 0 
	MOVWF       __OS_BestTask+1 
	MOVF        __OS_State+0, 0 
	MOVWF       R0 
	MOVLW       7
	ANDWF       R0, 1 
	MOVF        R0, 0 
	MOVWF       __OS_Best_Priority+0 
	MOVF        __OS_Best_Priority+0, 0 
	XORLW       0
	BTFSS       STATUS+0, 2 
	GOTO        L_main27
	GOTO        L_main12
L_main27:
L_main26:
___main__OS_SCHED_CONTINUE:
	MOVF        __OS_CurTask+1, 0 
	XORWF       __OS_LastTask+1, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__main53
	MOVF        __OS_LastTask+0, 0 
	XORWF       __OS_CurTask+0, 0 
L__main53:
	BTFSC       STATUS+0, 2 
	GOTO        L__main45
	GOTO        L_main11
L__main45:
L_main12:
	BTFSS       __OS_Flags+0, 4 
	GOTO        L_main30
	BCF         __OS_Flags+0, 3 
	MOVF        __OS_BestTask+0, 0 
	MOVWF       __OS_CurTask+0 
	MOVF        __OS_BestTask+1, 0 
	MOVWF       __OS_CurTask+1 
	MOVF        __OS_CurTask+0, 0 
	MOVWF       __OS_LastTask+0 
	MOVF        __OS_CurTask+1, 0 
	MOVWF       __OS_LastTask+1 
	MOVF        __OS_CurTask+0, 0 
	MOVWF       FSR0 
	MOVF        __OS_CurTask+1, 0 
	MOVWF       FSR0H 
	MOVLW       1
	MOVWF       R0 
	MOVLW       __OS_State+0
	MOVWF       FSR1 
	MOVLW       hi_addr(__OS_State+0)
	MOVWF       FSR1H 
L_main31:
	MOVF        POSTINC0+0, 0 
	MOVWF       POSTINC1+0 
	DECF        R0, 1 
	BTFSS       STATUS+0, 2 
	GOTO        L_main31
	BTFSS       __OS_State+0, 6 
	GOTO        L_main34
	BTFSC       __OS_State+0, 7 
	GOTO        L_main34
L__main44:
	GOTO        ___main__OS_SCHED_RUN
L_main34:
L_main30:
___main_SCHED_END:
	GOTO        L_main8
;First_OSA_Project.c,67 :: 		}
L_end_main:
	GOTO        $+0
; end of _main

_InitMCU:

;First_OSA_Project.c,70 :: 		void InitMCU(void)
;First_OSA_Project.c,79 :: 		ADCON1 |= 0X0F;
	MOVLW       15
	IORWF       ADCON1+0, 1 
;First_OSA_Project.c,82 :: 		TRISD = 0;                         // PORTD as output
	CLRF        TRISD+0 
;First_OSA_Project.c,83 :: 		PORTD = 0;                         // All LEDs are OFF
	CLRF        PORTD+0 
;First_OSA_Project.c,85 :: 		}
L_end_InitMCU:
	RETURN      0
; end of _InitMCU

_Task_LED0:

;First_OSA_Project.c,89 :: 		void Task_LED0(void)
;First_OSA_Project.c,91 :: 		for(;;) //infinite loop
L_Task_LED035:
;First_OSA_Project.c,93 :: 		LATD.RD0 = ~LATD.RD0;
	BTG         LATD+0, 0 
;First_OSA_Project.c,94 :: 		OS_Delay(300 ms);
	MOVLW       44
	MOVWF       FARG__OS_InitDelay_Delay+0 
	MOVLW       1
	MOVWF       FARG__OS_InitDelay_Delay+1 
	CALL        __OS_InitDelay+0, 0
	CALL        __OS_ReturnSave+0, 0
	NOP
;First_OSA_Project.c,102 :: 		}
	GOTO        L_Task_LED035
;First_OSA_Project.c,103 :: 		}
L_end_Task_LED0:
	RETURN      0
; end of _Task_LED0

_Task_LED1:

;First_OSA_Project.c,105 :: 		void Task_LED1(void)
;First_OSA_Project.c,107 :: 		for(;;)
L_Task_LED138:
;First_OSA_Project.c,109 :: 		LATD.RD1 = ~LATD.RD1;
	BTG         LATD+0, 1 
;First_OSA_Project.c,110 :: 		OS_Delay(300 ms);
	MOVLW       44
	MOVWF       FARG__OS_InitDelay_Delay+0 
	MOVLW       1
	MOVWF       FARG__OS_InitDelay_Delay+1 
	CALL        __OS_InitDelay+0, 0
	CALL        __OS_ReturnSave+0, 0
	NOP
;First_OSA_Project.c,111 :: 		}
	GOTO        L_Task_LED138
;First_OSA_Project.c,112 :: 		}
L_end_Task_LED1:
	RETURN      0
; end of _Task_LED1

_Task_LED2:

;First_OSA_Project.c,114 :: 		void Task_LED2(void)
;First_OSA_Project.c,116 :: 		for(;;)
L_Task_LED241:
;First_OSA_Project.c,118 :: 		LATD.RD2 = ~LATD.RD2;
	BTG         LATD+0, 2 
;First_OSA_Project.c,119 :: 		OS_Delay(300 ms);
	MOVLW       44
	MOVWF       FARG__OS_InitDelay_Delay+0 
	MOVLW       1
	MOVWF       FARG__OS_InitDelay_Delay+1 
	CALL        __OS_InitDelay+0, 0
	CALL        __OS_ReturnSave+0, 0
	NOP
;First_OSA_Project.c,120 :: 		}
	GOTO        L_Task_LED241
;First_OSA_Project.c,121 :: 		}
L_end_Task_LED2:
	RETURN      0
; end of _Task_LED2
