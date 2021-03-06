#include "io.h"

io::io(QDialog *parent,marray** _arr, unsigned int *_size)
	: Interface(parent)
{
	ui.setupUi(this);
	moduleName = QString::fromUtf8("Пропуск данных");
	MODpixmap.load(":/images/btn_next_green.png");
	type = Io;
	work = new ioWork((void*)this,&ui);
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
	dlg = nullptr;
	sw = nullptr;
	//sptime = nullptr;


/**/

        top = groupManager->addProperty(QString::fromUtf8("Пропуск данных"));
        fileName = stringManager->addProperty(QString::fromUtf8("Путь к файлу"));
        stringManager->setValue(fileName,"test.pcm");
        bShowSpectrum = boolManager->addProperty(QString::fromUtf8("Показать спектр"));
        boolManager->setValue(bShowSpectrum,false);
        enumBufSize = enumManager->addProperty(QString::fromUtf8("Считывать, Кб"));
        QStringList enumNames;
        enumNames << "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128" << "256" << "512" << "1024" << "2048";
        enumManager->setEnumNames(enumBufSize, enumNames);
        IntBufLoad = intManager->addProperty(QString::fromUtf8("Загрузка буфера, %"));
        intManager->setRange(IntBufLoad, 0, 100);
        intManager->setValue(IntBufLoad,0);
        IntBufLoad->setEnabled(false);
        fDataReceived = doubleManager->addProperty(QString::fromUtf8("Данных принято, Мб"));
        doubleManager->setValue(fDataReceived,0);
        fDataReceived->setEnabled(false);
        flagitem = flagManager->addProperty(QString::fromUtf8("Синхронизация по..."));
        QStringList flagList;
        flagList << QString::fromUtf8("Фазе") << QString::fromUtf8("Тактам") <<  QString::fromUtf8("Частоте");
        flagManager->setFlagNames(flagitem,flagList);
        flagManager->setValue(flagitem,5);
        //subBoolManager =flagManager->subBoolPropertyManager();
        fAverageSpeed = doubleManager->addProperty(QString::fromUtf8("Средняя скорость, Мб/с"));
        doubleManager->setValue(fAverageSpeed,0);
        fAverageSpeed->setEnabled(false);
        connect(boolManager.get(),SIGNAL(valueChanged(QtProperty*,bool)),this,SLOT(boolValueChaged(QtProperty*,bool)));
        connect(enumManager.get(),SIGNAL(valueChanged(QtProperty*,int)),this,SLOT(enumValueChaged(QtProperty*,int)));
        top->addSubProperty(enumBufSize);
        top->addSubProperty(bShowSpectrum);
        top->addSubProperty(fDataReceived);
        top->addSubProperty(fAverageSpeed);
        top->addSubProperty(IntBufLoad);
        treeView = std::shared_ptr<QtTreePropertyBrowser> (new QtTreePropertyBrowser(this));
        treeView->setFactoryForManager(boolManager.get(), checkBoxFactory.get());
        treeView->setFactoryForManager(intManager.get(), spinBoxFactory.get());
        treeView->setFactoryForManager(stringManager.get(), lineEditFactory.get());
        treeView->setFactoryForManager(enumManager.get(), comboBoxFactory.get());
        treeView->setFactoryForManager(doubleManager.get(), doubleSpinFactory.get());
        treeView->setFactoryForManager(subBoolManager.get(), checkBoxFactory.get());
        treeView->addProperty(top);
        treeView->setGeometry(10,20,this->width()-20,this->height()-30);
        treeView->setResizeMode(QtTreePropertyBrowser::Interactive);
        treeView->setSplitterPosition(200);
        treeView->setHeaderVisible(false);
        treeView->setPropertiesWithoutValueMarked(true);
        treeView->setRootIsDecorated(false);
        treeView->show();
        /**/
}

io::~io()
{
	delete timer;
//	delete work;
}

void io::boolValueChaged (QtProperty *property, bool value)
{

    bool ok=false;
    if (property == bShowSpectrum)
      {
        if (value)
          {
            dlg = std::shared_ptr<QDialog> (new QDialog());
            sw = std::shared_ptr<spectrumWidget> (new spectrumWidget(dlg.get(),(short*)work->buf,2*1024*1024));
            //sw->setGeometry(0,0,1200,350); //600
            dlg->setGeometry(100,100,1200,350);
            dlg->setModal(false);
            dlg->setWindowTitle(QString::fromUtf8("Спектр"));
            dlg->show();
          connect(sw->time.get(),SIGNAL(timeout()),sw.get(),SLOT(timeoutRepaint()));
          }
        else
          {
            sw=nullptr;
            dlg=nullptr;

          }
      }
}


void io::enumValueChaged(QtProperty * property, int value)
{
  if (property == enumBufSize)
    {
        bool ok=false;
        QString s;
        s = enumManager->enumNames(enumBufSize).at(value);
        unsigned int k = s.toUInt(&ok);
        if (ok)
          {
                  work->dataRead = k;
          }

    }
}

void io::update()
{
	QString s;
	float a=work->dataCounter;
	float f = a/(1024*1024);
	doubleManager->setValue(fDataReceived,f);

	if (input.size()!=0) intManager->setValue(IntBufLoad,(int)input.at(0)->load());
	QDateTime time;
	time = QDateTime::currentDateTime();
	qint64 msecs = abs(time.msecsTo(oldTime));
	AverageSpeed =(a-oldCount)*1000/msecs;
	if (oldCount != a)
	  {
	    doubleManager->setValue(fAverageSpeed,AverageSpeed/(1024*1024));
	  }
	oldTime = time;
	oldCount = a;

}



