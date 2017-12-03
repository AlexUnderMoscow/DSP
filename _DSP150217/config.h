#ifndef CONFIG_H
#define CONFIG_H

#define PLATFORM_WINDOWS  1
#define PLATFORM_UNIX  2

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#else
#define PLATFORM PLATFOR_UNIX
#endif


#if PLATFORM==PLATFORM_WINDOWS
#include <stdio.h>
#include <string.h>


#include <Windows.h>
#include "winsock.h"
//#pragma comment(lib,"libws2_32.a")
#pragma
#define HAVE_REMOTE
#include "pcap.h"
#include <iostream>
#include <conio.h>

#else


//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <sys/types.h>
//#include <iostream>
//#include <errno.h>
//#include <time.h>
//#include <sys/un.h>
//#include <unistd.h>
#endif


#include <QTimer>
#ifdef min
#undef min
#endif
#include <QTime>
#include <QVariant>
#include <QMessageBox>
#define packSize 512
#include "interface.h"
#include "ui_config.h"

struct header{
  char macdst[6];
  char macsrc[6];
  short EtherType;
};

struct ConfElement{
  unsigned short reg;
  unsigned char addr;
  char zero;
};

struct mypacket{
  header head;
  ConfElement data[packSize >> 2];
};

class configWork
{
public:
	long long dataCounter;
	unsigned int dataRead;  //по сколько Kбайт считывать
	Interface *inf;

	FILE *file;
	pcap_t *fp;
	u_char *packet;
	mypacket *pack;
	int packCount;
	int index;

	int percentFileLoad;
	int maxPackCount;

	configWork(void* _inf, Ui::config *_ui)
	{
		inf = (Interface*)_inf;
		inf->ready = true;
		ui = _ui;
		dataCounter = 0;
		overflow = false;
		packCount = 0;
		index = 0;
		percentFileLoad = 0;
		maxPackCount = 0;

	}

	~configWork()
	{
	//	inf = nullptr;
	}
	void setparams(FILE *_file,
		      pcap_t *_fp,
		      u_char *_packet,
		      mypacket *_pack,
		      int _packCount)
	{
	  file = _file;
	  fp = _fp;
	  packet = _packet;
	  pack = _pack;
	  packCount = _packCount;
	  maxPackCount =  _packCount;
	}

	void Do()
	{
		if (packCount!=0)
		  {
		    fseek(file, index*packSize, SEEK_SET);
		    fread((void*)pack->data,sizeof(char),packSize,file);
		//   while (pcap_sendpacket(fp, (u_char*)pack, sizeof(mypacket))!=0)
		//     {
		//       usleep(1);
		//     }
		    packCount--;
		    index++;
		    percentFileLoad = ((double)packCount * 100) / (double)maxPackCount;
		    usleep(1);   //при перегрузке выходного буфера сон
		  }
		else
		  {
		    index = 0;
		    usleep(1);
		  }


	}
	void init()
	{
		inf->ready = true;
	}
private:

	bool overflow;
	Ui::config *ui;
};
////////////////////ФОРМА/////////////////////////////
class config : public Interface
{
	Q_OBJECT
public:
   config(QDialog *parent,marray** _arr, unsigned int *_size);
        ~config();
        configWork * work;

	QtProperty *top;
	QtProperty *fileName;		//путь к файлу для обработки
	QtProperty *IntPercentFileLoad;	//процент отправки файла
	QtProperty *bLoadConf;		//старт загрузки параметров
	QtProperty *flagitem;		//рег 1 общее управление
	QtProperty *lampFlags;		//рег.1
	QtProperty *IntFreq;		//частота в герцах 4 000 000 Гц рег.2-3
	QtProperty *IntFreq2;		//частота в герцах 4 000 000 Гц рег.11-12
	QtProperty *IntCurrentFreq;	//мгновенная частота (с внутреннего регистра)
	QtProperty *fKuFreq;		//коэффициент усиления в петле частоты рег.4
	QtProperty *fKuPhase;		//коэффициент усиления в петле фазы рег.5
	QtProperty *enumFilterBandPass;	//выбор полосы пропускания фильтра
	QtProperty *enumEthDevice;	//выбор инерфейса
	QtProperty *IntClkFreq;		//символьная скорость генератора  рег.15-16
	QtProperty *IntClkFreqRecv;	//символьная скорость приемника  рег.9-10
	QtProperty *fKuSymbSpeed;	//коэффициент усиления в петле символьной скорости рег.
	QtProperty *fKuCLK;		//коэффициент усиления в петле тактов рег.
	QtProperty *fAruRef;		//опорное значение АРУ рег.19
	QtProperty *fAruAmp;		//коэффициент усиления в петле АРУ рег.20
	QtProperty *fAruAcc;		//предустановка аккумулятора АРУ рег.18


