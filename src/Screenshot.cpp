#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <WriteGIF.h>
#include <DateTime.h>

void flipImageVertically(unsigned char* rgbImage, int width, int height)
{
	unsigned char* line = new unsigned char[width * 3];
	for(int y=0; y<height/2; y++){
		unsigned char* line1 = rgbImage + 3 * width * y;
		unsigned char* line2 = rgbImage + 3 * width * (height - 1 - y);
		memcpy(line, line1, 3*width);
		memcpy(line1, line2, 3*width);
		memcpy(line2, line, 3*width);
	}
	delete[] line;
}

void makeScreenshot(HWND window, const char* fileName)
{
	if(! fileName){
		DateTime curDateTime;
		char dateTimeText[MAX_PATH];
		sprintf_s(dateTimeText, MAX_PATH, "%s.gif", curDateTime.longText);
		fileName = dateTimeText;
	}
	gif::GIF* g = gif::newGIF(0);
	RECT rect;
	GetClientRect(window, &rect);
	int w = rect.right - rect.left, h = rect.bottom - rect.top;
	unsigned char* rgb = new unsigned char[w * h * 3];
	glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, rgb);
	flipImageVertically(rgb, w, h);
	gif::addFrame(g, w, h, rgb, 0);
	delete[] rgb;
	gif::write(g, fileName);
	gif::dispose(g);
}
