#ifndef _PROGRAMMABLE_UI
#define _PROGRAMMABLE_UI


#include "ofMain.h"
#include "ofTrueTypeFontWS.h"
#include "buttonPanel.h"

class ProgrammableUI {

	public:
		ProgrammableUI();
		~ProgrammableUI();

		void setup();
		void update(int x, int y);
		void setupWindow();
		void setupWindowPosition(int x,int y);
		void setMode(int m);
		void draw(wstring state, int x, int y);
		void draw(wstring state, wstring help);
		void draw(string state, string help);
	
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseDragged(int x, int y, int button);	
		
	
		ofTrueTypeFontWS		fontWS, helpWS;

		int posX,posY; //window idle position
		float fScreenWidth; 
		float fScreenHeight; 
		int mode;
		int x_drag_start,y_drag_start;
		
		buttonPanel bp;
};

#endif
