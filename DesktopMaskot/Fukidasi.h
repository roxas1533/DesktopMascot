#pragma once
#include <gdiplus.h>
#include "loadPng.h"
#include "rand.h"
#include "Plan.h"
#include <thread>
#include "myfunc.h"
#include "CustomButton.h"
class yukari;
static HWND hwnd;

using namespace Gdiplus;
std::string nam = "C:\\Users\\roxas1533\\Downloads\\yukari\\yukari\\5313.png";
Bitmap* tex = getFileIStream(nam.c_str());
RECT rec2 = { 0, 0, 270, 650 };

const int NO[] = { 2,8,10 };
const int BO[] = { 3,4,12 };
const int buttonNum = 4;
const std::map<int, std::string> list[] = {
	{{1000,"・予定確認"},{1001,"・予定追加"},{1002,"・今日のソーティ"},{1003,"キャンセル"}},
	{{1004,"追加!"},{1005,"キャンセル"}}
};
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
	Fukidasi() {}
	virtual ~Fukidasi() {}
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

	virtual bool drawFuki(Graphics* g, HDC hdcMem) {
		if (count == 0) {
			constructor();
			showWindow(hwnd);
			InvalidateRect(hwnd, &rec2, TRUE);
		}
		if (CustomButton::cmpHash_.size() == 0) {
			addButton();
		}

		drawBack(g);
		DrawText(hdcMem, TEXT(text.c_str()), -1, &size, DT_WORDBREAK);

		time--;
		count++;
		return time == 0 ? true : false;
	}

	virtual void drawBack(Graphics* g) {
		g->DrawImage(tex, RectF(80, 150, 200, 52), 0, 0, 549, 144, UnitPixel);
		for (int j = 0; j < i; j++)
			g->DrawImage(tex, RectF(80, 202 + 52 * j, 200, 52), 0, 144, 549, 144, UnitPixel);
		g->DrawImage(tex, RectF(80, 202 + i * 52, 200, 52), 0, 288, 549, 82, UnitPixel);
	}

	virtual void constructor() {}
	virtual void addButton() {}
	virtual void selectButton(int id, HDC hdcMem) {}

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
	static std::list<std::unique_ptr<Fukidasi>> fuki;
};
std::list<std::unique_ptr<Fukidasi>> Fukidasi::fuki;

class Menu :public Fukidasi {
public:
	Menu() {
		time = -1;
		body = randRange(0, 10);
		emotion = NORMAL;
		dec = randRange(-9, 2);
		if (dec >= 0)
			dec = NO[dec];
		std::thread th1(Yomiage, "どうしましたか？");
		th1.detach();
	}

	virtual void constructor() override {
		if (fuki.front()->time > 1)
			fuki.front()->time = 1;
		addButton();

	}
	virtual void addButton() {
		CustomButton::destroyFlag = false;
		CustomButton* b[buttonNum];
		for (int i = 0; i < buttonNum; i++) {
			CustomButton::create(hwnd, &b[i], list[0].at(1000 + i), 95, 202 + i * 35, 1000 + i);
		}
		size = { 95, 202, 270, b[buttonNum - 1]->size.bottom };
		i = (b[buttonNum - 1]->size.bottom- 150 - 30) / 52;
	}
};

class YoteiTuika :public Fukidasi {
	HWND MO, DAY, H, Min, edit;
	static inline HFONT hFont = CreateFont(
		20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		VARIABLE_PITCH | FF_ROMAN, TEXT("たいぷ")
	);
	HINSTANCE h;
public:
	YoteiTuika() :h((HINSTANCE)(LONG64)GetWindowLong(hwnd, GWL_HINSTANCE)), MO(setSelectList(h, 0, 110, 230)),
		DAY(setSelectList(h, 1, 200, 230)), H(setSelectList(h, 3, 110, 275)), Min(setSelectList(h, 2, 200, 275)),
		edit(CreateWindowEx(0, TEXT("EDIT"), TEXT(""), WS_CHILD | WS_VISIBLE, 15, 350, 270, 22, hwnd, (HMENU)12222, h, NULL)) {
		time = -1;

		size = { 15, 190, 285,999 };
		i = (277) / 52;
		body = randRange(0, 10);
		emotion = NORMAL;
		dec = randRange(-9, 2);
		SendMessage(edit, WM_SETFONT, (WPARAM)hFont22, MAKELPARAM(FALSE, 0));
		if (dec >= 0)
			dec = NO[dec];
		std::thread th1(Yomiage, "予定ですね");
		th1.detach();
	}
	~YoteiTuika() override {
		DestroyWindow(MO);
		DestroyWindow(DAY);
		DestroyWindow(H);
		DestroyWindow(Min);
		DestroyWindow(edit);
	}
	virtual bool drawFuki(Graphics* g, HDC hdcMem) override {
		if (count == 0) {
			showWindow(hwnd);
			InvalidateRect(hwnd, &rec2, TRUE);
		}
		if (CustomButton::cmpHash_.size() == 0) {
			addButton();
		}

		drawBack(g);
		DrawText(hdcMem, TEXT("予定追加"), -1, &size, DT_WORDBREAK | DT_CENTER);
		DrawText(hdcMem, TEXT("\n\n             月     日\n時刻設定\n             時     分"), -1, &size, DT_WORDBREAK);
		DrawText(hdcMem, TEXT("\n\n\n\n\n\n予定内容"), -1, &size, DT_WORDBREAK | DT_CENTER);
		time--;
		count++;
		return time == 0 ? true : false;
	}
	virtual void drawBack(Graphics* g) override {
		g->DrawImage(tex, RectF(0, 150, 300, 52), 0, 0, 549, 144, UnitPixel);
		for (int j = 0; j < i; j++)
			g->DrawImage(tex, RectF(0, 202 + 52 * j, 300, 52), 0, 144, 549, 144, UnitPixel);
		g->DrawImage(tex, RectF(0, 202 + i * 52, 300, 52), 0, 288, 549, 82, UnitPixel);
	}

