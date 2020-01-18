#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include "Windows.h"
using namespace std;

int main(int argc, char *argv[]) {

	if (argc < 2) {
		MessageBoxA(NULL, "File(s) not specified.", NULL, MB_OK | MB_ICONERROR);
		return 1;
	}

	char answer;
	fstream file;
	int numFiles = argc - 1;

	if (numFiles > 1) {
		cout << "Selected " << argc - 1 << " files" << endl;
	}
	else {
		cout << "Selected \'" << argv[1] << "\'" << endl;
	}
	
	cout << "Are you sure you'd like to shred these files? (Y/N) ";
	answer = getchar();

	if (answer == 'n')
		return 0;

	for (int i = 0; i < numFiles; i++) {

		cout << "\nAttempting to open \'" << argv[i+1] << "\'...";

		file.open(argv[i+1], fstream::out | fstream::in | fstream::binary);

		if (!file.is_open()) {
			cout << "Failed!" << endl;
			cerr << "Aborting...";
			this_thread::sleep_for(chrono::seconds(2));
			cerr << "Almost Done!";
			return 1;
		}
		else {
			cout << "Success!" << endl;
		}

		file.seekg(0, file.end);
		UINT64 size = file.tellg();
		file.seekg(0);

		cout << "Overwriting binary data...";

		for (int i = 0; i < size; i++) {
			file.put(0x00);
		}

		cout << "Done!" << endl;

		cout << "Closing file...";

		file.close();

		if (!file.is_open()) {
			cout << "Done!" << endl;
		}
		else {
			cout << "Failed!" << endl;
			cerr << "Was unable to close file. Aborting..." << endl;
			this_thread::sleep_for(chrono::seconds(2));
			return 1;
		}

		remove(argv[i+1]);
	}

	this_thread::sleep_for(chrono::seconds(2));
	return 0;
}