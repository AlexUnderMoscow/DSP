

#include <QtGui>
#include <QLabel>
#include "mainwindow.h"
#include "diagramitem.h"
#include "diagramscene.h"
//#include "diagramtextitem.h"
#include "arrow.h"


const int InsertTextButton = 10000;

//! [0]
MainWindow::MainWindow()
{
    vecAct.clear();
    createActions();
    createToolBox();
    createMenus();
    scanDirectory();

    scene = new DiagramScene(itemMenu, this);
    scene->setSceneRect(QRectF(0, 0, 1980, 1200));
    connect(scene, SIGNAL(itemInserted(DiagramItem*)),
            this, SLOT(itemInserted(DiagramItem*)));
    connect(scene, SIGNAL(textInserted(QGraphicsTextItem*)),
        this, SLOT(textInserted(QGraphicsTextItem*)));
    connect(scene, SIGNAL(itemSelected(QGraphicsItem*)),
        this, SLOT(itemSelected(QGraphicsItem*)));
    createToolbars();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(toolBox);
    view = new QGraphicsView(scene);
    layout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    setWindowTitle(QString::fromUtf8("Обработка данных"));
    setUnifiedTitleAndToolBarOnMac(true);
	sceneScaleChanged();
	backgroundButtonGroupClicked();
    man = new Manager(this,scene,(marray**) &scene->elList, &scene->listSize);
	txt[0] = 0;

}

void MainWindow::backgroundButtonGroupClicked()
{
    scene->setBackgroundBrush(QPixmap(":/images/background3.png"));
    scene->update();
    view->update();
}

void MainWindow::buttonGroupClicked(int id) // клик на панели кнопок
{
    QList<QAbstractButton *> buttons = buttonGroup->buttons();
	QAbstractButton *b;
    foreach (QAbstractButton *button, buttons) {
    if (buttonGroup->button(id) != button)
        button->setChecked(false);
		b = button;
    }
    if (id == InsertTextButton) {
        scene->setMode(DiagramScene::InsertText);
    } else {
        scene->setItemType(moduleType(id),b);
        scene->setMode(DiagramScene::InsertItem);
    }
}

MainWindow::~MainWindow()
{
        //man->stopProcessing();
  mainClose();
}
void MainWindow::deleteItem()
{
	createArray();
	if (man->inWork) return;
    procThread th((marray**)&scene->elList,&scene->listSize);
	for (int index=0;index<scene->vel.size();index++)
	{
		if (scene->vel.at(index).inter->le->text()=="X")
		{
			Interface *temp = scene->vel.at(index).inter;
			Interface **t = &temp;
			th.proc(Delete,scene->vel.at(index).inter->type,nullptr,nullptr,
				t,nullptr);
			scene->vel.at(index).inter->widgetAddr=nullptr;
		}
	}
	for (int i=0;i<scene->vel.size();i++)
	{
		if (scene->vel.at(i).inter->widgetAddr==NULL)
		{
			scene->vel.remove(i);
		}
	}
}

void MainWindow::pointerGroupClicked(int)
{
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));//выбор режима по нажатию на кнопку
	if (scene->myMode==DiagramScene::InsertItem)
	{
		scene->myMode = DiagramScene::InsertText;
		scene->start_w=nullptr;
	}
}

void MainWindow::itemInserted(DiagramItem *item)				//вставлен блок
{
    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true); //вверху стрелка загорается
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));  //режим выбирается из рассчета состояния кнопки вверху
    buttonGroup->button(int(item->diagramType()))->setChecked(false); //на панели блоков нажатость убирается
}

void MainWindow::textInserted(QGraphicsTextItem *)
{
    buttonGroup->button(InsertTextButton)->setChecked(false);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}

void MainWindow::sceneScaleChanged()				//установление масштаба изображения
{
    double newScale = 1;							//один к одному
    QMatrix oldMatrix = view->matrix();
    view->resetMatrix();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
}

void MainWindow::fillButtonTriggered()
{
   // scene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
}

void MainWindow::lineButtonTriggered()
{
  //  scene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
}

void MainWindow::handleFontChange()
{
}