	/**/
private slots:
	void update();
	void boolValueChaged (QtProperty *property, bool value);
	void enumValueChaged (QtProperty *property, int value);

private:
	unsigned short double2fixed(double val, unsigned int N, unsigned int Q);
	double fixed2double(unsigned short val, unsigned int N, unsigned int Q);
	void freq2Code(unsigned short &Hi, unsigned short &Lo, unsigned int freq);
	unsigned int code2Freq(unsigned short &Hi, unsigned short &Lo);
	void packSend(mypacket* pack, pcap_t* fp, FILE* file);
	void payload(mypacket* pack, unsigned short RegData);
	void RegSend(mypacket* pack, unsigned short dat, unsigned char addr);

	FILE *file;
	unsigned char* buf;
	pcap_t *fp;
	pcap_if_t *alldevs, *d;
	char errbuf[PCAP_ERRBUF_SIZE];
	u_char packet[1032];
	mypacket pack;
	int globalIndex;

	const double filter200[16] = {
	  -0.001037011201042,-0.006065575144367,-0.007381718515173, -0.01051308196628,
	     -0.01204964907387,   -0.011352014394,-0.007196644006419, 0.001219174988818,
	      0.01418976576571,  0.03129120832032,  0.05134497069303,  0.07249811567114,
	      0.09246360270154,   0.1088756902069,   0.1196666836362,   0.1234289907801
	};

	const double filter400[16] = {
	  0.01545964404132,0.0006225963426453,-0.003570751687059,-0.009941763828672,
	     -0.01701476694219, -0.02262128971242, -0.02424405647202, -0.01957602635584,
	     -0.00715972674891,  0.01325187854643,  0.04025315755506,  0.07098541433223,
		0.101486971282,   0.1274007859154,   0.1447895289718,   0.1509052778523
	};

	const double filter500[16] = {
	    0.02794453748057, 0.006955368489024, 0.002287831842788,-0.005997895623596,
	    -0.0162944968733, -0.02591976548514, -0.03163839842621, -0.03019297483008,
	   -0.01933804259281, 0.001644950990816,  0.03139840235935,  0.06656190097559,
	     0.1022706100076,   0.1330289455753,   0.1538360989079,   0.1611886808673
	};

	const double filter700[16] = {
	  -0.02192767446813,  0.01154647218132,   0.0176279405585,  0.02278528466617,
	     0.02173279998809,  0.01152067915811,-0.006663329036219, -0.02693658376735,
	    -0.04016861379769, -0.03705391649289, -0.01184201630476,  0.03468567806659,
	     0.09436269707577,   0.1534551759559,   0.1968781959844,   0.2128091721926
	};

	const double filter800[16] = {
	  -0.002825514748134, -0.01915103411927,-0.007660391681611,-0.0001544920219404,
	      0.0131068457342,  0.02202594711636,  0.01858341270118,-6.31385633044e-05,
	    -0.02703614266548, -0.04669467210633, -0.04117444591557,6.410702801986e-05,
	     0.07275022075009,   0.1569590270803,   0.2242560158253,   0.2499332443581
	};

	const double filter1000[16] = {
	  0.01513400550756, 0.004916281366272,-0.005317530372892,  -0.0160543771664,
	     -0.01625973609252,-0.001151600553125,   0.0203709047867,  0.02920804031125,
	      0.01091143006238, -0.02841594628672, -0.05929574056057,  -0.0451839031154,
	      0.03089273924601,   0.1484565565893,   0.2558304890425,   0.2992053245912
	};

	const double filter1200[16] = {
	  -0.01060193050063,  0.02750712493783,   0.0206481452223, 0.004677801118632,
	     -0.01652442008022, -0.02314844240953,-0.003457121428276,  0.02772345592789,
	      0.03692640660158, 0.003908342716145,  -0.0499617049262, -0.06840162740359,
	    -0.004160944232424,   0.1344295451425,   0.2771315693439,   0.3375774556257
	};

	Ui::config ui;
	QDateTime oldTime;
	QDialog * dlg;
	QTimer *sptime;


};



#endif // CONFIG_H

