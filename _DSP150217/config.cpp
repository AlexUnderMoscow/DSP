#include "config.h"

config::config(QDialog *parent,marray** _arr, unsigned int *_size)
	: Interface(parent)
{
	ui.setupUi(this);
	moduleName = QString::fromUtf8("Конфигурация аппаратной платформы");
	MODpixmap.load(":/images/btn_next_green.png");
	type = Config;
	work = new configWork((void*)this,&ui);
	widgetAddr = (void*)work;
	form = this;
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(1000);
	oldTime.currentDateTime();
	createButton(this);
	arr = _arr;
	size=_size;
	dlg = NULL;
	sptime = NULL;

/*
	QtProperty *top;
	QtProperty *fileName;		//путь к файлу для обработки
	QtProperty *IntPercentFileLoad;	//процент отправки файла
	QtProperty *bLoadConf;		//старт загрузки параметров
	QtProperty *flagitem;		//рег 1 общее управление
	QtProperty *lampFlags;		//рег.1
	QtProperty *IntFreq;		//частота в герцах 4 000 000 Гц рег.2-3
	QtProperty *IntFreq2;		//частота в герцах 4 000 000 Гц рег.11-12
	QtProperty *fKuFreq;		//коэффициент усиления в петле частоты рег.4
	QtProperty *fKuPhase;		//коэффициент усиления в петле фазы рег.5
	QtProperty *enumFilterBandPass;	//выбор полосы пропускания фильтра
	QtProperty *enumEthDevice;	//выбор инерфейса
	QtProperty *IntClkFreq;		//символьная скорость  рег.9-10
	QtProperty *IntClkFreqRecv;	//символьная скорость приемника  рег.9-10
	QtProperty *fKuSymbSpeed;	//коэффициент усиления в петле символьной скорости рег.
	QtProperty *fKuCLK;		//коэффициент усиления в петле тактов рег.
	QtProperty *fAruRef;		//опорное значение АРУ рег.19
	QtProperty *fAruAmp;		//коэффициент усиления в петле АРУ рег.20
	QtProperty *fAruAcc;		//предустановка аккумулятора АРУ рег.18
*/

        top = groupManager->addProperty(QString::fromUtf8("Конфигурация АПК"));

        fileName = stringManager->addProperty(QString::fromUtf8("Путь к файлу"));
        stringManager->setValue(fileName,"sin.pcm"); //input.pcm

        bLoadConf = boolManager->addProperty(QString::fromUtf8("Загрузить конфигурацию"));
        boolManager->setValue(bLoadConf,false);

        enumFilterBandPass = enumManager->addProperty(QString::fromUtf8("Полоса пропускания ФНЧ, KHz"));
        QStringList enumNames;
        enumNames << "custom" <<"200" << "400" <<"500" << "700" << "800" << "1000" << "1200";
        enumManager->setEnumNames(enumFilterBandPass, enumNames);

        enumEthDevice = enumManager->addProperty(QString::fromUtf8("Сетевой контроллер"));


        IntFreq = intManager->addProperty(QString::fromUtf8("Начальная частота ОГ, Гц"));
        intManager->setRange(IntFreq, 0, 4000000);
        intManager->setValue(IntFreq,828000);
        IntFreq->setEnabled(true);

        IntPercentFileLoad = intManager->addProperty(QString::fromUtf8("Загрузка файла, %"));
        intManager->setRange(IntPercentFileLoad, 0, 100);
        intManager->setValue(IntPercentFileLoad,100);
        IntPercentFileLoad->setEnabled(false);

        IntFreq2 = intManager->addProperty(QString::fromUtf8("Начальная частота ОГ-2, Гц"));
        intManager->setRange(IntFreq2, 0, 4000000);
        intManager->setValue(IntFreq2,828000);  //821000
        IntFreq2->setEnabled(true);

        IntCurrentFreq = intManager->addProperty(QString::fromUtf8("Мгновенная частота ОГ, Гц"));
        intManager->setRange(IntCurrentFreq, 0, 4000000);
        intManager->setValue(IntCurrentFreq,821000);
        IntCurrentFreq->setEnabled(false);

        IntClkFreq = intManager->addProperty(QString::fromUtf8("Символьная скорость генератора"));
        intManager->setRange(IntClkFreq, 0, 1500000);
        intManager->setValue(IntClkFreq,200000);
        IntClkFreq->setEnabled(true);

        //QtProperty *IntClkFreqRecv;	//символьная скорость приемника  рег.9-10

        IntClkFreqRecv = intManager->addProperty(QString::fromUtf8("Символьная скорость приемника"));
        intManager->setRange(IntClkFreqRecv, 0, 1500000);
        intManager->setValue(IntClkFreqRecv,200030);
        IntClkFreqRecv->setEnabled(true);

        lampFlags = flagManager->addProperty(QString::fromUtf8("Световые индикаторы"));
         QStringList flagList;
         flagList << QString::fromUtf8("1") << QString::fromUtf8("2") <<  QString::fromUtf8("3") <<  QString::fromUtf8("4");
        flagManager->setFlagNames(lampFlags,flagList);
        flagManager->setValue(lampFlags,7);
        subBoolManager = flagManager->subBoolPropertyManager();

        flagitem = flagManager->addProperty(QString::fromUtf8("Подстройка..."));
         QStringList optionList;
         optionList << QString::fromUtf8("Фазы") << QString::fromUtf8("Тактов") <<  QString::fromUtf8("Амплитуды");
        flagManager->setFlagNames(flagitem,optionList);
        flagManager->setValue(flagitem,0);
        subBoolManager = flagManager->subBoolPropertyManager();

        fKuFreq = doubleManager->addProperty(QString::fromUtf8("K/y в петле частоты"));
        doubleManager->setValue(fKuFreq,-0.029);
        doubleManager->setDecimals(fKuFreq,3);
        fKuFreq->setEnabled(true);

        fKuPhase = doubleManager->addProperty(QString::fromUtf8("K/y в петле фазы"));
        doubleManager->setValue(fKuPhase,-0.779);
        doubleManager->setDecimals(fKuPhase,3);
        fKuPhase->setEnabled(true);

        fKuSymbSpeed = doubleManager->addProperty(QString::fromUtf8("K/y в петле симв.скор."));
        doubleManager->setValue(fKuSymbSpeed,-0.02);
        doubleManager->setDecimals(fKuSymbSpeed,3);
        fKuSymbSpeed->setEnabled(true);

        fKuCLK = doubleManager->addProperty(QString::fromUtf8("K/y в петле тактов"));
        doubleManager->setValue(fKuCLK,-0.01);
        doubleManager->setDecimals(fKuCLK,3);
        fKuCLK->setEnabled(true);

        fAruRef = doubleManager->addProperty(QString::fromUtf8("Опорное значение АРУ"));
        doubleManager->setValue(fAruRef,-0.375);
        doubleManager->setDecimals(fAruRef,3);
        fAruRef->setEnabled(true);

        fAruAmp = doubleManager->addProperty(QString::fromUtf8("K/y в петле АРУ"));
        doubleManager->setValue(fAruAmp,-0.015);
        doubleManager->setDecimals(fAruAmp,3);
        fAruAmp->setEnabled(true);

        fAruAcc = doubleManager->addProperty(QString::fromUtf8("Предустановка К/у АРУ"));
        doubleManager->setValue(fAruAcc, 5.1); //0.1
        doubleManager->setDecimals(fAruAcc,3);
        fAruAcc->setEnabled(true);

        connect(boolManager,SIGNAL(valueChanged(QtProperty*,bool)),this,SLOT(boolValueChaged(QtProperty*,bool)));
        connect(enumManager,SIGNAL(valueChanged(QtProperty*,int)),this,SLOT(enumValueChaged(QtProperty*,int)));

        top->addSubProperty(bLoadConf);
        top->addSubProperty(enumEthDevice);
        top->addSubProperty(flagitem);
        top->addSubProperty(IntFreq);
        top->addSubProperty(IntFreq2);
        //top->addSubProperty(IntCurrentFreq);
        top->addSubProperty(fKuFreq);
        top->addSubProperty(fKuPhase);
        top->addSubProperty(IntClkFreq);
        top->addSubProperty(IntClkFreqRecv);
        top->addSubProperty(fKuSymbSpeed);
        top->addSubProperty(fKuCLK);
        top->addSubProperty(fAruRef);
        top->addSubProperty(fAruAmp);
        top->addSubProperty(fAruAcc);
        top->addSubProperty(fileName);
        top->addSubProperty(IntPercentFileLoad);
        top->addSubProperty(enumFilterBandPass);
        top->addSubProperty(lampFlags);

        treeView = new QtTreePropertyBrowser(this);

        treeView->setFactoryForManager(boolManager, checkBoxFactory);
        treeView->setFactoryForManager(intManager, spinBoxFactory);
        treeView->setFactoryForManager(stringManager, lineEditFactory);
        treeView->setFactoryForManager(enumManager, comboBoxFactory);
        treeView->setFactoryForManager(doubleManager, doubleSpinFactory);
        treeView->setFactoryForManager(subBoolManager, checkBoxFactory);

        treeView->addProperty(top);
        treeView->setGeometry(10,20,this->width()-20,this->height()-30);
        treeView->setResizeMode(QtTreePropertyBrowser::Interactive);
        treeView->setSplitterPosition(200);
        treeView->setHeaderVisible(false);
        treeView->setPropertiesWithoutValueMarked(true);
        treeView->setRootIsDecorated(false);
        treeView->show();

        int i=0;
        if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
           {
               return;
           }
        QStringList eth;
        QString s;
        for(d=alldevs; d; d=d->next)
            {
            s.append(d->description);
            QString y = "'";
            int posStart = s.indexOf(y,0);
            int posEnd = s.indexOf(y,posStart+1);
            s=s.mid(posStart+1,posEnd-posStart-1);
            eth << s;
            s.clear();
             ++i;
            }

            if(i==0)
            {
               // printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
                return;
            }
            enumManager->setEnumNames(enumEthDevice, eth);
            enumManager->setValue(enumEthDevice,2);

}