void MainWindow::itemSelected(QGraphicsItem *item)
{
}

void MainWindow::about()
{
    QMessageBox::about(this, QString::fromUtf8("Обработка данных"),
        QString::fromUtf8("ПО <b>DSP</b> для  "
                          "создания тракта обработки данных."));
}

void MainWindow::createToolBox()
{
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);
    connect(buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(buttonGroupClicked(int)));
    QGridLayout *layout = new QGridLayout;
    int i = 0;

    layout->addWidget(createCellWidget(QString::fromUtf8("Файл"),File),i++, 0);
    layout->addWidget(createCellWidget(QString::fromUtf8("Адаптивный фильтр"),Adaptive),i++, 0);
    layout->addWidget(createCellWidget(QString::fromUtf8("Сохранить в файл"),OutputFile),i++, 0);
    layout->addWidget(createCellWidget(QString::fromUtf8("Пропуск данных"),Io),i++, 0);
    layout->addWidget(createCellWidget(QString::fromUtf8("Битовый генератор"),bitGen),i++, 0);
    layout->addWidget(createCellWidget(QString::fromUtf8("PSK модулятор"),PSK2modulator),i++, 0);
    layout->addWidget(createCellWidget(QString::fromUtf8("IQ демодулятор"),IQDemodulator),i++, 0);
    layout->addWidget(createCellWidget(QString::fromUtf8("Сумма"),Summ),i++, 0);
    layout->addWidget(createCellWidget(QString::fromUtf8("Побитное сравнение"),Xormod),i++, 0);
    layout->addWidget(createCellWidget(QString::fromUtf8("Модулятор QPSK"),QPSKmodulator),i++, 0);
    layout->addWidget(createCellWidget(QString::fromUtf8("UDP приемник"),test1mod),i++, 0);
    layout->addWidget(createCellWidget(QString::fromUtf8("Конфигуратор АПК"),Config),i++, 0);

    QGridLayout *textLayout = new QGridLayout;
    QWidget *textWidget = new QWidget;
    textWidget->setLayout(textLayout);
    layout->addWidget(textWidget, 1, 1);
    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);
    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(layout);
    backgroundButtonGroup = new QButtonGroup(this);
   // connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
   //         this, SLOT(backgroundButtonGroupClicked(QAbstractButton*)));
    QGridLayout *backgroundLayout = new QGridLayout;
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"),
                    ":/images/background3.png"), 1, 0);
    backgroundLayout->setRowStretch(2, 10);
    backgroundLayout->setColumnStretch(2, 10);
    QWidget *backgroundWidget = new QWidget;
    backgroundWidget->setLayout(backgroundLayout);
    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, QString::fromUtf8("Модули"));

}

