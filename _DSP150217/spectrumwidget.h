#ifndef SPECTRUMWIDGET_H
#define SPECTRUMWIDGET_H


#include <QWidget>
#include <QColor>
#include <QTimer>
#include <vector>
#include <QListWidget>
#include <QVector>
#include "qcustomplot.h"
#include "struct.h"

#ifndef M_PI 
#define M_PI 3.14159265358979323846
#endif
#define quantums 8000
#define periods 10
#define PTS 1024 //1024
#define AGC 4
using namespace std;
class spectrumWidget : public QObject
{
	Q_OBJECT

public:
	spectrumWidget(QWidget *parent, short * pcm, int _size);
	vector <double> spectr();
	~spectrumWidget();
	vector<short> dsin;
	vector<double> dfourier,temp;
	vector<double> masVec[8];
	int cnt;
	int step; int counts;
	int period;
	vector<double> sp;
	short* pcm;
	int size;
	int x;
	int y;
	int k_zoom;
	std::shared_ptr<QTimer> time;
	std::shared_ptr<QCustomPlot> cp;
protected:
	//void paintEvent(QPaintEvent *event);
	vector<double> FFTCalc(const vector<short>& dIn, int nn, int beginData);
private:
	QString fileName;
//	QPainter *painter;
	QListWidget *list;
	QVector<double> graphics;
	QVector<double> xvec;
private slots:
	void timeoutRepaint();

};

#endif // SPECTRUMWIDGET_H
