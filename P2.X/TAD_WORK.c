#include "TAD_WORK.h"
#include "TAD_WIFI.H"
#include <xc.h>

static unsigned char func = 0;
static unsigned char* localPort;
static unsigned char* remotePort;
static char instruction[47] = "AT+CIPSTART=\"UDP\",\"255.255.255.255\",1000,1002\r\n";
static char message[16] = "";

void setLocalPort(char local[], char remote[]) {
    localPort = local;
    remotePort = remote;
}

void callFunc(char var) {
    func = var;
}

char cipStart (char* destPort, char* localPort) {
    static char offset = 0;
    char i = 36 + offset;
    instruction[i] = destPort[offset];
    instruction[i+5] = localPort[offset];
    offset++;
    
    if (offset >= 4) {
        offset = 0;
        return 1;
    }
    return 0;
}

char setMessage(char* user, char* m) {
    static char i = 0;
    
    if (i < 4) {
        message[i] = user[i];
    }
    if (i == 4) {
        message[i] = ';';
    }
    if (i > 4) {
        message[i] == m[i-4];
    }
    if (i > 13) {
        message[i] = '\r';
        message[i++] = '\n';
        i = 0;
        return 1;
    }
    i++;
    return 0;
}

void TAD_WORK(void) {
    static unsigned char state = 0;
    switch (state) {
        case 0:
            state = func;
            break;
        case 1:
            if (transmitString("AT+CWLAP\r\n", 10)) {
                
                state--;
                func--;
            }
            break;
            
        case 2:
            
            if (transmitString("AT+CIPSTATUS\r\n", 14)) {
                state = 0;
                func = 0;
            }
            break;
            
        case 3:
            if (transmitString(instruction, 47)) {
                state = 0;
                func = 0;
            }
            break;
        case 4:
            if (transmitString("AT+CIPSEND=16\r\n", 15)) {
                state++;
            }
            break;
        case 5:
            if (transmitString(message, 15)) {
                state = 0;
                func = 0;
            }
            break;
        case 6:
            if (transmitString("AT+CIPCLOSE\r\n", 13)) {
                state = 0;
                func = 0;
            }
            break;
    }
}