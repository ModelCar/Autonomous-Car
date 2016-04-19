/*
 * communication.cpp
 *
 *  Created on: 13.05.2015
 *  Author: Ferdinand Goss, Julius Imbery, Markus Steimle
 */

#include "uart.h"
#include <stdio.h>
#include "communication.h"

// NOTE: All functions send the first byte 255 because the first byte received is dropped by the nano board


/****************************************************************************
  Function:
    void sendSpeed(int speed)

  Description:
    This function sends two bytes to the Nano-Board in order to set the speed of the vehicle

  Precondition:
	Call int uart0_setup(void) prior to use this function

  Parameters:
    int speed - speed from -100% till +100%

  Returns:
	None

  Remarks:
	First byte is the identifier for the Nano-Board (202 for speed).
	Second byte the percentage of the speed:
	speed = -100 means max. speed backward
	speed = +100 means max. speed forward
  ***************************************************************************/
void sendSpeed(int speed)
{
	uart_send_byte(255);
	uart_send_byte(1);
	uart_send_byte((char)(speed + 100));
}


/****************************************************************************
  Function:
    void sendSteeringAngle(double angle)

  Description:
    This function sends two bytes to the Nano-Board in order to set the steering of the vehicle

  Precondition:
	Call int uart0_setup(void) prior to use this function

  Parameters:
    double angle - steering angle from -17 to 17 degrees

  Returns:
	None

  Remarks:
	First byte is the identifier for the Nano-Board (2 for steering).
	Second byte the value to steer:
	angle = 0 means straight
	angle = -17 means complete left
	angle = 17 means complete right
  ***************************************************************************/
void sendSteeringAngle(double angle)
{
	
	if (angle< -17.0)
		angle=-17.0;
	else if (angle > 17.0)
		angle=17.0;
		
	angle+17.0;
	
	uart_send_byte(255);
	uart_send_byte(2);
	uart_send_byte((char)(angle*200.0/17.0));
}

/****************************************************************************
  Function:
    void sendSteering(int steering)

  Description:
    This function sends two bytes to the Nano-Board in order to set the steering of the vehicle

  Precondition:
	Call int uart0_setup(void) prior to use this function

  Parameters:
    int steering - steering from -100 to 100

  Returns:
	None

  Remarks:
	First byte is the identifier for the Nano-Board (201 for steering).
	Second byte the value to steer:
	steering = 0 means straight
	steering = -100 means complete left
	steering = 100 means complete right
  ***************************************************************************/
void sendSteering(int steering)
{
	uart_send_byte(255);
	uart_send_byte(2);
	uart_send_byte((char)(steering+100));
}

/****************************************************************************
  Function:
    void getGPS()

  Description:
    This function sends two bytes to the Nano-Board in order to get the GPS position

  Precondition:
	Call int uart0_setup(void) prior to use this function

  Parameters:
    None

  Returns:
	None

  Remarks:
	First byte is the identifier for the Nano-Board (3 for getGPS).
	Second byte doesn't matter because it is ignored by the Nano-Board.
  ***************************************************************************/
void getGPS()
{
	uart_send_byte(255);
	uart_send_byte(3);
	uart_send_byte(0);
}
