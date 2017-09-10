#ifndef SUMM_H
#define SUMM_H

#include <QTimer>
#include <QTime>
#include <QVariant>
#include "interface.h"
#include "spectrumwidget.h"
#include "ui_summ.h"



class summWork
{
public:
	long long dataCounter;
	unsigned int dataRead;  //по сколько Kбайт считывать
	Interface *inf;
	char buf[tmpBufSize];
	short* result;
	short* pcm;

	summWork(void* _inf, Ui::summ *_ui)
	{
		inf = (Interface*)_inf;
		inf->ready = true;
		ui = _ui;
		dataCounter = 0;
		ui->comboKbytes->addItem("1");
		ui->comboKbytes->addItem("2");
		ui->comboKbytes->addItem("4");
		ui->comboKbytes->addItem("8");
		ui->comboKbytes->addItem("16");
		ui->comboKbytes->addItem("32");
		ui->comboKbytes->addItem("64");
		ui->comboKbytes->addItem("128");
		ui->comboKbytes->addItem("512");
		ui->comboKbytes->addItem("1024");
		ui->comboKbytes->addItem("2048");
		dataRead = 1;
		ui->comboKbytes->setCurrentIndex(0);
		overflow = false;
	}

	~summWork()
	{
	//	inf = NULL;
	}
	void Do()
	{
		if (inf->input.size()==0)
		{
			return;
		}
		result = (short*)(buf+tmpBufSize/2);
		pcm = (short*)buf;
		int si = inf->input.size();
		int sz=0;
		for (int i=0;i<si;i++)
		{
			overflow = true;
			while (overflow)
			{
				sz = inf->input.at(i)->read((char*)buf,dataRead*1024); //
				if (sz==dataRead*1024)
				{
					overflow = false;
					for (unsigned int j=0;j<sz/sizeof(short);j++)
					{
						result[j]+=pcm[j];
					}
				}
				else
				{
					overflow = true;
					if (inf->input.at(i)->halt) return;
                    usleep(1);   //при перегрузке выходного буфера сон
				}	
			}
		}

		if (inf->output.size()==0)  //нет выходов / заглушка
		{
			dataCounter+=dataRead*1024;
			return;
		}

		sz=0;
		si = inf->output.size();
		for (int i=0;i<si;i++)
		{
			overflow = true;
			while (overflow)
			{
				sz = inf->output.at(i)->write((char*)result,dataRead*1024); //
				if (sz==dataRead*1024)
				{
					overflow = false;
					memset(result,0,dataRead*1024);
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
	Ui::summ *ui;
};

////////////////////////////////////ДИАЛОГ//////////////////
class summ : public Interface
{
	Q_OBJECT
public:
    summ(QDialog *parent,marray** _arr, unsigned int *_size);
	~summ();
	summWork * work;

private:
	Ui::summ ui;
	QDateTime oldTime;
	float oldCount;
	float AverageSpeed;
	QDialog * dlg;
	spectrumWidget *sw;
	QTimer *sptime;
private slots:
	void update();
	void indexChanged(QString s);
	void spectrumShow();
};

#endif // SUMM_H
