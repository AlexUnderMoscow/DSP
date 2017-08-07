

#ifndef ARROW_H
#define ARROW_H
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QWidget>
#include "diagramitem.h"
#include "fifo.h"
#include "struct.h"

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE
class Arrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };			//Стрелка с буфером

    Arrow(QWidget *startItem, QWidget *endItem, unsigned short startID, unsigned short stopID,
      QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
	~Arrow();

	fifo *queue;							//у стрелки циклический буфер
	QWidget *myStartItem;					//форма на начале стрелки
	unsigned short startID;					//идентификатор формы на начале стрелки
	QWidget *myEndItem;						//форма на конце стрелки
	unsigned short stopID;					//индентификатор на концу стрелки
	QGraphicsScene *sc;

    int type() const
        { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor(/*const QColor &color*/)
        {
			// myColor = color;
		}
    QWidget *startItem() const
        { return myStartItem; }
    QWidget *endItem() const
        { return myEndItem; }
    void updatePosition();
	void clear();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);
private:

   QColor myColor;

	bool inWidget( QWidget *w, float x, float y);				//попадает ли координата на форму
};


#endif
