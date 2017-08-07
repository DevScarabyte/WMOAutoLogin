#include <iostream>

int main(int argc, char** argv) {

	// check if debug is enabled
	bool debug = false;
	for (int i = 0; i < argc; i++)
		if (std::string(argv[i]).find("debug") != std::string::npos)
			debug = true;

	// if it is, be nice by letting the user know :/
	if (debug)
		std::cout << "---DEBUG MODE---" << std::endl;

	std::string executableDir(argv[0]); // Directory of executable
#ifdef WIN32 || _WIN32 // If win32
	std::string workingDir = executableDir.substr(0, executableDir.find_last_of('\\') + 1); // find last \ and cut string
#else // Win64 (or other, but is treated as win64)
	std::string workingDir = executableDir.substr(0, executableDir.find_last_of('/') + 1); // find last / and cut string
#endif // WIN32 || _WIN32

	// To save time later, build str that is path to program
	std::string AutoLogDir((debug) ? "WMO_AutoLogin-d.exe" : "WMO_AutoLogin.exe");
	std::string HtmlConvertDir("WMO_HtmlConverter.exe");
	std::string PyScript("Sync_to_Google_Calendar.bat");

	int autoLogRes = system(std::string(AutoLogDir).c_str()); // check return code
	switch (autoLogRes)
	{
		case -2:
			std::cout << "Unable to find or open 'Credentials.txt' in WMO_AutoLogin.exe" << std::endl;
			break;
		case -3:
			std::cout << "Failed to login to WalmartOne website in WMO_AutoLogin.exe" << std::endl;
			break;
		case -4:
			std::cout << "Failed to get WalmartOne schedule in WMO_AutoLogin.exe" << std::endl;
			break;
		case -5:
			std::cout << "Connection to WalmartOne website timed out in WMO_AutoLogin.exe" << std::endl;
			break;
		case 0:
			std::cout << "Schedule fetch from WalmartOne website complete in WMO_AutoLogin.exe" << std::endl;
			break;
		default:
			std::cout << "Unknown error in WMO_AutoLogin.exe" << std::endl;
			break;
	}
	int htmlConvertedRes = system(std::string(HtmlConvertDir).c_str());
	if (htmlConvertedRes != 0)
		std::cout << "IO error in WMO_HtmlConverter.exe" << std::endl; // failed to read/open file or failed to open/write file
	int sync_to_googleRes = system(std::string(PyScript).c_str());
	if (sync_to_googleRes != 0) // make sure python is installed or no such program or dir error is thrown
		std::cout << "Error writing schedule to Google calendar in WMOAutoSchedule.py" << std::endl;
	system("pause"); // allows to read input
	return 0;
}