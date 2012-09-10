#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <FloatToString/FloatToString.h>

namespace f2s
{

const char* const PowerOfTwoStrings[] = 
{
#include "PowerOfTwoStrings.h"
};

//!!! Sync these values with PowerOfTwoStrings.h
const unsigned int NumberOneRow = 128;
const unsigned int DotPosInRow = 39;

template<bool> struct CompileTimeCheck;
template<> struct CompileTimeCheck<true> {};
#define StaticCheck(expr) (CompileTimeCheck<(expr) != 0>())

//IEEE 754-1985 single precision floating-point number

//TODO: hide these constants in class SinglePrecisionFloatConstants
const int SignBit = 0x80000000L;
const int FracBitCount = 23;
const int FracMask = 0x7fffff;
const int HiddenBit = 0x800000;
const int ExpMask = 0xff;//after bit shift
const int ExpBias = 127;


//Divide decimal number, represented by string, by 2
int strDivBy2(char* str, int StrSize)
{
	int digit = 0;
	for(int i=0; i<StrSize; i++){
		if(str[i] == '.') continue;
		digit = digit * 10 + str[i] - '0';
		str[i] = digit / 2 + '0';
		digit = digit % 2;
	}
	return digit;
}

int strMulBy2(char* str, int StrSize)
{
	int n = 0;
	for(int i=StrSize-1; i>=0; i--){
		if(str[i] == '.') continue;
		n += (str[i] - '0') * 2;
		str[i] = n % 10 + '0';
		n /= 10;
	}
	return n;
}

int stringToInt(char* in)
{
	int res = 0;
	int size = strlen(in);
	static char s[250];
	strcpy(s, in);
	printf("%d: %s\n", 0, s);
	for(int i=0; i<32; i++){
		int digit = strDivBy2(s, size);
		printf("%d: %s\n", i+1, s);
		if(digit) res |= (1 << i);
	}
	for(int k=0; k<32; k++) if((res & (0x80000000>>k)) != 0) printf("1"); else printf("0"); printf("\n");
	return res;
}

int fracToInt(char* in)
{
	int res = 0;
	int size = strlen(in);
	static char s[250];
	strcpy(s, in);
	printf("%d: %s\n", 0, s);
	for(int i=0; i<50; i++){
		int digit = strMulBy2(s, size);
		//printf("%d: %s\n", i+1, s);
		//if(digit) res |= (1 << i);
		printf(digit ? "1" : "0");
	}
	printf("\n");
	//for(int k=0; k<32; k++) if((res & (0x80000000>>k)) != 0) printf("1"); else printf("0"); printf("\n");
	return res;
}

char* stripZeros(const char* const in, int InSize, char* out, int OutSize)
{
	int inBeg = 0;
	while(inBeg < InSize-1 && in[inBeg]=='0' && in[inBeg+1]!='.') inBeg++;
	printf("inBeg %d\n", inBeg);
	if(inBeg >= InSize){
		out[0] = 0;
		return out;
	}
	int inEnd = InSize-1;
	while(inEnd > 0 && (in[inEnd-1]=='0' || in[inEnd-1]=='.')){
		if(in[inEnd] == '.') break;
		inEnd --;
	}
	printf("inEnd %d\n", inEnd);
	int count = inEnd - inBeg;
	printf("count %d\n", count);
	if(count > OutSize-1) count = OutSize-1;
	for(int i=0; i<count; i++) out[i] = in[inBeg+i];
	out[count] = 0;
	return out;
}

char* stripZeros(char* buf, int BufSize)
{
	return stripZeros(buf, BufSize, buf, BufSize);
}

void sumRows(const char* const Rows[], int RowCount, int RowLen, int Bits, char* out)
{
	int n = 0;
	out[RowLen] = 0;
	//for(int j=0; j<RowCount; j++) if(Bits & (1 << j)) printf("%d: %s\n", j, Rows[RowCount-1-j]);
	//TODO:
	//Possible optimization
	//The number of rows traversed and digits added can be significantly reduced
	//for(int j=0; j<RowCount; j++) if(Bits & (1 << j)) printf("+ (%d) %s\n", j, Rows[RowCount-1-j]);
	for(int i=RowLen-1; i>=0; i--){
		if(Rows[0][i] == '.'){
			out[i] = '.';
			continue;
		}
		for(int j=0; j<RowCount; j++){
			if(Bits & (1 << j)){
				n += Rows[RowCount-1-j][i] - '0';
			}
		}
		out[i] = '0' + (n % 10);
		n /= 10;
	}
}

void subRow(char* str, const char* row, int RowLen)
{
	int borrow = 0;
	for(int i=RowLen-1; i>=0; i--){
		if(str[i] == '.') continue;
		int a = str[i] - '0' - borrow;
		int b = row[i] - '0';
		if(a < b){
			borrow += 10;
			a += 10;
		}
		str[i] = a - b + '0';
		borrow /= 10;
	}
}


const char* StringFromFloat::operator() (float f)
{
	value = s + 1;
	s[RowLen+1] = 0;
	StaticCheck(sizeof(int) == sizeof(float));
	unsigned int u = *(unsigned int*)&f;
	int biasExp = ((u >> FracBitCount) & ExpMask);
	int frac = u & FracMask;
	//todo: zero case
	if(biasExp == 0 && frac == 0){
		strcpy(value, "0");
	}else if(biasExp == 0xff && frac == 0){
		strcpy(value, "Inf");
	}else if(biasExp == 0xff && frac != 0){
		strcpy(value, "NaN");
	}else{
		int hiddenBit = biasExp ? HiddenBit : 0;
		int exp = biasExp - ExpBias + (biasExp ? 0 : 1);
		//printf("exponent %d\n", exp);
		int rowBits = frac | hiddenBit;
		//printf("first row %d\n", NumberOneRow - exp);
		sumRows(&PowerOfTwoStrings[NumberOneRow-exp], FracBitCount+1, RowLen, rowBits, s+1);
		//strip zeros
		//printf("unstripped: %s\n", value);
		{
			for(unsigned int i=RowLen-1; i>=0; i--){
				if(value[i] == '.') {value[i] = 0; break;}
				if(value[i] != '0') {value[i+1] = 0; break;}
			}
			for(unsigned int i=0; i<RowLen; i++){
				if(value[i] == 0) break;
				if(value[i] == '.') {value += i - 1; break;}
				if(value[i] != '0') {value += i; break;}
			}
		}
	}
	bool negative = (u & SignBit) != 0;
	if(negative) *(--value) = '-';
	return value;
}


bool rowIsGreater(const char* rowA, const char* rowB, int RowLen)
{
	for(int i=0; i<RowLen; i++){
		//printf("%d, ", i);
		if(rowA[i] == rowB[i]) continue;
		return (rowA[i] > rowB[i]);
	}
	return false;
}


float FloatFromString::operator() (const char* in)
{
	value = 0xffFFffFF;
	
	int inLen = 0;
	int inDotPos = 0;
	
	unsigned int signBit = (in[0] == '-') ? SignBit : 0;
	
	//Check validity and calc length
	{
		bool isZero = true;
		if(in[0] == '-' || in[0] == '+') in++;
		if(0 == _strnicmp(in, "inf", 3)){
			value = signBit | 0x7F800000;
			return value;
		}
		while(in[inLen]){
			if(in[inLen] == '.'){
				if(inDotPos == 0){
					inDotPos = inLen;
				}else{//ERROR: not a number
					printf("Error: Not a number\n");
					value = signBit | 0x7fFFffFF;
					return value;
				}
			}else if(in[inLen] < '0' || in[inLen] > '9'){
				printf("Error: Not a number\n");
				value = signBit | 0x7fFFffFF;
				return value;
			}
			isZero &= (in[inLen] == '0');
			inLen++;
		}
		if(isZero){
			value = signBit;
			return value;
		}
		if(inDotPos == 0) inDotPos = inLen;//string does not contain a dot
	}
	
	//Align length and decimal point position with rows
	{
		for(unsigned int i=0; i<RowLen; i++){
			int j = i + inDotPos - DotPosInRow;
			s[i] = (j>=0 && j<inLen) ? in[j] : '0';
		}
		s[DotPosInRow] = '.';
		s[RowLen] = 0;
	}
	
	//printf("%s\n", s);
	
	unsigned int frac = 0;
	int bit = 23;
	
	const char* const* Rows = PowerOfTwoStrings;
	const unsigned int RowCount = sizeof(PowerOfTwoStrings) / sizeof(PowerOfTwoStrings[0]);
	unsigned int row = 0;
	unsigned int digit = 0;
	//столько раз, сколько значащих битов (24 в single-precision)
	while(row < RowCount && rowIsGreater(Rows[row], s, RowLen)){
		//printf("row %d failed\n", row);
		row++;
	}
	if(row >= RowCount){
		printf("Error: row >= RowCount\n");
		return value;//ERROR: number too small for single precision
	}
	//printf("first row %d\n", row);
	int exp = NumberOneRow - row;
	//printf("exponent %d\n", exp);
	
	while(bit >= 0){
		//printf("%d\n", bit);
		//2. находим в таблице наибольшее число, меньшее или равное нашему числу
		//2.1 находим старший значащий разряд в нашем числе
		while(digit < RowLen && (s[digit] == '0' || digit == '.')) digit++;
		if(digit >= RowLen) break;
		//2.2 идем по таблице вниз, пока не находим в том же разряде число, <= нашему разряду
		while(bit >= 0 && row < RowCount && rowIsGreater(Rows[row], s, RowLen)){
			bit --;
			row++;
		}
		if(row >= RowCount) break;
		if(bit < 0) break;
		//printf("row %d\n", row);
		//3. включаем бит, соответствующий найденному числу
		frac |= (1 << bit);
		//4. вычитаем из нашего числа найденное
		//printf("- (%d) %s\n", bit, Rows[row]);
		subRow(s, PowerOfTwoStrings[row], RowLen);
		//printf("= %s\n", s);
	}
	if(exp < -126){//this will be denormalized number
		frac >>= (-126 - exp);
		exp = -ExpBias;
		//printf("new exponent %d\n", exp);
	}
	value = signBit | ((exp + ExpBias) << FracBitCount) | (frac & FracMask);
	return value;
}

}//s2f

