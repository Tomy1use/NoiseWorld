#pragma once

template<unsigned int Base, unsigned int Digits>
struct VanDerCorputSequence
{
    unsigned int digits[Digits];
    VanDerCorputSequence()
    {
        digits[0] = 1;
        for(unsigned int i=1; i<Digits; i++){
            digits[i] = 0;
        }
    }
	VanDerCorputSequence(const VanDerCorputSequence& c)
	{
		for(unsigned int i=0; i<Digits; i++){
			digits[i] = c.digits[i];
		}
	}
    operator float() const
    {
        float result = 0;
        unsigned int divisor = Base;
        for(unsigned int i=0; i<Digits; i++){
            result += float(digits[i]) / float(divisor);
            divisor *= Base;
        }
        return result;
    }
    unsigned int multiply(unsigned int factor) const
    {
        unsigned int result = 0;
        unsigned int divisor = Base;
        for(unsigned int i=0; i<Digits; i++){
            result += factor * digits[i] / divisor;
            divisor *= Base;
        }
        return result;
    }
    void advance()
    {
        for(unsigned int i=0; i<Digits; i++){
            digits[i] ++;
            if(digits[i] < Base){
                break;
            }
            digits[i] = 0;
        }
    }
	VanDerCorputSequence& operator ++ ()
	{
		advance();
		return *this;
	}
	VanDerCorputSequence operator ++ (int _)
	{
		VanDerCorputSequence copy(*this);
		advance();
		return copy;
	}
    unsigned int capacity()
	{
		int r = Base;
		for(unsigned i=1; i<Digits; i++){
			r *= Base;
		}
		return r;
	}
};