	void addButton() override {
		CustomButton::destroyFlag = false;
		CustomButton* b[buttonNum];
		for (int i = 0; i < list[1].size(); i++) {
			CustomButton::create(hwnd, &b[i], list[1].at(1004 + i), 40 + i * 100, 450, 1004 + i);
		}
	}

	void selectButton(int id, HDC hdcMem) override {
		char buf[1000];
		GetWindowText(edit, buf, 1000);
		std::string s = buf;
		int hour, month, day, min;
		try {
			hour = getIntFromHandle(H);
			min = getIntFromHandle(Min);
			month = getIntFromHandle(MO);
			day = getIntFromHandle(DAY);
		}
		catch (std::invalid_argument) {
			std::thread th1(Yomiage, "空欄があります！");
			th1.detach();
			return;
		}
		std::cout << month << "月" << day << "日" << hour << "時" << min << "分" << "\n";
		Fukidasi::fuki.front()->time = 1;
		std::string tex = std::to_string(month) + "月" + std::to_string(day) + "日の予定を追加しました！";
		Fukidasi::fuki.push_back(std::make_unique<Fukidasi>(tex, hdcMem, NORMAL, 80));
		plans.push_back(Plan{ month,day,hour,min,s });
		savePlan(hdcMem);
	}

	static HWND setSelectList(HINSTANCE l, int type, int x, int y) {
		HWND combo = CreateWindow(
			TEXT("COMBOBOX"), NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
			x, y, 50, 100, hwnd, (HMENU)TUIKA,
			l, NULL
		);
		switch (type)
		{
		case 0:
			for (int i = 1; i <= 12; i++)
				SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)TEXT(std::to_string(i).c_str()));
			break;
		case 1:
			for (int i = 1; i <= 31; i++)
				SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)TEXT(std::to_string(i).c_str()));
			break;
		case 2:
			for (int i = 0; i <= 50; i += 5)
				SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)TEXT(std::to_string(i).c_str()));
			break;
		case 3:
			for (int i = 0; i <= 23; i++)
				SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)TEXT(std::to_string(i).c_str()));
			break;
		default:
			break;
		}

		SendMessage(combo, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
		return combo;
	}

	void savePlan(HDC hdcMem) {
		if (!writePlan()) {
			fuki.push_back(std::make_unique<Fukidasi>("予定追加エラーです", hdcMem, NORMAL, 80));
		}
	};
};

class YoteiCheck :public Fukidasi {
public:
	YoteiCheck() {
		i = (277) / 52;
		body = randRange(0, 10);
		emotion = NORMAL;
		dec = randRange(-9, 2);
		size = { 15, 190, 285,999 };

		for (int i = 0; i < 4; i++) {
			s.push_back(RECT{ 15, 190 + i * 60, 285,190 + i * 60 + 50 });
		}
	}
	~YoteiCheck() {
		page = 0;
	}
	virtual bool drawFuki(Graphics* g, HDC hdcMem) override {
		if (count == 0) {
			showWindow(hwnd);
			InvalidateRect(hwnd, &rec2, TRUE);
		}
		if (CustomButton::cmpHash_.size() == 0) {
			addButton();
		}

		drawBack(g);
		SelectObject(hdcMem, GREEN_BRUSH);

		for (auto [i, p] : with_index(plans)) {
			if (i < (page + 1) * 4&& i >= page  * 4) {
				Rectangle(hdcMem, 25, s[i- page * 4].bottom, 275, s[i- page * 4].bottom + 4);
				std::string tex = std::to_string(p.month) + "月" + std::to_string(p.day) + "日" + std::to_string(p.hour) + "時" + std::to_string(p.min) + "分";
				DrawText(hdcMem, TEXT(std::string(tex + "\n" + p.text).c_str()), -1, &s[i- page * 4], DT_WORDBREAK);
			}
		}
		time--;
		count++;
		return time == 0 ? true : false;
	}
	virtual void drawBack(Graphics* g) override {
		g->DrawImage(tex, RectF(0, 150, 300, 52), 0, 0, 549, 144, UnitPixel);
		for (int j = 0; j < i; j++)
			g->DrawImage(tex, RectF(0, 202 + 52 * j, 300, 52), 0, 144, 549, 144, UnitPixel);
		g->DrawImage(tex, RectF(0, 202 + i * 52, 300, 52), 0, 288, 549, 82, UnitPixel);
	}
	void addButton() override {
		CustomButton::destroyFlag = false;
		std::cout << "予定数:" << plans.size()<<"\n";
		std::cout << "ページ数:" << page<<"\n";
		std::cout << "arere" << plans.size() - page * 4 <<"\n";


		if (plans.size() - page * 4 > 4) {
			CustomButton* next;
			CustomButton::create(hwnd, &next, "次へ", 210, 450, 1008);
		}
		if (page > 0) {
			CustomButton* back;
			CustomButton::create(hwnd, &back, "戻る", 60, 450, 1006);
		}
		CustomButton* ok;
		CustomButton::create(hwnd, &ok, "確認", 140, 450, 1007);
		
	}

	static void redrawButton() {
		for(auto b:CustomButton::cmpHash_)
			ShowWindow(b.second->bHwnd, SW_HIDE);
		InvalidateRect(hwnd, &rec2, TRUE);
		Fukidasi::fuki.front()->addButton();
	}
	inline static int page = 0;
private:
	std::vector<RECT> s;
};