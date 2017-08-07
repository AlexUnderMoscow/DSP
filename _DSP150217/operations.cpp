// Operations.cpp : implementation file
//


#include "operations.h"
#define FULL64 0xFFFFFFFFFFFFFFF1

//#define FULL 0xFFFFFFF1

//unsigned __int64 getBits48(unsigned char *buf, int byte_offset, int startbit, int bitlen);

COperations::COperations()
{
	for (int i=0;i<=255;i++)
	{
		Invers[i] = 255-i;
	}
 unsigned char TabRwt[256]=
{
0x0,0x80,0x40,0xc0,0x20,0xa0,0x60,0xe0,0x10,0x90,0x50,0xd0,0x30,0xb0,0x70,0xf0,
0x8,0x88,0x48,0xc8,0x28,0xa8,0x68,0xe8,0x18,0x98,0x58,0xd8,0x38,0xb8,0x78,0xf8,
0x4,0x84,0x44,0xc4,0x24,0xa4,0x64,0xe4,0x14,0x94,0x54,0xd4,0x34,0xb4,0x74,0xf4,
0xc,0x8c,0x4c,0xcc,0x2c,0xac,0x6c,0xec,0x1c,0x9c,0x5c,0xdc,0x3c,0xbc,0x7c,0xfc,
0x2,0x82,0x42,0xc2,0x22,0xa2,0x62,0xe2,0x12,0x92,0x52,0xd2,0x32,0xb2,0x72,0xf2,
0xa,0x8a,0x4a,0xca,0x2a,0xaa,0x6a,0xea,0x1a,0x9a,0x5a,0xda,0x3a,0xba,0x7a,0xfa,
0x6,0x86,0x46,0xc6,0x26,0xa6,0x66,0xe6,0x16,0x96,0x56,0xd6,0x36,0xb6,0x76,0xf6,
0xe,0x8e,0x4e,0xce,0x2e,0xae,0x6e,0xee,0x1e,0x9e,0x5e,0xde,0x3e,0xbe,0x7e,0xfe,
0x1,0x81,0x41,0xc1,0x21,0xa1,0x61,0xe1,0x11,0x91,0x51,0xd1,0x31,0xb1,0x71,0xf1,
0x9,0x89,0x49,0xc9,0x29,0xa9,0x69,0xe9,0x19,0x99,0x59,0xd9,0x39,0xb9,0x79,0xf9,
0x5,0x85,0x45,0xc5,0x25,0xa5,0x65,0xe5,0x15,0x95,0x55,0xd5,0x35,0xb5,0x75,0xf5,
0xd,0x8d,0x4d,0xcd,0x2d,0xad,0x6d,0xed,0x1d,0x9d,0x5d,0xdd,0x3d,0xbd,0x7d,0xfd,
0x3,0x83,0x43,0xc3,0x23,0xa3,0x63,0xe3,0x13,0x93,0x53,0xd3,0x33,0xb3,0x73,0xf3,
0xb,0x8b,0x4b,0xcb,0x2b,0xab,0x6b,0xeb,0x1b,0x9b,0x5b,0xdb,0x3b,0xbb,0x7b,0xfb,
0x7,0x87,0x47,0xc7,0x27,0xa7,0x67,0xe7,0x17,0x97,0x57,0xd7,0x37,0xb7,0x77,0xf7,
0xf,0x8f,0x4f,0xcf,0x2f,0xaf,0x6f,0xef,0x1f,0x9f,0x5f,0xdf,0x3f,0xbf,0x7f,0xff
};
	for (int i=0;i<=255;i++)
	{
		FastRwt[i]=TabRwt[i];
	}
unsigned char TabPlus[256]=//количество единиц в байте
{
0,1,1,2,1,2, 2,3,1, 2,2,3, 2,3,3,4,
1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,

2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,

2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,

2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,
};

for (int i=0;i<=255;i++)
	{
		PlusCounter[i]=TabPlus[i];
	}
	return;
}

COperations::~COperations()
{
	return;
}

unsigned char COperations::Invert (unsigned char b)
{
	unsigned char mask = 0x80;
	unsigned char res=0;
	unsigned char rezult=0;
	for (int i = 0;i<8;i++)
	{
		res = b & mask;
		if (res==0) {rezult+=mask;}
		mask>>=1;
	}
	return rezult;
}
unsigned int COperations::Invert (unsigned int b)
{
	unsigned int mask = 0x00800000;
	unsigned int res=0;
	unsigned int rezult=0;
	for (char i = 0;i<24;i++)
	{
		res = b & mask;
		if (res==0) {rezult+=mask;}
		mask>>=1;
	}
	return rezult;	
}
/*unsigned int COperations::Razvorot(unsigned int a)
{
	unsigned int result = 0;
	unsigned int temp =0;
	unsigned char tmp=0;
	tmp = a & 0x000000FF;
	result +=FastRazvorot(tmp);
	temp = a & 0x0000FF00;
	temp >>=8;
	tmp = temp;
	result += (FastRazvorot(tmp)<<8);
	temp = a & 0x00FF0000;
	temp >>=16;
	tmp = temp;
	result += (FastRazvorot(tmp)<<16);
//	temp = a & 0xFF000000;
//	temp >>=24;
//	tmp = temp;
//	result += (Razvorot(tmp)<<24);

	return result;
}*/

