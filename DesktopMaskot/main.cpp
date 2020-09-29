#include <Windows.h>
#include "MYBRUSH.h"
#include "loadPng.h"
#include "resource2.h"
#include "MYFONT.h"
#include <gdiplustypes.h>
#include "body.h"
#include "aitalk_wrapper.h"
#pragma comment(lib, "winmm.lib")
#pragma comment (lib,"Gdiplus.lib")
#define ID_TRAYICON     (1)
#define WM_TASKTRAY     (WM_APP + 1)

const int WIDTH = 500, HEIGHT = 620;
yukari y;
void setIcon(NOTIFYICONDATA& nid, HICON& hIcon, LPARAM lp, HWND hwnd);
 HBRUSH hbrEdit = NULL;
 HBRUSH hbrEditBox, hbrListBox;
 HDC hdcMem;


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	PAINTSTRUCT ps;
	RECT rc;
	POINT po;
	static HBITMAP hBitmap = NULL;
	static HDC  hdc;
	static BOOL blRight = TRUE;
	static HMENU hmenuR;
	static HICON hIcon;
	static RECT rec = { 270, 0, WIDTH, HEIGHT };


	static NOTIFYICONDATA nid = { 0 };
	HGDIOBJ hb;
	Gdiplus::Graphics* g;
	static COLORREF crEditBox, crListBox;
	static COMBOBOXINFO cbi = { sizeof(COMBOBOXINFO) };

	switch (msg) {
	case WM_DESTROY:
		DestroyWindow(hwnd);
		PostQuitMessage(0);
		DeleteObject(hFont22);
		DeleteObject(hFont20);
		Shell_NotifyIcon(NIM_DELETE, &nid);
		return 0;
	case WM_CREATE: {
		HMENU tmp = LoadMenu(((LPCREATESTRUCT)(lp))->hInstance, TEXT("yukari"));
		//アイコンロードとタスクトレイの設定
		setIcon(nid, hIcon, lp, hwnd);
		readPlan();
		hmenuR = GetSubMenu(tmp, 0);
		hdc = GetDC(hwnd);
		GetClientRect(GetDesktopWindow(), &rc);  	// デスクトップのサイズを取得
		hBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
		hdcMem = CreateCompatibleDC(NULL);		// カレントスクリーン互換
		SelectObject(hdcMem, hBitmap);		// MDCにビットマップを割り付け
		SetTimer(hwnd, 1, 33, NULL);
		getTranslate(translate);
		return 0;
	}
	case WM_TIMER:
		InvalidateRect(hwnd, &rec, TRUE);
		return 0;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		hdc = BeginPaint(hwnd, &ps);
		g = new Gdiplus::Graphics(hdcMem);
		hb = SelectObject(hdcMem, BRACK_BRUSH);
		Rectangle(hdcMem, -1, -1, WIDTH + 1, HEIGHT + 1);
		DeleteObject(hb);
		SelectObject(hdcMem, hFont22);
		SetTextColor(hdcMem, RGB(255, 255, 255));
		SetBkMode(hdcMem, TRANSPARENT);
		//デバッグ用ラインx
		//SelectObject(hdcMem, GREEN_BRUSH);
		//Rectangle(hdcMem, 0, 335, 500, 340);
		//デバッグ用ラインy
		//SelectObject(hdcMem, GREEN_BRUSH);
		//Rectangle(hdcMem, 95, 0, 100, 700);
		y.main(g, hdcMem, hwnd);
		BitBlt(hdc, 0, 0, WIDTH, HEIGHT, hdcMem, 0, 0, SRCCOPY);
		DeleteObject(hb);
		EndPaint(hwnd, &ps);
		delete(g);
	}
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
			Fukidasi::fuki.push_back(std::make_unique<Menu>());
		}
		else if (wp == 1000) {
			Fukidasi::fuki.front()->time = 1;
			if (plans.empty()) {
				Fukidasi::fuki.push_back(std::make_unique<Fukidasi>("予定はありません！", hdcMem, NORMAL, 80));
			}else
			Fukidasi::fuki.push_back(std::make_unique<YoteiCheck>());
		}
		else if (wp == 1001) {
			Fukidasi::fuki.front()->time = 1;
			Fukidasi::fuki.push_back(std::make_unique<YoteiTuika>());
		}
		else if (wp == 1002) {
			Fukidasi::fuki.front()->time = 1;
			Fukidasi::fuki.push_back(std::make_unique<Sortie>());
		}
		else if (wp == 1003||wp==1005|| wp == 1007) {
			Fukidasi::fuki.front()->time = 1;
		}
		else if(wp==1004){
			Fukidasi::fuki.front()->selectButton(wp, hdcMem);
		}
		else if (wp == 1008) {
			YoteiCheck::page++;
			YoteiCheck::redrawButton();
		}
		else if (wp == 1006) {
			YoteiCheck::page--;
			YoteiCheck::redrawButton();
		}
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
	case MM_WOM_DONE:
		std::cout << "オワリオワリ" << "\n";
	case MM_WOM_OPEN:
		std::cout << "ハジマリハジマリ" << "\n";

	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

void createAiTalk(AITalkWrapper &aitalk);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow) {
	HWND hwnd;
	MSG msg;
	WNDCLASS winc;
	createAiTalk(aitalk);

	/*if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
		AllocConsole();
	}
	FILE* fpOut = NULL;

	freopen_s(&fpOut, "CONOUT$", "w", stdout);*/
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
	Sortie t;
	SetLayeredWindowAttributes(hwnd, 0x01000000, 0, LWA_COLORKEY);
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	if (hwnd == NULL) return 0;
		while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	//FreeConsole();
	AITalkWrapper::closeLibrary();

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

void createAiTalk(AITalkWrapper& aitalk){
	std::string kana;
	AITalkWrapper::openLibrary("C:/Program Files (x86)/AHS/VOICEROID2", "ORXJC6AIWAUKDpDbH2al");
	AITalkWrapper::loadLanguage("standard");
	AITalkWrapper::loadPhraseDictionary("C:/Users/roxas1533/Documents/VOICEROID2/フレーズ辞書/user.pdic");
	AITalkWrapper::loadWordDictionary("C:/Users/roxas1533/Documents/VOICEROID2/単語辞書/user.wdic");
	AITalkWrapper::loadSymbolDictionary("C:/Users/roxas1533/Documents/VOICEROID2/記号ポーズ辞書/user.sdic");
	AITalkWrapper::loadVoice("yukari_emo_44", 1.4f, 1.19f, 2.0f);
}

