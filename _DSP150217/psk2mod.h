#ifndef PSK2MOD_H
#define PSK2MOD_H

#include <QTimer>
#include <QString>
#include <QVariant>
#include "interface.h"
#include "ui_psk2mod.h"
#include <cmath>
#define M_PI 3.14159265358979323846


class psk2ModWork
{
public:
	long long dataCounter;
	unsigned int flagCount;
	short shiftReg[350];
	unsigned short NumCoef;
	double thistime;
	double timeInterval;
	double freq;
	double symbolRate;
	int sr;
	int amp;
	double freqCLK; // частота тактовых импульсов
	double startPhase; //фаза тактов
	short old,curr;  // значения синусоиды тактов
	int dataLength;
	bool first;
	double AmpAwgn;
//////////////////////////////////
//	QFile logFile;
//	COperations ops;
//////////////////////////////////
	psk2ModWork(void* _inf, Ui::psk2Mod *_ui)
	{
		inf = (Interface*)_inf;
		ui = _ui;
		dataCounter = 0;
		overflow = false;
		NumCoef = 341;
		memset(shiftReg,0,(NumCoef+7)*sizeof(short));
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
////////////////////////////////////
	//	log.setFileName("testShort.txt");
	//	log.open(QIODevice::WriteOnly | QIODevice::Text);


	//

	}
	~psk2ModWork()
	{

	}
	void Do()
	{
	//	QTextStream out(&logFile);
			int size=-1;
			size = inf->input.at(0)->read(buf,1); //10
			if (size!=1)
			{
                usleep(1);
				return;
			}

		for (int j=0;j<1;j++)
		{
			unsigned char mask=0x01;

			while (mask>0)
			{
				curr = (short)(10000*cos(2 * M_PI *freqCLK*thistime+startPhase));

				if ((old < 0) && (curr >= 0)   /* косинусоида перешла 0 */)
				{
					unsigned short res = buf[j] & mask;
					if (res>0)  //единица
					{
						shiftReg[0] = amp;
					}
					else	//ноль
					{
						shiftReg[0] = -amp;
					}

					if (mask == 128)
					{
						mask =0;
					}
					mask = mask << 1;
				}
				else
				{
					shiftReg[0] = 0;
				}

				////сдвиг
				for (int j=0; j<sr;j++)
				{
					for (int i = NumCoef; i >= 0; i-=1)
					{
						shiftReg[i+1] = shiftReg[i];
					}
					shiftReg[0]=0;
				}
				////		

				float sum = filter();
				sum=200*sum*cos(thistime*freq*2*M_PI);
				if (ui->checkNoise->isChecked())
				{
					sum+=AmpAwgn*AWGN_generator();
				}
				short temp = (short)sum;
				short* buffer = (short*)&(buf[100]);
				*(buffer+dataLength/2)=(temp);
				//////////////////////////
				//out << temp<< "\n";
				//////////////////////////
				dataLength+=sizeof(short);
				thistime+=timeInterval;
				old = curr;

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
				if (inf->input.at(0)->halt) return;
                usleep(1);   //при перегрузке выходного буфера сон
			}
		}
		
	}
	void init()
	{
		memset(shiftReg,0,(NumCoef+8)*sizeof(short));
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
		for (unsigned int i = 0 ; i<inf->input.size();i++)
			inf->input.at(i)->setData();

	}

