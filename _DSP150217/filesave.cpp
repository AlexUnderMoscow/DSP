#include "filesave.h"


fileSave::fileSave(QDialog *parent,marray** _arr, unsigned int *_size)
	: Interface(parent)
{
	ui.setupUi(this);
	moduleName = QString::fromUtf8("Выходной файл");
	type = OutputFile;
	work = new fileSaveWork((void*)this,&ui);	
	widgetAddr = (void*)work;
	form = this;
	MODpixmap.load(":/images/floppy-32.png");
	connect(ui.bSave,SIGNAL(clicked()),this,SLOT(getFilePath()));
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(1000);
	createButton(this);
	arr = _arr;
	size=_size;
}

fileSave::~fileSave()
{
	delete timer;
//	delete work;
}

void fileSave::getFilePath()
{
	QString path;
	path = QFileDialog::getOpenFileName(this, QString::fromUtf8("Путь к файлу"), "", "");
	ui.lineFilePath->setText(path);
}

void fileSave::update()
{
	QString s;
	if (input.size()!=0) ui.progress->setValue((int)input.at(0)->load());
}
