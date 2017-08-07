#include "summ.h"

summ::summ(QDialog *parent,marray** _arr, unsigned int *_size)
: Interface(parent)
{
	ui.setupUi(this);
	moduleName = QString::fromUtf8("Cумматор");
	MODpixmap.load(":/images/linecolor.png");
	type = Summ;
	work = new summWork((void*)this,&ui);
	connect(ui.comboKbytes,SIGNAL(currentIndexChanged(QString)),this,SLOT(indexChanged(QString)));
//	connect(ui.checkSpectrum,SIGNAL(clicked()),this,SLOT(spectrumShow()));
	widgetAddr = (void*)work;
	form = this;
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(1000);
	oldCount = 0;
	oldTime.currentDateTime();
	AverageSpeed = 0;
	createButton(this);
	arr = _arr;
	size=_size;
}

summ::~summ()
{
//		delete timer;
}
void summ::update()
{
	QString s;
	float a;
	if (work!=NULL) 
	{
		a=work->dataCounter;
	}
	float f = a/(1024*1024);
	s.setNum(f);
	ui.dataCount->setText(s);
	if (input.size()!=0) ui.progress->setValue((int)input.at(0)->load());

	QDateTime time;
	time = QDateTime::currentDateTime();
	qint64 msecs = abs(time.msecsTo(oldTime));
	AverageSpeed = (AverageSpeed+((a-oldCount)/msecs)*1000)/2;
	if (oldCount != a) {
		ui.labAverageSpeed->setText(s.setNum(AverageSpeed/(1024*1024)));
	}
	oldTime = time;
	oldCount = a;

}

void summ::indexChanged(QString s)
{
	bool ok;
	unsigned int k = s.toUInt(&ok);
	if (ok)
	{
		work->dataRead = k;
	}
}

void summ::spectrumShow()
{
	if (ui.checkSpectrum->isChecked())
	{ //запуск формы FFT

		dlg = new QDialog();
		sw = new spectrumWidget(dlg,(short*)work->result,1024*1024);
		//sw->setGeometry(0,0,600,450);
		dlg->setGeometry(100,100,600,450);
		dlg->setModal(false);
		dlg->setWindowTitle(QString::fromUtf8("Спектр"));
		dlg->show();
		sptime=new QTimer();
		//sptime->setInterval(100);
		//sptime->start();
		//connect(sptime,SIGNAL(timeout()),sw,SLOT(update()));
	}
	else
	{  //закрытие формы FFT

		sptime->stop();
		dlg->close();
		delete sw;
		dlg=NULL;

		delete sptime;
	}
}

