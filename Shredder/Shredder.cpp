#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include "Windows.h"
using namespace std;

bool wipeFile(filesystem::path path) {
	wprintf(L"Attempting to open \'%s\'...", path.wstring().c_str());

	fstream file(path, fstream::out | fstream::in | fstream::binary);

	if ( !file.is_open() ) {
		wcout << "Failed!" << endl;
		return 1;
	}
	else {
		wcout << "Success!" << endl;
	}

	file.seekg( 0, file.end );
	uint64_t size = file.tellg();
	file.seekg( 0 );

	wcout << "Overwriting binary data...";

	for ( int i = 0; i < size; i++ ) {
		file.put( 0x00 );
	}

	wcout << "Done!" << endl;

	wcout << "Verifying...";

	file.seekg(0);
	bool success = true;
	for ( int i = 0; i < size; i++ ) {
		if (file.get() != 0x00)
			success = false;
			break;
	}
	if ( success ) {
		wcout << "Done!" << endl;
	}
	else {
		wcout << "Failed! Will not delete" << endl;
	}

	wcout << "Closing file...";

	file.close();

	if ( !file.is_open() ) {
		wcout << "Done!" << endl;
	}
	else {
		wcout << "Failed!" << endl;
		wcout << "Was unable to close file. Aborting..." << endl;
		this_thread::sleep_for( chrono::seconds( 2 ) );
		return 1;
	}

	if ( success )
		filesystem::remove( path );
	return !success;
}

int wmain(int argc, wchar_t *argv[]) {

	if (argc < 2) {
		MessageBoxA(NULL, "File(s) not specified.", NULL, MB_OK | MB_ICONERROR);
		return 1;
	}

	vector<filesystem::path> files;
	for (int i = 1; i < argc; i++) {
		if (filesystem::exists(argv[i])) {
			files.push_back(argv[i]);
		}
	}

	int numFiles = argc - 1;

	if (files.size() > 1) {
		wcout << "Selected " << files.size() << " files" << endl;
	}
	else {
		wprintf(L"Selected \'%s\'\n", files[0].wstring().c_str());
	}
	
	wcout << "Are you sure you'd like to shred these files/folders? (Y/N) ";

	if (tolower(getchar()) != 'y')
		return 0;

	fstream file;
	for (const auto& x : files) {
		if (filesystem::is_directory(x)) {
			for (const auto& y : filesystem::recursive_directory_iterator(x)) {
				if (filesystem::is_directory(y)) {
					continue;
				}
				if (wipeFile( y )) continue;
			}
			filesystem::remove_all(x);
		}
		else {
			wipeFile( x );
		}
	}
	wcout << "\n\nAll Done!" << endl;
	this_thread::sleep_for( chrono::seconds( 4 ) );
	return 0;
}