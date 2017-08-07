#include "test1.h"

test1::test1(QDialog *parent,marray** _arr, unsigned int *_size)
	: Interface(parent)
{
	ui.setupUi(this);
    moduleName = QString::fromUtf8("Прием данных по UDP");
	MODpixmap.load(":/images/linecolor.png");
	type = test1mod;
	work = new test1Work((void*)this,&ui);
//	connect(ui.comboKbytes,SIGNAL(currentIndexChanged(QString)),this,SLOT(indexChanged(QString)));
//	connect(ui.checkSpectrum,SIGNAL(clicked()),this,SLOT(spectrumShow()));
	widgetAddr = (void*)work;
	form = this;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
	createButton(this);
	arr = _arr;
	size=_size;
}

test1::~test1()
{

}

void test1::update()
{
    QString s,s1;
    s.setNum(work->numPackets,10);
    ui.packetNums->setText(s);
    s1.setNum(work->numOfLostPacket,10);
    ui.lostedPackets->setText(s1);
}
