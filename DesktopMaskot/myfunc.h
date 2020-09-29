#include <Windows.h>
#include <map>
#include <fstream>
#include <iostream>

#ifndef MYFUNC
#define MYFUNC

inline void hideWindow(HWND hwnd) {
	long style = GetWindowLong(hwnd, GWL_STYLE);
	style &= ~(WS_VISIBLE);
	SetWindowLong(hwnd, GWL_STYLE, style);
	SetWindowPos(hwnd, NULL, 0, 0, 0, 0, (SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED));
}

inline void showWindow(HWND hwnd) {
	long style = GetWindowLong(hwnd, GWL_STYLE);
	style |= (WS_VISIBLE);
	SetWindowLong(hwnd, GWL_STYLE, style);
	SetWindowPos(hwnd, NULL, 0, 0, 0, 0, (SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED));
}

inline SYSTEMTIME timeCheck() {
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	return sys;
}

inline int getIntFromHandle(HWND h) {
	char buf[1000];
	GetWindowText(h, buf, 100);
	try {
		int ret = std::stoi(std::string(buf));
		return ret;
	}
	catch (std::invalid_argument) {
		throw;
	}
}

template<typename Iterator>
class withIndexItrator :public Iterator {
	std::size_t i = 0;
public:
	withIndexItrator(Iterator iter): Iterator(iter){ }
	auto& operator ++()
	{
		++i;
		this->Iterator::operator ++();
		return *this;
	}
	auto operator *() const noexcept
	{
		return std::pair<
			std::size_t,
			typename std::iterator_traits<Iterator>::reference >
		{ i, * static_cast<Iterator const&>(*this) };
	}
};

template < typename Range >
class with_index {
	Range& range;
public:
	with_index(Range& range)
		: range(range)
	{ }

	auto begin() const
	{
		return withIndexItrator{ std::begin(range) };
	}
	auto end() const
	{
		return withIndexItrator{ std::end(range) };
	}
};


inline void getTranslate(std::map< std::string, std::string>& t) {
	std::ifstream ifs("trans.txt", std::ios::in);
	if (!ifs)
		return;
	std::string tmp;
	std::string str;
	while (getline(ifs, tmp)) {
		std::stringstream ss{ tmp };
		std::string s;
		std::string temp[2];
		for (int i = 0; getline(ss, s, ','); i++) {
			temp[i] = s;
		}
		t.insert(std::make_pair(temp[0], temp[1]));
	}
}
#endif // !MYFUNC
