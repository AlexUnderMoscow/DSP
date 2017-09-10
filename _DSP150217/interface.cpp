#include "interface.h"

Interface::Interface(QDialog *parent)
: QDialog(parent)
{
	ready = false;
	expand = true;
	form = parent;
	treeView = NULL;

        boolManager = new QtBoolPropertyManager();
        intManager = new QtIntPropertyManager();
        stringManager = new QtStringPropertyManager();
        enumManager = new QtEnumPropertyManager();
        doubleManager = new QtDoublePropertyManager();
        groupManager = new QtGroupPropertyManager();
        flagManager = new QtFlagPropertyManager();
        subBoolManager = new QtBoolPropertyManager();
        checkBoxFactory = new QtCheckBoxFactory();
        spinBoxFactory = new QtSpinBoxFactory();
        lineEditFactory = new QtLineEditFactory();
        comboBoxFactory = new QtEnumEditorFactory();
        doubleSpinFactory = new QtDoubleSpinBoxFactory();
}

Interface::~Interface()
{
	delete show;
}

void Interface::createButton(QDialog* dlg)
{
	show = new QToolButton(dlg);
	le = new QPushButton(dlg);
	le->setVisible(false);
	d = dlg;
	show->setGeometry(0,0,15,15);
	show->setText("...");
	connect(show,SIGNAL(clicked()),this,SLOT(expandForm()));
}
void Interface::expandForm()
{
	if (expand)
	{
		expand = false;
		oldheight = d->height();
		oldwidth = d->width();
		d->setGeometry(d->x(),d->y(),50,70);
		show->setGeometry(0,20,50,50);
		show->setText("");
		QIcon ico(MODpixmap);
		show->setIcon(ico);
		le->setGeometry(0,0,50,20);
		le->setEnabled(false);
		le->setVisible(true);
		if (treeView!=NULL) treeView->setVisible(false);
//		if (indicatorTree!=NULL) indicatorTree->setVisible(false);
	}
	else
	{
		expand = true;
		d->setGeometry(d->x(),d->y(),oldwidth,oldheight);
		show->setGeometry(0,0,15,15);
		QPixmap pix;
		QIcon ico(pix);
		show->setIcon(ico);
		show->setText("...");
		le->setVisible(false);
		if (treeView!=NULL) treeView->setVisible(true);
//		if (indicatorTree!=NULL) indicatorTree->setVisible(true);
	}
}
void Interface::deleteObject()
{

}
QString Interface::name()
{
	return moduleName;
}
void Interface::removeArrow(Arrow *arrow)
{
	int index = -1;
	for (int i=0;i<arrows.size();i++)
	{
		if (arrow == arrows.at(i))
		{
			index = i;
		}
	}
	if (index != -1)
		arrows.remove(index);
}

void Interface::removeArrows() //удаление стрелок у самого себя и у смежных модулей
{
	Arrow* ar;
	Interface* inter=NULL;
	for (int i=0; i<arrows.size();i++)
	{
		ar = arrows.at(i);
		inter = getInterface(ar->myEndItem);
		if ((inter!=NULL)&&(inter!=this)) inter->removeArrow(ar);
		inter = getInterface(ar->myStartItem);
		if ((inter!=NULL)&&(inter!=this)) inter->removeArrow(ar);
		arrows.at(i)->clear();
	}
	arrows.clear();
}

Interface* Interface::getInterface(QWidget* w)
{
	Interface* i = NULL;
	for (unsigned int index = 0; index<*size; index++)
	{
		if ((*arr)[index].widget == w)
		{
			i = (Interface*)((*arr)[index].inter);
		}
	}
	return i;
}

void Interface::addArrow(Arrow *arrow)
{
	arrows.push_back(arrow);
}

QPixmap Interface::image() const
{
	return MODpixmap;
}

void Interface::sortArrows()				//раскидывает стрелки по группам
{
	output.clear();
	input.clear();
	for (int i = 0; i< arrows.size();i++)
	{
		if (arrows.at(i)->stopID!=this->blockID)
		{
			output.push_back(arrows.at(i)->queue);
		}
		else
		{
			input.push_back(arrows.at(i)->queue);
		}
	}
}
