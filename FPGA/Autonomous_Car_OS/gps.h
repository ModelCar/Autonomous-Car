/*
 * gps.h
 *
 *  Created on: 27.10.2015
 *  Author: Valentin Novak
 */

#ifndef gps_INCLUDED
#define gps_INCLUDED

void gps_init();
void GPS();
unsigned char dataValid(unsigned char *String);
int char_to_int(char a);
#endif
