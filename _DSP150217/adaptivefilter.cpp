#include "adaptivefilter.h"

AdaptiveFilter::AdaptiveFilter( QDialog *parent, marray** _arr, unsigned int *_size)
	:Interface(parent)
{
	ui.setupUi(this);
	moduleName = QString::fromUtf8("Адаптивный фильтр");
	MODpixmap.load(":/images/artsfftscope.png");
	type = Adaptive;
	work = new AdaptiveWork((void*)this,&ui);
	widgetAddr = (void*)work;
	form = this;
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(1000);
	createButton(this);
	arr = _arr;
	size=_size;
}

AdaptiveFilter::~AdaptiveFilter()
{
	delete timer;
	//delete work;
}

void AdaptiveFilter::update()
{
	QString s;
	if (input.size()!=0) ui.progress->setValue((int)input.at(0)->load());
}
