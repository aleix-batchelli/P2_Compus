/*
 * File:   TAD_JOYSTICK.C
 * Author: LEX
 *
 * Created on 26 March 2024, 17:37
 */


#include <xc.h>

static unsigned char lastState = 0;

void init_ADC(void) {
    // Configure ADCON0 register
    ADCON0 = 0b0000011;
       
    
    // Configure ADCON1 register
    ADCON1 = 0b00001110;   
    
    ADCON2 = 0b00111110;
}

char getJoyState(void) {
    unsigned char state = 14;
    
    if (ADRESH >= 220) {
        LATDbits.LATD4 = 1;
        state = 16;
    } 
    else if (ADRESH <= 60) {
        LATDbits.LATD4 = 0;
        state = 17;
    }
    if (state == lastState) {
        return 14;
    }
    lastState = state;
    return state;  
}
