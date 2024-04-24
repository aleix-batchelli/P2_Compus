#include <xc.h>
#include "TAD_TECLAT.H"
#include "TAD_TIMER.H"


static unsigned char t;

void init_SPK (void) {
    TI_NewTimer(&t);
    TI_ResetTics(t);
    return;
}

void TAD_Speaker() {
    static unsigned char state = 0;
    switch (state) {
        case 0:  
            if (TeclaPremuda()) {
                state++;
            }
            break;
            
        case 1:
            if (!TeclaPremuda()) {    
                state--;
            }
            else {
                if (TI_GetTics(t) >= getKey()) {
                    LATD ^= 0b10000000;
                    TI_ResetTics(t);
                }    
            }
            break;
    }
}


