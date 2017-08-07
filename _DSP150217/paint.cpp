
#include "paint.h"

Paint::Paint(QWidget *parent,QPixmap _pix)
	: QWidget(parent)
{
	pix = &_pix;
}

Paint::~Paint()
{

}

void Paint::paintEvent(QPaintEvent *) //метод рисования
{
	QPainter pain(this);  //??
	pain.fillRect(rect(),QColor(70,70,70));// делает фон черным
	pain.drawPixmap(0,0,*pix);
}

