#ifndef __COM_H_INCLUDED__
#define __COM_H_INCLUDED__

/*
* File com.h
* Author: Lu Cheng
* for serial communicating
* Notes:
* (1)After remap the Serial1, should not use /dev/ttyS0 but /dev/ttyAMA0 instead. https://openenergymonitor.org/emon/node/12311
* (2)Use "gpio readall" the check the state of each pin
* (3)both of pin 15 and 16 should work on "ALT0" mode, "gpio mode 15 ALT0"
* (4)need sudo to execute, when in the pragramm which you need create documents, you can not open them with sudo created
* 
*
*/


#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>

#include <wiringPi.h>
#include <wiringSerial.h>

#define	LED	0
#define DATE_RECEIVE 3
#define DATE_TRANSFER 2


extern const char *cmd1;
extern const char *cmd2;

std::string exec(const char* cmd);
int SerialInit(int baudrate);
void sendSerialString(int device, std:: string date);
void sendsteeringspeed(int device, float speed, float steering);
int getdatefromstr(std::string str, int numberofdate,float *value);
void SerialReceive(int device, float *receivalue);



#endif
