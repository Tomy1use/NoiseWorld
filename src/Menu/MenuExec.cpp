#include <Menu/MenuExec.h>

MenuExec::MenuExec(const MenuConf* conf)
{
	menu = createMenu(conf->firstItem);
}

MenuExec::~MenuExec()
{
	DestroyMenu(menu);
}

HMENU MenuExec::createMenu(MenuConf::Item* item)
{
	HMENU menu = CreatePopupMenu();
	for( ; item; item = item->nextSibling){
		if(item->firstChild){
			HMENU submenu = createMenu(item->firstChild);
			addSubmenu(menu, item->data->caption, submenu);
		}else{
			if(item->data){
				addItem(menu, item->data->caption, (unsigned int)(item));
			}else{
				addSeparator(menu);
			}
		}
	}
	return menu;
}

void MenuExec::addSeparator(HMENU parent)
{
	MENUITEMINFO info;
	info.cbSize = sizeof(MENUITEMINFO);
	info.fMask = MIIM_FTYPE;
	info.fType = MFT_SEPARATOR;
	InsertMenuItem(parent, 0, true, &info);
}

void MenuExec::addItem(HMENU parent, const char* caption, unsigned int id)
{
	MENUITEMINFO info;
	info.cbSize = sizeof(MENUITEMINFO);
	info.fMask = MIIM_STRING + MIIM_ID;
	info.dwTypeData = LPSTR(caption);
	info.cch = strlen(caption);
	info.fState = MFS_ENABLED;
	info.wID = id;
	InsertMenuItem(parent, 0, true, &info);
}

void MenuExec::addSubmenu(HMENU parent, const char* caption, HMENU submenu)
{
	MENUITEMINFO info;
	info.cbSize = sizeof(MENUITEMINFO);
	info.fMask = MIIM_STRING + MIIM_SUBMENU;
	info.dwTypeData = LPSTR(caption);
	info.cch = strlen(caption);
	info.fState = MFS_ENABLED;
	info.hSubMenu = submenu;
	InsertMenuItem(parent, 0, true, &info);
}

MenuExec::Result MenuExec::execute(int x, int y, HWND window)
{
	unsigned int result = TrackPopupMenu(
		menu, TPM_RETURNCMD+TPM_NONOTIFY,
		x, y, 0, window, NULL);
	if(result){
		MenuConf::Item* item = (MenuConf::Item*)(result);
		return Result(item->data->name);
	}
	return Result();
}
