#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include "MYFONT.h"
#include <memory>
class CustomButton {
public:
	CustomButton(HWND hwnd, std::string text, int x, int y, int id) :hInst_((HINSTANCE)(LONG64)GetWindowLong(hwnd, GWL_HINSTANCE)), text(text) {
		size = { x,y,270 + x,y + 9999 };
		SIZE temp;
		HDC hdc = GetDC(hwnd);
		HFONT hOldFont = (HFONT)SelectObject(hdc, hFont20);
		GetTextExtentPoint32(hdc, TEXT(text.c_str()), lstrlen(TEXT(text.c_str())), &temp);
		ReleaseDC(hwnd, hdc);
		int h = DrawText(GetDC(hwnd), TEXT(text.c_str()), -1, &size, DT_WORDBREAK | DT_CALCRECT);
		bHwnd = CreateWindow(
			TEXT("BUTTON"), NULL,
			WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
			x, y, temp.cx, h,
			hwnd, (HMENU)(LONG64)id, hInst_, NULL
		);
		DefaultButtonProc = (WNDPROC)GetWindowLong(bHwnd, GWL_WNDPROC);
		SetWindowLongPtr(bHwnd, GWL_WNDPROC, (LONG_PTR)(CustomButtonProc));
		size = { x,y,temp.cx + x,y + h + 10 };
		drawSize = { 0,0,temp.cx,h + 10 };
	}
	static LRESULT CALLBACK CustomButtonProc(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam) {
		std::shared_ptr<CustomButton> p = CustomButton::getObjPtr(hWnd);

		// プロシージャ呼び出し
		if (p) {
			LRESULT res = p->localButtonProc(hWnd, mes, wParam, lParam);
			// もしWM_DESTROYを捕まえたらボタンを消去する
			if (mes == WM_DESTROY) {
				std::map< HWND, std::shared_ptr<CustomButton> >::iterator it = CustomButton::cmpHash_.find(hWnd);
				if (it != CustomButton::cmpHash_.end()) {
					CustomButton::cmpHash_.erase(it);
				}
			}
			return res;
		}
		return 0L;
	}
	static std::shared_ptr<CustomButton> getObjPtr(HWND hWnd) {
		std::map< HWND, std::shared_ptr<CustomButton> >::iterator it = CustomButton::cmpHash_.find(hWnd);
		if (it != CustomButton::cmpHash_.end())
			return it->second;
		return NULL;
	}

	static void create(HWND hParentWnd, CustomButton** out, std::string text, int x, int y, int id) {
		*out = new CustomButton(hParentWnd, text, x, y, id);
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
			if (destroyFlag) {
				DestroyWindow(bHwnd);
			}
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hWnd, &ps);
			//HDC hdc_mem = CreateCompatibleDC(hDC);
			//デバッグ用背景
			SelectObject(hDC, hFont20);
			SelectObject(hDC, GetStockObject(GRAY_BRUSH));
			Rectangle(hDC, 0, 0, drawSize.right, drawSize.bottom);

			SetBkMode(hDC, TRANSPARENT);
			SetTextColor(hDC, RGB(0, 0, 255));
			DrawText(hDC, TEXT(text.c_str()), -1, &drawSize, DT_WORDBREAK);

			//BitBlt(hDC, 0, 0, size.right, size.bottom, hdc_mem, 0, 0, SRCAND);
			//DeleteDC(hdc_mem);
			EndPaint(hWnd, &ps);
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
	static std::map<HWND, std::shared_ptr<CustomButton>> cmpHash_;
	const HINSTANCE hInst_;
	std::string text;
	RECT size;
	RECT drawSize;
	static inline  bool destroyFlag = false;
};
std::map<HWND, std::shared_ptr<CustomButton>> CustomButton::cmpHash_;
