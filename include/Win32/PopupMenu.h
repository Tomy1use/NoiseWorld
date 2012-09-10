#pragma once

#include <windows.h>
#include <string.h>

struct PopupMenu
{
    HMENU menu;
    PopupMenu()
        : menu(NULL)
    {
        menu = CreatePopupMenu();
    }
    ~PopupMenu()
    {
        DestroyMenu(menu);
    }
    operator HMENU () {return menu;}
    void item(const char* name, unsigned int id)
    {
        MENUITEMINFO info;
        info.cbSize = sizeof(MENUITEMINFO);
        info.fMask = MIIM_STRING + MIIM_ID;
        info.dwTypeData = LPSTR(name);
        info.cch = strlen(name);
        info.fState = MFS_ENABLED;
        info.wID = id;
        InsertMenuItem(menu, 0, true, &info);
    }
    void combo(bool checked, const char* name, unsigned int id)
    {
        MENUITEMINFO info;
        info.cbSize = sizeof(MENUITEMINFO);
        info.fMask = MIIM_STRING + MIIM_ID + MIIM_FTYPE + MIIM_STATE;
        info.dwTypeData = LPSTR(name);
        info.cch = strlen(name);
        info.fState = MFS_ENABLED + checked ? MFS_CHECKED : 0;
        info.wID = id;
        info.fType = MFT_RADIOCHECK;
        InsertMenuItem(menu, 0, true, &info);
    }
	void checkbox(bool checked, const char* name, unsigned int id)
    {
        MENUITEMINFO info;
        info.cbSize = sizeof(MENUITEMINFO);
        info.fMask = MIIM_STRING + MIIM_ID + MIIM_FTYPE + MIIM_STATE;
        info.dwTypeData = LPSTR(name);
        info.cch = strlen(name);
        info.fState = MFS_ENABLED + checked ? MFS_CHECKED : 0;
        info.wID = id;
        //info.fType = MFT_RADIOCHECK;
        InsertMenuItem(menu, 0, true, &info);
    }
    void submenu(const char* name, HMENU sub)
    {
        MENUITEMINFO info;
        info.cbSize = sizeof(MENUITEMINFO);
        info.fMask = MIIM_STRING + MIIM_SUBMENU;
        info.dwTypeData = LPSTR(name);
        info.cch = strlen(name);
        info.fState = MFS_ENABLED;
        info.hSubMenu = sub;
        InsertMenuItem(menu, 0, true, &info);
    }
    unsigned int execute(int x, int y, HWND window)
    {
        return TrackPopupMenu(
            menu, TPM_RETURNCMD+TPM_NONOTIFY,
            x, y, 0, window, NULL);
    }
};
