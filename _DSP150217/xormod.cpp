#include "xormod.h"

xormod::xormod(QDialog *parent,marray** _arr, unsigned int *_size)
: Interface(parent)
{
	ui.setupUi(this);
	moduleName = QString::fromUtf8("Побитное сравнение");
	MODpixmap.load(":/images/xor.png");
    type = Xormod;
	work = new xorWork((void*)this,&ui);
	widgetAddr = (void*)work;
	form = this;
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(1000);
	createButton(this);
	arr = _arr;
	size=_size;
}

xormod::~xormod()
{

}

void xormod::update()
{
	QString s;
	if (input.size()!=0) ui.progress->setValue((int)input.at(0)->load());
		ui.labSync->setText("OK");
		ui.labErrCount->setText(s.setNum(work->errCount));
		ui.labBitCount->setText(s.setNum(work->totalCount));
		ui.labVer->setText(s.setNum(work->Perr));
}
