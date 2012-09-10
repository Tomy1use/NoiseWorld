#pragma once



struct RandomNumber
{
    unsigned number;
    RandomNumber(unsigned seed=0): number(seed) {}
	RandomNumber(const RandomNumber& copy): number(copy.number) {}
    void advance()
    {
        const unsigned a = 1103515245;
        const unsigned c = 12345;
        number = a * number + c;
    }
    operator int() const
    {
        return (number >> 16);
    }
    operator float() const
    {
        return float(number >> 16) / float(0xffFF);
    }
    float operator() (float a, float b) const
    {
        return a + (b - a) * float(number >> 16) / float(0xffFF);
    }
    int operator() (int a, int b) const
    {
        if(a < b) return a + (b - a) * (number >> 16) / 0xffFF;
        return a;
    }
	RandomNumber& operator ++ (int _)
	{
		advance();
		return *this;
	}
	RandomNumber operator ++ ()
	{
		RandomNumber copy(*this);
		advance();
		return copy;
	}
};
