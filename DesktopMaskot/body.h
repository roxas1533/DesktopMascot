#pragma once
#include <gdiplus.h>
#include "loadPng.h"
#include "Fukidasi.h"
#include <list>
using namespace Gdiplus;

class yukari {
public:
	yukari() {
		getTex(tex);

	}
	void main(Graphics* g, HDC hdcMem) {
		//std::cout << GetLastError();

		time++;
		if (time == 100) {
			fuki.push_back(Fukidasi("カタカナ語が苦手な方は「組見本」と呼ぶとよいでしょう。ダミーテキストはダミー文書やダミー文章とも呼ばれることがあります。カタカナ語が苦手な方は「組見本」と呼ぶとよいでしょう。なお、組見本の「組」とは文字組のことです。活字印刷時代の用語だったと思います。このダミーテキストは自由に改変することが出来ます。ダミーテキストはダミー文書やダミー文章とも呼ばれることがあります。",hdcMem,50));

		}
		normal(g);
		std::list<Fukidasi>::iterator it = fuki.begin();
		while (it != fuki.end()) {
			if (it->drawFuki(g, hdcMem)) {
				it = fuki.erase(it);
			}
			else {
				break;
			}
		}
	}
	void normal(Graphics* g) {
		g->DrawImage(tex[BODY][0][0], 220, 0);
		g->DrawImage(tex[EYE][0][0], 220, 0);
		g->DrawImage(tex[KUTI][0][0], 220, 0);
		g->DrawImage(tex[MAYU][0][0], 220, 0);
	}


private:
	std::vector<std::vector<std::vector<Bitmap*>>> tex;
	std::list<Fukidasi> fuki;
	int time=0;
};