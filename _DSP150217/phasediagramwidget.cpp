#include "phasediagramwidget.h"
#include <math.h>
#include <vector>
#include <QtGui>
#include <QLabel>
#include <QFileInfoList>
#include <QDir>
#include <QStringList>

using namespace std;
#define SIZ 1000
#define showPoints 1000
#define vertOtst 20
#define horOtst 20
#define cent 128


phaseDiagramWidget::phaseDiagramWidget(QWidget *parent)
: QWidget(parent)
{
	cosSample = new short[SIZ];
	sinSample = new short[SIZ];
	memset(cosSample,0,SIZ*sizeof(short));
	memset(sinSample,0,SIZ*sizeof(short));
	rBlock = 0;
	wBlock = 0;
	load = 0;
}

phaseDiagramWidget::~phaseDiagramWidget()
{
	delete [] cosSample;
	delete [] sinSample;
}

void phaseDiagramWidget::addSample(short x, short y)
{

	cosSample[wBlock] = x;
	sinSample[wBlock] = y;
	wBlock++;
	if (wBlock==SIZ)
	{
		wBlock = 0;
	}
	if (load<SIZ) load++;
	
	if (load==showPoints)
	{
		if (rBlock==SIZ)
		{
			rBlock = 0;
		}
		else
		{
			rBlock++;
		}
	} 
	
}

void phaseDiagramWidget::paintEvent(QPaintEvent * /*event*/)
{
	QPainter painter(this);
	painter.fillRect(rect(), Qt::white);
	painter.setPen(Qt::black);
	painter.drawLine(0+horOtst,cent*2+vertOtst,cent*2+horOtst,cent*2+vertOtst);
	painter.drawLine(0+horOtst,vertOtst,0+horOtst,cent*2+vertOtst);
	painter.drawLine(cent*2+horOtst,vertOtst,cent*2+horOtst,cent*2+vertOtst);
	painter.drawLine(0+horOtst,vertOtst,cent*2+horOtst,vertOtst);
	QPen pen;  // creates a default pen
	pen.setStyle(Qt::DashDotLine);
	pen.setBrush(Qt::black);
	painter.setPen(pen);
	painter.drawLine(horOtst,cent+vertOtst,cent*2+horOtst,cent+vertOtst);
	painter.drawLine(cent+horOtst,vertOtst,(cent/2)*2+horOtst,cent*2+vertOtst);
	painter.setPen(Qt::black);
	QString textHz = QString::fromUtf8("I");
	painter.drawText(cent*2+horOtst+10,cent+vertOtst, textHz);
	textHz = QString::fromUtf8("Q");	
	painter.drawText(cent+horOtst,10, textHz);
	QPointF pt;
	pen.setColor(Qt::blue);
	pen.setWidth(2);
	painter.setPen(pen);
  for (int i=0;i<load;i++)
  {
	pt.setX(cent+horOtst+cosSample[i]/20);
	pt.setY(cent+vertOtst+sinSample[i]/20);
	painter.drawPoint(pt);
  }
}






	







