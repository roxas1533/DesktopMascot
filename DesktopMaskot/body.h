#pragma once
#include <gdiplus.h>
#include "loadPng.h"
#include "Fukidasi.h"
#include "Plan.h"
#include <list>
#include <time.h>
#include <memory>
using namespace Gdiplus;


class yukari {
public:
	yukari() {
		decrate = randRange(-6, 6);
	}
	void main(Graphics* g, HDC hdcMem, HWND hwndd) {

		if (time == 0) {
			pnow = timeCheck();
			flink[0] = randRange(0, time + 200);
			getTex(tex);
			wakeUp(hdcMem);
			hwnd = hwndd;
			//テスト用吹き出し
			//fuki.push_back(Fukidasi());
			zikanwari = readZikan();
		}

		std::string tex;
		if ((tex = checkPlans(pnow)) != "") {
			Fukidasi::fuki.push_back(std::make_unique<Fukidasi>(tex + "、予定の時間です！", hdcMem, NORMAL, 80));
		}
		std::list<std::unique_ptr<Fukidasi>>::iterator it = Fukidasi::fuki.begin();
		while (it != Fukidasi::fuki.end()) {
			if ((*it)->drawFuki(g, hdcMem)) {
				nowTalking = false;
				normal(g);
				CustomButton::destroyFlag = true;
				it = Fukidasi::fuki.erase(it);
				InvalidateRect(hwnd, &rec2, TRUE);
			}
			else {
				nowTalking = true;
				switch ((*it)->emotion)
				{
				case NORMAL:
					tNormal(g, (*it)->body, (*it)->dec, (*it)->eye);
					break;
				case BOTH:
					tBoth(g, (*it)->body, (*it)->dec, (*it)->eye);
				default:
					break;
				}

				break;
			}
		}
		if (!nowTalking)
			normal(g);
		time++;
	}
	void normal(Graphics* g) {
		if (flink[0] <= time) {
			flink[1]++;
		}
		if (flink[1] == 5) {
			flink[0] = randRange(time, time + 200);
			flink[1] = 0;
		}
		g->DrawImage(tex[BODY][0][0], 220, 0);
		g->DrawImage(tex[EYE][0][flink[1] % 4], 220, 0);
		g->DrawImage(tex[KUTI][0][0], 220, 0);
		g->DrawImage(tex[MAYU][0][0], 220, 0);
		if (decrate >= 0)
			g->DrawImage(tex[OTHER][decrate][0], 220, 0);
		if (decrate == -1) {
			g->DrawImage(tex[USIRO][21][0], 220, 0);
		}
	}

	void tNormal(Graphics* g, int body, int dec, int eye) {
		g->DrawImage(tex[BODY][body][0], 220, 0);
		g->DrawImage(tex[EYE][0][0], 220, 0);
		g->DrawImage(tex[KUTI][0][3], 220, 0);
		g->DrawImage(tex[MAYU][0][0], 220, 0);
		if (dec >= 0)
			g->DrawImage(tex[USIRO][dec][0], 220, 0);
		if (decrate >= 0)
			g->DrawImage(tex[OTHER][decrate][0], 220, 0);
		if (decrate == -1) {
			g->DrawImage(tex[USIRO][21][0], 220, 0);
		}
	}

	void tBoth(Graphics* g, int body, int dec, int eye) {
		g->DrawImage(tex[BODY][5][0], 220, 0);
		g->DrawImage(tex[EYE][eye][0], 220, 0);
		g->DrawImage(tex[KUTI][2][0], 220, 0);
		g->DrawImage(tex[MAYU][1][0], 220, 0);
		if (dec >= 0)
			g->DrawImage(tex[USIRO][dec][0], 220, 0);
		if (decrate >= 0)
			g->DrawImage(tex[OTHER][decrate][0], 220, 0);
		if (decrate == -1) {
			g->DrawImage(tex[USIRO][21][0], 220, 0);
		}
	}

	void wakeUp(HDC hdcMem) {
		std::string text = "これはバグです！";
		EMO emo = NORMAL;
		std::cout << pnow.wHour;
		if (pnow.wHour >= 4 && pnow.wHour <= 7) {
			text = "おはようございます！早起きですね！";
		}
		else if (pnow.wHour >= 8 && pnow.wHour <= 10) {
			text = "おはようございます！";
		}
		else if (pnow.wHour >= 11 && pnow.wHour <= 12) {
			text = "おはようございます！\nもうお昼です...";
		}
		else if ((pnow.wHour >= 13 && pnow.wHour <= 23) || (pnow.wHour >= 0 && pnow.wHour <= 3)) {
			text = "";
		}
		if (strcmp(text.c_str(), ""))
			Fukidasi::fuki.push_back(std::make_unique<Fukidasi>(text, hdcMem, emo, 80));
	}

	void zihou(HDC hdcMem) {
		std::string text = "これは時報のバグです！";
		EMO emo = NORMAL;
		if (checkTime(0, 56, 0,40)) {
			text = "12時です！ごはんの準備です！";
			flag[0] = true;
		}
		else if (checkTime(18, 0, 1)) {
			text = "18時です！ごはん...食べたいです..";
			flag[1] = true;
		}
		else if (checkTime(0, 0, 2)) {
			text = "日付、変わりましたよ！";
			flag[2] = true;
		}
		else if (checkTime(1, 30, 3)) {
			emo = BOTH;
			text = "1時半です！そろそろ寝てください！";
			flag[3] = true;
		}
		else {
			text = "";
		}
		if (strcmp(text.c_str(), "")) {
			Fukidasi::fuki.push_back(std::make_unique<Fukidasi>(text, hdcMem, emo, 80));
		}
		if (checkTime(3, 0, 100)) {
			for (int i = 0; i < 100; i++) {
				flag[i] = false;
			}
		}
	}

	void zikanwariCheck(HDC hdcMem) {
		std::string text = "これは時報のバグです！";
		EMO emo = NORMAL;
		if (checkTime(8, 50, 4) && checkYoubi(1)) {
			text = "1限がはじまりますよ！";
			flag[4] = true;
		}
			else if (checkTime(10, 40, 5)&& checkYoubi(2)) {
			text = "2限がはじまりますよ！";
			flag[5] = true;
		}
			else if (checkTime(13, 10, 6) && checkYoubi(3)) {
			text = "3限がはじまります！";
			flag[6] = true;
		}
			else if (checkTime(15, 0, 7) && checkYoubi(4)) {
			text = "4限がはじまります。おやつは、ありますか？";
			flag[7] = true;
		}
			else if (checkTime(16, 50, 8) && checkYoubi(5)) {
			text = "5限がはじまります。最後です！";
			flag[8] = true;
		}
			else
			text = "";
		if (strcmp(text.c_str(), "")) {
			Fukidasi::fuki.push_back(std::make_unique<Fukidasi>(text, hdcMem, emo, 80));
		}
	}
	bool checkTime(int h, int m, int flagNum, int s = 0) {
		return pnow.wHour == h && pnow.wMinute == m && pnow.wSecond == s && !flag[flagNum];
	}
	bool checkYoubi(int gen) {
		if (pnow.wDayOfWeek == 0 || pnow.wDayOfWeek == 6)
			return false;
		return zikanwari[gen-1][pnow.wDayOfWeek-1];
	}

	SYSTEMTIME pnow;
private:
	std::vector<std::vector<std::vector<Bitmap*>>> tex;
	int time = 0;
	bool flag[100];
	bool nowTalking = false;
	long long flink[2] = { 0 ,0 };
	int decrate = 0;
	std::vector<std::vector<bool>> zikanwari;

};