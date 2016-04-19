/*
 * communication.h
 *
 *  Created on: 13.05.2015
 *  Author: Ferdinand Goss, Julius Imbery, Markus Steimle
 */

#ifndef communication_INCLUDED
#define communication_INCLUDED

#include "uart.h"

extern int  uart0_filestream;

void sendSpeed(int);

void sendSteering(int);

void sendSteeringAngle(int);

void getGPS();

#endif
