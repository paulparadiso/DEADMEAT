/*
 *  lcd.cpp
 *  openFrameworks
 *
 *  Created by Paul Paradiso on 12/2/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "lcd.h"

LCD::LCD(){
}

void LCD::writeMsg(char* _msg, int _len, ofSerial *_ser){
	unsigned char start = 'b';
	unsigned char end = '\r';
	_ser->writeByte(start);
	//serial.writeBytes((unsigned char*)_msg,sizeof(_msg));
	for(int i = 0; i < _len;i++){
		_ser->writeByte(_msg[i]);
		printf("Writing %c \n",_msg[i]);
	}
	_ser->writeByte(end);
}