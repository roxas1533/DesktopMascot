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
			fuki.push_back(Fukidasi("�J�^�J�i�ꂪ���ȕ��́u�g���{�v�ƌĂԂƂ悢�ł��傤�B�_�~�[�e�L�X�g�̓_�~�[������_�~�[���͂Ƃ��Ă΂�邱�Ƃ�����܂��B�J�^�J�i�ꂪ���ȕ��́u�g���{�v�ƌĂԂƂ悢�ł��傤�B�Ȃ��A�g���{�́u�g�v�Ƃ͕����g�̂��Ƃł��B�����������̗p�ꂾ�����Ǝv���܂��B���̃_�~�[�e�L�X�g�͎��R�ɉ��ς��邱�Ƃ��o���܂��B�_�~�[�e�L�X�g�̓_�~�[������_�~�[���͂Ƃ��Ă΂�邱�Ƃ�����܂��B",hdcMem,50));

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