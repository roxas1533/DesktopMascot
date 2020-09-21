#pragma once
#include <Windows.h>
#include <string>
#include <map>
class CustomButton {
public:
	CustomButton(HWND hwnd, std::string text, int x, int y,int id) :hInst_((HINSTANCE)(LONG64)GetWindowLong(hwnd, GWL_HINSTANCE)), text(text) {
		size = { 95 + x,y,270 + x,y + 9999 };

		int h = DrawText(GetDC(hwnd), TEXT(text.c_str()), -1, &size, DT_WORDBREAK | DT_CALCRECT);

		bHwnd = CreateWindow(
			TEXT("BUTTON"), NULL,
			WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
			95 + x, y, 270, h,
			hwnd, (HMENU)(LONG64)id, hInst_, NULL
		);
		DefaultButtonProc = (WNDPROC)GetWindowLong(bHwnd, GWL_WNDPROC);
		SetWindowLongPtr(bHwnd, GWL_WNDPROC, (LONG_PTR)(CustomButtonProc));
		size = { 0,0,175 + x,h + 10 };

	}
	DWORD getUniqueID() {
		static DWORD id = 1000;
		return id++;
	}
	static LRESULT CALLBACK CustomButtonProc(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam) {
		CustomButton* p = CustomButton::getObjPtr(hWnd);

		// プロシージャ呼び出し
		if (p) {
			LRESULT res = p->localButtonProc(hWnd, mes, wParam, lParam);
			// もしWM_DESTROYを捕まえたらボタンを消去する
			//if (mes == WM_DESTROY) {
			//	std::cout << "keisya";
			//	std::map< HWND, CustomButton* >::iterator it = CustomButton::cmpHash_.find(hWnd);
			//	if (it != CustomButton::cmpHash_.end()) {
			//		DestroyWindow(it->second->bHwnd);
			//		delete it->second;
			//		CustomButton::cmpHash_.erase(it);
			//	}
			//}
			return res;
		}
		return 0L;
	}
	static CustomButton* getObjPtr(HWND hWnd) {
		std::map< HWND, CustomButton* >::iterator it = CustomButton::cmpHash_.find(hWnd);
		if (it != CustomButton::cmpHash_.end())
			return it->second;
		return NULL;
	}

	static void create(HWND hParentWnd, CustomButton** out, std::string text, int x, int y,int id) {
		*out = new CustomButton(hParentWnd, text, x, y,id);
		CustomButton::regist(*out);
	}

	static void regist(CustomButton* pObj) {
		HWND hWnd = pObj->bHwnd;
		CustomButton::cmpHash_.insert(std::pair< HWND, CustomButton* >(hWnd, pObj));
	}

	virtual LRESULT localButtonProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		HFONT hFont;
		HFONT hFont2;

		switch (msg)
		{
		case WM_ERASEBKGND:
			return FALSE;
		case WM_PAINT: {
			if (destroyFlag)
				DestroyWindow(bHwnd);
			hFont = CreateFont(
				20, 0, 0, 0, FW_BOLD, FALSE, TRUE, FALSE,
				SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
				VARIABLE_PITCH | FF_ROMAN, TEXT("たいぷ")
			);
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hWnd, &ps);
			HDC hdc_mem = CreateCompatibleDC(hDC);
			SelectObject(hDC, hFont);
			//デバッグ用背景

		/*	SelectObject(hDC, GetStockObject(GRAY_BRUSH));
			Rectangle(hDC, size.left, size.top, size.right, size.bottom);*/


			SetBkMode(hDC, TRANSPARENT);
			SetTextColor(hDC, RGB(0, 0, 255));
			DrawText(hDC, TEXT(text.c_str()), -1, &size, DT_WORDBREAK);

			BitBlt(hDC, 0, 0, size.right, size.bottom, hdc_mem, 0, 0, SRCAND);
			DeleteDC(hdc_mem);
			EndPaint(hWnd, &ps);
			DeleteObject(hFont);
		}
		}

		return CallWindowProc(DefaultButtonProc, hWnd, msg, wParam, lParam);
	};
	void requestRedraw() {
		// 再描画要求
		RECT R;
		SetRect(&R, 0, 0, size.right, size.bottom);
		InvalidateRect(bHwnd, &R, false);
	}

	void eraseMe() {

	}

	HWND bHwnd;
	WNDPROC  DefaultButtonProc;
	static std::map<HWND, CustomButton*> cmpHash_;
	const HINSTANCE hInst_;
	std::string text;
	RECT size;
	static inline  bool destroyFlag=false;
};
std::map<HWND, CustomButton*> CustomButton::cmpHash_;
