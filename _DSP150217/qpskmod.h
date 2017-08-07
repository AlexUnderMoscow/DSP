#ifndef QPSKMOD_H
#define QPSKMOD_H

#include "interface.h"
#include "ui_qpskmod.h"
#include <math.h>
#include <QVariant>
#define M 32
#define M_PI 3.14159265358979323846

//double AWGN_generator();

class qpskModWork
{
public:
	long long dataCounter;
	unsigned int flagCount;

	/*__declspec(align(16))*/ float shiftRegCos[341+39];
	/*__declspec(align(16))*/ float shiftRegSin[341+39];

	unsigned short NumCoef;
	double thistime;
	double timeInterval;
	double freq;
	double symbolRate;
	int sr;
	int amp;
	double freqCLK; // частота тактовых импульсов
	double startPhase; //фаза тактов
	float old,curr;  // значения синусоиды тактов
	int dataLength;
	bool first;
	double AmpAwgn;

	qpskModWork(void* _inf, Ui::QPSKmod *_ui)
	{
		inf = (Interface*)_inf;
		ui = _ui;
		dataCounter = 0;
		overflow = false;
		NumCoef = 341;

		timeInterval = 125E-10;
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
		amp = 800;

		old =0 ;
		curr = 0;
		startPhase = 0;
		first = false; ///
		dataLength = 0;
		AmpAwgn = 1;
	}
	~qpskModWork()
	{

	}
	void Do()
	{
		//	QTextStream out(&logFile);
			 float resSSE;				//тут результат будет

			int size=-1;
			size = inf->input.at(0)->read(buf,1); //10
			if (size!=1)
			{
                usleep(1);
				return;
			}

		for (int y=0;y<1;y++)
		{
			unsigned char maskCos=0x01;
			unsigned char maskSin=0x02;

			while (maskCos>0)
			{
				////сдвиг  дает 20% к скорости переписать
				for (int i = NumCoef; i >= 0; i-=1)
				{
					shiftRegCos[i+1] = shiftRegCos[i];
					shiftRegSin[i+1] = shiftRegSin[i];
				}
				/*__declspec(align(4))*/ float tmpCur;
				tmpCur = 10000*cos(2* M_PI *freqCLK*thistime+startPhase);		////переписать +10%
				
				if ((old < 0) && (tmpCur >= 0)   /* косинусоида перешла 0 */)
				{
					unsigned short res = buf[0] & maskCos;
					if (res>0)
					{
						shiftRegCos[0] = amp;
					}
					else
					{
						shiftRegCos[0] = -amp;
					}
					res = buf[0] & maskSin;
					if (res>0)
					{
						shiftRegSin[0] = amp;
					}
					else
					{
						shiftRegSin[0] = -amp;
					}
					

					if (maskSin == 128)   //10000000
					{
						maskCos =0;
						maskSin =0;
					}
					maskCos = maskCos << 2;
					maskSin = maskSin << 2;
				}
				else
				{
					shiftRegCos[0] = 0;
					shiftRegSin[0] = 0;
				}
				
				float sumCos=0;
				/**/
				

				resSSE = 0;										//обнулили результат
				unsigned short halfNumCoef = NumCoef>>1;
				resSSE+=coef[halfNumCoef]*shiftRegCos[halfNumCoef];
				for (int i=0;i<halfNumCoef;i++)
				{
					resSSE+=coef[halfNumCoef+i]*(shiftRegCos[halfNumCoef-i]+shiftRegCos[halfNumCoef+i]);
				}
				
				sumCos=200*resSSE*cos(thistime*freq*2*M_PI);		//переписать +10%
				float sumSin=0;
				/**/
				resSSE = 0;										//обнулили результат
				
				resSSE+=coef[halfNumCoef]*shiftRegSin[halfNumCoef];
	
				for (int i=0;i<halfNumCoef;i++)
				{
					resSSE+=coef[halfNumCoef+i]*(shiftRegSin[halfNumCoef-i]+shiftRegSin[halfNumCoef+i]);
				}
				/**/
				//sum = resSSE;
				sumSin=200*resSSE*sin(thistime*freq*2*M_PI);		//переписать +10%
				sumCos+=sumSin;
				if (ui->checkNoise->isChecked())
				{
					sumCos+=AmpAwgn*AWGN_generator();		//переписать +16%
				}
				//sum = tmpCur;
				short temp = (short)sumCos;
				short* buffer = (short*)&(buf[100]);
				*(buffer+dataLength/2)=(temp);
				//////////////////////////
				//out << temp<< "\n";
				//////////////////////////
				dataLength+=sizeof(short);
				thistime+=timeInterval;
				old = tmpCur;

			}
}
		int sz =-1;
		overflow = true;
		while (overflow)
		{
			sz = inf->output.at(0)->write(buf+100,dataLength);
			if (sz==dataLength)
			{
				dataCounter+=dataLength;
				overflow = false;
				dataLength=0;
			}
			else
			{
				overflow = true;
				if (inf->output.at(0)->halt) return;
                usleep(1);   //при перегрузке выходного буфера сон
			}
		}	
	}
	void init()
	{
		dataCounter = 0;
		thistime=0;
		freq = 8000000;	//типа 8 МГц по нашим раскладам оцифровка 40 МГц
		old =0 ;
		curr = 0;
		startPhase = 0;
		first = false; ///
		dataLength = 0;
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
		inf->ready = true;

		freqCLK = sr*1E6;
		dispCalc(ui->doubleSNR->value());

		for (int i=0;i<380;i++)		//очистка массива
		{
			coef[i] = 0;
			shiftRegCos[i]=0;
			shiftRegSin[i]=0;
		}
		for (int i=0;i<341;i++)		//загрузка массива
		{
			coef[i] = h[i];
		}

		for (unsigned int i = 0 ; i<inf->input.size();i++)
			inf->input.at(i)->setData();
	}

	void dispCalc(double db)
	{
		AmpAwgn = 1*sqrt((2000*2000*(1/freqCLK))/(timeInterval*4*pow(10,db/10)*1));
	}

private:
	float filter()
	{
		float sum=0;
		for (int i=0;i<NumCoef;i++)
		{
			sum=sum+h[i]*shiftRegCos[i+1];
		}
		return sum;
	}
	float coef[341+39];

	double AWGN_generator()
	{/* Generates additive white Gaussian Noise samples with zero mean and a standard deviation of 1. */

		double temp1;
		double temp2;
		double result;
		int p;
		p = 1;
		while( p > 0 )
		{
			temp2 = ( rand() / ( (double)RAND_MAX ) ); /*  rand() function generates an
													   integer between 0 and  RAND_MAX,
													   which is defined in stdlib.h.
													   */
			if ( temp2 == 0 )
			{// temp2 is >= (RAND_MAX / 2)
				p = 1;
			}// end if
			else
			{// temp2 is < (RAND_MAX / 2)
				p = -1;
			}// end else

		}// end while()

		temp1 = cos( ( 2.0 * (double)M_PI ) * rand() / ( (double)RAND_MAX ) );
		result = sqrt( -2.0 * log( temp2 ) ) * temp1;
		return result;        // return the generated random sample to the caller
	}// end AWGN_generator()

	char buf[tmpBufSize];
	bool overflow;
	Interface *inf;
	Ui::QPSKmod *ui;
};

class QPSKmod : public Interface
{
	Q_OBJECT

public:
    QPSKmod(QDialog *parent, marray** _arr, unsigned int *_size);
	~QPSKmod();
	qpskModWork * work;

private:
	Ui::QPSKmod ui;
private slots:
	void update();
};

#endif // QPSKMOD_H
