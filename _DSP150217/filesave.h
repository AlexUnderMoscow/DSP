#ifndef FILESAVE_H
#define FILESAVE_H


#include "ui_filesave.h"
#include "interface.h"
#include <QFile>
#include <QFileDialog>
#include <QTimer>
#include <QVariant>

class fileSaveWork
{
public:
	bool radioSignal;
	fileSaveWork(void* _inf, Ui::fileSave *_ui)
	{
		inf = (Interface*)_inf;
		ui = _ui;
		writeIndex = 0;
	}
	~fileSaveWork()
	{
		fileEt.close();
	}
	void Do()
	{
		int size = inf->input.at(0)->read(buf,testBlockSize);  //blockSize

		if (size == 0)
		{
            usleep(1);
			return;
		}

		////////////////////////////////////////////////////////////попытка сохранять с разворотом байт...
		if (radioSignal)
		{
			short * b = (short*) buf;
			for (unsigned i=0;i < testBlockSize/2;i++) //blockSize
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
		
		//////////////////////////////////////////////////////////

		fileEt.seek(writeIndex);
		fileEt.write(buf,testBlockSize);  //blockSize
		writeIndex+=(testBlockSize);   //blockSize
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
		if (fileEt.isOpen()) fileEt.close();
		if (!fileEt.open(QIODevice::WriteOnly | QIODevice::Unbuffered))
		{
			inf->error = QString::fromUtf8("Файл не может быть открыт");
			inf->ready = false;
			return;
		}
		inf->ready = true;
		radioSignal = ui->checkRadio->isChecked();	//сохранять с разворотом байт или нет
		for (unsigned int i = 0 ; i<inf->input.size();i++)
			inf->input.at(i)->setData();
	}
private:
	QFile fileEt;
	char buf[tmpBufSize];
	long long writeIndex;
	Interface *inf;
	Ui::fileSave *ui;
};

class fileSave : public Interface
{
	Q_OBJECT

public:
    fileSave(QDialog *parent,marray** _arr, unsigned int *_size);
	~fileSave();
	fileSaveWork* work;
private:
	Ui::fileSave ui;

	
private slots:
	void getFilePath();
	void update();
};

#endif // FILESAVE_H
