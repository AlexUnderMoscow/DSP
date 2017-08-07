#include "fileclass.h"

fileClass::fileClass(QDialog *parent,marray** _arr, unsigned int *_size)
	: Interface(parent)
{
	ui.setupUi(this);
	moduleName = QString::fromUtf8("Входной файл");
	type = File;
	work = new fileWork((void*)this,&ui);
	widgetAddr = (void*)work;
	form = this;
	timer = new QTimer(this);
//	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
//	timer->start(1000);
	MODpixmap.load(":/images/open.png");
	connect(ui.bOpen,SIGNAL(clicked()),this,SLOT(getFilePath()));	
	createButton(this);
	arr = _arr;
	size=_size;
}

fileClass::~fileClass()
{
	delete timer;
//	delete work;
}

void fileClass::getFilePath()
{
	QString path;
	path = QFileDialog::getOpenFileName(this, QString::fromUtf8("Путь к файлу"), "", "");
	ui.lineFilePath->setText(path);
}

void fileClass::update()
{
// 	QString s;
// 	if (output.size()!=0) ui.labBufLoad->setText(s.setNum(output.at(0)->load()));
}
