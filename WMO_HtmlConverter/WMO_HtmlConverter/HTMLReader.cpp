#include "HTMLReader.h"

HTMLReader::HTMLReader(std::string path) {
	Dates = std::vector<ShiftData>();
	ReadFile(path);
}

HTMLReader::~HTMLReader() {

}

std::vector<ShiftData>* HTMLReader::GetDates() {
	return &Dates;
}

void HTMLReader::ReadFile(std::string path) {
	std::fstream fStream = std::fstream(path);
	if (!fStream) {
		std::cout << "Failed to find or open file. " << path << std::endl;
		exit(-1); // File not found error
	}

	std::string line;
	std::string month_and_year = "ERROR";
	while (std::getline(fStream, line)) { // read file line by line
		if (str_contains(line, "onclick=updateMonth(-1)")) {
			unsigned int sPos = strpos((char*)line.c_str(), "</DIV>", 1) + 6;
			month_and_year = line.substr(sPos);
		}
		else if (str_contains(line, "Meal:")) { // Search for this str since all work days will contain it
			if (str_contains(line, "<SPAN class=notMonthDate>")) {
				// is not a date for this month
			}
			else { // work days without a meal contain this, if you work part time this is what may show up :/ as I do :/ 
				unsigned int sPos = strpos((char*)line.c_str(), "<B>", 1) + 3, ePos = strpos((char*)line.c_str(), "</B>", 1);
				std::string hours = line.substr(sPos, ePos - sPos); // ^ +3 is adding offset so htat <B> is excluded from src
				sPos = strpos((char*)line.c_str(), "<BR>", 1) + 4, ePos = strpos((char*)line.c_str(), "</SPAN>", 2);
				std::string position = line.substr(sPos, ePos - sPos);
				sPos = strpos((char*)line.c_str(), "<SPAN>", 2) + 6, ePos = strpos((char*)line.c_str(), "</SPAN>", 3);
				std::string meal = line.substr(sPos, ePos - sPos);
				sPos = strpos((char*)line.c_str(), "<SPAN class=cellDate>", 1) + 21, ePos = strpos((char*)line.c_str(), "</SPAN>", 1);
				std::string day_of_month = line.substr(sPos, ePos - sPos);
				ShiftData cShift = ShiftData();
				cShift.Date = std::string(day_of_month + " " + month_and_year);
				sPos = 0, ePos = strpos((char*)hours.c_str(), " ", 1);
				cShift.ShiftStart = hours.substr(sPos, ePos - sPos);
				sPos = strpos((char*)hours.c_str(), " ", 2) + 1, ePos = 0;
				cShift.ShiftEnd = hours.substr(sPos);
				cShift.Meal = meal;
				cShift.Position = position;
				Dates.emplace_back(cShift);
			}
		}
		else if (str_contains(line, "Meal<")) { // work days with a meal contain this
			if (str_contains(line, "<SPAN class=notMonthDate>")) {
				// is not a date for this month
			}
			else {
				unsigned int sPos = strpos((char*)line.c_str(), "<B>", 1) + 3, ePos = strpos((char*)line.c_str(), "</B>", 1);
				std::string hours = line.substr(sPos, ePos - sPos);
				sPos = strpos((char*)line.c_str(), "<BR>", 1) + 4, ePos = strpos((char*)line.c_str(), "</SPAN>", 2);
				std::string position = line.substr(sPos, ePos - sPos);
				sPos = strpos((char*)line.c_str(), "<B>", 2) + 3, ePos = strpos((char*)line.c_str(), "</B>", 2);
				std::string meal = line.substr(sPos, ePos - sPos);
				sPos = strpos((char*)line.c_str(), "<SPAN class=cellDate>", 1) + 21, ePos = strpos((char*)line.c_str(), "</SPAN>", 1);
				std::string day_of_month = line.substr(sPos, ePos - sPos);
				ShiftData cShift = ShiftData();
				cShift.Date = std::string(day_of_month + " " + month_and_year);
				sPos = 0, ePos = strpos((char*)hours.c_str(), " ", 1);
				cShift.ShiftStart = hours.substr(sPos, ePos - sPos);
				sPos = strpos((char*)hours.c_str(), " ", 2) + 1, ePos = 0;
				cShift.ShiftEnd = hours.substr(sPos);
				cShift.Meal = "Meal: " + meal;
				cShift.Position = position;
				Dates.emplace_back(cShift);
			}
		}
	}

	fStream.close();
}

bool HTMLReader::str_contains(std::string base, std::string find) {
	return (base.find(find) != std::string::npos) ? true : false;
}

int HTMLReader::strpos(char * haystack, char * needle, int nth) { // ret -1 for npos
	char *res = haystack;
	for (int i = 1; i <= nth; i++)
	{
		res = strstr(res, needle);
		if (!res)
			return -1;
		else if (i != nth)
			res = res++;
	}
	return res - haystack;
}