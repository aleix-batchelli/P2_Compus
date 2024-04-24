/*
 * File:   TAD_TECLAT.c
 * Author: LEX
 *
 * Created on 18 March 2024, 18:12
 */


#include <xc.h>
#include "TAD_TIMER.H"
#include "LcTLCD.h"

static unsigned char tecla = 0, i = 0;
static unsigned char teclaPremuda = 0;
static unsigned char digitNumber = 0;
static unsigned char enable = 0;
unsigned char t, t1;

void keyboardEnable () {
    tecla = 0;
    enable = 1;
}

void keyboardDisable () {
    enable = 0;
}

char TeclaPremuda(void) {
    return teclaPremuda;
}

char getDigitNumber(void) {
    return digitNumber;
}

void resetDigitNumber (void) {
    digitNumber = 0;
}

void init_KEYB (void) {
    TI_NewTimer(&t);
    TI_NewTimer(&t1);
}

void rotarFila(void) {
    unsigned char temp = (LATB << 1); 
    if(temp & 0b00010000) {
        temp = temp >> 4;
    }
    LATB = temp;
}

char keyFlag(void) {
   return (PORTB & 0b01110000);
}

char getKeyCode (void) {
    return ((LATB & 0x0F) | (PORTB & 0x70));
}

char getTecla() {
    return tecla;
}

char getKey(void) {
    unsigned char msb = (getKeyCode() & 0b11110000) >> 4;
    if (msb == 4 ) msb--;
    unsigned char lsb = (getKeyCode() & 0b00001111);
    if (lsb == 0b00001000) {
        return msb;
    }
    else if (lsb == 0b00000001) {
        return msb + 3;
    }
    else if (lsb == 0b00000010) {
        return msb + 6;
    }
    else {
        return msb + 9;
    }
}

char hashtagSelected(void) {
    return getKey() == 12;
}

char decode(char key) {
    //todo: optimitzar
    if (i) {
        if (key == 2 || key == 3 || key == 7 || key == 4) {
            return ((key - 2)*3 + i) + 'A' - 1;
        }
        else if (key == 9) {
            return ((key - 2)*3 + i) + 'A';
        }
        else if (key == 5 || key == 6) {
            return ((key - 3)*3 + i) + 'A' + 2;
        }
        else if (key == 11) {
            return ' ';
        }
        else {
            return ((key - 4)*4 + i) + 'A' + 2;
        }
    }
    if (key == 11) {
        return '0';
    }
    return key + '0';
} 

void TAD_Teclat() {
    static unsigned char estat = 0;
    //static unsigned char teclaPremuda = 0;
    
    switch (estat) {
       case 0:
           rotarFila();
           estat++;
           break;
       case 1:
           
           if (tecla) {
               if (TI_GetTics(t1) >= 4000) {
                   if (enable && tecla != 12) {
                       LcPutChar(decode(tecla));
                       digitNumber++;
                       tecla = 0;
                       i = 0;
                   }
               }
           }
           else {
               TI_ResetTics(t1);
           }
           
           
           if (keyFlag()) {
               TI_ResetTics(t);
               estat++; 
           }
           else {
               estat--;  
           }    
           break;
       case 2:
            if (TI_GetTics(t) >= 64) {
                if (!keyFlag()) {
                    estat--;
                    estat--;
                }
                else {
                    teclaPremuda++;
                    TI_ResetTics(t);
                    if (!tecla) {
                        tecla = getKey();
                        //LcPutChar(decode(tecla));
                    }
                    else if (tecla == getKey()) {
                        
                        
                        if (tecla == 1) i = 0;
                        else if (tecla == 11) {
                            if (i++ >= 2) {
                                i = 0;
                            }
                        }
                        else if (tecla == 7 || tecla == 9) {
                            if (i++ >= 4) {
                            i = 0;
                            }
                        }
                        else {
                            if (i++ >= 3) {
                            i = 0;
                            }
                        }
                    }
                    else {
                        if (enable && tecla != 12) {
                            LcPutChar(decode(tecla));
                            digitNumber++;
                            i = 0;
                        }
                        TI_ResetTics(t1);
                        tecla = getKey();
                    }
                    estat++;
                }
            }

           break;

       case 3:
            if (TI_GetTics(t) >= 64) {
                if (!keyFlag()) {
                    teclaPremuda--;
                    estat = 1;
                }
                else {
                    TI_ResetTics(t);
                }
            }
           break;

   } 
}
