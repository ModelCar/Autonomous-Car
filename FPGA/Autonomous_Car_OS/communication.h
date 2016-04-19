/*
 * comunication.h
 *
 *  Created on: 13.05.2015
 *  Author: Ferdinand Goss, Julius Imbery, Markus Steimle
 */

#ifndef communication_INCLUDED
#define communication_INCLUDED

void communication_init();

void sendToBoard(unsigned char);

unsigned char receiveFromBoard();

#endif