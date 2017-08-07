#include "manager.h"
Manager::Manager(QObject *parent,DiagramScene* _scene,marray** _arr, unsigned int *_size)
: QThread(parent)
{
	scene = _scene;
	matrix = NULL;
	pool=NULL;
	inWork = false;
	arr= _arr;
	sizeM=_size;
	pool = NULL;
}

Manager::~Manager()
{
	//stopProcessing();
/*	if (matrix!=NULL)
		delete [] matrix;
	if (pool!=NULL)	delete pool;*/
}
void Manager::initMatrix()
{
	if (matrix!=NULL) {matrix = NULL;}
	int size = scene->vel.size();
	matrix = new bool [size*size];
	for (int i = 0; i<size*size;i++)
	{
		matrix[i] = false;
	}

	//обнулили  матрицу переходов

	//создаем матрицу переходов
	unsigned short thisID;
	unsigned short arrowID;
	for (int i = 0; i<size;i++)
	{
		thisID = scene->vel.at(i).inter->blockID;
		for (int j = 0; j<scene->vel.at(i).inter->arrows.size();j++)
		{
			arrowID = scene->vel.at(i).inter->arrows.at(j)->stopID;
			if	(thisID != arrowID)
			{
				matrix[i*size+arrowID] = true;
			}
		}
	}
	return;
}

void Manager::init()
{
	initMatrix();
	// сортируем входы выходы интерфейсов
	int size = scene->vel.size();
	for (int i = 0; i<size;i++)
	{
		scene->vel.at(i).inter->sortArrows();
		//scene->vel.at(i).inter->form->setEnabled(false);
	}

	//инициализировать модули
	nonReady = false;
    procThread th((marray**)&scene->elList,&scene->listSize);
	for (int i = 0; i < size;i++)
	{
		Interface *inf = scene->vel.at(i).inter;
		th.proc(Init,inf->type,NULL,NULL,&inf,&nonReady);
	}

	if (nonReady) //выходим и ничего не запускаем потом как нить индикаторами показать что ошибки есть
	{
		return;
	}
	//создаем бесконечный цикл обработки
	if (pool!=NULL)
	{

	}
	else
	{
		pool = new ObjectPool(this,arr,this->sizeM);			//создание пула потоков
	}
	int c = scene->listSize;

		for (int i = 0; i < c;i++)
		{
			trd = pool->createNewObject();
			trd->setAddr((void*)scene->elList[i].inter);
			trd->start(QThread::LowPriority);
		}
		inWork=true;
}

void Manager::run()
{
	init();
}
void Manager::stopProcessing(bool onlyStop)
{
	if (pool==NULL)
	{
		return;
	}
		for (size_t i = 0; i < pool->m_pool.size(); ++i)
		{
			pool->m_pool[i].instance->processStop(onlyStop);
			pool->m_pool[i].instance->quit();
		}		
		inWork = false;
}
