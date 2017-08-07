#include "iqdem.h"

IQdem::IQdem(QDialog *parent, marray** _arr, unsigned int *_size)
	: Interface(parent)
{
	ui.setupUi(this);
	moduleName = QString::fromUtf8("Квадратурный демодулятор");
	MODpixmap.load(":/images/4qam.png");
	type = IQDemodulator;
	pdw = NULL;
	work = new iqDemWork((void*)this,&ui, &pdw);
	widgetAddr = (void*)work;
	form = this;
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	connect(ui.phaseDial, SIGNAL(valueChanged(int)), this, SLOT(phaseUpdate(int)));
	connect(ui.clkDial, SIGNAL(valueChanged(int)), this, SLOT(clkUpdate(int)));
	connect(ui.phDiag,SIGNAL(clicked()),this,SLOT(diagramShow()));
	timer->start(1000);
	createButton(this);
	arr = _arr;
	size=_size;
	dlg = NULL;
	pdw = NULL;
}

IQdem::~IQdem()
{
//	delete work;
//	if (dlg!=NULL) delete dlg;
//	if (pdw!=NULL) delete pdw;
}

void IQdem::update()
{
	QString s;
	if (input.size()!=0) ui.progress->setValue((int)input.at(0)->load());
	s.setNum(work->symCount);
	ui.labSymbolCount->setText(s);
}

void IQdem::phaseUpdate(int val)
{
	double temp=val;
	temp = ui.phaseDial->value();
	temp = temp /57;
	work->addPhase=temp;
}
void IQdem::clkUpdate(int val)
{
	double temp=val;
	temp =ui.clkDial->value();
	temp = temp /57;
	work->addClk=temp;
}

void IQdem::diagramShow()
{
	if (ui.phDiag->isChecked())
	{ //запуск формы FFT

		dlg = new QDialog();
		pdw = new phaseDiagramWidget(dlg);
		pdw->setGeometry(0,0,300,300);
		dlg->setGeometry(100,100,300,300);
		dlg->setModal(false);
        dlg->setWindowTitle(QString::fromUtf8("Фазовая диаграмма"));
		dlg->show();
		sptime=new QTimer();
 		sptime->setInterval(100);
 		sptime->start();
 		connect(sptime,SIGNAL(timeout()),pdw,SLOT(update()));
	}
	else
	{  //закрытие формы FFT

		sptime->stop();
		dlg->close();
		delete pdw;
		pdw = NULL;
		delete dlg;
		dlg=NULL;

		delete sptime;
	}
}


