
#pragma once 
#include "ofMain.h"
#include "buttonRect.h"

class buttonToggle : public buttonRect{
	
	public:
	
	buttonToggle(){
		mode=0;
	}

	void setup( string onName, float xIn, float yIn, float w, float h, string fn ){
		state_title.clear();
		state_title.push_back(onName);
		
		ofImage icon;
		if(fn.length()>0){
			hasIcon.push_back(true);
			icon.loadImage(fn);
			icons.push_back(icon);
		}
		else{
			hasIcon.push_back(false);
			icon.loadImage("images/empty75.png");
			icons.push_back(icon);
		}
		
		num_mode=1;
		x		= xIn;
		y		= yIn;
		width	= w;
		height	= h;
	}
	bool update(float xIn, float yIn){	
		if( inRect(xIn, yIn) )
		{
			mode=(++mode)%num_mode;
			return true;
		}else
		{
			return false;
		}
	}	
	void update(){	
		mode=(++mode)%num_mode;
	}	
	void set(int i){	
		mode=i%num_mode;
	}	
	void reset(){
		mode=0;
	}	
	void addState(string s, string fn){
		state_title.push_back(s);
		ofImage icon;
		icon.loadImage(fn);
		icons.push_back(icon);
		if(fn.length()>0)	hasIcon.push_back(true);
		else			hasIcon.push_back(false);
		num_mode++;
	}
	
	void draw(){
		ofFill();
		if(active)	ofSetColor(150,150,255);	
		else ofSetColor(180, 180.0, 180);	
		ofRect(x, y, width, height); 

		ofNoFill();
		ofSetColor(30, 30, 30);
		ofRect(x, y, width, height); 

		float textWidth = 8.0f * state_title[mode].length();
		float remainX = (width - textWidth)/2;

		float textHeight = 14.0f;
		float remainY = (height - textHeight)/2.0f + (textHeight/2.0f);
		
		ofSetColor(255,255,255);
		icons[mode].draw(x,y);
		if(hasIcon[mode]){
		}
		else{
			ofSetColor(0,0,0);
			ofDrawBitmapString(state_title[mode], x + 5, y + remainY);//x + remainX, y + remainY);
		}
	}
		
	int mode; 
	vector<string> state_title;
	vector<ofImage> icons;
	vector<bool> hasIcon;
	int num_mode;
	
};