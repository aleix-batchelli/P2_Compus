#include <xc.h>
#include "LcTLCD.h"
#include "TAD_TECLAT.H"
#include "TAD_TIMER.H"
#include "TAD_JOYSTICK.H"
#include "TAD_WIFI.H"
#include "TAD_UART.H"
#include "TAD_WORK.h"

#define MENU1 1
#define MENU2 2
#define MENU3 3
#define MENU4 4
#define CIP_CLOSE 6

static unsigned char t;
static char menus[][23] = {
    {"1: LIST ACCESS POINTS "},
    {"2: SHOW CONN. STATUS  "},
    {"3: CHANGE CHAT MODE   "},
    {"4: ENTER CHAT MODE    "},
    {"5: LIST LAST MSGS.    "},
    {"6: CLOSE CONNECTION   "},
    {"                      "}
};


static char userFlag = 0;
static char bye[13] = "bye bye ";
static char menuTop; //menu bootom = menuTOp + 1, canviar.
static char menuBottom;
static char index = 0;
static unsigned char i = 0;
static unsigned char offset = 0;
static char a;
static unsigned char count = 0;
static char name[4] = "    ";
static char message[16] = "                ";

static unsigned char remotePort[4];
static unsigned char localPort[4];

void printMarquesina() {
    static char row = 0;
    if (menus[menuTop][index+i] == '\0') {
        offset = 23;
        i++;
    }
    if (!row) {
        LcGotoXY(i, 0);
        LcPutChar(menus[menuTop][index+i-offset]);
        row++;
    }
    else {
        LcGotoXY(i, 1);
        LcPutChar(menus[menuBottom][index+i-offset]);
        row--;
        i++;
    }
}

void init_Menu (void) {
    TI_NewTimer(&t);
}

char clearMessage() {
    static unsigned char i = 0;
    if (i < 16) {
        message[i] = ' ';
        i++;
        return 0;
    }
    i = 0;
    return 1;
}

