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