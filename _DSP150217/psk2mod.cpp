#include "psk2mod.h"

psk2Mod::psk2Mod(QDialog *parent,marray** _arr, unsigned int *_size)
	: Interface(parent)
{
	ui.setupUi(this);
	moduleName = QString::fromUtf8("Модулятор ФМ-2");
	MODpixmap.load(":/images/image4.png");
	type = PSK2modulator;
	work = new psk2ModWork((void*)this,&ui);
	widgetAddr = (void*)work;
	form = this;
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(1000);
	createButton(this);
	arr = _arr;
	size=_size;
}

psk2Mod::~psk2Mod()
{
	delete timer;
}

void psk2Mod::update()
{
	if (input.size()!=0) 
	{
		ui.progress->setValue((int)input.at(0)->load());
	}
}