void MainWindow::createActions()
{
    sendBackAction = new QAction(QIcon(":/images/sendtoback.png"),
                                 tr(""), this);

    saveConfig = new QAction(QIcon(),QString::fromUtf8("Сохранить как..."),this);
    description = new QAction(QIcon(),QString::fromUtf8("Описание..."),this);
    connect(saveConfig, SIGNAL(triggered()),this, SLOT(saveConf()));
    connect(description, SIGNAL(triggered()),this, SLOT(showDescription()));
    deleteAction = new QAction(QIcon(":/images/delete.png"),QString::fromUtf8("Delete"), this);
    deleteAction->setShortcut(QString::fromUtf8("Удалить"));
    deleteAction->setStatusTip(QString::fromUtf8("Удалить блок"));
    connect(deleteAction, SIGNAL(triggered()),this, SLOT(deleteItem()));
    QIcon ico(":/images/play.png");
    startStop = new QAction(ico,QString::fromUtf8("Старт"), this);
    connect(startStop, SIGNAL(triggered()),this, SLOT(startStopAction()));
    exitAction = new QAction(QString::fromUtf8("Выход"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(QString::fromUtf8("Выход"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(mainClose()));
    boldAction = new QAction(tr("Bold"), this);
    italicAction = new QAction(QIcon(":/images/italic.png"),tr("Italic"), this);
    underlineAction = new QAction(QIcon(":/images/underline.png"),tr("Underline"), this);
    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("Ctrl+B"));
    connect(aboutAction, SIGNAL(triggered()),this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(QString::fromUtf8("Управление"));
    fileMenu->addAction(startStop);
    fileMenu->addAction(exitAction);
    confMenu = menuBar()->addMenu(QString::fromUtf8("Конфигурация..."));
    confMenu->addAction(description);
    confMenu ->addAction(saveConfig);
    itemMenu = menuBar()->addMenu(QString::fromUtf8("Блок"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    aboutMenu = menuBar()->addMenu(QString::fromUtf8("Помощь"));
    aboutMenu->addAction(aboutAction);
}
void MainWindow::mainClose()
{
  man->stopProcessing(false);
  close();
  return;
}

void MainWindow::createArray()
{
	scene->listSize = scene->vel.size();
	if (scene->elList != nullptr)
	{
		delete [] scene->elList;
	}
	scene->elList = new element[scene->listSize];
	for (unsigned int i = 0; i<scene->listSize;i++)
	{
		scene->elList[i] = scene->vel.at(i);
	}
}
void MainWindow::startStopAction()
{
	QIcon icoStart(":/images/play.png");
	QIcon icoStop(":/images/stop-24.png");
	createArray();

	if (scene->vel.size() == 0) return;

	if (scene->working==true)
	{
		scene->working = false;
		man->stopProcessing(true);
		startStop->setIcon(icoStart);
	}
	else
	{				//пока временно тут инициализация манагера
		scene->working = true;
		man->run();	
		startStop->setIcon(icoStop);
		
		if (man->nonReady)
		{	
			scene->working = false;
			startStop->setIcon(icoStart);

			QString name, error;
			int col = 0;
			for (int i = 0; i < scene->vel.size();i++)
			{
				error = scene->vel.at(i).inter->error;
				if (error!="")
				{
					col++;
				}	
			}	
			QDialog * dlg = new QDialog();
			QTableWidget *tw = new QTableWidget(col,2,dlg);

			QStringList sl;
			sl <<QString::fromUtf8("Модуль") <<QString::fromUtf8("Ошибка") ;
			tw->setHorizontalHeaderLabels(sl);
			col=0;
			for (int i = 0; i < scene->vel.size();i++)
			{
				name = scene->vel.at(i).inter->name();
				error = scene->vel.at(i).inter->error;
				if (error!="")
				{
					QTableWidgetItem *nameItem = new QTableWidgetItem(name);
					tw->setItem(col, 0, nameItem);
					QTableWidgetItem *errorItem = new QTableWidgetItem(error);
					tw->setItem(col, 1, errorItem);
					col++;
				}	
			}
			tw->setGeometry(0,0,250,250);
			dlg->setGeometry(100,100,250,250);
			dlg->setModal(true);
			dlg->setWindowTitle(QString::fromUtf8("Ошибки"));
			dlg->show();
		}
	}
}
void MainWindow::createToolbars()
{
    editToolBar = addToolBar(QString::fromUtf8("Редактировать"));
    editToolBar->addAction(startStop);
    editToolBar->addAction(deleteAction);
    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));
    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/linepointer.png"));
    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton,
                                int(DiagramScene::InsertLine));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(pointerGroupClicked(int)));
    QStringList scales;
    pointerToolbar = addToolBar(tr("Pointer type"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);
}

QWidget *MainWindow::createBackgroundCellWidget(const QString &text,
                        const QString &image)
{
    QToolButton *button = new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    backgroundButtonGroup->addButton(button);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    return widget;
}

QWidget *MainWindow::createCellWidget(const QString &text,moduleType type)
{
    QPixmap pix;
    procThread th(nullptr,nullptr);
    th.proc(getImage,type,&pix,nullptr,nullptr,nullptr);
    QIcon icon(pix);
    QToolButton *button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroup->addButton(button, int(type));
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    return widget;
}


QMenu *MainWindow::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue")
          << tr("yellow");

    QMenu *colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()),
                this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor) {
            colorMenu->setDefaultAction(action);
        }
    }
    return colorMenu;
}

