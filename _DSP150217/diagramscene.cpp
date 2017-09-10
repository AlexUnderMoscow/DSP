

#include <QtGui>

#include "diagramscene.h"
#include "arrow.h"


DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    myItemType = File;
    line = 0;
//    textItem = 0;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;
	working = false;
	elList = NULL;
	dx = 0;
	dy = 0;
}

void DiagramScene::setMode(Mode mode)
{
    myMode = mode; //установка режима
}

void DiagramScene::setItemType(moduleType type,QAbstractButton *but)
{
    myItemType = type;
	mybut = but;
}


void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)//нажали на поле кнопку
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    QPointF po;
    po = mouseEvent->scenePos();
    int x,y;
    x = po.x();
    y = po.y();


	element el;
	start_w = NULL;
	stop_w = NULL;

	el.inter  = NULL;
	el.proxy = NULL;

    procThread th((marray**)&elList,&listSize);



   switch (myMode) {
        case InsertItem:

		th.proc(newModule,myItemType,NULL,&el.proxy,&el.inter,NULL);
		el.proxy->setWindowModality(Qt::NonModal);
		el.proxy->setGeometry(x,y,el.proxy->width(),el.proxy->height());
		addWidget(el.proxy);
		el.inter->blockID = vel.size();			//блок имеет ID по индексу в векторе
		vel.push_back(el);
		el.proxy->show();

		  break;

        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
            addItem(line);

			
			start_w = whatCurentWidget(x,y,&startID);
			//if (start_w!=NULL) start_w->setVisible(false);

	break;
		
		case MoveItem:
			start_w = whatCurentWidget(x,y,&startID);

			if (start_w!=NULL)
			{
				//if (y < (start_w->y()+20))
				//  {
				    dx = x - start_w->x();
				    dy = y - start_w->y();
				//  }

			}
			
		break;
       
		case InsertText:
			start_w = whatCurentWidget(x,y,&startID);
			Interface *inter = getInterface(start_w);
			if (start_w!=NULL)
			{
				dx = x - start_w->x();
				dy = y - start_w->y();
				if (inter->le->text()!="X")
				{
					inter->le->setText("X");
				}else
				{
					inter->le->setText("");
				}
			}
			
			myMode = MoveItem;
			break;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
   //QGraphicsScene::update();
}


QWidget* DiagramScene::whatCurentWidget(int mx, int my, unsigned short *ID)
{
	bool inX,inY;
	QWidget * w;
	w = NULL;
	int left,right,up,down;
	left=right=up=down = 0;
	for (int i = 0; i < vel.size(); i++) 
	{
		inX = false;
		inY = false;
		left = vel.at(i).proxy->x();
		right = vel.at(i).proxy->x()+vel.at(i).proxy->width();
		up = vel.at(i).proxy->y();
		down = vel.at(i).proxy->y()+vel.at(i).proxy->height();

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
			w = vel.at(i).proxy;
			*ID = vel.at(i).inter->blockID;
		}		
	}
	return w;
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && line != 0) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } 
	if (myMode == MoveItem) 
	{
		QPointF p = mouseEvent->scenePos();
		int ymousepos = p.y();
		if (start_w!=NULL)
		{

		   // if (ymousepos > (start_w->y()+40)) return;

			start_w->setGeometry(p.x()-dx,p.y()-dy,start_w->width(),start_w->height());
			Interface * infr = getInterface(start_w);
			//QGraphicsScene::mouseMoveEvent(mouseEvent);
			infr->le->setText("");
			for (int i = 0; i<infr->arrows.size();i++)	
			{
				infr->arrows.at(i)->update();
			}	
			QGraphicsScene::mouseMoveEvent(mouseEvent);
			QGraphicsScene::update();
		}
    }
}



void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QPointF po;
	po = mouseEvent->scenePos();
	int x,y;
	x = po.x();
	y = po.y();
	stop_w = whatCurentWidget(x,y,&stopID);
	switch (myMode) 
	{
		case InsertItem:
			myMode = InsertText;
			mybut->setChecked(false);
			break;
		 case InsertLine:
			 if ((start_w==NULL)||(stop_w==NULL))
			 {
				 removeItem(line);
				 return;
			 }
			 if ((stop_w!=NULL) && (start_w!=NULL))
			 {
				 removeItem(line);
				 if (start_w != stop_w)
				 {
					 Arrow *arrow = new Arrow(start_w,stop_w,startID,stopID,NULL,this);
					 getInterface(start_w)->addArrow(arrow);
					 getInterface(stop_w)->addArrow(arrow);
					 arrow->setZValue(-1000.0);
					 addItem(arrow);
					 arrow->updatePosition();
				 }
			 }
			 break;

		 case MoveItem:
			myMode = InsertText;
			 break;
	}
	
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
	QGraphicsScene::update();
}

Interface* DiagramScene::getInterface(QWidget* w)
{
	Interface* i = NULL;
	for (int index = 0; index<vel.size(); index++)
	{
		if (vel.at(index).proxy == w)
		{
			i = vel.at(index).inter;
		}
	}
	return i;
}

bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}

void DiagramScene::loadScheme(QFile* file)
{
	cfgHeader head;
	cfgStorage str;
	unsigned int offset=0;
	file->seek(offset);
	offset+=sizeof(cfgHeader);
	file->read((char*)&head,offset);
	unsigned int col = head.modCount;
	for (unsigned int i= 0; i<col;i++)
	{
		file->seek(offset);
		file->read((char*)&str,sizeof(cfgStorage));
		//добавляем формы
		/*str.type
		str.x
		str.y*/
		element el;
        procThread th((marray**)&elList,&listSize);
		th.proc(newModule,str.type,NULL,&el.proxy,&el.inter,NULL);
		el.proxy->setWindowModality(Qt::NonModal);
		el.proxy->setGeometry(str.x,str.y,el.proxy->width(),el.proxy->height());
		addWidget(el.proxy);
		el.inter->blockID = vel.size();			//блок имеет ID по индексу в векторе
		vel.push_back(el);
		el.proxy->show();
		//
		offset+=sizeof(cfgStorage);
	}

	/*межсоединения*/
	bool * b = new bool[col*col];
	file->seek(offset);
	file->read((char*)b,col*col);
	unsigned int ver=0;
	unsigned int hor=0;
	for (unsigned int i = 0;i < col*col; i++)
	{
			if (b[i])
			{
				start_w = vel.at(ver).proxy;
				stop_w = vel.at(hor).proxy;
				startID = ver;
				stopID = hor;
				Arrow *arrow = new Arrow(start_w,stop_w,startID,stopID,NULL,this);

				getInterface(start_w)->addArrow(arrow);
				getInterface(stop_w)->addArrow(arrow);
				arrow->setZValue(-1000.0);
				addItem(arrow);
				arrow->updatePosition();
			}
			hor++;
			if (hor==col)
			{
				hor=0;
				ver++;
			}
	}
	/**/

	start_w=NULL;
}

