

#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include "diagramitem.h"
//#include "diagramtextitem.h"
#include <QVariant>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>

#include "interface.h"
#include "struct.h"
#include "thread.h"
#include <vector>
#include <QGraphicsProxyWidget>
#include <QVector>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

struct element {
	QWidget * proxy;
	Interface * inter;
};

//! [0]
class DiagramScene : public QGraphicsScene					//сцена для видимых элементов
{
    Q_OBJECT

public:
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem }; //режимы работы сцены
    DiagramScene(QMenu *itemMenu, QObject *parent = 0);
	Interface* getInterface(QWidget* w);
	void loadScheme(QFile* file);
    QFont font() const
        { return myFont; }
    QColor textColor() const
        { return myTextColor; }
    QColor itemColor() const
        { return myItemColor; }
    QColor lineColor() const
        { return myLineColor; }

	QVector<element> vel;					//вектор модулей с интерфейсами
	element* elList;
	unsigned int listSize;
	QWidget * start_w;					//стартовая форма (для стрелки)
	QWidget * stop_w;					//конечная форма
	unsigned short startID;					//идентификатор стартовой формы
	unsigned short stopID;					//идентификатор конечной формы
	bool working;						//признак включения работы для манагера
	
	Mode myMode;

public slots:
    void setMode(Mode mode);
    void setItemType(moduleType type,QAbstractButton *but);
   // void editorLostFocus(DiagramTextItem *item);

signals:
    void itemInserted(DiagramItem *item);
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    bool isItemChange(int type);
    QWidget* whatCurentWidget(int x, int y, unsigned short *ID);	//что за форма под мышкой и какой у нее ID
    moduleType myItemType;
    QMenu *myItemMenu;
    QAbstractButton *mybut;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;
    QFont myFont;
 //   DiagramTextItem *textItem;
    QColor myTextColor;
    QColor myItemColor;
    QColor myLineColor;
    int dx,dy;
};


#endif
