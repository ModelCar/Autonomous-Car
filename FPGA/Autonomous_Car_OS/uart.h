
/*
 * uart.h
 *
 *  Created on: 13.05.2015
 *  Author: Ferdinand Goss, Julius Imbery, Markus Steimle
 */

#ifndef uart_INCLUDED
#define uart_INCLUDED

void uart_init(long Address, unsigned char flag);
void uart_send_byte(long, unsigned char);
unsigned char uart_receive_byte(long);
void uart_send_string(char sendString[]);

#endif
