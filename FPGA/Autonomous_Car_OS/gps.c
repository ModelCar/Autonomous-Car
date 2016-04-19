/*
 * gps.c
 *
 *  Created on: 27.10.2015
 *  Author: Valentin Novak
 */

#include "communication.h"
#include "uart.h"
#include <system.h>
#include "altera_avalon_uart_regs.h"
#include <stdio.h>
#include <string.h>

int dataValid(char *String);
int char_to_int(char a);

void gps_init()
{
	uart_init(NEW_ULTRASOUND2_BASE, ALTERA_AVALON_UART_CONTROL_RRDY_MSK);
}

/****************************************************************************
Function:
	void GPS(char *String)

Description:
	Function checks if the command we got is GPGGA. Then it checks checksum. At the end it parses
	latitude and longitude from the data and sends it to MX6 board via UART.

Precondition:
	None

Parameters:
 	 char *String - ASCII string that we get GPS module.

Returns:
 	 None

Remarks:
	None
***************************************************************************/
void GPS(char *String)
{
	char Command[5];
	int i = 0;
	while(String[i] != 0x2C && i < 6) { i++; }; // We find first occurrence of comma (',') in the String.

	strncpy(Command, String, i); // We copy characters (from first to comma) from String to Command.

	int b = strncmp(String,"$GPGGA",i); // We compare if command, we got is equal to GPGGA. Returns 0, when strings are identical.
	if (b == 0)
	{
		int valid = dataValid(String); // We check if checksum at the end of the string is correct. Returns 1 if it is correct.

		if (valid)
		{
			//printf("%s\n",String);
			char Lat[20];
			char Long[20];

			// We parse latitude (between second and third comma) from the data and send it to the MX6 board through UART channel 3.
			i++;
			while(String[i] != 0x2C && i < 20) { i++; };
			i++;
			int temp = i;
			while(String[i] != 0x2C && i < 30) { i++; };
			strncpy(Lat, &String[temp], i - temp);
			Lat[i-temp] = 0x00;

			// We parse longitude (between fourth and fifth comma) from the data and send it to the MX6 board through UART channel 3.
			i++;
			while(String[i] != 0x2C && i < 35) { i++; };
			i++;
			temp = i;
			while(String[i] != 0x2C && i < 40) { i++; };
			strncpy(Long, &String[temp], i - temp);
			Long[i-temp] = 0x00;


			uart_send_string("48.76");
			for(i=0;i<50000;i++);
			uart_send_string("11.26");

			//uart_send_string(Lat);
			//uart_send_string(Long);
		}
		else
		{
			//printf("Incorrect Checksum!\n");
		}
	}
	else
	{
		//printf("Wrong command!\n");
	}
}

/****************************************************************************
Function:
	int dataValid(char *String)

Description:
	This function input is string from GPS module. It calculates checksum (XOR of all bytes between $ and *) and
	checks if it matches with the checksum at the end of the string. Returns 0 if data is not valid and 1 if data is valid.

Precondition:
	None

Parameters:
 	 char *String - ASCII string that we get from GPS module

Returns:
 	 int dataValid - 0 - not valid / 1 - valid.

Remarks:
	None
***************************************************************************/
int dataValid(char *String)
{
	int i = 1;
	char calcChecksum = 0x00;

	// Calculates checksum (XOR of all bytes between $ and *). Maximum characters in NMEA standard is 80!
	while (String[i] != 0x2A && i < 80)
	{
		calcChecksum = calcChecksum ^ String[i];
		i++;
	}
	if (i > 80) return 0;

	char retChecksum = char_to_int(String[i+1]); // Converts first character of received checksum (one after *) to integer.
	char temp = char_to_int(String[i+2]); // Converts second character of received checksum (two after *) to integer.
	retChecksum =(retChecksum << 4) | temp; // Combines both numbers into one.
	if (calcChecksum == retChecksum) // Compares received and calculated checksum.
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/****************************************************************************
Function:
	int char_to_int(char a)

Description:
	Function input is ASCII character. Function returns integer (hex) value of this character.
	Works for ASCII characters 0-9 and A-F.	Example: '4' -> 4 or 0x04, 'B' -> 11 or 0x0B.

Precondition:
	None

Parameters:
 	 char a - ASCII character that we want to convert to integer.

Returns:
 	 int - Integer value of ASCII character.

Remarks:
	None
***************************************************************************/
int char_to_int(char a)
{
    int first = a / 16 - 3; // Gets the first number of the character
    int second = a % 16; // Gets the second number of the character
    int result = first*10 + second; // Combines numbers
    if(result > 9) result--; // Corrects result for the characters between A and F.
    return result;
}
