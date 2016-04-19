
/*
 * communication.c
 *
 *  Created on: 13.05.2015
 *  Author: Ferdinand Goss, Julius Imbery, Markus Steimle
 */

#include "communication.h"
#include "uart.h"
#include <system.h>
#include "altera_avalon_uart_regs.h"


/****************************************************************************
  Function:
    void communication_init()

  Description:
    This function initializes the communication with the Raspberry PI.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    communication is realized via UART3
  ***************************************************************************/
void communication_init()
{
	uart_init(NEW_ULTRASOUND3_BASE, ALTERA_AVALON_UART_CONTROL_RRDY_MSK);
}


/****************************************************************************
  Function:
    void sendToBoard(unsigned char data)

  Description:
    This function sends one Byte to the Raspberry PI

  Precondition:
    Call communication_init() prior to use this function

  Parameters:
    unsigned char data - Byte to be send

  Returns:
    None

  Remarks:
    communication is realized via UART3
  ***************************************************************************/
void sendToBoard(unsigned char data)
{
	uart_send_byte(NEW_ULTRASOUND3_BASE, data);
}

/****************************************************************************
  Function:
    void receiveFromBoard()

  Description:
    This function receives one Byte from the Raspberry PI

  Precondition:
    Call communication_init() prior to use this function

  Parameters:
    None

  Returns:
    unsigned char -  received Byte

  Remarks:
    communication is realized via UART3
  ***************************************************************************/
unsigned char receiveFromBoard()
{
	return uart_receive_byte(NEW_ULTRASOUND3_BASE);
}
