#ifndef PHASEDIAGRAMWIDGET_H
#define PHASEDIAGRAMWIDGET_H

#include <QWidget>
#include <QColor>
#include <QTimer>
#include <vector>
#include <QListWidget>

#ifndef M_PI 
#define M_PI 3.14159265358979323846
#endif

class phaseDiagramWidget : public QWidget
{
	Q_OBJECT
public:
	phaseDiagramWidget(QWidget *parent);
	void addSample(short x, short y);
	~phaseDiagramWidget();
	short* cosSample;
	short* sinSample;
	int load;
	int rBlock;
	int wBlock;	
protected:
	void paintEvent(QPaintEvent *event);
private:
};

#endif // PHASEDIAGRAMWIDGET_H