	void dispCalc(double db)
	{
		AmpAwgn = sqrt((2000*2000*(1/freqCLK))/(timeInterval*4*pow(10,db/10)*1));
	}
private:
	float filter()
	{
		const double h[] = {				//импульсная характеристика
			4.09661210914523E-04,
			4.27563171908392E-04,
			4.44422556755601E-04,
			4.60148152540584E-04,
			4.74649822147305E-04,
			4.87838795598329E-04,
			4.99627963580757E-04,
			5.09932172400618E-04,
			5.18668519592585E-04,
			5.2575664939908E-04,
			5.31119047321953E-04,
			5.34681332941112E-04,
			5.36372550187661E-04,
			5.36125454254468E-04,
			5.33876794324478E-04,
			5.29567591296745E-04,
			5.23143409691933E-04,
			5.14554622923052E-04,
			5.03756671123377E-04,
			4.90710310732002E-04,
			4.75381855048071E-04,
			4.57743404977666E-04,
			4.37773069212371E-04,
			4.15455173095825E-04,
			3.90780455454005E-04,
			3.63746252686514E-04,
			3.3435666943986E-04,
			3.02622735209362E-04,
			2.685625462441E-04,
			2.32201392158925E-04,
			1.93571866689176E-04,
			1.52713962057098E-04,
			1.09675146454096E-04,
			6.45104241798303E-05,
			1.72823780175274E-05,
			-3.19388065351193E-05,
			-8.30753355187856E-05,
			-1.36041819545648E-04,
			-1.90745305865519E-04,
			-2.470853296726E-04,
			-3.04953984814419E-04,
			-3.64236014017544E-04,
			-4.24808918696923E-04,
			-4.86543088367205E-04,
			-5.49301949625474E-04,
			-6.12942134625346E-04,
			-6.77313668912673E-04,
			-7.42260178443178E-04,
			-8.07619115552347E-04,
			-8.73222003598067E-04,
			-9.38894699946736E-04,
			-1.00445767692428E-03,
			-1.06972632030455E-03,
			-1.13451124485941E-03,
			-1.19861862644698E-03,
			-1.26185055006823E-03,
			-1.32400537327582E-03,
			-1.38487810427496E-03,
			-1.44426079401233E-03,
			-1.50194294150724E-03,
			-1.55771191163842E-03,
			-1.61135336456105E-03,
			-1.66265169589127E-03,
			-1.71139048675977E-03,
			-1.75735296280262E-03,
			-1.80032246112604E-03,
			-1.84008290425189E-03,
			-1.87641928002404E-03,
			-1.90911812643012E-03,
			-1.93796802027111E-03,
			-1.96276006859058E-03,
			-1.98328840175811E-03,
			-1.99935066708589E-03,
			-2.01074852184559E-03,
			-2.01728812454212E-03,
			-2.01878062329449E-03,
			-2.01504264016918E-03,
			-2.00589675031015E-03,
			-1.99117195471095E-03,
			-1.9707041454783E-03,
			-1.94433656244385E-03,
			-1.91192023999011E-03,
			-1.87331444296988E-03,
			-1.82838709061326E-03,
			-1.77701516733528E-03,
			-1.71908511937764E-03,
			-1.65449323624238E-03,
			-1.58314601590145E-03,
			-1.50496051279544E-03,
			-1.41986466766652E-03,
			-1.32779761830487E-03,
			-1.22870999032465E-03,
			-1.12256416712471E-03,
			-1.00933453823071E-03,
			-8.89007725258965E-04,
			-7.61582784788143E-04,
			-6.27071387472708E-04,
			-4.85497972781792E-04,
			-3.3689987879871E-04,
			-1.81327446569437E-04,
			-1.88440985433488E-05,
			1.50473609294471E-04,
			3.26535961942366E-04,
			5.09240086837051E-04,
			6.98469982469029E-04,
			8.94096574322466E-04,
			1.09597779824214E-03,
			1.30395871126813E-03,
			1.51787162991648E-03,
			1.73753629582124E-03,
			1.9627600685906E-03,
			2.19333814566672E-03,
			2.4290538089163E-03,
			2.66967869761642E-03,
			2.91497310743836E-03,
			3.16468631497048E-03,
			3.41855692726115E-03,
			3.67631325580259E-03,
			3.93767371431816E-03,
			4.2023472396583E-03,
			4.47003373505422E-03,
			4.74042453492403E-03,
			5.01320289037298E-03,
			5.28804447447875E-03,
			5.56461790640312E-03,
			5.84258529332462E-03,
			6.12160278914144E-03,
			6.40132116885159E-03,
			6.68138641747633E-03,
			6.96144033235583E-03,
			7.24112113761075E-03,
			7.52006410952828E-03,
			7.79790221160727E-03,
			8.07426673796471E-03,
			8.3487879637815E-03,
			8.62109580145024E-03,
			8.8908204610638E-03,
			9.15759311387158E-03,
			9.42104655731774E-03,
			9.68081588026747E-03,
			9.93653912702185E-03,
			0.01018785795872,
			1.04344183107279E-02,
			1.06758710446187E-02,
			1.09118725933558E-02,
			1.11420855983011E-02,
			1.13661795366867E-02,
			1.15838313382024E-02,
			1.17947259893755E-02,
			1.19985571244396E-02,
			1.21950276014185E-02,
			1.23838500621795E-02,
			1.25647474752429E-02,
			0.012737453660171,
			1.29017137923975E-02,
			1.30572848873992E-02,
			1.32039362631547E-02,
			1.33414499798836E-02,
			1.34696212561045E-02,
			1.35882588601026E-02,
			1.36971854759526E-02,
			1.37962380432931E-02,
			1.38852680701095E-02,
			1.39641419178389E-02,
			1.40327410581727E-02,
			1.40909623009906E-02,
			1.41387179929286E-02,
			1.41759361861414E-02,
			1.42025607768906E-02,
			1.42185516136528E-02,
			1.42238845745103E-02,
			1.42185516136528E-02,
			1.42025607768906E-02,
			1.41759361861414E-02,
			1.41387179929286E-02,
			1.40909623009906E-02,
			1.40327410581727E-02,
			1.39641419178389E-02,
			1.38852680701095E-02,
			1.37962380432931E-02,
			1.36971854759526E-02,
			1.35882588601026E-02,
			1.34696212561045E-02,
			1.33414499798836E-02,
			1.32039362631547E-02,
			1.30572848873992E-02,
			1.29017137923975E-02,
			0.012737453660171,
			1.25647474752429E-02,
			1.23838500621795E-02,
			1.21950276014185E-02,
			1.19985571244396E-02,
			1.17947259893755E-02,
			1.15838313382024E-02,
			1.13661795366867E-02,
			1.11420855983011E-02,
			1.09118725933558E-02,
			1.06758710446187E-02,
			1.04344183107279E-02,
			0.01018785795872,
			9.93653912702185E-03,
			9.68081588026747E-03,
			9.42104655731774E-03,
			9.15759311387158E-03,
			8.8908204610638E-03,
			8.62109580145024E-03,
			8.3487879637815E-03,
			8.07426673796471E-03,
			7.79790221160727E-03,
			7.52006410952828E-03,
			7.24112113761075E-03,
			6.96144033235583E-03,
			6.68138641747633E-03,
			6.40132116885159E-03,
			6.12160278914144E-03,
			5.84258529332462E-03,
			5.56461790640312E-03,
			5.28804447447875E-03,
			5.01320289037298E-03,
			4.74042453492403E-03,
			4.47003373505422E-03,
			4.2023472396583E-03,
			3.93767371431816E-03,
			3.67631325580259E-03,
			3.41855692726115E-03,
			3.16468631497048E-03,
			2.91497310743836E-03,
			2.66967869761642E-03,
			2.4290538089163E-03,
			2.19333814566672E-03,
			1.9627600685906E-03,
			1.73753629582124E-03,
			1.51787162991648E-03,
			1.30395871126813E-03,
			1.09597779824214E-03,
			8.94096574322466E-04,
			6.98469982469029E-04,
			5.09240086837051E-04,
			3.26535961942366E-04,
			1.50473609294471E-04,
			-1.88440985433488E-05,
			-1.81327446569437E-04,
			-3.3689987879871E-04,
			-4.85497972781792E-04,
			-6.27071387472708E-04,
			-7.61582784788143E-04,
			-8.89007725258965E-04,
			-1.00933453823071E-03,
			-1.12256416712471E-03,
			-1.22870999032465E-03,
			-1.32779761830487E-03,
			-1.41986466766652E-03,
			-1.50496051279544E-03,
			-1.58314601590145E-03,
			-1.65449323624238E-03,
			-1.71908511937764E-03,
			-1.77701516733528E-03,
			-1.82838709061326E-03,
			-1.87331444296988E-03,
			-1.91192023999011E-03,
			-1.94433656244385E-03,
			-1.9707041454783E-03,
			-1.99117195471095E-03,
			-2.00589675031015E-03,
			-2.01504264016918E-03,
			-2.01878062329449E-03,
			-2.01728812454212E-03,
			-2.01074852184559E-03,
			-1.99935066708589E-03,
			-1.98328840175811E-03,
			-1.96276006859058E-03,
			-1.93796802027111E-03,
			-1.90911812643012E-03,
			-1.87641928002404E-03,
			-1.84008290425189E-03,
			-1.80032246112604E-03,
			-1.75735296280262E-03,
			-1.71139048675977E-03,
			-1.66265169589127E-03,
			-1.61135336456105E-03,
			-1.55771191163842E-03,
			-1.50194294150724E-03,
			-1.44426079401233E-03,
			-1.38487810427496E-03,
			-1.32400537327582E-03,
			-1.26185055006823E-03,
			-1.19861862644698E-03,
			-1.13451124485941E-03,
			-1.06972632030455E-03,
			-1.00445767692428E-03,
			-9.38894699946736E-04,
			-8.73222003598067E-04,
			-8.07619115552347E-04,
			-7.42260178443178E-04,
			-6.77313668912673E-04,
			-6.12942134625346E-04,
			-5.49301949625474E-04,
			-4.86543088367205E-04,
			-4.24808918696923E-04,
			-3.64236014017544E-04,
			-3.04953984814419E-04,
			-2.470853296726E-04,
			-1.90745305865519E-04,
			-1.36041819545648E-04,
			-8.30753355187856E-05,
			-3.19388065351193E-05,
			1.72823780175274E-05,
			6.45104241798303E-05,
			1.09675146454096E-04,
			1.52713962057098E-04,
			1.93571866689176E-04,
			2.32201392158925E-04,
			2.685625462441E-04,
			3.02622735209362E-04,
			3.3435666943986E-04,
			3.63746252686514E-04,
			3.90780455454005E-04,
			4.15455173095825E-04,
			4.37773069212371E-04,
			4.57743404977666E-04,
			4.75381855048071E-04,
			4.90710310732002E-04,
			5.03756671123377E-04,
			5.14554622923052E-04,
			5.23143409691933E-04,
			5.29567591296745E-04,
			5.33876794324478E-04,
			5.36125454254468E-04,
			5.36372550187661E-04,
			5.34681332941112E-04,
			5.31119047321953E-04,
			5.2575664939908E-04,
			5.18668519592585E-04,
			5.09932172400618E-04,
			4.99627963580757E-04,
			4.87838795598329E-04,
			4.74649822147305E-04,
			4.60148152540584E-04,
			4.44422556755601E-04,
			4.27563171908392E-04,
			4.09661210914523E-04
		};
		
		float sum=0;
		for (int i=0;i<NumCoef;i++)
		{
			sum=sum+h[i]*shiftReg[i+1];
		}
		
		return sum;
	}

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

		temp1 = cos( ( 2.0 * M_PI ) * rand() / ( (double)RAND_MAX ) );
		result = sqrt( -2.0 * log( temp2 ) ) * temp1;


		return result;        // return the generated random sample to the caller

	}// end AWGN_generator()

	char buf[tmpBufSize];
	bool overflow;
	Interface *inf;
	Ui::psk2Mod *ui;
};

class psk2Mod : public Interface
{
	Q_OBJECT

public:
    psk2Mod(QDialog *parent,marray** _arr, unsigned int *_size);
	~psk2Mod();
	psk2ModWork * work;

private:
	Ui::psk2Mod ui;
private slots:
		void update();
		
};
#endif // PSK2MOD_H