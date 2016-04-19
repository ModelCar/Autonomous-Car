/*
 * ultrasound.c
 *
 *  Created on: 13.05.2015
 *  Author: Ferdinand Goss, Julius Imbery, Markus Steimle
 */

#include <system.h>
#include "uart.h"
#include "ultrasound.h"


/****************************************************************************
  Function:
    void ultrasound_init(long Address)

  Description:
    This function initializes the UART for the Ultrasound.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/
void ultrasound_init()
{
	uart_init(NEW_ULTRASOUND0_BASE, 0x00);
	uart_init(NEW_ULTRASOUND1_BASE, 0x00);
}



/****************************************************************************
  Function:
    unsigned int ultrasound_read(long Address, unsigned char settings)

  Description:
   // This function receives one Byte via the corresponding UART from the Address

  Precondition:
    Call ultrasound_init(long Address) prior to use this function

  Parameters:
    long Address 			- Address of the UART (and therefore of the Ultrasound)
	unsigned char settings 	- defines maximum distance to measure (1: 11m; 0: 5m)

  Returns:
    unsigned int - distance in mm

  Remarks:
    Distance in meter
  ***************************************************************************/
unsigned int ultrasound_read(long Address, unsigned char settings) 
{
	unsigned int i;
	unsigned char res_l, res_h;
	uart_send_byte(Address, 0xe8);
	for (i = 0; i < 300; i++);
	uart_send_byte(Address, 0x02);
	for (i = 0; i < 300; i++);
	if (settings)
		uart_send_byte(Address, 0xb8); 
	else
		uart_send_byte(Address, 0xb0); 
	for (i = 0; i < 300; i++);
	res_h = uart_receive_byte(Address);
	for (i = 0; i < 300; i++);
	res_l = uart_receive_byte(Address);
	return (((unsigned int) res_h) << 8) + res_l; 
}