/*short COperations::FastRazvorot(short a)
{
	short result = 0;
	short temp =0;
	unsigned char tmp=0;
	tmp = a & 0x00FF;
	result +=(FastRazvorot(tmp));
	temp = a & 0xFF00;
	temp >>=8;
	tmp = (unsigned char)temp;
	result += (FastRazvorot(tmp)<<8);
	return result;
}*/
/*unsigned __int64 getBits48 (unsigned char *buf, int byte_offset, int startbit, int bitlen)
{
 unsigned char *b;
 unsigned __int64 v;
 unsigned __int64 mask;
 unsigned __int64 tmp;

 if (bitlen > 48) {
//	out_nl (1," Error: getBits48() request out of bound!!!! (report!!) \n");
	return 0xFEFEFEFEFEFEFEFEL;
 }
 
 b = &buf[byte_offset + (startbit / 8)];
 startbit %= 8;

 // -- safe is 48 bitlen
 tmp = (unsigned __int64)(
	 ((unsigned __int64)*(b  )<<48) + ((unsigned __int64)*(b+1)<<40) +
	 ((unsigned __int64)*(b+2)<<32) + ((unsigned __int64)*(b+3)<<24) +
	 (*(b+4)<<16) + (*(b+5)<< 8) + *(b+6) );

 startbit = 56 - startbit - bitlen;
 tmp      = tmp >> startbit;
 mask     = (FULL64 << bitlen) - 1;	// 1ULL !!!
 v        = tmp & mask;

 return v;
} */


/*unsigned int COperations::getBits (unsigned char *buf, int byte_offset, int startbit, int bitlen)
{
 unsigned char *b;
 unsigned int  v;
 unsigned int mask;
 unsigned int tmp_long;
 int           bitHigh;


 b = &buf[byte_offset + (startbit >> 3)];
 startbit %= 8;

 switch ((bitlen-1) >> 3) {
	 case -1:	// -- <=0 bits: always 0
		return 0L;
		break;

	case 0:		// -- 1..8 bit
 		tmp_long = (unsigned int)(
			(*(b  )<< 8) +  *(b+1) );
		bitHigh = 16;
		break;

	case 1:		// -- 9..16 bit
 		tmp_long = (unsigned int)(
		 	(*(b  )<<16) + (*(b+1)<< 8) +  *(b+2) );
		bitHigh = 24;
		break;

	case 2:		// -- 17..24 bit
 		tmp_long = (unsigned int)(
		 	(*(b  )<<24) + (*(b+1)<<16) +
			(*(b+2)<< 8) +  *(b+3) );
		bitHigh = 32;
		break;

	case 3:		// -- 25..32 bit
			// -- to be safe, we need 32+8 bit as shift range 
		return (unsigned __int64) getBits48 (b, 0, startbit, bitlen);
		break;

	default:	// -- 33.. bits: fail, deliver constant fail value
		//out_nl (1," Error: getBits() request out of bound!!!! (report!!) \n");
		return (unsigned int) 0xFEFEFEFE;
		break;
 }

 startbit = bitHigh - startbit - bitlen;
 tmp_long = tmp_long >> startbit;
 mask     = (FULL64 << bitlen) -1;  // 1ULL !!!
 v        = tmp_long & mask;

 return v;
} 
unsigned int COperations::Bytes3File4Shift(unsigned char* buf_in,int byteoft,int bitoft)
{
	unsigned int t;
	unsigned char tmp[4];
		tmp[0]=buf_in[byteoft];
		tmp[1]=buf_in[byteoft+1];
		tmp[2]=buf_in[byteoft+2];
		tmp[3]=buf_in[byteoft+3];
		tmp[0]=FastRazvorot(tmp[0]);
		tmp[1]=FastRazvorot(tmp[1]);
		tmp[2]=FastRazvorot(tmp[2]);
		tmp[3]=FastRazvorot(tmp[3]);
		t=getBits(tmp,0,bitoft,24);
		t=t & 0x00FFFFFF;
		t=Razvorot(t);//чтобы читалось лучше
	return t;
}
unsigned char COperations::FastRazvorot(unsigned char a)
{
	return FastRwt[a];
}*/