config::~config()
{
	delete timer;
}

void config::freq2Code(unsigned short &Hi, unsigned short &Lo, unsigned int freq)
{
  double f = freq;
  double max = 4000000;
  double code = (0x003FFFFF*f)/max;
  unsigned int c = code;
  Hi = (c & 0xFFFF0000) >> 16;
  Lo = c & 0x0000FFFF;
}

unsigned int config::code2Freq(unsigned short &Hi, unsigned short &Lo)
{
  return 0;
}

double config::fixed2double(unsigned short val, unsigned int N, unsigned int Q)
{
  double result=0;
  unsigned short mask=0;
  unsigned short integer = val << (16-N+1);
  integer = integer >> (16-N+1);
  integer = integer >> Q;
  result+=integer;

  mask = 1;
  mask = mask << Q;                 //что такое 1 в этой системе счисления
  double one = mask;                //для пропорции 1
  mask = val;
  mask = mask << (16-Q);
  mask = mask >> (16-Q);
  double x = mask;                     //дробная часть
  result += (1.0 * x)/one;            //пропорцией определяем дробь в битах
  /*определяем знак и умножаем на -1 если отрицательный*/
  mask = 0x8000;
  mask = mask >> (16-N);
  if ((val & mask)!=0)
    {
      result=result*(-1);
    }
  return result;
}

