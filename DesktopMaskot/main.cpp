#include <Windows.h>
#include "MYBRUSH.h"
#include "loadPng.h"
#include "resource2.h"
#include <gdiplustypes.h>
#include "body.h"
#pragma comment (lib,"Gdiplus.lib")


const int WIDTH = 500, HEIGHT = 620;
yukari y;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	PAINTSTRUCT ps;
	RECT rc;
	POINT po;
	static HBITMAP hBitmap = NULL;
	static HDC     hdcMem;
	static HMENU tmp, hmenuR = NULL;
	static BOOL blRight = TRUE;
	HGDIOBJ hb;
	HFONT hFont;
	Gdiplus::Graphics* g;
	Gdiplus::RectF const src = Gdiplus::RectF(0, 0, 200, 200);
	switch (msg) {
	case WM_DESTROY:
		DestroyWindow(hwnd);
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		HDC hdc;
		tmp = LoadMenu(((LPCREATESTRUCT)(lp))->hInstance, TEXT("yukari"));
		hmenuR = GetSubMenu(tmp, 0);
		hdc = GetDC(hwnd);
		GetClientRect(GetDesktopWindow(), &rc);  	// デスクトップのサイズを取得
		hBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
		hdcMem = CreateCompatibleDC(NULL);		// カレントスクリーン互換
		SelectObject(hdcMem, hBitmap);		// MDCにビットマップを割り付け
		SetTimer(hwnd, 1, 33, NULL);
		return 0;
	case WM_TIMER:
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hFont = CreateFont(
			22,0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			VARIABLE_PITCH | FF_ROMAN, TEXT("たいぷ")
		);
		PAINTSTRUCT ps;
		hdc = BeginPaint(hwnd, &ps);
		g = new Gdiplus::Graphics(hdcMem);
		hb = SelectObject(hdcMem, BRACK_BRUSH);
		//std::cout << ",SelectObject:" << hb;
		Rectangle(hdcMem, -1, -1, WIDTH+1, HEIGHT+1);
		DeleteObject(hb);
		SelectObject(hdcMem,hFont);
		SetTextColor(hdcMem, RGB(255, 255, 255));
		SetBkMode(hdcMem, TRANSPARENT);
		y.main(g, hdcMem);
		BitBlt(hdc, 0, 0, WIDTH, HEIGHT, hdcMem, 0, 0, SRCCOPY);
		DeleteObject(hFont);
		DeleteObject(hb);
		EndPaint(hwnd, &ps);
		//std::cout << "\n";
		delete(g);
		return 0;
	case WM_RBUTTONUP:
		po.x = LOWORD(lp);
		po.y = HIWORD(lp);
		ClientToScreen(hwnd, &po);
		TrackPopupMenu(hmenuR, TPM_LEFTALIGN | TPM_BOTTOMALIGN, po.x, po.y, 0, hwnd, NULL);
		return 0;
	case WM_COMMAND:
		if (wp == 40006) {
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}
		return 0;
	case WM_ERASEBKGND:
		return FALSE;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow) {
	HWND hwnd;
	MSG msg;
	WNDCLASS winc;


	if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
		AllocConsole();
	}
	FILE* fpOut = NULL;

	freopen_s(&fpOut, "CONOUT$", "w", stdout);
	std::cout << "読み込み完了";
	winc.style = CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc = WndProc;
	winc.cbClsExtra = winc.cbWndExtra = 0;
	winc.hInstance = hInstance;
	winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winc.hCursor = LoadCursor(NULL, IDC_ARROW);
	winc.hbrBackground = BRACK_BRUSH;
	winc.lpszMenuName = NULL;
	winc.lpszClassName = TEXT("yukari");
	RECT DISPLAY_SIZE;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &DISPLAY_SIZE, 0);
	if (!RegisterClass(&winc)) return 0;

	hwnd = CreateWindowEx(
		WS_EX_LAYERED,
		TEXT("yukari"), TEXT("ゆかりさんますこっと"),
		WS_POPUP | WS_VISIBLE,
		DISPLAY_SIZE.right - WIDTH, DISPLAY_SIZE.bottom - HEIGHT, WIDTH, HEIGHT, NULL, NULL,
		hInstance, NULL
	);

	SetLayeredWindowAttributes(hwnd, 0x00000000, 0, LWA_COLORKEY);
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	if (hwnd == NULL) return 0;
	while (GetMessage(&msg, NULL, 0, 0)) DispatchMessage(&msg);
	FreeConsole();

	return msg.wParam;
}