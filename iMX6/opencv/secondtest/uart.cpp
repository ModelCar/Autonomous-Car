
/*
 * uart.cpp
 *
 *  Created on: 13.05.2015
 *  Author: Ferdinand Goss, Julius Imbery, Markus Steimle
 *  Modified by: Robert Loesch
 * 
 *  Changes by Roger Roesch:
 *  Added timeout support, flush command, start, stop characters, optional errors
 *  Changed: uart_setup needs port as parameter
 */

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <chrono>       // timeouts
#include <thread>				// sleeping
#include <errno.h>      // Error number definitions
#include "uart.h"

using namespace std;
using namespace std::chrono;

int  uart0_filestream = -1;

/****************************************************************************
 *  Function:
 *    int uart0_setup(void)
 * 
 *  Description:
 *    This function initializes the UART0
 * 
 *  Precondition:
 *    None
 * 
 *  Parameters:
 *	None
 * 
 *  Returns:
 *    int uart_filestream - number of corresponding uart of the Raspberry, -1 if an error occurs
 * 
 *  Remarks:
 *	None
 ***************************************************************************/
int uart0_setup(const char *port)
{
	uart0_filestream = open(port,O_RDWR | O_NOCTTY/* | O_NDELAY | O_NONBLOCK*/);
	/* Error Handling */
	if ( uart0_filestream < 0 ) {
		cout << "Error " << errno << " opening " << port << ": " << strerror (errno) << endl;
	}
	struct termios tty;
	struct termios tty_old;
	memset (&tty, 0, sizeof tty);
	
	/* Error Handling */
	if ( tcgetattr ( uart0_filestream, &tty ) != 0 ) {
		std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
	}
	
	/* Save old tty parameters */
	tty_old = tty;
	
	/* Set Baud Rate */
	cfsetospeed (&tty, (speed_t)B115200);
	cfsetispeed (&tty, (speed_t)B115200);
	
	/* Setting other Port Stuff */
	tty.c_cflag     &=  ~PARENB;            // Make 8n1
	tty.c_cflag     &=  ~CSTOPB;
	tty.c_cflag     &=  ~CSIZE;
	tty.c_cflag     |=  CS8;
	
	tty.c_cflag     &=  ~CRTSCTS;           // no flow control
	tty.c_cc[VMIN]   =  1;                  // read doesn't block
	tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
	tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
	
	/* Make raw */
	cfmakeraw(&tty);
	
	/* Flush Port, then applies attributes */
	tcflush( uart0_filestream, TCIFLUSH );
	if ( tcsetattr ( uart0_filestream, TCSANOW, &tty ) != 0) {
		std::cout << "Error " << errno << " from tcsetattr" << std::endl;
	}
	fcntl(uart0_filestream, F_SETFL,fcntl(uart0_filestream, F_GETFL) |  O_NONBLOCK);
	return uart0_filestream;
}

/****************************************************************************
 *  Function:
 *	int uart_send_byte(char data)
 * 
 *  Description:
 *    This function sends one byte via the UART
 * 
 *  Precondition:
 *    Call uart0_setup() prior to use this function
 * 
 *  Parameters:
 *	char data - Byte to be send
 * 
 *  Returns:
 *    -1 when an error occurs
 *	1 when byte was successfully transmitted
 * 
 *  Remarks:
 *	prints the status and the sent byte or an error at the terminal
 ***************************************************************************/
int uart_send_byte(char data)
{
	if (uart0_filestream != -1) {
		int n_written = 0;
		int spot = 0;
		n_written = write( uart0_filestream, &data, 1 );
		spot += n_written;
		if (n_written < 0) {
			printf("[ERROR] UART TX \n");
		}
		else {
			data = '\n';
			if (write( uart0_filestream, &data, 1 )!=0);
				return 1;
		}
		
	}
	return 0;
}


/****************************************************************************
 *  Function:
 *	int uart_receive(void)
 * 
 *  Description:
 *    This function receives bytes via the UART
 * 
 *  Precondition:
 *	Call uart0_setup() prior to use this function
 * 
 *  Parameters:
 *	None
 * 
 *  Returns:
 *    -1 when an error occurs
 *	1 when at least one byte was successfully received
 * 
 *  Remarks:
 *	prints the status and the received byte(s) or an error at the terminal
 ***************************************************************************/
int uart_receive(char* buffer, int64_t timeout/*=50*/,  int expectedlen /*=1000000*/, int breaking/*=-1*/, int starting_char/*=-1*/,int showerror/*=0*/)
{
	/* Whole response*/
	int n = 0;
	
	int spot = 0;
	char buf = '\0';
	
	if (uart0_filestream != -1) {
		
		// used for timeout control
		int64_t ms_current, ms_starting;
		ms_starting = system_clock::now().time_since_epoch() / milliseconds(1);
		
		do {
			n = read( uart0_filestream, &buf, 1 );
			// check starting char
			if (starting_char >=0 && buf != starting_char && n>0)
				continue;
			else if (starting_char!=-1 && n>0) 
				starting_char=-1;
			// check ending char
			if (buf == (char)breaking)
				break;
			sprintf( &buffer[spot], "%c", buf );
			if (n>0)
				spot += n;
			ms_current = system_clock::now().time_since_epoch() / milliseconds(1);
			this_thread::sleep_for(chrono::milliseconds(0));
			if (ms_current - ms_starting > timeout) 
				timeout=-1;
		} while( spot < expectedlen && timeout>0 );
		
		// show errors if requested
		if (timeout ==-1)
			if (showerror)
				cout << "!!!Timeout error" << endl;
		if (n < -1  && spot == 0) {
			if (showerror)
				printf("Error reading: %i,%s\n",n, strerror(errno));
			return 0;
		}
		buffer[spot+1]='\0';
		return spot;
	}
	return 0;
}

/****************************************************************************
 *  Function:
 *	int uart_flush()
 * 
 *  Description:
 *    This function clears all input buffers on the UART
 * 
 *  Precondition:
 *    Call uart0_setup(port) prior to use this function
 * 
 *  Parameters:
 *	none
 * 
 *  Returns:
 *    -1 when an error occurs
 *	1 when byte was successfully transmitted
 * 
 *  Remarks:
 *	prints the status and the sent byte(s) or an error at the terminal
 ***************************************************************************/
int uart_flush(){
	if (tcflush(uart0_filestream,TCIOFLUSH)==-1)
		printf("Error Flushing: %s\n", strerror(errno));;
	return 1;
}

/****************************************************************************
 *  Function:
 *	int uart_send_string(char str[])
 * 
 *  Description:
 *    This function sends a string via the UART
 * 
 *  Precondition:
 *    Call uart0_setup() prior to use this function
 * 
 *  Parameters:
 *	char data - Byte to be send
 * 
 *  Returns:
 *    -1 when an error occurs
 *	1 when byte was successfully transmitted
 * 
 *  Remarks:
 *	prints the status and the sent byte(s) or an error at the terminal
 ***************************************************************************/
int uart_send_string(char* str, int length)
{
	if (uart0_filestream != -1) {
		int n_written = 0;
		int spot = 0;
		do {
			n_written = write( uart0_filestream, &str[spot], length );
			spot += n_written;
			if (n_written < 0) {
				printf("[ERROR] UART TX \n");
			}
			else {
				return 1;
			}
		} while (str[spot-1] != '\r' && n_written > 0);
		
	}
	return -1;
}
