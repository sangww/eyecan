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


#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <stdio.h>

HHOOK g_hMouseHook;
HHOOK g_hKeyHook;
int _mouse_x;
int _mouse_y;
int _mouse_code;
int _key_code;
int _key_state;

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		PMSLLHOOKSTRUCT pmll = (PMSLLHOOKSTRUCT) lParam;
		_mouse_x = pmll->pt.x;
		_mouse_y = pmll->pt.y;
		_mouse_code = wParam;
	}
	if(_mouse_code==519 || _mouse_code==520)
		return 1;
	return CallNextHookEx(g_hMouseHook, nCode, wParam, lParam);
}
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{	
	_key_state = wParam;
	_key_code = ((KBDLLHOOKSTRUCT *) lParam)->vkCode;
	if(_key_code == 119) return 1;
	return CallNextHookEx(g_hKeyHook, nCode, wParam, lParam);
}