void TAD_Menu(void) {
    static unsigned char state = 0;               // VIGILAR QUE HO HEM DE TORNAR A POSAR A 0
    char flag;
    switch (state) {
        case 0:
            userFlag = 0;
            keyboardEnable();
            LcClear();
            LcGotoXY(0, 0);
            menuTop = 0;
            menuBottom = 1;
            resetDigitNumber();
            state++;
            callFunc(CIP_CLOSE);
            break;
            
        case 1:
            
            printString("DEST.PORT:      ");
            state++;
            break;
            
        case 2:
            if (getDigitNumber() == 4) {
                TI_ResetTics(t);
                resetDigitNumber();
                state++;
            }
            else {
                remotePort[getDigitNumber()] = decode(getTecla());
            }
            
            break;
            
        case 3:
            if (TI_GetTics(t) >= 5000) {
                LcClear();
                LcGotoXY(0, 0);
                state++;
            }
            break;
            
        case 4:
            printString("REC.PORT:       ");
            state++;
            break;
            
        case 5:
            if (getDigitNumber() == 4) {
                TI_ResetTics(t);
                state++;
                resetDigitNumber();
            }
            else {
               localPort[getDigitNumber()] = decode(getTecla());
            }
            break;
            
        case 6:
            if (TI_GetTics(t) >= 5000) {
                LcClear();
                LcGotoXY(0, 0);
                state++;
            }
            break;
            
        case 7:     
            if (!userFlag) {
                printString("USER NAME:      ");
                userFlag++;
                state++;
            } else {
                state = 10;
                callFunc(MENU3);
            }
            break;
        case 8:
            if (getDigitNumber() == 3) {
                TI_ResetTics(t);
                state++;
                resetDigitNumber();
                bye[11] = '!';
                bye[12] = '\0';
            }
            else {
                bye[8+getDigitNumber()] = decode(getTecla());
                name[getDigitNumber()] = decode(getTecla());
            }
            break;
            
        case 9:
            if (TI_GetTics(t) >= 5000) {
               callFunc(MENU3);
               TI_ResetTics(t);
               state++;
            }
            break;
            
        case 10:
            if (cipStart (remotePort, localPort)) {
                state++;
                count = 0;
            }
            break;
            
        case 11:
            flag = getFlag();
            clearFlag();
            if (flag == 1) {
                state++;
                count = 0;
            }
            else if (count >= 3 &&  TI_GetTics(t) >= 4000) {
                count = 0;
                state = 0;
            }
            else if (flag == 2 || TI_GetTics(t) >= 50000) {
                callFunc(MENU3);
                TI_ResetTics(t);
                count++;
            }
            break;
            
        case 12:
            //FER COMPROVACIONS CONNEXIO WIFI
            keyboardDisable();
            state++;
            i = 0;
            offset = 0;
            index = 0;
            TI_ResetTics(t);
            break;
            
        case 13:
            if (TI_GetTics(t)) {
                printMarquesina();
                TI_ResetTics(t);
                if (i >= 16) state++;
            }
            
            break;
        
        case 14:
            
            if (TI_GetTics(t) >= 5000) {
                index++;
                TI_ResetTics(t);
                if (index  >= 23) {
                    index = 0;
                }
                i = 0;
                offset = 0;
                state--;
                state--;
            }
            state++;
            break;
            
        case 15:
            if (hashtagSelected()) {
                state = 90;
            }
            else if (!ADCON0bits.GO_DONE) {
                state = getJoyState();
                ADCON0bits.GO_DONE = 1;
            }
            else {
               state--; 
            }
            break;
            
        case 90:
            if (!hashtagSelected()){
                state = 18 + menuTop;
                LcClear();
            }
            break;
            
        case 16:
            //down
            if (!(menuBottom == 6)) {
                menuTop++;
                menuBottom++;
                state = 12;
            }
            else {
                state = 13;
            }
            break;
        case 17:
            //up
            if (!(menuTop == 0)) {
                menuTop--;
                menuBottom--;
                state = 12;
            }
            else {
                state = 14;
            }
            break;
            
        case 18:
            //1. List acces points
            printString("SCANNING...");
            callFunc(MENU1);
            TI_ResetTics(t);
            state = 24;
            break;
            
        case 19:
            //2
            printString("SCANNING...");
            callFunc(MENU2);  
            TI_ResetTics(t);
            state = 24;
            break;
            
        case 20:
            //3
            resetDigitNumber();
            keyboardEnable();
            LcClear();
            state = 1;
            break;
            
        case 21:
            //4
            if (clearMessage()) {
                LcClear();  
                resetDigitNumber();
                keyboardEnable();
                state = 25;
            }
            break;
            
        case 22:
            //5
            break;
            
        case 24:
            if (getFlag() || TI_GetTics(t) >= 50000) {
                clearFlag();
                state = 13;
            }
            break;
            
        case 23:
            if (TI_GetTics(t) >= 8000) {
                LcClear();
                //printar bye bye
                state = 0;
            }
            break;
            
        case 25:
            if (hashtagSelected()) {
                resetDigitNumber();
                callFunc(MENU2);
                keyboardDisable();
                clearFlag();
                state++;
            }
            message[getDigitNumber()] = decode(getTecla());
            break;
        case 26:
            flag = getFlag();
            if (flag) {
                state = 29;
            }
            /*if (flag == 1) {
                state = 29;
            }
            else if (flag == 2) {
                callFunc(MENU3);
                state++;
            }*/
            break;
        case 27:
            flag = getFlag();
            if (flag == 1) {
                state = 29;
            }
            else if (flag == 2) {
                printString("ERROR");
                TI_ResetTics(t);
                state++;
            }
            break;
        case 28:
            if (TI_GetTics(t) >= 25000) {
                TI_ResetTics(t);
                state = 11;
            }
            break;
        case 29:
            if (setMessage(name, message)) {
                state++;
            }
            break;
        case 30:
            callFunc(MENU4);
            state = 21;
            break;
        case 31:
            /*
             flag = getFlag();
            if (flag == 1) {
                state = 21;
            }
             */
            break;
            
            
    }
}