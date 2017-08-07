

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QToolBox>
#include <QGraphicsView>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>
#include <QVariant>
#include "struct.h"
#include "diagramitem.h"
#include "manager.h"
#include "thread.h"
#include "descrform.h"

class DiagramScene;

QT_BEGIN_NAMESPACE
class QAction;
class QToolBox;
class QSpinBox;
class QComboBox;

class QButtonGroup;
class QLineEdit;
class QGraphicsTextItem;
class QFont;
class QToolButton;
class QAbstractButton;
class QGraphicsView;
QT_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
   MainWindow();
   ~MainWindow();
private slots:
    void backgroundButtonGroupClicked();
    void buttonGroupClicked(int id);
    void deleteItem();
    void pointerGroupClicked(int id);
    void itemInserted(DiagramItem *item);
    void textInserted(QGraphicsTextItem *item);
    void sceneScaleChanged();
    void fillButtonTriggered();
    void lineButtonTriggered();
    void handleFontChange();
    void itemSelected(QGraphicsItem *item);
    void about();
	void startStopAction();
	void saveConf();
	void showDescription();
	void schemeChange();
	void mainClose();

private:
	char txt[256];
    void createToolBox();
    void createActions();
    void createMenus();
    void createToolbars();
    QWidget *createBackgroundCellWidget(const QString &text,
                                        const QString &image);
    QWidget *createCellWidget(const QString &text,
                              moduleType type);
    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QIcon createColorToolButtonIcon(const QString &image, QColor color);
    QIcon createColorIcon(QColor color);
	void createArray();
	void scanDirectory();

	QFileInfoList listInfo;
    DiagramScene *scene;
    QGraphicsView *view;
    QAction *exitAction;
    QAction *addAction;
    QAction *deleteAction;

	QAction *startStop;
	QAction *saveConfig;
	QAction *description;

    QAction *toFrontAction;
    QAction *sendBackAction;
    QAction *aboutAction;
    QMenu *fileMenu;
	QMenu *confMenu;
    QMenu *itemMenu;
    QMenu *aboutMenu;
    QToolBar *textToolBar;
    QToolBar *editToolBar;
    QToolBar *colorToolBar;
    QToolBar *pointerToolbar;
    QComboBox *sceneScaleCombo;
    QComboBox *itemColorCombo;
    QComboBox *textColorCombo;
    QComboBox *fontSizeCombo;

    QToolBox *toolBox;
    QButtonGroup *buttonGroup;
    QButtonGroup *pointerTypeGroup;
    QButtonGroup *backgroundButtonGroup;
    QToolButton *fontColorToolButton;
    QToolButton *fillColorToolButton;
    QToolButton *lineColorToolButton;
    QAction *boldAction;
    QAction *underlineAction;
    QAction *italicAction;
    QAction *textAction;
    QAction *fillAction;
    QAction *lineAction;

	Manager *man; //указатель на управляющего
	DescrForm * descr;
	std::vector<QAction*> vecAct;

};
//! [0]

#endif
