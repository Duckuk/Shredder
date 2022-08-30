#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <thread>
#include "Windows.h"
using namespace std;

int wmain(int argc, wchar_t *argv[]) {

	if (argc < 2) {
		MessageBoxA(NULL, "File(s) not specified.", NULL, MB_OK | MB_ICONERROR);
		return 1;
	}

	char answer;
	fstream file;
	int numFiles = argc - 1;

	if (numFiles > 1) {
		wcout << "Selected " << argc - 1 << " files" << endl;
	}
	else {
		wprintf(L"Selected \'%s\'\n", argv[1]);
	}
	
	wcout << "Are you sure you'd like to shred these files? (Y/N) ";
	answer = getchar();

	if (answer == 'n')
		return 0;

	for (int i = 0; i < numFiles; i++) {

		wprintf(L"Attempting to open \'%s\'...", argv[i+1]);

		file.open(argv[i+1], fstream::out | fstream::in | fstream::binary);

		if (!file.is_open()) {
			wcout << "Failed!" << endl;
			wcout << "Aborting...";
			this_thread::sleep_for(chrono::seconds(2));
			wcout << "Almost Done!";
			return 1;
		}
		else {
			wcout << "Success!" << endl;
		}

		file.seekg(0, file.end);
		uint64_t size = file.tellg();
		file.seekg(0);

		wcout << "Overwriting binary data...";

		for (int i = 0; i < size; i++) {
			file.put(0x00);
		}

		wcout << "Done!" << endl;

		wcout << "Closing file...";

		file.close();

		if (!file.is_open()) {
			wcout << "Done!" << endl;
		}
		else {
			wcout << "Failed!" << endl;
			wcout << "Was unable to close file. Aborting..." << endl;
			this_thread::sleep_for(chrono::seconds(2));
			return 1;
		}

		_wremove(argv[i+1]);
	}

	this_thread::sleep_for(chrono::seconds(2));
	return 0;
}