#pragma once
#include <string>
#include <list>
#include<Windows.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "myfunc.h"

#ifndef PLAN
#define PLAN
struct Plan
{
	const int month, day, hour, min;
	const std::string text;
	auto operator<(const Plan& right) {
		if (month != right.month)
			return month < right.month;
		if (day != right.day)
			return day < right.day;
		if (hour != right.hour)
			return hour < right.hour;
		if (min != right.min)
			return min < right.min;
	}
};
std::list<Plan> plans;

inline bool writePlan() {
	try {
		plans.sort();
		std::ofstream outputfile("plans.txt");
		for (auto p : plans) {
			outputfile << p.month << "," << p.day << "," << p.hour << "," << p.min << "," << p.text << "\n";
		}
		outputfile.close();
	}
	catch (std::exception) {
		return FALSE;
	}
	return TRUE;
}


inline bool readPlan() {
	std::ifstream ifs("plans.txt", std::ios::in);
	if (!ifs)
		return FALSE;
	std::string tmp;
	std::string str;
	int sum = 0;
	SYSTEMTIME now = timeCheck();
	while (getline(ifs, tmp)) {
		std::stringstream ss{ tmp };
		std::string s;
		int data[4];
		for (int i = 0; getline(ss, s, ','); i++) {
			if (i != 4)
				data[i] = std::stoi(s);
		}
		Plan p = { data[0],data[1],data[2],data[3],s };
		Plan nowP = { now.wMonth,now.wDay,now.wHour,now.wMinute,"" };
		if (!(p < nowP))
			plans.push_back(p);
	}
}


inline std::vector<std::vector<bool>> readZikan() {
	std::ifstream ifs("zikanwari.txt", std::ios::in);
	std::vector<std::vector<bool>> z(5,std::vector<bool>(5));
	if (!ifs)
		exit(1);
	std::string tmp;
	std::string str;
	for (int j = 0; getline(ifs, tmp); j++) {
		std::stringstream ss{ tmp };
		std::string s;
		for (int i = 0; getline(ss, s, ','); i++) {
			z[j][i] = s == "true" ? true : false;
		}
	}
	for (auto t : z) {
		for (auto u : t)
			std::cout << u;
		std:: cout << "\n";
	}
	return z;
}


inline std::string checkPlans(SYSTEMTIME pnow) {
	std::list<Plan>::iterator it = plans.begin();
	while (it != plans.end()) {
		if (pnow.wHour == (*it).hour && pnow.wMinute == (*it).min && pnow.wMonth == (*it).month && pnow.wDay == (*it).day) {
			std::string t = (*it).text;
			it = plans.erase(it);
			writePlan();
			return t;
		}
		++it;
	}
	return "";
}

#endif // !PLAN
