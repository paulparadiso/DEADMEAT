#ifndef _LCD_H
#define _LCD_H
/*
 *  lcd.h
 *  openFrameworks
 *
 *  Created by Paul Paradiso on 12/2/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"

class LCD{
public:
	
	LCD();
	void writeMsg(char * _msg, int _len, ofSerial *_ser);

};

#endif