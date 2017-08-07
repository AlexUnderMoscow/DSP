

#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPixmapItem>
#include <QList>
#include "struct.h"
#include <QRect>

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
QT_END_NAMESPACE

//class Arrow;

//! [0]
class DiagramItem : public QGraphicsPolygonItem //, public fileClass  //Polygon
{
public:
    enum { Type = UserType + 15 };


    DiagramItem(moduleType diagramType, QMenu *contextMenu,
        QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);


   moduleType diagramType() const
        { return myDiagramType; }

    QPixmap image() const;
    int type() const
        { return Type;}

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);


private:
    moduleType myDiagramType;
    QPolygonF myPolygon;
    QMenu *myContextMenu;

};


#endif
