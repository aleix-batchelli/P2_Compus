#include <xc.h>
#include "TAD_TIMER.H"
#include "TAD_TECLAT.H"
#include "TAD_SPEAKER.H"
#include "LcTLCD.h"
#include "TAD_MENU.H"
#include "TAD_JOYSTICK.H"
#include "TAD_UART.H"
#include "TAD_WIFI.H"
#include "TAD_WORK.h"

#pragma config OSC = HSPLL //INTIO2
#pragma config PBADEN = DIG
#pragma config MCLRE = OFF
#pragma config DEBUG = OFF
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config WDT = OFF
#pragma config LVP = OFF

void initPorts(void) {
    TRISA = 0;
    TRISB = 0;
    TRISC = 1;
    TRISD = 0;
    TRISE = 0;
    TRISEbits.TRISE0 = 1;
    LATD = 0;
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB6 = 1;
    TRISBbits.TRISB7 = 1;
    LATB = 0;
    LATA = 0;
    LATEbits.LATE1 = 1;
    LATBbits.LATB0 = 1;
    TRISAbits.TRISA0 = 1;
    LATDbits.LATD0 = 0;
}

void __interrupt() RSI() {
    if (INTCONbits.TMR0IF) {
        RSI_Timer0();
    }
}

void main(void) {
    initPorts();
    
    init_SPK();
    init_KEYB();
    init_Menu();
    init_ADC();
    init_UART();
    init_WIFI();

    TI_Init();
    
    LcInit(2, 16);
    LcClear();
    LcCursorOff();
    LcGotoXY(0,0);
    INTCONbits.GIE = 1;
    
    while (1) {
        /*TAD_Menu();
        TAD_Teclat();
        TAD_Speaker();*/
        TAD_UART();
        
        sendByte(getData());
        /*TAD_LcD();
        TAD_WIFI();
        TAD_WORK();
        */
    }
    return;
}





