#ifndef _WIN_ACTION
#define _WIN_ACTION

#define _WIN32_WINNT 0x0500
#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <WinUser.h>

namespace winaction
{
	//input simulation: TODO: to another manager (generic input simulator)

	//mouse action
	void MoveMouse(int _x, int _y, double scale_x, double scale_y);
	void sendMouseButton(DWORD c=0);

	//reset numlock
	void ResetNumLock();


	void LeftClick (DWORD c=0 );
	void RightClick (DWORD c=0 );
	void DoubleClick (DWORD c=0 );		
	void LeftDown (DWORD c=0 );
	void RightDown (DWORD c=0 );
	void LeftUp (DWORD c=0 );
	void RightUp (DWORD c=0 );
	void MiddleDown (DWORD c=0 );
	void MiddleUp (DWORD c=0 );
	void ScrollUp (DWORD c=0 ); //TODO: scroll speed
	void ScrollDown (DWORD c=0 );

	//key combination
	void AltTab ( );
	void AltF4 ( );

	//generic key action
	void sendVKey(DWORD c);
	void sendVKeyUp(DWORD c);
	void sendVKeyDown(DWORD c);
	void sendSKey(DWORD c);
	void sendSKeyDelay(DWORD c);
	void sendSKeyDelay(DWORD c, int delay);
	void sendSKeyUp(DWORD c);
	void sendSKeyDown(DWORD c);


	//AUDIO UI
	void playAlarmSound(DWORD c);
	void playSound(DWORD c);
}

#endif