unsigned short config::double2fixed(double val, unsigned int N, unsigned int Q)
{
  unsigned short result=0;
  unsigned short mask=0;
  unsigned short integer=0;
  unsigned short fractional=0;
  integer = abs(val);               //целая часть числа
  mask = integer << Q;              //двигается на количество бит дробной части
  result+=mask;                     //записываем в результат
  result = result << 16-N+1;
  result = result >> 16-N+1;        //очистка знака
  double frac = abs(val)-integer;   //дробь
  mask = 1;
  mask = mask << Q;                 //что такое 1 в этой системе счисления
  double one = mask;                //для пропорции 1
  double x = 0;
  x = (one * frac) /1.0;            //пропорцией определяем дробь в битах
  mask = x;
  result+=mask;

  mask = 0x8000;                    //где будет знак "-"?
  mask = mask >> (16-N);            //тут
  if (val<0)
    {
      result += mask;
    }
  return result;
}

void config::boolValueChaged (QtProperty *property, bool value)
{

int i;
    if (property == bLoadConf)
      {
        if (value)
          {//загрузка

            int inum = enumManager->value(enumEthDevice);

            for(d=alldevs, i=0; i< inum ;d=d->next, i++);

               /* Open the device */
               if ( (fp= pcap_open(d->name,          // name of the device
                                         65536,            // portion of the packet to capture.
                                                           // 65536 guarantees that the whole packet will be captured on all the link layers
                                         PCAP_OPENFLAG_PROMISCUOUS,    // promiscuous mode
                                         1000,             // read timeout
                                         NULL,             // authentication on the remote machine
                                         errbuf            // error buffer
                                         ) ) == NULL)
               {
                   //fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
                   /* Free the device list */
                   pcap_freealldevs(alldevs);
                   return;
               }
               //QMessageBox msgBox;
              // std::string descr(d->description);
               //msgBox.setText(QString::fromStdString(descr));
               //msgBox.exec();
               file = fopen( stringManager->value(fileName).toStdString().c_str(), "rb" );
               if ( file == NULL) {
                     //printf("\nCould not open file");
                   }
                 else {
                     packSend(&pack, fp, file);       //!!!!!!!!!!!
                 }
          }
        else
          { //останоака
              fclose(file);
          }
    }
}

