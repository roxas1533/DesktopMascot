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
		//�A�C�R�����[�h�ƃ^�X�N�g���C�̐ݒ�
		setIcon(nid, hIcon, lp, hwnd);
		readPlan();
		hmenuR = GetSubMenu(tmp, 0);
		hdc = GetDC(hwnd);
		GetClientRect(GetDesktopWindow(), &rc);  	// �f�X�N�g�b�v�̃T�C�Y���擾
		hBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
		hdcMem = CreateCompatibleDC(NULL);		// �J�����g�X�N���[���݊�
		SelectObject(hdcMem, hBitmap);		// MDC�Ƀr�b�g�}�b�v������t��
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
		//�f�o�b�O�p���C��x
		//SelectObject(hdcMem, GREEN_BRUSH);
		//Rectangle(hdcMem, 0, 335, 500, 340);
		//�f�o�b�O�p���C��y
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
				Fukidasi::fuki.push_back(std::make_unique<Fukidasi>("�\��͂���܂���I", hdcMem, NORMAL, 80));
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
		if (wp == ID_TRAYICON) {       // �A�C�R���̎��ʃR�[�h
			switch (lp) {
			case WM_MOUSEMOVE:
				/* �}�E�X�ړ��̏��� */
				break;
			case WM_LBUTTONDOWN:
				/* ���{�^���������ꂽ���� */
				break;
			case WM_RBUTTONDOWN:
				/* �E�{�^���������ꂽ���� */
				break;
			case WM_LBUTTONDBLCLK:
				showWindow(hwnd);
				Shell_NotifyIcon(NIM_DELETE, &nid);
				break;
			case WM_RBUTTONDBLCLK:
				/* �E�_�u���N���b�N���ꂽ���� */
				break;
			default:
				/* ��L�ȊO�̏��� */
				break;
			}
		}
	case MM_WOM_DONE:
		std::cout << "�I�����I����" << "\n";
	case MM_WOM_OPEN:
		std::cout << "�n�W�}���n�W�}��" << "\n";

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
		TEXT("yukari"), TEXT("�䂩�肳��܂�������"),
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
	nid.hWnd = hwnd;             // �E�C���h�E�̃n���h��
	nid.hIcon = hIcon;            // �A�C�R���̃n���h��
	nid.uID = ID_TRAYICON;      // �A�C�R�����ʎq�̒萔
	nid.uCallbackMessage = WM_TASKTRAY;      // �ʒm���b�Z�[�W��
	lstrcpy(nid.szTip, TEXT("�䂩�肳��܂�������"));       // �`�b�v�w���v�̕�����
}

void createAiTalk(AITalkWrapper& aitalk){
	std::string kana;
	AITalkWrapper::openLibrary("C:/Program Files (x86)/AHS/VOICEROID2", "ORXJC6AIWAUKDpDbH2al");
	AITalkWrapper::loadLanguage("standard");
	AITalkWrapper::loadPhraseDictionary("C:/Users/roxas1533/Documents/VOICEROID2/�t���[�Y����/user.pdic");
	AITalkWrapper::loadWordDictionary("C:/Users/roxas1533/Documents/VOICEROID2/�P�ꎫ��/user.wdic");
	AITalkWrapper::loadSymbolDictionary("C:/Users/roxas1533/Documents/VOICEROID2/�L���|�[�Y����/user.sdic");
	AITalkWrapper::loadVoice("yukari_emo_44", 1.4f, 1.19f, 2.0f);
}