QIcon MainWindow::createColorToolButtonIcon(const QString &imageFile,
                        QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    QRect target(0, 0, 50, 60);
    QRect source(0, 0, 42, 42);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);
    return QIcon(pixmap);
}

QIcon MainWindow::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);
    return QIcon(pixmap);
}

void MainWindow::saveConf()
{
	QString path;
	path = QFileDialog::getSaveFileName(this, QString::fromUtf8("Путь к файлу"), "", "");
	man->initMatrix();
	bool b = false;
	char * data;
	unsigned int dataSize = 0;
	cfgHeader head;
	int tmp = sizeof(cfgHeader);
	dataSize+=sizeof(cfgHeader);
	memcpy(head.descr,txt,256);
	head.modCount = scene->vel.size();
	head.signature = cfgSignature;
	dataSize+=sizeof(cfgStorage)*head.modCount;
	dataSize+=head.modCount*head.modCount;
	data = new char[dataSize];
	tmp = sizeof(cfgHeader);
	memcpy(data,&head,tmp);
	cfgStorage stg;
	for (int i=0;i<scene->vel.size();i++)
	{
		stg.type=scene->vel.at(i).inter->type;
		stg.x=scene->vel.at(i).proxy->x();
		stg.y=scene->vel.at(i).proxy->y();
		memcpy(data+tmp+i*sizeof(cfgStorage),&stg,sizeof(cfgStorage));
	}
	int tmp2 = scene->vel.size();
	memcpy(data+tmp+tmp2*sizeof(cfgStorage),(char*)man->matrix,tmp2*tmp2);
	QFile fileCfg(path);
	fileCfg.open(QIODevice::WriteOnly | QIODevice::Unbuffered);					//открываем файл
	b=fileCfg.seek(0);	
	fileCfg.write(data,dataSize);																	
	fileCfg.close();	
	delete [] data;
	//закрываем файл
}

void MainWindow::showDescription()
{
	descr = new DescrForm(this,txt);
	descr->setWindowModality(Qt::ApplicationModal);	
	descr->show();		
}


void MainWindow::scanDirectory()
{
	QDir dir;
	QStringList filters;
	QString str;
	str = "*";
	str.append(exten);
	cfgHeader head;
	filters << str; 
	confMenu->addSeparator();
	dir.setNameFilters(filters);				//задаем фильтр файлов по расширению 
	listInfo = dir.entryInfoList();				//получаем список файлов
	for (int i = 0; i < listInfo.size(); ++i)		//для всего списка
	{
		QFileInfo fileInfo = listInfo.at(i);
		str = fileInfo.fileName();
		QFile fileCfg(str);
		fileCfg.open(QIODevice::ReadOnly | QIODevice::Unbuffered); //открываем файл
		fileCfg.seek(0);	
		fileCfg.read((char*)&head,sizeof(cfgHeader));
		if (head.signature==cfgSignature)
		{
			QAction* act = new QAction(QIcon(),QString::fromLocal8Bit(head.descr),this);
			act->setCheckable(true);
			confMenu->addAction(act);
			vecAct.push_back(act);
			connect(act,SIGNAL(triggered()),this,SLOT(schemeChange()));
		}
		fileCfg.close();	
	}
}

void MainWindow::schemeChange()
{
	QString text,str;
	QFileInfo fileInfo;
	cfgHeader head;
	for (unsigned int i=0;i<vecAct.size();i++)
	{
		if (vecAct.at(i)->isChecked())
		{
			text = vecAct.at(i)->text();
			scene->vel.clear();
		}
	}

	for (int i=0;i<listInfo.size();i++)
	{
		fileInfo = listInfo.at(i);
		str = fileInfo.fileName();
		QFile fileCfg(str);
		fileCfg.open(QIODevice::ReadOnly | QIODevice::Unbuffered);					//открываем файл
		fileCfg.seek(0);	
		fileCfg.read((char*)&head,sizeof(cfgHeader));
		if (QString::fromLocal8Bit(head.descr)==text)
		{
			scene->loadScheme(&fileCfg);
			fileCfg.close();
			for (int i=0;i<vecAct.size();i++)
			{
				vecAct.at(i)->setChecked(false);
			}
			return;
		}
		fileCfg.close();
	}

	
}
