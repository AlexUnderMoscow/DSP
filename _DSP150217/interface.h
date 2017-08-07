#ifndef INTERFACE_H
#define INTERFACE_H

#include <QPainter> 
#include <QFRame>
#include <QTimer>
#include <QDialog>
#include <QPolygonF>
#include <QToolButton>
#include <QPushButton>
#include <QGraphicsScene>
#include <vector>

#include "qtpropertymanager.h"
#include "qteditorfactory.h"
#include "qttreepropertybrowser.h"
#include "qtbuttonpropertybrowser.h"
#include "qtgroupboxpropertybrowser.h"

#include "arrow.h"
#include "struct.h"
#include <cmath>

class myTreeBrowser : public QtTreePropertyBrowser
{
  Q_OBJECT
public:
  myTreeBrowser(QWidget * parent=0)
    :QtTreePropertyBrowser(parent) {}
  virtual ~myTreeBrowser() {}
signals:
    void valueChanged(QtBrowserItem* it);
 private:
  void itemChanged(QtBrowserItem* it)
  {
    emit valueChanged(it);
  }

};


class Interface : public QDialog 		//общее для всех модулей
{
	Q_OBJECT
public:
	Interface(QDialog *parent = 0);
	~Interface();
	QString name();					//функция возвращает имя
	QString moduleName;				//имя модуля
	unsigned short blockID;				//ID блока
	moduleType type;				//тип модуля
	QVector<Arrow *> arrows;			//список стрелок
	std::vector<fifo*> input;			//список буферов входных
	std::vector<fifo*> output;			//список буферов выходных
	void sortArrows();				//упорядочить буфера (вход/выход)
	void removeArrow(Arrow *arrow);
	void removeArrows();
	moduleType modType() const
	{ return type; }
	void addArrow(Arrow *arrow);
	QPixmap image() const;
	QPolygonF myPolygon;
	void deleteObject();
	void createButton(QDialog* dlg);
	Interface* getInterface(QWidget* w);

	QFrame* dragWidget;

	bool ready;						//готовность данных
	void * widgetAddr;					//адрес  обработчика
	QString error;
	QToolButton *show;
	QDialog *form;
	QPixmap MODpixmap;
	QPushButton * le;
	QTimer *timer;

	marray** arr;
	unsigned int *size;
/*------------------------------------------------------*/

	QtBoolPropertyManager *boolManager;
	QtIntPropertyManager *intManager;
	QtStringPropertyManager *stringManager;
	QtSizePropertyManager *sizeManager;
	QtRectPropertyManager *rectManager;
	QtSizePolicyPropertyManager *sizePolicyManager;
	QtEnumPropertyManager *enumManager;
	QtGroupPropertyManager *groupManager;
	QtDoublePropertyManager *doubleManager;
	QtFlagPropertyManager *flagManager;
	QtBoolPropertyManager *subBoolManager;

	QtTreePropertyBrowser *treeView;

	QtCheckBoxFactory *checkBoxFactory;
	QtSpinBoxFactory *spinBoxFactory;
	QtSliderFactory *sliderFactory;
	QtScrollBarFactory *scrollBarFactory;
	QtLineEditFactory *lineEditFactory;
	QtEnumEditorFactory *comboBoxFactory;
	QtDoubleSpinBoxFactory *doubleSpinFactory;
/*------------------------------------------------------*/
private:
	bool expand;
	QDialog* d;
	int oldheight;
	int oldwidth;

private slots:
	void expandForm();
	
};

#endif // INTERFACE_H
