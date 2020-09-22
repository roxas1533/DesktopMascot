#include <Windows.h>
#include "MYBRUSH.h"
#include "loadPng.h"
#include "resource2.h"
#include <gdiplustypes.h>
#include "body.h"
#pragma comment (lib,"Gdiplus.lib")
#define ID_TRAYICON     (1)
#define WM_TASKTRAY     (WM_APP + 1)

const int WIDTH = 500, HEIGHT = 620;
yukari y;
void setIcon(NOTIFYICONDATA& nid, HICON& hIcon, LPARAM lp, HWND hwnd);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	PAINTSTRUCT ps;
	RECT rc;
	POINT po;
	static HBITMAP hBitmap = NULL;
	static HDC     hdcMem, hdc;
	static BOOL blRight = TRUE;
	static HMENU hmenuR;
	static HICON hIcon;
	static RECT rec = { 270, 0, WIDTH, HEIGHT };

	static NOTIFYICONDATA nid = { 0 };
	HGDIOBJ hb;
	static HFONT hFont = CreateFont(
		22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		VARIABLE_PITCH | FF_ROMAN, TEXT("たいぷ")
	);
	Gdiplus::Graphics* g;
	static COLORREF crEditBox, crListBox;
	static HBRUSH hbrEditBox, hbrListBox;
	static COMBOBOXINFO cbi = { sizeof(COMBOBOXINFO) };

	switch (msg) {
	case WM_DESTROY:
		DestroyWindow(hwnd);
		PostQuitMessage(0);
		Shell_NotifyIcon(NIM_DELETE, &nid);
		return 0;
	case WM_CREATE:{
		HMENU tmp = LoadMenu(((LPCREATESTRUCT)(lp))->hInstance, TEXT("yukari"));
		//アイコンロードとタスクトレイの設定
		setIcon(nid, hIcon, lp, hwnd);

		hmenuR = GetSubMenu(tmp, 0);
		hdc = GetDC(hwnd);
		GetClientRect(GetDesktopWindow(), &rc);  	// デスクトップのサイズを取得
		hBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
		hdcMem = CreateCompatibleDC(NULL);		// カレントスクリーン互換
		SelectObject(hdcMem, hBitmap);		// MDCにビットマップを割り付け
		SetTimer(hwnd, 1, 33, NULL);

		return 0;
		}
	case WM_TIMER:
		InvalidateRect(hwnd, &rec, TRUE);
		return 0;
	case WM_PAINT:
		PAINTSTRUCT ps;
		hdc = BeginPaint(hwnd, &ps);
		g = new Gdiplus::Graphics(hdcMem);
		hb = SelectObject(hdcMem, BRACK_BRUSH);
		Rectangle(hdcMem, -1, -1, WIDTH + 1, HEIGHT + 1);
		DeleteObject(hb);
		SelectObject(hdcMem, hFont);
		SetTextColor(hdcMem, RGB(255, 255, 255));
		SetBkMode(hdcMem, TRANSPARENT);
		//デバッグ用ライン
		//SelectObject(hdcMem, GREEN_BRUSH);
		//Rectangle(hdcMem, 0, 202, 500, 205);
		y.main(g, hdcMem, hwnd);
		BitBlt(hdc, 0, 0, WIDTH, HEIGHT, hdcMem, 0, 0, SRCCOPY);
		DeleteObject(hFont);
		DeleteObject(hb);
		EndPaint(hwnd, &ps);
		//std::cout << "\n";
		//std::cout << GetLastError()<<"\n";
		//std::cout<< SendMessage(
		//	(HWND)bw,  // コンボボックスのハンドル
		//	(UINT)CB_GETCURSEL,0,0);
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
		else if (wp == 40007) {
			Shell_NotifyIcon(NIM_ADD, &nid);
			hideWindow(hwnd);
		}
		else if (wp == 40004) {
			CustomButton::destroyFlag = false;
			y.fuki.push_back(std::make_unique<Fukidasi>(0));
		}
		else if (wp==1000) {
			y.fuki.front()->time = 1;
			y.fuki.push_back(std::make_unique<YoteiTuika>());
		}
		else if (wp == 1003) {
			y.fuki.front()->time = 1;
		}
		std::cout << wp<<"\n";
		return 0;
	case WM_ERASEBKGND:
		return FALSE;
	case CB_GETCURSEL:

		break;
	case WM_TASKTRAY:
		if (wp == ID_TRAYICON) {       // アイコンの識別コード
			switch (lp) {
			case WM_MOUSEMOVE:
				/* マウス移動の処理 */
				break;
			case WM_LBUTTONDOWN:
				/* 左ボタンが押された処理 */
				break;
			case WM_RBUTTONDOWN:
				/* 右ボタンが押された処理 */
				break;
			case WM_LBUTTONDBLCLK:
				showWindow(hwnd);
				Shell_NotifyIcon(NIM_DELETE, &nid);
				break;
			case WM_RBUTTONDBLCLK:
				/* 右ダブルクリックされた処理 */
				break;
			default:
				/* 上記以外の処理 */
				break;
			}
		}
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
		WS_EX_LAYERED | WS_EX_TOOLWINDOW,
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

void setIcon(NOTIFYICONDATA& nid, HICON& hIcon, LPARAM lp, HWND hwnd) {
	hIcon = LoadIcon(((LPCREATESTRUCT)(lp))->hInstance, TEXT("YICO"));
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.uFlags = (NIF_ICON | NIF_MESSAGE | NIF_TIP);
	nid.hWnd = hwnd;             // ウインドウのハンドル
	nid.hIcon = hIcon;            // アイコンのハンドル
	nid.uID = ID_TRAYICON;      // アイコン識別子の定数
	nid.uCallbackMessage = WM_TASKTRAY;      // 通知メッセージの
	lstrcpy(nid.szTip, TEXT("ゆかりさんますこっと"));       // チップヘルプの文字列
}

