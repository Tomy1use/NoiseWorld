#pragma once


namespace f2s
{
	//!!! Sync this value with PowerOfTwoStrings.h
	const unsigned int RowLen = 189;
	
	struct StringFromFloat
	{
		char s[RowLen+2];
		char* value;
		operator const char*() const {return value;}
		StringFromFloat(): value(s) {s[0] = 0;}
		StringFromFloat(float f): value(s) {operator()(f);}
		const char* operator() (float f);
	};
	
	struct FloatFromString
	{
		unsigned int value;
		char s[RowLen+1];
		operator float() const {return *(float*)&value;}
		FloatFromString(): value(0xffFFffFF) {}
		FloatFromString(const char* in): value(0xffFFffFF) {operator()(in);}
		float operator() (const char* in);
	};
}
