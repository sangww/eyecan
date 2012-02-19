#include "WinAction.h"
#include "appDelegate.h"

//input simulation: TODO: to another manager (generic input simulator)
void winaction::MoveMouse(int _x, int _y, double scale_x, double scale_y)
{	
	double fx = (_x)*scale_x;
	double fy = (_y)*scale_y;
	INPUT  Input={0};
	Input.type      = INPUT_MOUSE;
	Input.mi.dwFlags  = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;
	Input.mi.dx = (LONG)fx;
	Input.mi.dy = (LONG)fy;
	::SendInput(1,&Input,sizeof(INPUT));
}
void winaction::sendMouseButton (DWORD c )
{  
  INPUT    Input={0};
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = c;
  ::SendInput(1,&Input,sizeof(INPUT));
}


//reset numlock
void winaction::ResetNumLock()
{
	if(0 != GetKeyState(VK_NUMLOCK))// if numlock state is off
	{    
		INPUT input;    
		size_t sz = sizeof(INPUT);    
		memset(&input, 0, sz);    
		input.type   = INPUT_KEYBOARD;    
		input.ki.wVk = VK_NUMLOCK;    
		SendInput(1, &input, sz);
	}
}


//mouse actions
void winaction::LeftClick (DWORD c )
{  
	sendMouseButton(MOUSEEVENTF_LEFTDOWN);
	sendMouseButton(MOUSEEVENTF_LEFTUP);
}
void winaction::RightClick (DWORD c )
{  
	sendMouseButton(MOUSEEVENTF_RIGHTDOWN);
	sendMouseButton(MOUSEEVENTF_RIGHTUP);
}
void winaction::LeftDown (DWORD c )
{  
	sendMouseButton(MOUSEEVENTF_LEFTDOWN);
}
void winaction::RightDown (DWORD c )
{  
	sendMouseButton(MOUSEEVENTF_RIGHTDOWN);
}
void winaction::LeftUp (DWORD c )
{  
	sendMouseButton(MOUSEEVENTF_LEFTUP);
}
void winaction::RightUp (DWORD c )
{  
	sendMouseButton(MOUSEEVENTF_RIGHTUP);
}
void winaction::MiddleDown (DWORD c )
{  
	sendMouseButton(MOUSEEVENTF_MIDDLEDOWN);
}
void winaction::MiddleUp (DWORD c )
{  
	sendMouseButton(MOUSEEVENTF_MIDDLEUP);
}
void winaction::DoubleClick (DWORD c )
{  
	LeftClick();
	LeftClick();
}
void winaction::ScrollUp (DWORD c )
{  
	INPUT in;
	in.type = INPUT_MOUSE;
	in.mi.dx = 0;
	in.mi.dy = 0;
	in.mi.dwFlags = MOUSEEVENTF_WHEEL;
	in.mi.time = 0;
	in.mi.dwExtraInfo = 0;
	in.mi.mouseData = (DWORD)(0.1*WHEEL_DELTA);
	SendInput(1,&in,sizeof(in));
}
void winaction::ScrollDown (DWORD c )
{  
	INPUT in;
	in.type = INPUT_MOUSE;
	in.mi.dx = 0;
	in.mi.dy = 0;
	in.mi.dwFlags = MOUSEEVENTF_WHEEL;
	in.mi.time = 0;
	in.mi.dwExtraInfo = 0;
	in.mi.mouseData = (DWORD)(-0.1*WHEEL_DELTA);
	SendInput(1,&in,sizeof(in));
}




//generic sendKey
void winaction::sendVKey(DWORD c)
{  
	KEYBDINPUT  kb={0};
	INPUT    Input={0};  
	// down 
	kb.wVk  = (WORD)c;  
	Input.type  = INPUT_KEYBOARD;
	Input.ki  = kb;
	::SendInput(1,&Input,sizeof(Input));
	//clear
	::ZeroMemory(&kb,sizeof(KEYBDINPUT));
	::ZeroMemory(&Input,sizeof(INPUT));
	// up
	kb.dwFlags  =  KEYEVENTF_KEYUP;
	kb.wVk    =  (WORD)c;
	Input.type  =  INPUT_KEYBOARD;
	Input.ki  =  kb;
	::SendInput(1,&Input,sizeof(Input));
}
void winaction::sendVKeyDown(DWORD c)
{  
	KEYBDINPUT  kb={0};
	INPUT    Input={0};  
	// down 
	kb.wVk  = (WORD)c;  
	Input.type  = INPUT_KEYBOARD;
	Input.ki  = kb;
	::SendInput(1,&Input,sizeof(Input));
}
void winaction::sendVKeyUp(DWORD c)
{  
	KEYBDINPUT  kb={0};
	INPUT    Input={0};  
	// up
	kb.dwFlags  =  KEYEVENTF_KEYUP;
	kb.wVk    =  (WORD)c;
	Input.type  =  INPUT_KEYBOARD;
	Input.ki  =  kb;
	::SendInput(1,&Input,sizeof(Input));
}

