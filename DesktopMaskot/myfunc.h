#include <Windows.h>

inline void hideWindow(HWND hwnd) {
	long style = GetWindowLong(hwnd, GWL_STYLE);
	style &= ~(WS_VISIBLE);
	SetWindowLong(hwnd, GWL_STYLE, style);
	SetWindowPos(hwnd, NULL, 0, 0, 0, 0, (SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED));
}

inline void showWindow(HWND hwnd) {
	long style = GetWindowLong(hwnd, GWL_STYLE);
	style |= (WS_VISIBLE);
	SetWindowLong(hwnd, GWL_STYLE, style);
	SetWindowPos(hwnd, NULL, 0, 0, 0, 0, (SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED));
}