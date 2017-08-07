#ifndef BITGENERATOR_H
#define BITGENERATOR_H

#include "interface.h"
#include "ui_bitgenerator.h"
#include <QTimer>
#include <QVariant>

class bitGeneratorWork
{
public:
	long long dataCounter;
	unsigned int flagCount;
	unsigned int dataCount;
	bitGeneratorWork(void* _inf, Ui::bitGenerator *_ui)
	{
		inf = (Interface*)_inf;
		ui = _ui;
		dataCounter = 0;
		overflow = false;
		flagCount = 256;
		ui->lineFlag->setInputMask("9999");
		ui->lineFlag->setText("2");
		ui->lineData->setInputMask("9999");
		ui->lineData->setText("8196");
	}
	~bitGeneratorWork()
	{

	}
	void Do()
	{
		int size=0;

		if (ui->checkFlag->isChecked())
		{
			for (unsigned int i=0;i<flagCount;i++)
			{
				buf[size+i] = 0x7E;
			}
			size+=flagCount;
		}
		/*else
		{
			for (int i=0;i<dataCount;i++)
			{
					buf[size+i] = (char)i;
			}
			size+=dataCount;

		}*/
		
		if (ui->checkData->isChecked())
		{
			for (unsigned int i=0;i<dataCount>>4;i++)
			{
				for (unsigned int j = 0; j<dataCount>>4;j++)
				{
					buf[size+i*(dataCount>>4)+j] = j+0xF0;
				}
			}
			size+=dataCount;
		}
		
	if (size==0)
	{
		return;
	}
	if (inf->output.size()==0)
	{
		return;
	}
for (unsigned int j=0;j<inf->output.size();j++)
{
		overflow = true;
		int sz;
		while (overflow)
		{
			//if (inf->in)
			sz = inf->output.at(j)->write((char*)buf,size); //
			if (sz==size)
			{
				overflow = false;
				dataCounter+=size;
			}
			else
			{
				overflow = true;
				if (inf->output.at(j)->halt) return;
                usleep(1);   //при перегрузке выходного буфера сон
			}	
		}
}
		
	}
	void init()
	{
		bool ok;
		QString f = ui->lineFlag->text();
		int fl = f.toInt(&ok);
		if (ok)
		{
			flagCount = fl;
		}
		else
		{
			flagCount = 256;
			ui->lineFlag->setText("256");
		}
		f = ui->lineData->text();
		fl = f.toInt(&ok);
		if (ok)
		{
			dataCount = fl;
		}
		else
		{
			dataCount = 256;
		}
		inf->ready = true;
	}
private:

	char buf[tmpBufSize];
	bool overflow;
	Interface *inf;
	Ui::bitGenerator *ui;
};

class bitGenerator : public Interface
{
	Q_OBJECT

public:
    bitGenerator(QDialog *parent,marray** _arr, unsigned int *_size);
	~bitGenerator();
	bitGeneratorWork* work;

private:
	Ui::bitGenerator ui;
};

#endif // BITGENERATOR_H
