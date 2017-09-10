#ifndef IO_H
#define IO_H
#include <QTimer>
#ifdef min
#undef min
#endif
#include <QTime>
#include <QVariant>
#include <QMessageBox>
#include "interface.h"
#include "spectrumwidget.h"
#include "ui_io.h"
class ioWork
{
public:
	long long dataCounter;
	unsigned int dataRead;  //по сколько Kбайт считывать
	Interface *inf;
	char buf[tmpBufSize];

	ioWork(void* _inf, Ui::io *_ui)
	{
		inf = (Interface*)_inf;
		inf->ready = true;
		ui = _ui;
		dataCounter = 0;
		dataRead = 1;
		overflow = false;
	}

	~ioWork()
	{
	//	inf = NULL;
	}
	void Do()
	{
		int size;
		size = inf->input.at(0)->read(buf,dataRead*1024);
		if (size==0)
		{
			usleep(1);
			return;
		}
		if (inf->output.size()==0)  //нет выходов / заглушка
		{
			dataCounter+=dataRead*1024;
			return;
		}

		int sz=0;
		int si = inf->output.size();
		for (int i=0;i<si;i++)
		{
			overflow = true;
			while (overflow)
			{
				sz = inf->output.at(i)->write((char*)buf,dataRead*1024); //
				if (sz==dataRead*1024)
				{
					overflow = false;
				}
				else
				{
					overflow = true;
					if (inf->output.at(i)->halt) return;
					usleep(1);   //при перегрузке выходного буфера сон
				}	
			}
		}
			dataCounter+=dataRead*1024;
	}
	void init()
	{
		inf->ready = true;
		for (unsigned int i = 0 ; i<inf->input.size();i++)
		inf->input.at(i)->setData();
	}
private:

	bool overflow;
	Ui::io *ui;
};
////////////////////ФОРМА/////////////////////////////
class io : public Interface
{
	Q_OBJECT
public:
    io(QDialog *parent,marray** _arr, unsigned int *_size);
	~io();
	ioWork * work;

	QtProperty *top;
	QtProperty *enumBufSize;
	QtProperty *fileName;
	QtProperty *bShowSpectrum;
	QtProperty *fDataReceived;
	QtProperty *fAverageSpeed;
	QtProperty *flagitem;
	QtProperty *IntBufLoad;

	/**/
private slots:
	void update();
	void boolValueChaged (QtProperty *property,bool value);
	void enumValueChaged (QtProperty * property, int value);

private:
	Ui::io ui;
	QDateTime oldTime;
	float oldCount;
	float AverageSpeed;
	QDialog * dlg;
	spectrumWidget *sw;
	QTimer *sptime;


};



#endif // IO_H

