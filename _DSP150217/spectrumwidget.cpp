#include "spectrumwidget.h"
#include <math.h>
#include <vector>
#include <QtGui>
#include <QLabel>
#include <QFileInfoList>
#include <QDir>
#include <QStringList>

#define YY 300

using namespace std;
vector<double> spectrumWidget::FFTCalc(const vector<short>& dIn, int nn, int beginData)
{
	int j, n, m, mmax, istep;
	unsigned int i;
	double tempr, tempi, wtemp, theta, wpr, wpi, wr, wi;
	int isign = -1;
	vector<double> data(nn*2 + 1);
	j = 0;
	for (i = beginData; i < beginData + nn; i++)
	{
		if (i < dIn.size())
		{
			data[j*2]   = 0;
			data[j*2+1] = dIn[i];
		}
		else
		{
			data[j*2]   = 0;
			data[j*2+1] = 0;
		}
		j++;
	}
	n = nn << 1;
	j = 1;
	i = 1;
	while (i < n)
	{
		if (j > i)
		{
			tempr = data[i];   data[i]   = data[j];   data[j]   = tempr;
			tempr = data[i+1]; data[i+1] = data[j+1]; data[j+1] = tempr;
		}
		m = n >> 1;
		while ((m >= 2) && (j > m))
		{
			j = j - m;
			m = m >> 1;
		}
		j = j + m;
		i = i + 2;
	}
	mmax = 2;
	while (n > mmax)
	{
		istep = 2 * mmax;
		theta = 2.0*M_PI / (isign * mmax);
		wtemp = sin(0.5 * theta);  
		wpr   = -2.0 * wtemp * wtemp;
		wpi   = sin(theta);
		wr    = 1.0;
		wi    = 0.0;
		m    = 1;
		while (m < mmax)
		{
			i = m;
			while (i < n)
			{
				j         = i + mmax;
				tempr     = wr * data[j] - wi * data[j+1];
				tempi     = wr * data[j+1] + wi * data[j];
				data[j]   = data[i] - tempr;
				data[j+1] = data[i+1] - tempi;
				data[i]   = data[i] + tempr;
				data[i+1] = data[i+1] + tempi;
				i         = i + istep;
			}
			wtemp = wr;
			wr    = wtemp * wpr - wi * wpi + wr;
			wi    = wi * wpr + wtemp * wpi + wi;
			m     = m + 2;
		}
		mmax = istep;
	}
	//vector<double> dOut(nn / 2);
	sp.clear();
	for (i = 0; i < (nn / 2); i++)
	{
		sp.push_back(sqrt( data[i*2] * data[i*2] + data[i*2+1] * data[i*2+1] ));
	}
	return sp;
}

spectrumWidget::spectrumWidget(QWidget *parent, short* _pcm, int _size)
: QObject(parent)
{
	x = 225; 
	y = 200-100;
	pcm = _pcm;
	size = _size;
	k_zoom = 10;
	step =0;
	counts=0;
	period = quantums / periods;
	
	/**/
	time = new QTimer();
	time->setInterval(50);
	time->start();
	cp=new QCustomPlot(parent);
	cp->setGeometry(0,0,1200,350);
	cp->show();
	cp->clearGraphs();
	cp->addGraph();
	cp->xAxis->setRange(0,4000);
	cp->yAxis->setRange(-100,-20);
	cp->xAxis->setLabel(QString::fromUtf8("Частота, Гц"));
	cp->yAxis->setLabel("dB");

	//cp->replot();
	cnt=0;
	for (int i=0;i<PTS;i++)
	  {
	    masVec[0].push_back(0);
	     masVec[1].push_back(0);
	      masVec[2].push_back(0);
	       masVec[3].push_back(0);

	       masVec[4].push_back(0);
		masVec[5].push_back(0);
		 masVec[6].push_back(0);
		  masVec[7].push_back(0);
	       dfourier.push_back(0);
	  }
}

void spectrumWidget::timeoutRepaint()
{
    dsin.clear();
    for (int i=0; i<PTS*4; i++)
    {
            short tmp = pcm[i];
            short tmp1=0;
            char* t = (char*)&tmp;
            char* t1 = (char*)&tmp1;
            *(t1) = *(t+1);
            *(t1+1)=*t;
            dsin.push_back(tmp);
    }

    //dfourier = FFTCalc(dsin, PTS*2, 1);
    masVec[cnt].clear();
    masVec[cnt++] = FFTCalc(dsin, PTS*2, 1);
    if (cnt==8) cnt =0;

    for (unsigned int i = 0; i<masVec[0].size();i++)
    {
        dfourier[i]=(double)(20*log10((masVec[0].at(i)+masVec[1].at(i)+masVec[2].at(i)+masVec[3].at(i)+
        masVec[4].at(i)+masVec[5].at(i)+masVec[6].at(i)+masVec[7].at(i) )/8))-160;
    }

    /**/

    /**/
    QVector<double> graphics;
    QVector<double> x;
    for (int i=0;i<dfourier.size();i++)
      {
        x.push_back(i*(4000./PTS));
        graphics.push_back(dfourier.at(i));
      }
    cp->graph(0)->setData(x,graphics);
    cp->replot();
}

spectrumWidget::~spectrumWidget()
{
  delete time;
  delete cp;
}


vector <double> spectrumWidget::spectr()
{
	return sp;
}




