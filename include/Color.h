#pragma once


struct Color
{
	union{
		struct{
			unsigned char B, G, R, A;
		};
		int value;
	};
	Color() {}
	Color(int value) {this->value = value;}
	Color(int r, int g, int b) {R = r, G = g, B = b;}
    Color(int r, int g, int b, int a) {R = r, G = g, B = b, A = a;}
	void set(int r, int g, int b) {R = r, G = g, B = b;}
	void set(int value) {R = value, G = value, B = value;}
	void inc(int r, int g, int b){
		int S = r + R;
		int T = g + G;
		int U = b + B;
		R = (S < 255) ? S : 255;
		G = (T < 255) ? T : 255;
		B = (U < 255) ? U : 255;
	}
    Color operator * (const unsigned int f) const
    {
        return Color(R * f / 255, G * f / 255, B * f / 255);
    }
};


Color average(Color a, Color b, Color c, Color d);

static inline Color lerp(Color a, Color b, int t)
{
    return Color(
        (int(a.R) * (255 - t) + int(b.R) * t) / 256,
        (int(a.G) * (255 - t) + int(b.G) * t) / 256,
        (int(a.B) * (255 - t) + int(b.B) * t) / 256);
}
