#pragma once

#include <FloatToString/FloatToString.h>

/*
	Todo:
	
	Write class TokenStream which can read/write objects, arrays, strings and numbers.
	Move vector/matrix/etc. specifics to separate functions/classes.
*/

struct DataFile
{
	FILE* f;
	bool ok;
	f2s::StringFromFloat sff;
	int tabs;
	bool needTabs;
	bool needComma;
	bool needSpace;
	
	DataFile(FILE* f): f(f), ok(true), tabs(0), needTabs(false), needComma(false), needSpace(false) {}
	~DataFile() {if(f) fclose(f);}
	operator FILE* () {return f;}
	template<typename Foo> 
	DataFile& operator << (const Foo& foo)
	{
		ok &= (fwrite(&foo, sizeof(Foo), 1, f) == 1);
		return *this;
	}
	DataFile& operator << (char c)
	{
		if(c == '\n') {needComma = false; needSpace = false;}
		if(c == '}' || c == ']') {tabs--; needComma = false; needSpace = false;}
		if(needComma) {ok &= !!fputc(',', f); needComma = false;}
		if(needSpace) {ok &= !!fputc(' ', f); needSpace = false;}
		if(needTabs) for(int i=0; i<tabs; i++) ok &= ('\t' == fputc('\t', f));
		if(c == '{' || c == '[') tabs++;
		ok &= (c == fputc(c, f));
		needTabs = (c == '\n');
		return *this;
	}
	DataFile& operator << (const char* s)
	{
		for(const char* c=s; *c; c++) *this << *c;
		needSpace = true;
		return *this;
	}
	DataFile& operator << (float a)
	{
		*this << (const char*)sff(a);
		needComma = true;
		needSpace = true;
		return *this;
	}
	DataFile& operator << (const Vector& v)
	{
		*this << '[' << v.x << v.y << v.z << ']';
		needComma  = true;
		needSpace = true;
		return *this;
	}
	DataFile& operator << (const Axes& a)
	{
		*this << '[' << a.x << a.y << a.z << ']';
		return *this;
	}
	DataFile& operator << (const Matrix& m)
	{
		*this << "{\n" << "axes" << m.axes << "\norigin" << m.origin << "\n}";
		return *this;
	}
};
