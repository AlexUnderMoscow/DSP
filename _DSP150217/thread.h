#ifndef THREAD_H
#define THREAD_H

#include <QThread>

#include "fileclass.h"
#include "adaptivefilter.h"
#include "filesave.h"
#include "io.h"
#include "bitgenerator.h"
#include "psk2mod.h"
#include "iqdem.h"
#include "summ.h"
#include "xormod.h"
#include "qpskmod.h"
#include "test1.h"
#include "config.h"

#include "interface.h"
#include "struct.h"

class procThread : public QThread
{
	Q_OBJECT
public:
    procThread(marray** arr, unsigned int *size);
	~procThread();
	void setAddr(void* addr);
	void run();
	void proc(reason rs, moduleType type,QPixmap * pix, QWidget **w, Interface **inf, bool *ready);
	void processStop (bool onlyStop);

private:

	void* addr;
	bool inProcess;					 //признак выполнения бесконечного цикла работы
    marray** arr;
	unsigned int *size;
	Interface* inf;
};

#endif // THREAD_H
