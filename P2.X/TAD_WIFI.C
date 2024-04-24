/*
 * File:   TAD_WIFI.C
 * Author: LEX
 *
 * Created on 27 March 2024, 14:04
 */


#include <xc.h>
#include "TAD_TIMER.H"
#include "TAD_UART.H"

static unsigned char t;

void init_WIFI(void) {
    TI_NewTimer(&t);
    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCONbits.BRG16 = 1;
    SPBRGH = 0b00010000;    //0b00001000;
    SPBRG =  0b01000110;    //0b00100010;
    TXSTAbits.TXEN = 1;
    PIR1bits.TXIF = 0;
    PIR1bits.RCIF = 0;
                                    
}

void transmit (char data) {
    //TXSTAbits.TXEN = 1;
    
    TXREG = data;            
     
    //TXSTAbits.TXEN = 0;
    
}

char transmitString(char* data, char len) {
    static char i = 0;
   
    if (i < len) {
        if (PIR1bits.TXIF) {
            transmit(data[i]);
            i++;
        }
        return 0;
    }
    i = 0;
    return 1;
}

void TAD_WIFI(void) {
    static unsigned char state = 0;
    static char var;
    switch  (state) {
        case 0:
            if (PIR1bits.RCIF) {
                state++;
                TI_ResetTics(t);
                var = RCREG;
            }
            break;
        case 1:
            if (TI_GetTics(t)) {
                if (sendByte(var)) {
                    state--;
                }
                TI_ResetTics(t);
            }
            break;
    }
}