void config::packSend(mypacket* pack, pcap_t* fp, FILE* file)
{
  unsigned short addr=0;
  unsigned short index = 0x1C;
  globalIndex = 0;

        pack->head.EtherType=0;               //0x1111; samples
        payload(pack,index);
          for (int j = 0; j< 6; j++)
            {
              pack->head.macdst[j] = 0x22;
              pack->head.macsrc[j] = 0x11;
           }
      // отправка даных
      int s= sizeof(mypacket);
      pcap_sendpacket(fp, (u_char*)pack, sizeof(mypacket) /* size */);


      pack->head.EtherType=0x1111;               //0x1111; samples
      /*открытие файла - погрузка 512 сэмплов в цикле*/
      fseek(file, 0L, SEEK_END);
      int fileSize = ftell(file);
      fseek(file, 0L, SEEK_SET);
      //printf("\nFile size = %d bytes", fileSize);
      int packCount = fileSize / packSize;
      work->setparams(file,fp,packet,pack,packCount);
}

void config::payload(mypacket* pack, unsigned short RegData)
{

  unsigned short dat;
  unsigned short* pdat = &dat;
  unsigned char* c;
  unsigned int pksz = (packSize >> 2);

  memset(pack->data,0,1024);
  //заливка данных
  globalIndex = 0;
/*лампы*/                     // 0-3
  dat = flagManager->value(lampFlags);
  RegSend(pack,dat,1);          //11  11    00 00 00 00   00 01
                                //    mux                 lamps
/*частота*/
  unsigned short Hi,Lo;
  freq2Code(Hi,Lo,intManager->value(IntFreq));
  RegSend(pack,Hi,3);
  RegSend(pack,Lo,2);

  freq2Code(Hi,Lo,intManager->value(IntFreq2));
  RegSend(pack,Lo,11); //GUN2Lo
  RegSend(pack,Hi,12); //GUN2Hi

  RegSend(pack,double2fixed(doubleManager->value(fKuFreq),16,9),4); //Ku freq
  RegSend(pack,double2fixed(doubleManager->value(fKuPhase),16,9),5); //Ku phase

  RegSend(pack,double2fixed(doubleManager->value(fKuSymbSpeed),16,9),6); //Ku freq
  RegSend(pack,double2fixed(doubleManager->value(fKuCLK),16,9),7); //Ku phase
 /*коэффициенты фильтра*/
  double *coeficient;
  switch (enumManager->value(enumFilterBandPass))
    {
      case 1:
          coeficient = (double*)filter200;
         break;
     case 2:
      coeficient = (double*)filter400;
       break;
     case 3:
      coeficient = (double*)filter500;
       break;
    case 4:
     coeficient = (double*)filter700;
      break;
    case  5:
     coeficient = (double*)filter800;
      break;
    case 6:
     coeficient = (double*)filter1000;
      break;
    case 7:
     coeficient = (double*)filter1200;
      break;
      default:
      coeficient = (double*)filter1200;
      break;
    }

  int index = 15;
  while (index!=-1)
    {
      RegSend(pack,double2fixed(coeficient[index--],10,9),13);
    }

  freq2Code(Hi,Lo,intManager->value(IntClkFreq));
  RegSend(pack,Hi,16); //HiFreqCLK
  RegSend(pack,Lo,15); //LoFreqCLK

  freq2Code(Hi,Lo,intManager->value(IntClkFreqRecv));
  RegSend(pack,Hi,10); //HiFreqCLK
  RegSend(pack,Lo,9); //LoFreqCLK

  RegSend(pack,double2fixed(doubleManager->value(fAruAcc),16,9),18); // 0.015 accumulator preset
  RegSend(pack,double2fixed(doubleManager->value(fAruRef),16,9),19); // (-1) * desired Amplitude max -0.375
  RegSend(pack,double2fixed(doubleManager->value(fAruAmp),16,9),20); // koefficient amp ARU         -0.015
//отсылаем бит 15 = 1 для старта CLK проекта на ПЛИС
  dat += 0x8000; ;
  RegSend(pack,dat,1);
  //данные готовы
}


void config::RegSend(mypacket* pack, unsigned short dat, unsigned char addr)
{
  unsigned short addrtosenf;
   unsigned short* pdat = &dat;
   *((unsigned char*)(&addrtosenf)+0)=*((unsigned char*)(pdat)+1);
   *((unsigned char*)(&addrtosenf)+1)=*((unsigned char*)(pdat)+0);
   pack->data[globalIndex].reg=addrtosenf;
   pack->data[globalIndex].addr=addr;
   pack->data[globalIndex].zero=0;
   globalIndex++;
  return;
}
void config::enumValueChaged(QtProperty * property, int value)
{
 /* if (property == enumBufSize)
    {
        bool ok=false;
        QString s;
        s = enumManager->enumNames(enumBufSize).at(value);
        unsigned int k = s.toUInt(&ok);
        if (ok)
          {
                  work->dataRead = k;
          }
    }*/
}

void config::update()
{
        intManager->setValue(IntPercentFileLoad,work->percentFileLoad);
}



