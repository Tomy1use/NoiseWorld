

struct Keyboard
{
	union{
		char keys[256];
		struct
		{
			char
			x00, escape, num1, num2, num3, num4, num5, num6, num7, num8, num9, num0, minus, equals, backSpace, tab,
			q, w, e, r, t, y, u, i, o, p, leftBracket, rightBracket, Return, leftControl, a, s,
			d, f, g, h, j, k, l, semicolon, apostrophe, grave, leftShift, backSlash, z, x, c, v,
			b, n, m, comma, period, slash, rightShift, multiply, leftAlt, space, capsLock, f1, f2, f3, f4, f5,
			f6, f7, f8, f9, f10, numLock, scrollLock, numPad7, numPad8, numPad9, subtract, numPad4, numPad5, numPad6, add, numPad1,
			numPad2, numPad3, numPad0, decimal, x54, x55, x56, f11, f12, f13, f14, f15, x5c, x5d, x5e, x5f,
			x60, x61, x62, x63, x64, x65, x66, x67, x68, x69, x6a, x6b, x6c, x6d, x6e, x6f,
			x70, x71, x72, x73, x74, x75, x76, x77, x78, x79, x7a, x7b, x7c, x7d, x7e, x7f,
			x80, x81, x82, x83, x84, x85, x86, x87, x88, x89, x8a, x8b, x8c, x8d, x8e, x8f,
			x90, x91, x92, x93, x94, x95, x96, x97, x98, x99, x9a, x9b, numPadEnter, rightControl, x9e, x9f,
			xa0, xa1, xa2, xa3, xa4, xa5, xa6, xa7, xa8, xa9, xaa, xab, xac, xad, xae, xaf,
			xb0, xb1, xb2, xb3, xb4, numPadDivide, xb6, xb7, rightAlt, xb9, xba, xbb, xbc, xbd, xbe, xbf,
			xc0, xc1, xc2, xc3, xc4, pause, xc6, home,  up, pageUp, xca, left, xcc, right, xce, end,
			down, pageDown, insert, Delete, xd4, xd5, xd6, xd7, xd8, xd9, xda, xdb, xdc, xdd, xde, xdf,
			xe0, xe1, xe2, xe3, xe4, xe5, xe6, xe7, xe8, xe9, xea, xeb, xec, ed, xee, xef,
			xf0, xf1, xf2, xf3, xf4, xf5, xf6, xf7, xf8, xf9, xfa, xfb, xfc, xfd, xfe, xff;
		};
	};
};


struct Keys
{
	static Keyboard down, wasDown, pressed, released;
	static bool anyKeyDown, anyKeyPressed;
	static void process();
};


struct Mouse
{
	static int x, y, movX, movY;
	static bool rightDown, rightWasDown, rightPressed, rightReleased;
	static bool leftDown, leftWasDown, leftPressed, leftReleased;
	static void process();
};

