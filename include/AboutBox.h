#pragma once

struct AboutBox
{
	static const unsigned int MaxText = 300;
	char text[MaxText];
	AboutBox(const char* aboutTxt)
	{
		text[0] = 0;
		FILE* f = fopen(aboutTxt, "rb");
		if(! f) {printf("AboutBox: failed to open %s\n", aboutTxt); return;}
		int bytesRead = fread(text, 1, MaxText-1, f);
		text[bytesRead] = 0;
		fclose(f);
	}
	void display(HWND window)
	{
		MessageBox(window, text, "About Shapecraft", MB_OK);
	}
};
