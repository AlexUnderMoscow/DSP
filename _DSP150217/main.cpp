
#include <QApplication>
#include "mainwindow.h"
#include "fifo.h"

int main(int argv, char *args[])
{
    Q_INIT_RESOURCE(diagramscene);
    QApplication app(argv, args);
    MainWindow mainWindow;
    mainWindow.setGeometry(100, 100, 900, 500);
    mainWindow.show();
    return app.exec();
}

//проблемы
/*


*/

/*
Идеи:
-многопоточный согласованный фильтр с числом потоков = число ядер процессора - 1
	решить проблему с расщеплением входного потока на разные потоки с небольшим наложением
	решить проблему с безопасной уборкой наложения
-автоматический перерасчет коэффицентов фильтра исходя из полосы оцифровки и символьной скорости
	
*/

/*  ДОБАВЛЕНИЕ МОДУЛЯ*/
/*
1 - меню ПРОЕКТ - добавить класс - QtGui класс ("test1")
2 - в файле test1.h указываем базовый класс  : public Interface
3 - в файле test1.h прописываем параметры конструктора - "QDialog *parent,array** _arr, unsigned int *_size"
4 - в файле test1.h в разделе public класса test1 добаляем член  - "test1Work * work"
5 - в файле test1.h добавить #include "interface.h"
6 - в файле test1.сpp прописать параметры конструктора  "QDialog *parent,array** _arr, unsigned int *_size"
7 - в файле test1.сpp в конструктор скопировать из работающего модуля и исправить инициализацию членов класса
8 - в файле struct.h добавляем новый тип модуля (должен отсличаться от имени класса)
9 - в файле thread.h добавляем #include "test1.h"
10 - в файле thread.cpp копируем и вставляем конструкцию case: ...
11 - там же заменить имена класса окна и класса обработчика
12 - в файле mainwindow.cpp добавить "layout->addWidget(createCellWidget(QString::fromUtf8("UDP приемник"),test1mod),10, 0);"
*/
