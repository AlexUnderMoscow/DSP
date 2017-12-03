
#include <QtGui>
#include "arrow.h"
#include <math.h>

const qreal Pi = 3.1428;

Arrow::Arrow(QWidget *startItem, QWidget *endItem, unsigned short _startID, unsigned short _stopID,
         QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent)
{
    myStartItem = startItem;
    myEndItem = endItem;
    startID = _startID;
    stopID = _stopID;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    myColor = Qt::black;
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    queue = std::shared_ptr<fifo>(new fifo(tmpBufSize));
    sc = scene;
}
Arrow::~Arrow()
{

}
void Arrow::clear()
{	
	sc->removeItem(this);						//!!!!
}

QRectF Arrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;
    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    return path;
}

void Arrow::updatePosition()
{
    QPointF s(myStartItem->x()+myStartItem->width()/2,
		myStartItem->y()+myStartItem->height()/2);
    QPointF f(myEndItem->x()+myEndItem->width()/2,
		myEndItem->y()+myEndItem->height()/2);
    QLineF thisline(s, f);
    setLine(thisline);
}

bool Arrow::inWidget(QWidget *w, float mx, float my)
{
			int up,down,left,right;
			bool inX,inY;
			inX = false;
			inY = false;
			left = w->x();
			right = w->x()+w->width();
			up = w->y();
			down = w->y()+w->height();
			if ((mx > left) && (mx < right))
			{
				inX = true;
			}
			if ((my > up) && (my < down))
			{
				inY = true;
			}
			if (inX && inY)
			{
				return true;
			}		
			else
			{
				return false;
			}
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen myPen = pen();
    myPen.setColor(myColor);
//    qreal arrowSize = 20;
    painter->setPen(myPen);
    painter->setBrush(myColor);

	QPointF str(myStartItem->x()+myStartItem->width()/2,
		myStartItem->y()+myStartItem->height()/2);
	QPointF stp(myEndItem->x()+myEndItem->width()/2,
		myEndItem->y()+myEndItem->height()/2);
   
	QLineF centerLine(str, stp);
	float dx = centerLine.dx();
	float dy = centerLine.dy();
	float rel = dy/dx;
	float angle = atan(rel);
	int i = 0;
	QPointF newpoint;
	newpoint = stp;
	while (inWidget(myEndItem,newpoint.x(),newpoint.y()))
	{
		if ((dx>=0) && (dy>=0))
		{
			newpoint.setX(stp.x()-i*cos(angle));
			newpoint.setY(stp.y()-i*sin(angle));
		}
		if (((dx<0) && (dy<0)))
		{
			newpoint.setX(stp.x()+i*cos(angle));
			newpoint.setY(stp.y()+i*sin(angle));
		}
		if ((dx>=0) && (dy<0))
		{
			newpoint.setX(stp.x()-i*cos(angle));
			newpoint.setY(stp.y()-i*sin(angle));
		}
		if ((dx<0) && (dy>=0))
		{
			newpoint.setX(stp.x()+i*cos(angle));
			newpoint.setY(stp.y()+i*sin(angle));
		}
		i++;
	}
	centerLine.setPoints(str, newpoint);
	painter->drawLine(centerLine);
	QPointF tmp1, tmp2;
	int a =10; //длина концов стрелок
	if (dx>=0)
	{
		tmp1.setX(newpoint.x()-a*cos(angle+0.2));
		tmp2.setX(newpoint.x()-a*cos(angle-0.2));
	}
	else
	{
		tmp1.setX(newpoint.x()+a*cos(angle+0.2));
		tmp2.setX(newpoint.x()+a*cos(angle-0.2));
	}

	if (dy>=0)
	{
		tmp1.setY(newpoint.y()-a*sin(angle+0.2));
		tmp2.setY(newpoint.y()-a*sin(angle-0.2));
	}
	else
	{
		tmp1.setY(newpoint.y()+a*sin(angle+0.2));
		tmp2.setY(newpoint.y()+a*sin(angle-0.2));
	}
	if ((dx>=0) && (dy<0))
	{
		tmp1.setY(newpoint.y()+a*sin(angle+0.2+Pi));
		tmp2.setY(newpoint.y()+a*sin(angle-0.2+Pi));
	}
	if ((dx<0) && (dy>=0))
	{
		tmp1.setY(newpoint.y()+a*sin(angle+0.2));
		tmp2.setY(newpoint.y()+a*sin(angle-0.2));
	}
	QLineF ar1(newpoint,tmp1);
	painter->drawLine(ar1);
	QLineF ar2(newpoint,tmp2);
	painter->drawLine(ar2);
}

