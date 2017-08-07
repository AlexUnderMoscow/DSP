#include "descrform.h"

DescrForm::DescrForm(QWidget *parent,char* text)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.textEdit,SIGNAL(textChanged()),this,SLOT(descrChanged()));
	connect(ui.bSave,SIGNAL(clicked()),this,SLOT(saveTest()));
	txt = text;
	setWindowTitle(QString::fromUtf8("Описание конфигурации"));
}

DescrForm::~DescrForm()
{

}
void DescrForm::descrChanged()
{
	QChar c(0);

	unsigned int remain = 257 - ui.textEdit->toPlainText().size();
	if (remain == 0)
	{
		str= ui.textEdit->toPlainText();
		str.truncate(255);
		ui.textEdit->setPlainText(str);
		ui.textEdit->setCursorWidth(1);
		return;
	}
	QString s;
	s.setNum(remain-1);
	QString first = QString::fromUtf8("Осталось ");
	QString last = QString::fromUtf8(" символов");
	ui.labRemain->setText(first+s+last);
}

void DescrForm::saveTest()
{
	QByteArray ba;
	str= ui.textEdit->toPlainText();
	ba=str.toLocal8Bit();
	unsigned int i;
	for (i=0;i<ba.size();i++)
	{
		txt[i]=ba.at(i);
	}
	txt[i]=0;
	close();
}
