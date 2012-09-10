#pragma once

#include <Windows.h>

struct FileEnvironment
{
	char curOpenFile[MAX_PATH];
	static const int MaxRecentFiles = 10;
	char recentFiles[MaxRecentFiles][MAX_PATH];
	int mostRecentFileIndex;
	OPENFILENAME ofn, sfn;
	
	FileEnvironment(const char* recentFilesTxt)
		: mostRecentFileIndex(0)
	{
		curOpenFile[0] = 0;
		for(int i=0; i<MaxRecentFiles; i++) recentFiles[i][0] = 0;
		ZeroMemory(&ofn, sizeof(ofn));
		ZeroMemory(&sfn, sizeof(sfn));
	}
	~FileEnvironment()
	{
	}
	
	bool openFileDialog(HWND window)
	{
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = window;
		ofn.lpstrFilter = "Shapecraft Scenes (*.e)\0*.e\0All Files\0*.*\0";
		ofn.lpstrFile = curOpenFile;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrInitialDir = ".";
		if(GetOpenFileName(&ofn)){
			printf("FileEnvironment::openFileDialog-> %s\n", ofn.lpstrFile);
			return true;
		}
		return false;
	}
	
	bool saveFileDialog(HWND window)
	{
		ZeroMemory(&sfn, sizeof(sfn));
		sfn.lStructSize = sizeof(sfn);
		sfn.hwndOwner = window;
		//sfn.lpstrFilter = "Shapecraft Scenes (*.e)\0*.e\0All Files\0*.*\0";
		sfn.lpstrFilter = "Shapecraft Scenes (*.txt)\0*.txt\0All Files\0*.*\0";
		sfn.lpstrFile = curOpenFile;
		sfn.nMaxFile = MAX_PATH;
		sfn.lpstrInitialDir = ".";
		if(GetSaveFileName(&sfn)){
			printf("FileEnvironment::saveFileDialog-> %s\n", sfn.lpstrFile);
			return true;
		}
		return false;
	}
	
	const char* saveFileName() {return sfn.lpstrFile;}
	const char* openFileName() {return ofn.lpstrFile;}
	void setCurrentOpenFile(const char* value) {strncpy(curOpenFile, value, MAX_PATH);}
};
