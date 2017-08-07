#include "qpskmod.h"

QPSKmod::QPSKmod(QDialog *parent,marray** _arr, unsigned int *_size)
	: Interface(parent)
{
	ui.setupUi(this);
	moduleName = QString::fromUtf8("Модулятор ФМ-4");
	MODpixmap.load(":/images/image4.png");
	type = QPSKmodulator;
	work = new qpskModWork((void*)this,&ui);
	widgetAddr = (void*)work;
	form = this;
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(1000);
	createButton(this);
	arr = _arr;
	size=_size;
}

QPSKmod::~QPSKmod()
{

}
void QPSKmod::update()
{
	if (input.size()!=0) 
	{
		ui.progress->setValue((int)input.at(0)->load());
	}
}
