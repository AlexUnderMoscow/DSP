#include "fifo.h"

fifo::fifo(unsigned int size)
{
    buf = std::shared_ptr<std::vector<char>>(new std::vector<char>);    //buf = std::make_shared<char*>(new char[size]);
    buf->resize(size);
    buffer = buf->data();                                                      //buffer = (void*)&((*buf)[0]);
	bufSize = size;
	readIndex = 0;
	writeIndex = 0;
	dataSize = 0;
	loadBuf = 0;
	halt=false;
}

fifo::~fifo(void)
{

}
void fifo::setData()
{
	dataSize = 0;
	readIndex = 0;
	writeIndex = 0;
	loadBuf = 0;
}


unsigned int fifo::write(char* _buf, unsigned int len)
{
	critical.lock();
	if (len>bufSize) 
	{
		critical.unlock();
		return 0;
	}
	if (dataSize+len>=(bufSize))
	{
		critical.unlock();
		return 0;  //не пишем ничего если перегрузка
	}

	if ((writeIndex+len) <= bufSize) // обычная запись
	{
		memcpy(buffer+writeIndex,_buf,len);
		writeIndex+=len;
		dataSize+=len;
	}
	else  //запись через размер циклического буфера
	{
		int tmp = len-bufSize+writeIndex;
		memcpy(buffer+writeIndex,_buf,bufSize - writeIndex);
		memcpy(buffer,_buf+bufSize - writeIndex,tmp);
		writeIndex = tmp;
		dataSize+=len;
	}	
	critical.unlock();
	return len;
}
unsigned int fifo::read(char* _buf, unsigned int len)
{
	critical.lock();
	if (dataSize == 0) 
	{
		critical.unlock();
		return 0;
	}
	if (len>=dataSize) 
	{
		critical.unlock();
		return 0;
	}

	if ((readIndex+len)<=bufSize) /*обычное считываение*/
	{
		memcpy(_buf,buffer+readIndex,len);
		readIndex+=len;
		dataSize-=len;
	}
	else /*считываение через границу циклического буфера*/
	{
		int tmp = len-bufSize+readIndex;
		memcpy(_buf,buffer+readIndex,bufSize - readIndex);
		memcpy(_buf+bufSize-readIndex,buffer,tmp);
		readIndex=tmp;
		dataSize-=len;
	}
	critical.unlock();
	return len;
}
void fifo::Lock()
{
  critical.lock();
  this->dataSize=0;
  this->readIndex=0;
  this->writeIndex=0;
  halt=true;
  critical.unlock();
}

unsigned int fifo::getBufSize()
{
	return bufSize;
}
unsigned int fifo::getDataSize()
{
	return dataSize;
}
unsigned char fifo::load()     //загрузка в процентах
{
	float a = (float)dataSize;
	a  = a / bufSize;
	a = (a*100);
	return (unsigned char)a;	
}
