#ifndef ADAPTIVEFILTER_H
#define ADAPTIVEFILTER_H

//#define block 1024 //2048

#include "ui_adaptivefilter.h"
#include "interface.h"
#include <QTimer>
#include <QVariant>

class AdaptiveWork
{
public:
	long long dataCounter;
	short sample;
	double time;
	unsigned short freq;
	AdaptiveWork(void* _inf, Ui::AdaptiveFilter *_ui)
	{
		inf = (Interface*)_inf;
		ui = _ui;
		dataCounter = 0;
		overflow = false;
		sample = 0;
		time = 0;
		freq = 0;
	}
	~AdaptiveWork()
	{

	}
	void Do()
	{
		int size;
		
	//		size = inf->input.at(0)->read(buf,1024);
		
	//	if (size==0)
	//	{
	//		usleep(1);
	//		return;
	//	}
		short *tmp = (short*)buf;
		for (int i = 0; i<512;i++)
		  {
		    //*(tmp+i) = sample++;
		    /*if (sample == 10000)
		      {
			sample = 0;
		      }*/
		    *(tmp+i) = 10000*cos(freq*2*3.14159*time);
		    time = time + 0.000125;

		  }
		freq++;

		if (freq == 4000) {freq = 0;}
		overflow = true;
		while (overflow)
		{
			int sz = inf->output.at(0)->write(buf,1024);  //попытка отправить того что пришло и выполнение рутины тормозящей
		if (sz==1024)
		{

			overflow = false;
		}
		else
		{
			overflow = true;
			if (inf->output.at(0)->halt) return;
			usleep(1);   //при перегрузке выходного буфера сон
			}
		}	
	}
	void init()
	{
		inf->ready = true;
		for (unsigned int i = 0 ; i<inf->input.size();i++)
			inf->input.at(i)->setData();
	}
private:

	char buf[tmpBufSize];
	bool overflow;
	Interface *inf;
	Ui::AdaptiveFilter *ui;
};
////////////////////ФОРМА/////////////////////////////

class AdaptiveFilter : public Interface
{
	Q_OBJECT

public:
    AdaptiveFilter( QDialog *parent,marray** _arr, unsigned int *_size);
	~AdaptiveFilter();
	AdaptiveWork* work;
private:
	Ui::AdaptiveFilter ui;
private slots:
	void update();
};

#endif // ADAPTIVEFILTER_H
