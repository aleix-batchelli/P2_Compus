#ifndef T_UART_H
#define T_UART_H

char sendByte(char _byte);

char getData(void);

void TAD_UART(void);

void init_UART(void);

char sendString (char* string, char len);

void debug();

void clearFlag();

char getFlag();

#endif
