#pragma once
#include <random>
std::random_device rnd;     // 非決定的な乱数生成器
static std::mt19937 mt(rnd());

inline int randRange(int a, int b) {
	std::uniform_int_distribution<> rand1(a, b);
	return rand1(mt);
}