void winaction::sendSKey(DWORD c)
{
	sendSKeyDelay(c,0);
}
void winaction::sendSKeyDelay(DWORD c)
{
	sendSKeyDelay(c,100);
}
void winaction::sendSKeyDelay(DWORD c, int delay)
{  
	KEYBDINPUT  kb={0};
	INPUT    Input={0};  
	// down 
	kb.dwFlags  =  KEYEVENTF_SCANCODE;
	kb.wScan  = (WORD)c;;  
	Input.type  = INPUT_KEYBOARD;
	Input.ki  = kb;
	::SendInput(1,&Input,sizeof(Input));
	//clear
	::ZeroMemory(&kb,sizeof(KEYBDINPUT));
	::ZeroMemory(&Input,sizeof(INPUT));

	//if delay
	if(delay>0)
		Sleep(delay);

	// up
	kb.dwFlags  =  KEYEVENTF_KEYUP|KEYEVENTF_SCANCODE;
	kb.wScan  = (WORD)c;;  
	Input.type  =  INPUT_KEYBOARD;
	Input.ki  =  kb;
	::SendInput(1,&Input,sizeof(Input));
}
void winaction::sendSKeyDown(DWORD c)
{  
	KEYBDINPUT  kb={0};
	INPUT    Input={0};  
	// down 
	kb.dwFlags  =  KEYEVENTF_SCANCODE;
	kb.wScan = (WORD)c;
	Input.type  = INPUT_KEYBOARD;
	Input.ki  = kb;
	::SendInput(1,&Input,sizeof(Input));
}
void winaction::sendSKeyUp(DWORD c)
{  
	KEYBDINPUT  kb={0};
	INPUT    Input={0};  
	// up
	kb.dwFlags  =  KEYEVENTF_KEYUP|KEYEVENTF_SCANCODE;
	kb.wScan  = (WORD)c;  
	Input.type  =  INPUT_KEYBOARD;
	Input.ki  =  kb;
	::SendInput(1,&Input,sizeof(Input));
}




//special key combination - TODO:use functions above
void winaction::AltTab ( )
{  
	KEYBDINPUT k1 = {VK_MENU, 0, 0, 0, 0};
	KEYBDINPUT k2 = {VK_TAB, 0,0,0, 0};
	KEYBDINPUT k3 = {VK_TAB, 0,KEYEVENTF_KEYUP,0, 0};
	KEYBDINPUT k4 = {VK_MENU, 0,KEYEVENTF_KEYUP,0, 0};
	INPUT in1 = {INPUT_KEYBOARD};
	in1.ki = k1;
	INPUT in2 = {INPUT_KEYBOARD};
	in2.ki = k2;
	INPUT in3 = {INPUT_KEYBOARD};
	in3.ki = k3;
	INPUT in4 = {INPUT_KEYBOARD};
	in4.ki = k4;
		
	SendInput(1, &in1, sizeof(INPUT));
	SendInput(1, &in2, sizeof(INPUT));
	SendInput(1, &in3, sizeof(INPUT));
	SendInput(1, &in4, sizeof(INPUT));
}
void winaction::AltF4 ( )
{  
	KEYBDINPUT k1 = {VK_MENU, 0, 0, 0, 0};
	KEYBDINPUT k2 = {VK_F4, 0,0,0, 0};
	KEYBDINPUT k3 = {VK_F4, 0,KEYEVENTF_KEYUP,0, 0};
	KEYBDINPUT k4 = {VK_MENU, 0,KEYEVENTF_KEYUP,0, 0};
	INPUT in1 = {INPUT_KEYBOARD};
	in1.ki = k1;
	INPUT in2 = {INPUT_KEYBOARD};
	in2.ki = k2;
	INPUT in3 = {INPUT_KEYBOARD};
	in3.ki = k3;
	INPUT in4 = {INPUT_KEYBOARD};
	in4.ki = k4;
		
	SendInput(1, &in1, sizeof(INPUT));
	SendInput(1, &in2, sizeof(INPUT));
	SendInput(1, &in3, sizeof(INPUT));
	SendInput(1, &in4, sizeof(INPUT));
}



//AUDIO UI
void winaction::playAlarmSound(DWORD c)
{  
	app->playAlarmSound(c);
}
void winaction::playSound(DWORD c)
{  
	app->playSound(c);
}