#pragma once
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

enum NAME {
	BODY = 0,
	EYE,
	HAIR,
	KUTI,
	MAYU,
	OTHER,
	USIRO,
};


inline bool getFileNmaes(std::string folderPath, std::vector<std::string>& file_names) {
	using namespace std::filesystem;
	directory_iterator iter(folderPath), end;
	std::error_code err;
	for (; iter != end && !err; iter.increment(err)) {
		const directory_entry entry = *iter;

		file_names.push_back(entry.path().string());
		//printf("%s\n", file_names.back().c_str());
	}
	//printf("---------------------------------------------------------------------------------\n");
	/* ÉGÉâÅ[èàóù */
	if (err) {
		std::cout << err.value() << std::endl;
		std::cout << err.message() << std::endl;
		return false;
	}
	return true;
}