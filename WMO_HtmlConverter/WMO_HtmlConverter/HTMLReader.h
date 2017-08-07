#pragma once
#ifndef _HTML_READER_H_
#define _HTML_READER_H_
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ostream>

typedef struct ShiftData {
	std::string Date = "";
	std::string ShiftStart = "";
	std::string ShiftEnd = "";
	std::string Position = "";
	std::string Meal = "";

	std::string toFinalStr() { // TODO: change to const char*
		return std::string(Date + ShiftStart + " - " + ShiftEnd + " " + Meal + " " + Position);
	}
};

class HTMLReader
{
	public:
		HTMLReader(std::string path);
		~HTMLReader();

		std::vector<ShiftData>* GetDates();

	private:
		std::vector<ShiftData> Dates; // vector contain work dates and hours worked
		void ReadFile(std::string path); // Reads specified file and fills dates into vector
		bool str_contains(std::string base, std::string find); // Check if a string contains another specified string
		int strpos(char *haystack, char *needle, int nth); // needle in a haystack, find nth occurance of str, returns -1 if not found
};

#endif // !_HTML_READER_H_