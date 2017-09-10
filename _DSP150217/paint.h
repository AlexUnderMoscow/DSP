#ifndef PAINT_H
#define PAINT_H

#include <QWidget>
#include <QPainter>

class Paint : public QWidget
{
	Q_OBJECT

public:
	Paint(QWidget *parent, QPixmap pix);
	~Paint();
private:
	QPixmap *pix;
protected:
	void paintEvent(QPaintEvent *);//метод рисования
};

#endif // PAINT_H
