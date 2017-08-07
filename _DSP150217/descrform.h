#ifndef DESCRFORM_H
#define DESCRFORM_H

#include <QDialog>
#include <QVariant>
#include "ui_descrform.h"

class DescrForm : public QDialog
{
	Q_OBJECT

public:
	DescrForm(QWidget *parent,	char* text);
	~DescrForm();

private:
	Ui::DescrForm ui;
	char* txt;
	QString str;
private slots:
	void descrChanged();
	void saveTest();
}; 

#endif // DESCRFORM_H
