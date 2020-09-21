#pragma once
#include <gdiplus.h>
#include "loadPng.h"
#include "rand.h"
#include <thread>
#include "myfunc.h"
#include "CustomButton.h"

static HWND hwnd;

using namespace Gdiplus;
std::string nam = "C:\\Users\\roxas1533\\Downloads\\yukari\\yukari\\5313.png";
Bitmap* tex = getFileIStream(nam.c_str());

const int NO[] = { 2,8,10 };
const int BO[] = { 3,4,12 };

const int buttonNum = 4;
const std::map<int,std::string > list{ {1000,"・予定確認"},{1001,"・予定追加"},{1002,"・今日のソーティ"},{1003,"キャンセル"} };
enum EMO {
	NORMAL = 0,
	HAPPY,
	ANG,
	BOTH,
};
bool flag = false;
void Yomiage(std::string te) {
	std::string vpath = "C:\\Users\\roxas1533\\source\\repos\\roxas1533\\DesktopMascot\\Debug\\Vsay.exe ";
	vpath += te + " -p 1.4 -v 2.0 -s 1.19";
	char* cstr = new char[vpath.size() + 1]; // メモリ確保

	strcpy_s(cstr, vpath.size() + 1, vpath.c_str());        // コピー
	PROCESS_INFORMATION p;
	STARTUPINFO s;
	ZeroMemory(&s, sizeof(s));
	s.cb = sizeof(s);
	while (1) {
		if (!flag)
		{
			//TerminateProcess(p.hProcess, 0);
			break;
		}
	}
	flag = true;
	//std::cout<<"待ち"<<WaitForSingleObject(process.hProcess, INFINITE)<<"\n";
	CreateProcess(
		NULL, // 実行可能モジュールの名
		cstr, // コマンドラインの文字列
		NULL, // セキュリティ記述子
		NULL,// セキュリティ記述子
		FALSE, // ハンドルの継承オプション
		NULL, // 作成のフラグ
		NULL,// 新しい環境ブロック
		NULL, // カレントディレクトリの名前
		&s, // スタートアップ情報
		&p // プロセス情報
	);
	delete[] cstr; // メモリ解放
	::WaitForSingleObject(p.hProcess, INFINITE);
	flag = false;

}


class Fukidasi {
public:
	Fukidasi(std::string te, HDC hdcMem, EMO emo, int time = -1) :text(te), time(time), emotion(emo) {
		size = { 90, 180, 290, 9999 };
		int h = DrawText(hdcMem, TEXT(text.c_str()), -1, &size, DT_WORDBREAK | DT_CALCRECT);
		i = (h - 30) / 52 >= 2 ? (h - 30) / 52 : 1;
		size = { 95, 202, 270, 202 + h };
		body = randRange(0, 10);
		dec = randRange(-9, 2);
			switch (emo)
			{
			case NORMAL:
				if (dec >= 0)
				dec = NO[dec];
				break;
			case BOTH:
				if (dec >= 0)
				dec = BO[dec];
				eye = 27;
				break;
			default:
				break;
			}
			
			std::thread th1(Yomiage, te);
			th1.detach();
	}

	Fukidasi() {
		time = -1;
		CustomButton* b[buttonNum];
		for (int i = 0; i < buttonNum; i++) {
			CustomButton::create(hwnd, &b[i], list.at(1000+i), 0, 202 + i * 35,1000+i);
		}
		size = { 95, 202, 270, b[buttonNum - 1]->size.bottom };
		i = (b[buttonNum - 1]->size.bottom+202 - 95 - 30) / 52;
		body = randRange(0, 10);
		emotion = NORMAL;
		dec = randRange(-9, 2);
		if (dec >= 0)
			dec = NO[dec];
		std::thread th1(Yomiage, "どうしましたか？");
		th1.detach();
	}


	bool drawFuki(Graphics* g, HDC hdcMem) {
		if (count == 0)
			showWindow(hwnd);

		g->DrawImage(tex, RectF(80, 150, 200, 52), 0, 0, 549, 144, UnitPixel);
		for (int j = 0; j < i; j++)
			g->DrawImage(tex, RectF(80, 202 + 52 * j, 200, 52), 0, 144, 549, 144, UnitPixel);
		g->DrawImage(tex, RectF(80, 202 + i * 52, 200, 52), 0, 288, 549, 82, UnitPixel);
		DrawText(hdcMem, TEXT(text.c_str()), -1, &size, DT_WORDBREAK);

		time--;
		count++;
		return time == 0 ? true : false;
	}

	long long nowTalking = 0;
	RECT size;
	std::string text;
	int i;
	int time;
	EMO emotion;
	int dec;
	int eye;
	int body;
	int count = 0;
};
