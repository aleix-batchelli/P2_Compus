#include <xc.h>
#include "TAD_UART.H"
#include "TAD_TIMER.H"

#define TX  LATEbits.LATE1
#define RX  PORTEbits.RE0

#define RCIF (flags & 1)

static unsigned char tx;
static unsigned char rc;
static unsigned char rc_data;
static unsigned char flags;
static unsigned char flag = 0;

static char tx_i = 0;
static char tx_aux;

char sendByte(char _byte) {
    static char i = 0;
    static char aux;
    
      if (!i) {
        aux = _byte;
        TX = 0;
        i++;
        return 0;
    }
    else if (i >= 9) {
        TX = 1;
        i = 0;
        return 1;
    }
    else {
        TX = aux & 1;
        aux >>= 1;
        i++;
        return 0;
    }  
    }
    
}

char sendString (char* string, char len) {
    static char i = 0;
    
    if (i < len && TI_GetTics(tx)) {
        if (sendByte(string[i])) {
            i++;
        }
        TI_ResetTics(tx);
        return 0;
    }
    i = 0;
    return 1;
}

char getData(void) {
    return rc_data;
}

char getFlag() {
    return flag;
}

void clearFlag() {
    flag = 0;
}


void TAD_UART(void) {
    static unsigned char state = 0;
    static unsigned char i = 0;
    
    switch (state) {
        case 0:
            if (!RX) {
                TI_ResetTics(rc);
                rc_data = 0;
                state++;
            }
            break;
        case 1:
            if (TI_GetTics(rc)) {
                i = 0;
                state++;
            }
            break;
        case 2:
            if (i < 8) {
                rc_data |= (RX * 0x80);
                rc_data >>= 1;
                TI_ResetTics(rc);
                state++;
            }
            else {                
                i = 0;
                state = 4;
                flags = 0b00000001;
                
            }
            break;
        case 3:
            if (TI_GetTics(rc)) {
                i++;
                state--;
            }
            break;
        case 4:
            if (rc_data == 0x06) {
                flag = 1;
            }
            else if (rc_data == 0x15) {
                flag = 2;
            }
            state = 0;
            TI_ResetTics(tx);
            break;
    }
}


void init_UART(void) {
    TI_NewTimer(&tx);
    TI_NewTimer(&rc);
}