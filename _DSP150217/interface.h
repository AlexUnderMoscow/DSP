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
	QVector<Arrow*> arrows;				//список стрелок
	std::vector<std::shared_ptr<fifo>> input;	//список буферов входных
	std::vector<std::shared_ptr<fifo>> output;	//список буферов выходных
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
	std::shared_ptr<QToolButton> show;
	QDialog *form;
	QPixmap MODpixmap;
	std::shared_ptr<QPushButton> le;
	QTimer *timer;

	marray** arr;
	unsigned int *size;
/*------------------------------------------------------*/

	std::shared_ptr<QtBoolPropertyManager> boolManager;
	std::shared_ptr<QtIntPropertyManager> intManager;
	std::shared_ptr<QtStringPropertyManager> stringManager;
	std::shared_ptr<QtSizePropertyManager> sizeManager;
	std::shared_ptr<QtRectPropertyManager> rectManager;
	std::shared_ptr<QtSizePolicyPropertyManager> sizePolicyManager;
	std::shared_ptr<QtEnumPropertyManager> enumManager;
	std::shared_ptr<QtGroupPropertyManager> groupManager;
	std::shared_ptr<QtDoublePropertyManager> doubleManager;
	std::shared_ptr<QtFlagPropertyManager> flagManager;
	std::shared_ptr<QtBoolPropertyManager> subBoolManager;

	std::shared_ptr<QtTreePropertyBrowser> treeView;

	std::shared_ptr<QtCheckBoxFactory> checkBoxFactory;
	std::shared_ptr<QtSpinBoxFactory> spinBoxFactory;
	std::shared_ptr<QtSliderFactory> sliderFactory;
	std::shared_ptr<QtScrollBarFactory> scrollBarFactory;
	std::shared_ptr<QtLineEditFactory> lineEditFactory;
	std::shared_ptr<QtEnumEditorFactory> comboBoxFactory;
	std::shared_ptr<QtDoubleSpinBoxFactory> doubleSpinFactory;
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
