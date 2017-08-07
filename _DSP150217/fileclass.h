#ifndef FILECLASS_H
#define FILECLASS_H

#include "ui_fileclass.h"
#include "interface.h"
#include <QFile>
#include <QVariant>
#include <QFileDialog>
#include <QTimer>

////////////////ОБРАБОТКА /////////////////////////

class fileWork
{
public:
	bool radioSignal;
	bool overflow;
	bool cycle;
	fileWork(void* _inf, Ui::fileClass *_ui)
	{
		inf = (Interface*)_inf;
		ui = _ui;
	}

	~fileWork()
	{
		fileEt.close();
	}

	void Do() 
	{
		if (readIndex+bufSize>fileEt.size())  //больше чем файл не считается
		{
				if (cycle) 
				{
					readIndex = 0 ;
					fileEt.seek(readIndex);
					fileEt.read(buf,bufSize);
				}
				else
				{
					 usleep(1);
					 return;
				}
		}
		else
		{
			fileEt.seek(readIndex);
			fileEt.read(buf,bufSize);
		}

			///////////////////////////////////////разворот байт
			if (radioSignal)
			{
				short * b = (short*) buf;
				for (unsigned i=0;i < bufSize/2;i++)
				{
					short temp = *(b+i);
					short temp1=0;
					char* t = (char*)&temp;
					char* t1 = (char*)&temp1;
					*(t1) = *(t+1);
					*(t1+1)=*t;
					*(b+i)=temp1;
				}
			}
			//////////////////////////////////////

			overflow = true;
			int sz;
			while (overflow)
			{
				sz = inf->output.at(0)->write((char*)buf,bufSize); //
				if (sz==bufSize)
				{
					overflow = false;
					readIndex+=bufSize;
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
		inf->error = "";
		if (ui->lineFilePath->text()=="")
		{
			inf->error = QString::fromUtf8("Не задано имя файла");
			inf->ready = false;
			return;
		}
		QString path;
		path = ui->lineFilePath->text();
		fileEt.setFileName(path);
		if (!fileEt.open(QIODevice::ReadOnly | QIODevice::Unbuffered))
		{
			inf->error = QString::fromUtf8("Файл не может быть открыт");
			inf->ready = false;
			return;
		}
		inf->ready = true;
		readIndex = 0;
		radioSignal = ui->checkRadio->isChecked();	//читать с разворотом байт или нет
		cycle = ui->checkCycle->isChecked();		//циклическая передача
		if (fileEt.size()<tmpBufSize/2)
		{
			bufSize=fileEt.size();
		}
		else
		{
			bufSize=blockSizeRead;
		}
	}
private:
	QFile fileEt;
	unsigned int bufSize;
	char buf[tmpBufSize];
	long long readIndex;
	Interface *inf;
	Ui::fileClass *ui;

};

//////////////////  Ф  О  Р   М   А		/////////////
class fileClass : public Interface 
{
	Q_OBJECT

public:
    fileClass(QDialog *parent,marray** _arr, unsigned int *_size);
	~fileClass();
	fileWork* work;
private:
	Ui::fileClass ui;
	QTimer *timer;
private slots:
	void getFilePath();
	void update();
};

#endif // FILECLASS_H
