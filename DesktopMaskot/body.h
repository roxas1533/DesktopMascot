#pragma once
#include <gdiplus.h>
#include "loadPng.h"
#include "Fukidasi.h"
#include <list>
#include <time.h>
using namespace Gdiplus;

class yukari {
public:
	yukari() {
		decrate = randRange(-6, 6);
	}
	void main(Graphics* g, HDC hdcMem,HWND hwndd) {
		if (time == 0) {
			flink[0] = randRange(0, time + 200);
			getTex(tex);
			wakeUp(hdcMem);
			hwnd = hwndd;
			//テスト用吹き出し
			//fuki.push_back(Fukidasi());

	
		}

		zihou(hdcMem);
		//std::cout << fuki.size();
		std::list<Fukidasi>::iterator it = fuki.begin();
		while (it != fuki.end()) {
			if (it->drawFuki(g, hdcMem)) {
				nowTalking = false;
				normal(g);
				CustomButton::destroyFlag=true;
				it = fuki.erase(it);
			}
			else {
				nowTalking = true;
				switch (it->emotion)
				{
				case NORMAL:
					tNormal(g, it->body, it->dec, it->eye);
					break;
				case BOTH:
					tBoth(g, it->body, it->dec, it->eye);
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
		SYSTEMTIME pnow = timeCheck();
		std::string text = "これはバグです！";
		EMO emo = NORMAL;
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
			fuki.push_back(Fukidasi(text, hdcMem, emo, 80));
	}

	void zihou(HDC hdcMem) {
		SYSTEMTIME pnow = timeCheck();
		std::string text = "これは時報のバグです！";
		EMO emo = NORMAL;
		//std::cout << pnow.wHour <<":"<< pnow.wMinute << ":"<< pnow.wSecond<<":"<< pnow.wMilliseconds <<"\n";

		if (pnow.wHour == 12 && pnow.wMinute == 0 && pnow.wSecond == 00 && !flag[0]) {
			text = "12時です！ごはんの準備です！";
			flag[0] = true;
		}
		else if (pnow.wHour == 18 && pnow.wMinute == 00 && pnow.wSecond == 00 && !flag[1]) {
			text = "18時です！ごはん...食べたいです..";
			flag[1] = true;
		}
		else if (pnow.wHour == 00 && pnow.wMinute == 00 && pnow.wSecond == 00 && !flag[2]) {
			text = "日付、変わりましたよ！";
			flag[2] = true;
		}
		else if (pnow.wHour == 01 && pnow.wMinute == 30 && pnow.wSecond == 00 && !flag[2]) {
			emo = BOTH;
			text = "一時半です！そろそろ寝てください！";
			flag[2] = true;
		}
		else {
			text = "";
		}
		if (strcmp(text.c_str(), "")) {
			fuki.push_back(Fukidasi(text, hdcMem, emo, 80));
		}
		if (pnow.wHour == 3 && pnow.wMinute == 00 && pnow.wSecond == 00) {
			for (int i = 0; i < 100; i++) {
				flag[i] = false;
			}
		}
	}

	SYSTEMTIME timeCheck() {
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		return sys;
	}
	std::list<Fukidasi> fuki;

private:
	std::vector<std::vector<std::vector<Bitmap*>>> tex;
	int time = 0;
	bool flag[100];
	bool nowTalking = false;
	long long flink[2] = { 0 ,0 };
	int decrate = 0;
};