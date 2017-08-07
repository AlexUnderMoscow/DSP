#ifndef MANAGER_H
#define MANAGER_H

#include <QThread>
#include <QTableWidget>
#include "diagramscene.h"
#include "pool.h"
#include "thread.h"
#include "struct.h"

class Manager : public QThread	//управляющтй всеми процессам
{
	Q_OBJECT
public:
	DiagramScene* scene;		//указатель на сцену
    Manager(QObject *parent, DiagramScene* scene, marray** arr, unsigned int *size);
	~Manager();
	bool *matrix;			//матрица взаимосвязей
	bool nonReady;
	bool inWork;
	void init();			//инициализация менеджера
	void run();
	void stopProcessing(bool onlyStop);
	void initMatrix();
private:
	
	ObjectPool *pool;			//указатель на пул потоков
	procThread *trd;
    marray** arr;
	unsigned int *sizeM;
	
};

#endif // MANAGER_H



