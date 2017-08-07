#include "HTMLReader.h"

int main(int argc, char** argv) {
	
	std::string executableDir(argv[0]); // Directory of executable
	#ifdef WIN32 || _WIN32 // If win32
		std::string workingDir = executableDir.substr(0, executableDir.find_last_of('\\') + 1); // find last \ and cut string
	#else // Win64 (or other, but is treated as win64)
		std::string workingDir = executableDir.substr(0, executableDir.find_last_of('/') + 1); // find last / and cut string
	#endif // WIN32 || _WIN32

	std::string CMonthSchedule(workingDir + "Schedule.html"); // dir of current month schedule
	std::string NMonthSchedule(workingDir + "NSchedule.html"); // dir of next month schedule

	HTMLReader CMonthReader(CMonthSchedule);
	HTMLReader NMonthReader(NMonthSchedule);

	std::ofstream FileWriter = std::ofstream();
	FileWriter.open(workingDir + "Shifts.txt");
	if (!FileWriter.good()) {
		std::cout << "Failed to find or open file. " << workingDir << "Shifts.txt" << std::endl;
		exit(-1); // File not found error
	}
	
	for (ShiftData cshift : *CMonthReader.GetDates()) {
		FileWriter << cshift.toFinalStr() << "\n";
	}

	for (ShiftData cshift : *NMonthReader.GetDates()) {
		FileWriter << cshift.toFinalStr() << "\n";
	}

	FileWriter.close();
	return 0;
}