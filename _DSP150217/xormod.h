#ifndef XORMOD_H
#define XORMOD_H

//#include <QDialog>
#include <QVariant>
#include "interface.h"
#include "ui_xormod.h"
#define dataXorRead 1024


class xorWork
{
public:
	double errCount;
	double totalCount;
	double Perr;
	Interface *inf;
	char buf[tmpBufSize];
    xorWork(void* _inf, Ui::xormod *_ui)
	{
		inf = (Interface*)_inf;
		inf->ready = true;
		ui = _ui;
		overflow = false;
		errCount=0;
		totalCount=0;
		Perr = 0;
		sync1 = false;
		sync2 = false;
		FNF=false;
		Corr = false;
		offset1 = 0;
		offset2=0;
	}

	~xorWork()
	{
		//	inf = NULL;
	}
	void Do()
	{
		int sz=0;
		unsigned int* a;
		unsigned int* b;
		for (int i=0;i<2;i++)
		{
			overflow = true;
			while (overflow)
			{
				sz = inf->input.at(i)->read((char*)(buf+i*sz),dataXorRead); //
				if (sz==dataXorRead)
				{
					overflow = false;	
				}
				else
				{
					overflow = true;
					if (inf->input.at(i)->halt) return;
                    usleep(1);   //при перегрузке входного буфера сон
				}	
			}
		}
		a = (unsigned int*)buf;
		b = (unsigned int*)(buf+sz);
		////////////////синхронизация

		if (!sync1)
		{//поиск смещения на начало потока

		}
		if (!sync2)
		{

		}

		unsigned int mask;
		unsigned int result1,result2;
		for (int i=0;i < (sz>>2) ;i++)
		{
			//перебор байт
			totalCount = totalCount+32;
			if (a[i]!=b[i])
			{
				mask=0x00000001;
				while (mask>0)
				{
					result1 = a[i] & mask;
					result2 = b[i] & mask;
					if (result2!=result1)
					{
						errCount=errCount+1;
					}
					mask = mask << 1;
				}
			}
		}
		Perr = errCount / totalCount;
		//
		//errCount=errCount+1;
		//
	}
	void init()
	{
		if (inf->input.size()<2)
		{
			inf->error=QString::fromLocal8Bit("Менее 2 входов");
			inf->ready = false;
			return;
		}
		inf->ready = true;
		for (unsigned int i = 0 ; i<inf->input.size();i++)
			inf->input.at(i)->setData();
	}
private:
	bool sync1,sync2;			//признаки синхронизации
	int offset1,offset2;			//смещения в битах относительно начал циклов
	bool overflow;				//нет данных во входных буферах
	bool FNF,Corr;				//признаки выбора метода синхронизации
    Ui::xormod *ui;
};

////////////////////ФОРМА/////////////////////////////

class xormod : public Interface
{
	Q_OBJECT

public:
    xormod(QDialog *parent,marray** _arr, unsigned int *_size);
    ~xormod();
	xorWork * work;

private:
    Ui::xormod ui;
private slots:
	void update();
};

#endif // XORMOD_H
