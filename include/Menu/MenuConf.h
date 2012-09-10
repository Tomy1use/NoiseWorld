#pragma once

#include <stdio.h>

struct MenuConf
{
	struct Hotkey
	{
		static const int StringSize = 20;
		char string[StringSize];
		Hotkey* nextSibling;
		Hotkey(): nextSibling(0) {string[0] = 0;}
	};
	struct ItemData
	{
		static const int NameSize = 50;
		char name[NameSize];
		static const int CaptionSize = 50;
		char caption[CaptionSize];
		Hotkey* hotkeys;
		ItemData(): hotkeys(0) {
			name[0] = 0;
			caption[0] = 0;
		}
	};
	struct Item
	{
		Item* nextSibling;
		Item* firstChild;
		Item* parent;
		int indent;
		ItemData* data;
		Item(ItemData* data): nextSibling(0), firstChild(0), parent(0), indent(0), data(data) {}
	};
	
	const char* filename;
	int lineIndex;
	int curChar;
	int indentChar;
	int parentIndent;
	Item* firstItem;
	Item* curParent;
	int caret;
	
	MenuConf(const char* filename);
	bool isAlpha(char c) {return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z';}
	bool isNum(char c) {return '0' <= c && c <= '9';}
	bool isTokenChar(char c) {return isAlpha(c) || isNum(c) || c == '_';}
	bool isHotkeyChar(char c) {return isTokenChar(c) || c == '+' || c == '-';}
	int nextChar(FILE* f);
	void eatLine(FILE* f);
	template<class Item>
	void addSibling(Item* sibling, Item* item)
	{
		while(sibling->nextSibling) sibling = sibling->nextSibling;
		sibling->nextSibling = item;
	}
	void addChild(Item* parent, Item* item);
	void printItems(Item* item, int indent=0);
	bool readToken(FILE* f, char* token, int TokenSize);
	bool readHotkey(FILE* f, char* hotkey, int HotkeySize);
	bool readQuotedString(FILE* f, char* string, int StringSize);
	void eatWhitespace(FILE* f);
	void addHotkey(ItemData* data, Hotkey* hotkey);
	bool readItemNameIfPresent(FILE* f, Item* item);
	bool readItemCaption(FILE* f, Item* item);
	bool readItemHotkeysIfPresent(FILE* f, Item* item);
};
