#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <gdiplustypes.h>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include "aitalk_wrapper.h"
#include <string>
#include "MYBRUSH.h"
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

#ifndef MYFUNC
#define MYFUNC

#define CANCEL 1003
#define ADDPLAN 1001
#define CHECKPLAN 1000
#define KADAI 1004
#define TODAYSORTIE 1002
#define WIDTH 500
#define HEIGHT 620

enum STOP_NOTIFICATION {
	NONE,
	tenMIn,
	thrMin,
	HOUR,
	NEVER
};
extern STOP_NOTIFICATION sn;
extern WAVEHDR hdr;
extern HMENU hmenuR;
extern MENUITEMINFO menuInfo;
namespace myfunc {
	inline void resetTimer() {
		menuInfo.cbSize = sizeof(MENUITEMINFO);
		menuInfo.fMask = MIIM_STATE;
		for (int i = 40002; i <= 40005; i++) {
			GetMenuItemInfo(hmenuR, i, FALSE, &menuInfo);
			menuInfo.fState = MFS_UNCHECKED;
			sn = NONE;
			SetMenuItemInfo(hmenuR, i, FALSE, &menuInfo);
		}
	}
		inline void wakeErr(std::string errtext) {
			MessageBoxA(NULL, TEXT(errtext.c_str()), "起動エラー", MB_OK | MB_ICONSTOP);
			exit(1);
		}
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

		inline SYSTEMTIME timeCheck() {
			SYSTEMTIME sys;
			GetLocalTime(&sys);
			return sys;
		}

		inline int getIntFromHandle(HWND h) {
			char buf[1000];
			GetWindowText(h, buf, 100);
			try {
				int ret = std::stoi(std::string(buf));
				return ret;
			}
			catch (std::invalid_argument) {
				throw;
			}
		}

		template<typename Iterator>
		class withIndexItrator :public Iterator {
			std::size_t i = 0;
		public:
			withIndexItrator(Iterator iter) : Iterator(iter) { }
			auto& operator ++()
			{
				++i;
				this->Iterator::operator ++();
				return *this;
			}
			auto operator *() const noexcept
			{
				return std::pair<
					std::size_t,
					typename std::iterator_traits<Iterator>::reference >
				{ i, * static_cast<Iterator const&>(*this) };
			}
		};

		template < typename Range >
		class with_index {
			Range& range;
		public:
			with_index(Range& range)
				: range(range)
			{ }

			auto begin() const
			{
				return withIndexItrator{ std::begin(range) };
			}
			auto end() const
			{
				return withIndexItrator{ std::end(range) };
			}
		};

		inline void getTranslate(std::map< std::string, std::string> & t) {
			std::ifstream ifs("trans.txt", std::ios::in);
			if (!ifs)
				wakeErr("翻訳ファイルがありません");
			std::string stmp;
			std::string str;
			while (getline(ifs, stmp)) {
				std::stringstream stringStream{ stmp };
				std::string s;
				std::string temp[2];
				for (int i = 0; getline(stringStream, s, ','); i++) {
					temp[i] = s;
				}
				t.insert(std::make_pair(temp[0], temp[1]));
			}
		}
	}
#endif // !MYFUNC
