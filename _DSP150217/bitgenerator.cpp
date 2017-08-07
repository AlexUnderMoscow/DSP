#include "bitgenerator.h"

bitGenerator::bitGenerator(QDialog *parent, marray** _arr, unsigned int *_size)
	: Interface(parent)
{
	ui.setupUi(this);
	moduleName = QString::fromUtf8("Битовый генератор");
	MODpixmap.load(":/images/bitgen.png");
	type = bitGen;
	work = new bitGeneratorWork((void*)this,&ui);
	widgetAddr = (void*)work;
	form = this;
	timer = new QTimer(this);
	//connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	//timer->start(1000);
	createButton(this);
	arr = _arr;
	size=_size;
}

bitGenerator::~bitGenerator()
{
	delete timer;
}
