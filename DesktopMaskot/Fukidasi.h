#pragma once
#include <gdiplus.h>
#include "loadPng.h"
#include "rand.h"
#include "Plan.h"
#include "myfunc.h"
#include "CustomButton.h"
#include <regex>
#include "httpGet.h"
#include <srell2_800/srell.hpp>
using namespace Gdiplus;
class yukari;
static HWND hwnd;
std::map< std::string, std::string> translate;
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
AITalkWrapper aitalk;
void Yomiage(std::string te) {
	std::string kana;
	aitalk.textToKana(te, &kana);
	aitalk.kanaToSpeech(kana, &speech);
	if (speech.empty() == false) {
		WAVEFORMATEX format;
		format.wFormatTag = WAVE_FORMAT_PCM;
		format.nChannels = 1;
		format.nSamplesPerSec = 44100;
		format.nAvgBytesPerSec = 44100 * 2;
		format.nBlockAlign = 2;
		format.wBitsPerSample = 16;
		format.cbSize = 52428;
		waveOutOpen(&hwo, WAVE_MAPPER, &format, (DWORD_PTR)hwnd, 0, CALLBACK_WINDOW);

		hdr.lpData = (LPSTR)speech.data();
		hdr.dwBufferLength = speech.size() * 2;
		hdr.dwFlags = 0;
		waveOutPrepareHeader(hwo, &hdr, sizeof(WAVEHDR));
		waveOutWrite(hwo, &hdr, sizeof(WAVEHDR));
	}

}

class Fukidasi {
public:
	Fukidasi() {
		myfunc::showWindow(hwnd);
		Shell_NotifyIcon(NIM_DELETE, &nid);
	}
	virtual ~Fukidasi() {}
	Fukidasi(std::string te, HDC hdcMem, EMO emo, int time = -1) :text(te), time(time), emotion(emo) {
		myfunc::showWindow(hwnd);
		Shell_NotifyIcon(NIM_DELETE, &nid);
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

		Yomiage(te);
	}

	virtual bool drawFuki(Graphics* g, HDC hdcMem) {
		if (count == 0) {
			constructor();
			myfunc::showWindow(hwnd);
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
		Yomiage("どうしましたか？");
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
		i = (b[buttonNum - 1]->size.bottom - 150 - 30) / 52;
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
		Yomiage( "予定ですね");
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
			hour = myfunc::getIntFromHandle(H);
			min = myfunc::getIntFromHandle(Min);
			month = myfunc::getIntFromHandle(MO);
			day = myfunc::getIntFromHandle(DAY);
		}
		catch (std::invalid_argument) {
			Yomiage("空欄があります！");
			return;
		}
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
			InvalidateRect(hwnd, &rec2, TRUE);
		}
		if (CustomButton::cmpHash_.size() == 0) {
			addButton();
		}

		drawBack(g);
		SelectObject(hdcMem, GREEN_BRUSH);

		for (auto [i, p] : myfunc::with_index(plans)) {
			if (i < (page + 1) * 4 && i >= page * 4) {
				Rectangle(hdcMem, 25, s[i - page * 4].bottom, 275, s[i - page * 4].bottom + 4);
				std::string tex = std::to_string(p.month) + "月" + std::to_string(p.day) + "日" + std::to_string(p.hour) + "時" + std::to_string(p.min) + "分";
				DrawText(hdcMem, TEXT(std::string(tex + "\n" + p.text).c_str()), -1, &s[i - page * 4], DT_WORDBREAK);
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
		for (auto b : CustomButton::cmpHash_)
			ShowWindow(b.second->bHwnd, SW_HIDE);
		InvalidateRect(hwnd, &rec2, TRUE);
		Fukidasi::fuki.front()->addButton();
	}
	inline static int page = 0;
private:
	std::vector<RECT> s;
};

class Sortie :public Fukidasi {
public:
	Sortie() {
		i = (277) / 52;
		body = randRange(0, 10);
		emotion = NORMAL;
		dec = randRange(-9, 2);
		json data = getHttp();
		for (auto [i,sortie] : myfunc::with_index(data["variants"])) {
			srell::smatch smatch;
			srell::regex re(R"((?<=\().*(?=\)))");
			srell::regex dub(R"((?<=").*(?="))");

			std::string pl = sortie["node"].dump();
			if (srell::regex_search(pl, smatch, re))
				planet[i] =  translate[smatch[0]];
			pl = sortie["missionType"].dump();
			if (srell::regex_search(pl, smatch, dub))
				mission[i] = translate[smatch[0]];
			pl = sortie["modifier"].dump();
			if (srell::regex_search(pl, smatch, dub))
				modifier[i] = translate[smatch[0]];
		}
		//for (int i = 0; i < 3; i++) {
		//	std::cout << planet[i] << "," << mission[i] << "," << modifier[i] << "\n";
		//}
	}
	void addButton() override {
		CustomButton::destroyFlag = false;
			CustomButton* ok;
			CustomButton::create(hwnd, &ok, "確認", 130, 450, 1007);
	}
	virtual bool drawFuki(Graphics* g, HDC hdcMem) override {
		if (count == 0) {
			myfunc::showWindow(hwnd);
			InvalidateRect(hwnd, &rec2, TRUE);
		}
		if (CustomButton::cmpHash_.size() == 0) {
			addButton();
		}

		drawBack(g);
		for (int i = 0; i < 3; i++) {
			size = { 15, 190 + i * 80, 285,999 };
			std::string te = std::to_string(i+1)+". "+planet[i] + " : " + mission[i];
			std::string te2 ="\n"+modifier[i];
			DrawText(hdcMem, TEXT(te.c_str()), -1, &size, DT_WORDBREAK);
			DrawText(hdcMem, TEXT(te2.c_str()), -1, &size, DT_WORDBREAK);
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
private:
	std::string planet[3];
	std::string mission[3];
	std::string modifier[3];
	std::string faction;
	std::string boss;
};