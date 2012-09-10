#pragma once

#include <windows.h>
#include <Menu/MenuConf.h>

struct MenuExec
{
	struct Result
	{
		static const int MaxPath = MAX_PATH;
		char path[MaxPath];
		Result() {path[0] = 0;}
		Result(const char* value) {strncpy(path, value, MaxPath);}
		bool operator == (const char* value){return 0 == strcmp(path, value);}
		operator const char* () {return path;}
	};
	
	HMENU menu;
	
	MenuExec(const MenuConf* conf);
	~MenuExec();
	HMENU createMenu(MenuConf::Item* item);
	void addSeparator(HMENU parent);
	void addItem(HMENU parent, const char* caption, unsigned int id);
	void addSubmenu(HMENU parent, const char* caption, HMENU submenu);
	Result execute(int x, int y, HWND window);
};
