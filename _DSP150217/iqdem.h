#ifndef IQDEM_H
#define IQDEM_H

#include <math.h>
#include <QVariant>
#include "struct.h"
#include "interface.h"
#include "ui_iqdem.h"
#include "phasediagramwidget.h"

#define readBlock 2*1024
#define samples 1

class iqDemWork
{
public:
	long long dataCounter;

	double shiftRegCos[350];
	double shiftRegSin[350];

	unsigned short NumCoef;
	double thistime;
	double timeInterval;
	double freq;
	double symbolRate;
	int sr;
	int amp;
	double startPhase;
	double symbolFreq;
	double clkPhase;
	int ostatok;
	short clkCosine[samples]; 
	short	cosineOld;
	double symCount;
	bool first,first2;
	double addPhase;
	double addClk;
	unsigned char mask;
	int index;
	unsigned char  outbits[10];

	iqDemWork(void* _inf, Ui::IQdem *_ui, phaseDiagramWidget **_pdw)
	{
		inf = (Interface*)_inf;
		ui = _ui;
		dataCounter = 0;
		overflow = false;
		NumCoef = 341;
		memset(shiftRegCos,0,(NumCoef+8)*sizeof(double));
		memset(shiftRegSin,0,(NumCoef+8)*sizeof(double));
		timeInterval = 125E-10;
		startPhase = 0;
		//startPhase+=(pi/4-pi/16);
		symbolFreq = 1E6;  ////
		clkPhase = -M_PI/2;
		//clkPhase-=(pi/4-pi/16-pi/32-pi/64-pi/128);
		thistime=0;
		freq = 8000000;	//типа 8 МГц по нашим раскладам оцифровка 40 МГц
		ui->lineFreqEdit->setInputMask("99999999");
		QString s;
		ui->lineFreqEdit->setText("08000000");
		symbolRate = 1000000;
		ui->comboSymbolRate->addItem("1");
		ui->comboSymbolRate->addItem("2");
		ui->comboSymbolRate->addItem("4");
		ui->comboSymbolRate->addItem("8");
		ui->comboSymbolRate->addItem("16");
		ui->comboModulation->addItem(QString::fromUtf8("PSK"));
		ui->comboModulation->addItem(QString::fromUtf8("QPSK"));
		amp = 800;
		ostatok=0;
		pdw = _pdw;
		first = true;
		first2 = true;
		symCount = 0;
		addPhase = 0;
		addClk = 0;
		memset((char*)clkCosine,0,samples*sizeof(short));
		cosineOld = 0;
		mask = 0x01;
		index=0;
		memset(outbits,0,10);
	}
	~iqDemWork()
	{

	}
	void Do()
	{
		unsigned int size;
		int sz=-1;
		int dataRead  =		2*readBlock; // 2*samples

		if (inf->input.size()>0)
		{
			sz = inf->input.at(0)->read((char*)buf,dataRead);
			if (sz!=dataRead)
			{
                usleep(1);
				return;
			}
		}

		size = dataRead;
		/////////////////////////////////////////////////////////////////////////////

		float * co = (float*)(buf+size);
		//short * shco = (short*)(buf+size);
		float * si = (float*)(buf+size+(size/sizeof(short))*sizeof(float));
		
///////////////это нужно для синхронизации битовой////////////////
if (first)
{
	mask = mask << 4;		//3
	outbits[0] = 14;		//6
	first = false;
	if (ui->comboModulation->currentIndex()==1)
	{
		outbits[0] = 0;
		mask = mask >> 4;
	}
}
///////////////////////////////////////////////////////////////////

for (int j = 0; j<readBlock;j++)
{
	buf[0]=buf[j];

		for (unsigned int i=0;i<samples;i++)
		{ //перемножение на опорное колебание
		//	*(shco+i) = buf[i]*cos(time*freq*2*pi+startPhase);
			*(co+i) = buf[i]*cos(thistime*freq*2*M_PI+startPhase+addPhase);
			*(si+i) = buf[i]*sin(thistime*freq*2*M_PI+startPhase+addPhase);
			clkCosine[i] = 10000*cos(symbolRate*2*M_PI*thistime+clkPhase+addClk);
			thistime+=timeInterval;
		}
		
		for (unsigned int i=0;i<samples;i++)
		{ //согласованная фильтрация
			
			/*смещение сдвигового регистра*/
			shiftFilter(sr);
			shiftRegCos[0] = *(co+i);
			shiftRegSin[0] = *(si+i);
			/*фильтрация*/
			*(co+i) = (float)filter(sr,1);   //
			//*(shco+i) = (short)filter(sr,1);	//!!!!!
			*(si+i) = (float)filter(sr,2);
		}

		//memset(shco,0,size);
		for (unsigned int i=0;i<samples;i++)
		{ //берем отсчеты корреляционной функции
			if ((cosineOld<0) && (clkCosine[i]>=0))
			{ //переход с минуса на плюс - берем отсчет и запоминиаем
				//*(shco+i) = (short)*(co+i);
				symCount++;
				if (*pdw!=NULL)
				{
					(*pdw)->addSample((short)*(co+i),(short)*(si+i));
				}
				//тут соберем буфер бит для отправки демодулированных данных
				short resCos = (short)*(co+i);
				short resSin = (short)*(si+i);

				if (mask==0)
				{
					mask = 0x01;
					index++;
				}

				if (resCos>=0)
				{// 1
					outbits[index] = mask | outbits[index];
				}
				mask = mask << 1;
				if (ui->comboModulation->currentIndex()==1)
				{	
					if (resSin>=0)
					{// 1
						outbits[index] = mask | outbits[index];
					}
					mask = mask << 1;
				}
				
				if (mask==0)
				{
					mask = 0x01;
					index++;
				}

				if (index==10)
				{//отправка блока
					if (inf->output.size()!=0)
					{
						overflow = true;
						while (overflow)
						{
							
							if (first2)
							{
								inf->output.at(0)->write((char*)(outbits+1),9);
								first2 = false;
								sz=10;
							}
							else
							{
								sz = inf->output.at(0)->write((char*)outbits,index); //
							}

							if (sz==10)
							{
								overflow = false;
							}
							else
							{
								overflow = true;
								if (inf->output.at(0)->halt) return;
                                usleep(1);   //при перегрузке выходного буфера сон
							}	
						}
					}
					memset(outbits,0,10);
					index=0;
				}
			}
			else
			{
				//*(co+i) = 0;
			}
			cosineOld = clkCosine[i];
		}
}	
		
	}
	void init()
	{
		for (unsigned int i = 0 ; i<inf->input.size();i++)
			inf->input.at(i)->setData();
		dataCounter = 0;
	//	overflow = false;
	//	NumCoef = 341;
		memset(shiftRegCos,0,(NumCoef+8)*sizeof(double));
		memset(shiftRegSin,0,(NumCoef+8)*sizeof(double));
	//	timeInterval = 125E-10;
		startPhase = 0;
		//startPhase+=(pi/4-pi/16);
		symbolFreq = 1E6;  ////
		clkPhase = -M_PI/2;
		//clkPhase-=(pi/4-pi/16-pi/32-pi/64-pi/128);
		thistime=0;
		amp = 800;
		first = true;
		first2 = true;
		symCount = 0;
		addPhase = 0;
		addClk = 0;
		memset((char*)clkCosine, 0,samples*sizeof(short));
		cosineOld = 0;
		QString s = ui->comboSymbolRate->currentText();
		bool ok;
		QString f = ui->lineFreqEdit->text();
		double fr = f.toDouble(&ok);
		if (ok)
		{
			freq = fr;
		}
		else
		{
			freq = 4000000;
			ui->lineFreqEdit->setText("4000000");
		}
		sr = s.toDouble(&ok);
		if (ok)
		{
			symbolRate = sr*1000000;
		}
		else
		{
			symbolRate = 1E6;
		}
		inf->ready = true;

	}
private:
	double filter(int sr,char w)
	{
		double sum=0;
		unsigned short halfNumCoef = (NumCoef/sr)>>1;
		if (w==1)
		{
			sum+=h[halfNumCoef]*shiftRegCos[halfNumCoef];
			for (int i=0;i<halfNumCoef;i++)
			{
				sum+=h[halfNumCoef+i]*(shiftRegCos[halfNumCoef-i]+shiftRegCos[halfNumCoef+i]);
			}
		}
		else
		{
			sum+=h[halfNumCoef]*shiftRegSin[halfNumCoef];
			for (int i=0;i<halfNumCoef;i++)
			{
				sum+=h[halfNumCoef+i]*(shiftRegSin[halfNumCoef-i]+shiftRegSin[halfNumCoef+i]);
			}
		}
		
		return sum*sr;
	}

	void shiftFilter(int sr)
	{
		for (int j=0;j<sr;j++)
		{
			for (int i = 0; i<NumCoef;i++)
			{
				shiftRegCos[341-i]=shiftRegCos[340-i];
				shiftRegSin[341-i]=shiftRegSin[340-i];
			}
		}
		
		return;
	}

	short buf[tmpBufSize];
	bool overflow;
	Interface *inf;
	Ui::IQdem *ui;
	phaseDiagramWidget **pdw;
};

class IQdem : public Interface
{
	Q_OBJECT
public:
    IQdem(QDialog *parent,marray** _arr, unsigned int *_size);
	~IQdem();
	iqDemWork * work;
private:
	Ui::IQdem ui;
	QDialog * dlg;
	phaseDiagramWidget *pdw;
		QTimer *sptime;
private slots:
		void update();
		void diagramShow();
		void phaseUpdate(int val);
		void clkUpdate(int val);

};

#endif // IQDEM_H
