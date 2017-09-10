// operations.h : header file

#ifndef OPERATIONS_H
#define OPERATIONS_H

class COperations  
{
public:
	COperations();
	~COperations();
	//unsigned char FastRazvorot(unsigned char a);//быстрый разворот байт
	//short FastRazvorot(short a);//быстрый разворот байт
	//unsigned int Razvorot(unsigned int a);
	unsigned char Invert (unsigned char b);//быстрая инверсия байта
	//unsigned int getBits (unsigned char *buf, int byte_offset, int startbit, int bitlen);//возвращает биты из буфера с некоторым смещением
	unsigned int Invert (unsigned int b);
	unsigned int Bytes3File4Shift(unsigned char* buff,int byteofst,int bitoft);//сдвигает биты в 4 байтах на несколько позиций
	unsigned char Invers[256];
	unsigned char FastRwt[256];
	unsigned char PlusCounter[256];
protected:	
};
#endif
