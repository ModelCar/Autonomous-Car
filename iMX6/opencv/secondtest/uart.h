
/*
 * uart.h
 *
 *  Created on: 13.05.2015
 *  Author: Ferdinand Goss, Julius Imbery, Markus Steimle
 *  Changes by Roger Roesch:
 *  Added timeout support, flush command, start, stop characters, optional errors
 *  Changed: uart_setup needs port as parameter
 */
#include <cstdint>
#ifndef uart_INCLUDED
#define uart_INCLUDED


extern int  uart0_filestream;

int uart0_setup(const char *port);

int uart_flush();

int uart_send_byte(char);

int uart_send_string(char*, int);

int uart_receive(char* buffer,  int64_t timeout=50, int expectedlen=100000, int breaking=-1, int starting_char=-1, int showerror=0);



#endif
