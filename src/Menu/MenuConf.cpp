#include <stdio.h>
#include <conio.h>
#include <Menu/MenuConf.h>

MenuConf::MenuConf(const char* filename)
	: filename(filename)
	, lineIndex(0)
	, indentChar(0)
	, parentIndent(0)
	, firstItem(0)
	, curParent(0)
	, caret(0)
{
	printf("MenuConf (%s)\n", filename);
	FILE* f = fopen(filename, "rt");
	if(! f){
		printf("MenuConf: failed open %s", filename);
		return;
	}
	nextChar(f);
	while(true){
		caret = 0;
		lineIndex ++;
		if(curChar == EOF) break;
		int indent = 0;
		while(curChar == ' ' || curChar == '\t'){
			if(indentChar == 0) indentChar = curChar;
			if(indentChar != curChar){
				printf("Mixed tabs and whitespace in %s.\nUse either tabs or whitespace for indentation.\n", filename);
				return;
			}
			indent ++;
			nextChar(f);
		}
		if(curChar == EOF){
			//printf("%d: EOF\n", lineIndex);
			break;
		}else if(curChar == '\n'){
			//printf("%d: blank\n", lineIndex);
			nextChar(f);
		}else if(isAlpha(curChar) || curChar=='"'){
			//printf("%d: item\n", lineIndex);
			//eatLine(f);
			Item* item = new Item(new ItemData());
			item->indent = indent;
			if(readItemNameIfPresent(f, item)){
				if(readItemCaption(f, item)){
					readItemHotkeysIfPresent(f, item);
				}
			}
			eatLine(f);
			while(curParent && item->indent <= curParent->indent){
				curParent = curParent->parent;
			}
			addChild(curParent, item);
			curParent = item;
		}else if(curChar == '-' && nextChar(f) == '-' && nextChar(f) == '-'){
			//separator
			Item* item = new Item(NULL);
			item->indent = indent;
			while(curParent && item->indent <= curParent->indent) curParent = curParent->parent;
			addChild(curParent, item);
			curParent = item;
			eatLine(f);
		}else{
			//comment
			eatLine(f);
		}
	}
	fclose(f);
	printItems(firstItem, 0);
}

int MenuConf::nextChar(FILE* f)
{
	caret ++;
	curChar = fgetc(f); 
	//if(curChar != '\n') putch(curChar);
	return curChar;
}

void MenuConf::eatLine(FILE* f)
{
	while(! (curChar == '\n' || curChar == EOF)) nextChar(f);
	nextChar(f);//eat newline
}

void MenuConf::addChild(Item* parent, Item* item)
{
	if(parent){
		item->nextSibling = parent->firstChild;
		parent->firstChild = item;
		item->parent = parent;
	}else{
		if(firstItem){
			item->nextSibling = firstItem;
			firstItem = item;
		}else{
			firstItem = item;
		}
	}
}

void MenuConf::printItems(Item* item, int indent)
{
	while(item){
		for(int i=0; i<indent; i++) _putch(' ');
		if(item->data){
			if(item->data->name[0]){
				printf("%s: ", item->data->name);
			}
			printf("'%s'", item->data->caption);
			if(item->data->hotkeys){
				printf(", [");
				for(Hotkey* h=item->data->hotkeys; h; h=h->nextSibling){
					printf(h->string);
					if(h->nextSibling) printf(", ");
				}
				printf("]");
			}
			printf("\n");
		}else{
			printf("---\n");
		}
		printItems(item->firstChild, indent+2);
		item = item->nextSibling;
	}
}

bool MenuConf::readToken(FILE* f, char* token, int TokenSize)
{
	if(! isTokenChar(curChar)){
		printf("%d, %d: Expected token character\n", lineIndex, caret);
		return false;
	}
	for(int i=0; i<TokenSize; i++){
		if(! isTokenChar(curChar)){
			token[i] = 0;
			return true;
		}
		token[i] = curChar;
		nextChar(f);
	}
	token[TokenSize-1] = 0;
	printf("%d, %d: Token too long\n", lineIndex, caret);
	return false;
}

bool MenuConf::readHotkey(FILE* f, char* hotkey, int HotkeySize)
{
	if(! isHotkeyChar(curChar)){
		printf("%d, %d: Expected hotkey character\n", lineIndex, caret);
		return false;
	}
	for(int i=0; i<HotkeySize; i++){
		if(! isHotkeyChar(curChar)){
			hotkey[i] = 0;
			return true;
		}
		hotkey[i] = curChar;
		nextChar(f);
	}
	hotkey[HotkeySize-1] = 0;
	printf("%d, %d: Hotkey too long\n", lineIndex, caret);
	return false;
}

bool MenuConf::readQuotedString(FILE* f, char* string, int StringSize)
{
	if(curChar == '"' || curChar == '\''){
		int closingChar = curChar;
		nextChar(f);
		for(int i=0; i<StringSize; i++){
			if(curChar == '\n' || curChar == EOF){
				printf("%d, %d: Unexpected %s in string\n", lineIndex, caret, curChar=='\n'?"newline":"EOF");
				string[i] = 0;
				return false;
			}
			if(curChar == closingChar){
				string[i] = 0;
				nextChar(f);
				return true;
			}
			string[i] = curChar;
			nextChar(f);
		}
		string[StringSize-1] = 0;
		printf("%d, %d: String too long\n", lineIndex, caret);
		return false;
	}else{
		return readToken(f, string, StringSize);
	}
}

void MenuConf::eatWhitespace(FILE* f)
{
	while(curChar == ' ' || curChar == '\t') nextChar(f);
}

void MenuConf::addHotkey(ItemData* data, Hotkey* hotkey)
{
	if(data->hotkeys == NULL){
		data->hotkeys = hotkey;
	}else{
		addSibling(data->hotkeys, hotkey);
	}
}

bool MenuConf::readItemNameIfPresent(FILE* f, Item* item)
{
	eatWhitespace(f);
	if(! isAlpha(curChar)){
		return true;//Name not present
	}
	if(! readToken(f, item->data->name, ItemData::NameSize)){
		return false;
	}
	eatWhitespace(f);
	if(curChar != ':'){
		printf("%d, %d: Expected ':'\n", lineIndex, caret);
		return false;
	}
	nextChar(f);
	return true;
}

bool MenuConf::readItemCaption(FILE* f, Item* item)
{
	eatWhitespace(f);
	return readQuotedString(f, item->data->caption, ItemData::CaptionSize);
}
	
bool MenuConf::readItemHotkeysIfPresent(FILE* f, Item* item)
{
	eatWhitespace(f);
	if(curChar == ','){
		nextChar(f);
		eatWhitespace(f);
		if(curChar != '['){
			printf("%d, %d: Expected '['\n", lineIndex, caret);
			return false;
		}else{
			nextChar(f);
			while(true){
				eatWhitespace(f);
				if(curChar == ']'){
					nextChar(f);
					return false;
				}
				if(curChar == '\n' || curChar == EOF){
					printf("%d, %d: Expected hotkey or ']'\n", lineIndex, caret);
					return false;
				}
				if(item->data->hotkeys){
					if(curChar != ','){
						printf("%d, %d: Expected ','\n", lineIndex, caret);
						return false;
					}
					nextChar(f);
				}
				eatWhitespace(f);
				Hotkey* hotkey = new Hotkey();
				if(readHotkey(f, hotkey->string, Hotkey::StringSize)){
					addHotkey(item->data, hotkey);
				}
			}
		}
	}
	return true;
}
