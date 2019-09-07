    #include <osa.h>

// Config TIMER2 @ 1 ms (FOSC = 8MHz)
/*
  1.000 = P_cycleMachine * Prescaler * Postscale * (PR2 + 1)
  1.000 = 0.5us * 4 * 10 * (PR2 + 1)                         //1:10 Postscaler
  PR2 = 49                                                   //Prescaler is 4
*/
#define T2CON_CONST     0B01001101   //Timer2 ON, Prescaler 1:4 e Postscale 1:10
#define PR2_CONST       (49)         //1us para FOSC
#define ms /1

//Prototypes
void    InitMCU (void);
void    TickTimerIE(void);

void    Task_LED0 (void);
void    Task_LED1 (void);
void    Task_LED2 (void);

// Tells the mikroC compiler linker that the tasks will be called
// Indirectly by the SO.
#pragma funcall main Task_LED0
#pragma funcall main Task_LED1
#pragma funcall main Task_LED2

void INTERRUPT_HIGH() iv 0x0008 ics ICS_AUTO {
    if (TMR2IF_bit)
    {
        TMR2IF_bit = 0;
        OS_Timer();
    }
}

void TickTimerIE(void)
{
     // Load TIMER2 initializers (OS_TickTimer)
    T2CON = T2CON_CONST;
    PR2 = PR2_CONST;
    // Enable General Interruptions
    INTCON.GIEH = 1;
    INTCON.GIEL = 1;
    RCON.IPEN = 1;
    // Enable TIMER2 interruption (OS_TickTimer)
    TMR2IE_bit = 1;
    TMR2IP_bit = 1;
    T2CON.TMR2ON = 1;
}

void main (void)
{
    InitMCU();                         // Inicialize MCU's peripheral
    TickTimerIE();                     // Config TickTimer interruptions

    OS_Init();                         // Inicialize RTO's

    // Criate all tasks. Max priority = 0. Min priority = 7
    OS_Task_Create(0, Task_LED0);      //Criate task LE0 (Max priority)
    OS_Task_Create(0, Task_LED1);      //Criate task LE1 (Max priority)
    OS_Task_Create(0, Task_LED2);      //Criate task LE2 (Max priority)

    OS_EI();                           // Enable interrupts

    //.....
    OS_Run();                          // Run the scheduler

}


void InitMCU(void)
{
 #ifdef P18F45K22
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
 #else
    ADCON1 |= 0X0F;
 #endif

    TRISD = 0;                         // PORTD as output
    PORTD = 0;                         // All LEDs are OFF

}



void Task_LED0(void)
{
    for(;;) //infinite loop
    {
        LATD.RD0 = ~LATD.RD0;
        OS_Delay(300 ms);
        //Delay_ms(300);               // this block does not return
                                       // the control to OS,
                                       // causes LED0 to blink and the others
                                       // do not blink.
        //OS_Yield();                  // returns control to OS, but
                                       // switches quickly,
                                       // and appears to be on permanently
    }
}

void Task_LED1(void)
{
    for(;;)
    {
        LATD.RD1 = ~LATD.RD1;
        OS_Delay(300 ms);
    }
}

void Task_LED2(void)
{
    for(;;)
    {
        LATD.RD2 = ~LATD.RD2;
        OS_Delay(300 ms);
    }
}
