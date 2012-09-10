#pragma once

#include <string.h>

template<int BufferSize>
struct PathString
{
	int curPos;
	char buffer[BufferSize];
	PathString()
		: curPos(0)
	{
		buffer[0] = 0;
	}
	void pushChar(char c)
	{
		if(curPos < BufferSize-1){
			buffer[curPos++] = c;
			buffer[curPos] = 0;
		}
	}
	void pushId(const char* id)
	{
		if(curPos < BufferSize-1){
			buffer[curPos++] = '.';
		}
		while(curPos < BufferSize-1 && *id){
			buffer[curPos++] = *id++;
		}
		buffer[curPos] = 0;
	}
	void pushIndex(int i)
	{
		if(curPos < BufferSize-1){
			buffer[curPos++] = '[';
		}
		if(curPos < BufferSize-1){
			curPos += snprintf(&buffer[curPos], BufferSize-curPos, "%d", i);
		}
		if(curPos < BufferSize-1){
			buffer[curPos++] = ']';
		}
		buffer[curPos] = 0;
	}
	void pop()
	{
		while(curPos > 0){
			--curPos;
			if(buffer[curPos] == '[' || buffer[curPos] == '.'){
				break;
			}
		}
		buffer[curPos] = 0;
	}
};
