/*
 *  buttonRect.h
 *  openFrameworks
 *
 *  Created by theo on 17/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once 

#include "ofMain.h"

class buttonRect : public ofRectangle{
	
	public:
	
	buttonRect(){
		active  = false;
		hasLeft = true;
		counter = 0.0f;
	}
	
	bool inRect(float xIn, float yIn){
		if( xIn >= x+ofGetWindowPositionX() && yIn >= y+ofGetWindowPositionY()
			&& xIn <= x+ofGetWindowPositionX() + width && yIn <= y+ofGetWindowPositionY() + height){
			return true;
		}
		return false;
	}
	
	
	bool getState(){
		return active;
	}
	
	
	bool active;
	bool hasLeft;
	float counter;


	void draw(){	
	}

};