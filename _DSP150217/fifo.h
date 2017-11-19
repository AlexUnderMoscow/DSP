#pragma once

#include "struct.h"
#include <mutex>
//#include <ctime>
class fifo
{
public:
	fifo(unsigned int size);
	~fifo(void);
	unsigned int write(char* buf, unsigned int len);	//записать в буфер
	unsigned int read(char* buf, unsigned int len);		//читать из буфера
	unsigned int getBufSize();				//вернуть размер буфера
	unsigned int getDataSize();				//вернуть размер данных в буфере
	unsigned char load();					//загрузка в процентах
	void deleteBuf();
	void setData();
	void Lock();
	bool halt;
private:
	unsigned int bufSize;							// размер буфера вообще
	unsigned int dataSize;							 //количество данных
	char * buf;								// указатель на буфер
	unsigned int readIndex;							//индекс на чтение
	unsigned int writeIndex;						//индекс на запись
	std::mutex critical;
	int loadBuf;
};
