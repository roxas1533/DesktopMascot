#pragma once
#include <gdiplus.h>
#include "loadPng.h"

using namespace Gdiplus;
std::string nam = "C:\\Users\\roxas1533\\Downloads\\yukari\\yukari\\5313.png";
Bitmap* tex = getFileIStream(nam.c_str());
class Fukidasi {
public:
	Fukidasi(std::string te, HDC hdcMem, int time = -1) :text(te),time(time) {
		size = { 90, 180, 290, 9999 };
		int h = DrawText(hdcMem,TEXT(text.c_str()), -1, &size, DT_WORDBREAK);
		i = (h - 30) / 52 >= 2 ? (h - 30) / 52 : 1;
		std::cout << i;
		size = { 90, 180, 290, 180+h };

	}
	bool drawFuki(Graphics* g, HDC hdcMem) {

		g->DrawImage(tex, RectF(80, 150, 200, 52), 0, 0, 549, 144, UnitPixel);
		for (int j = 0; j < i; j++)
			g->DrawImage(tex, RectF(80, 202 + 52 * j, 200, 52), 0, 144, 549, 144, UnitPixel);
		g->DrawImage(tex, RectF(80, 202 + i * 52, 200, 52), 0, 288, 549, 82, UnitPixel);
		DrawText(hdcMem, TEXT(text.c_str()), -1, &size, DT_WORDBREAK);

		time--;
		return time == 0 ? true : false;
	}

	long long nowTalking = 0;
	RECT size;
	std::string text;
	int i;
	int time;
};
