/*
    eyeCan: a program that enables people to use PC with their eyes
    Copyright (C) 2012  Sang-won Leigh

    eyeCan is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    eyeCan is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


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
