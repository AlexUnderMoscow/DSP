#include "thread.h"

procThread::procThread(marray** _arr, unsigned int *_size)
{
	inProcess = false;
	arr = _arr;
	size=_size;
}

procThread::~procThread()
{
	//this->processStop();
}
void procThread::setAddr(void* _addr)
{
	addr = _addr;
}
void procThread::run()
{
	this->proc(Processing,None,NULL,NULL,NULL,NULL);
}
void procThread::proc(reason rs, moduleType type,QPixmap * pix, QWidget **w, Interface** inter, bool *ready)
{
	if (rs!=Init) inf = (Interface*)addr;
	if (rs==Init) inf = *inter; 
	if (rs==Delete) inf = *inter;
	moduleType Mytype;
	if (rs==Processing) 
	{
		inProcess = true;
		Mytype = inf->type;
	}

	if (type!=None) Mytype = type;
	switch (Mytype)
	{
	case File:
		if (rs==Processing)
		{
			while (inProcess)
			{
				((fileWork*)(inf->widgetAddr))->Do();
			}
		}
		if (rs==getImage)
		{
			fileClass *fil;
			fil = new fileClass(NULL,NULL,NULL);
			inf = (Interface*)fil;
			*pix = inf->image();
			delete fil;
		}
		if (rs==newModule)
		{
			fileClass *fil;
			fil = new fileClass(NULL,arr,size);
			*w = ((QWidget *)fil);
			*inter = ((Interface*)fil);
		}
		if (rs==Init)
		{
			((fileWork*)(inf->widgetAddr))->init();
			if (!((fileWork*)(inf->ready))) *ready = true;
		}
		if (rs==Delete)
		{
			inf->removeArrows();
			inf->form->close();
			fileWork * f = (fileWork *)inf->widgetAddr;
			delete f;
		}
		break;

	case Adaptive:
		if (rs==Processing)
		{
			while (inProcess)
			{
				((AdaptiveWork*)(inf->widgetAddr))->Do();
			}
		}
		if (rs==getImage)
		{
			AdaptiveFilter *fil;
			fil = new AdaptiveFilter(NULL,NULL,NULL);
			inf = (Interface*)fil;
			*pix = inf->image();
			delete fil;
		}
		if (rs==newModule)
		{
			AdaptiveFilter *fil;
			fil = new AdaptiveFilter(NULL,arr,size);
			*w = (QWidget *)fil;
			*inter = (Interface*)fil;
		}
		if (rs==Init)
		{
			((AdaptiveWork*)(inf->widgetAddr))->init();
			if (!((AdaptiveWork*)(inf->ready))) *ready = true;
		}
		if (rs==Delete)
		{
			inf->removeArrows();
			inf->timer->stop();
			inf->form->close();
			AdaptiveWork * f = (AdaptiveWork *)inf->widgetAddr;
			delete f;
		}
		break;

		/**/
	  case Config:
		  if (rs==Processing)
		  {
			  while (inProcess)
			  {
				  ((configWork*)(inf->widgetAddr))->Do();
			  }
		  }
		  if (rs==getImage)
		  {
			  config *fil;
			  fil = new config(NULL,NULL,NULL);
			  inf = (Interface*)fil;
			  *pix = inf->image();
			  delete fil;
		  }
		  if (rs==newModule)
		  {
			  config *fil;
			  fil = new config(NULL,arr,size);
			  *w = (QWidget *)fil;
			  *inter = (Interface*)fil;
		  }
		  if (rs==Init)
		  {
			  ((configWork*)(inf->widgetAddr))->init();
			  if (!((configWork*)(inf->ready))) *ready = true;
		  }
		  if (rs==Delete)
		  {
			  inf->removeArrows();
			  inf->timer->stop();
			  inf->form->close();
			  configWork * f = (configWork *)inf->widgetAddr;
			  delete f;
		  }
		  break;
		/**/

	case OutputFile:
		if (rs==Processing)
		{
			while (inProcess)
			{
				((fileSaveWork*)(inf->widgetAddr))->Do();	
			}
		}

		if (rs==getImage)
		{
			fileSave *fil;
			fil = new fileSave(NULL,NULL,NULL);
			inf = (Interface*)fil;
			*pix = inf->image();
			delete fil;
		}
		if (rs==newModule)
		{
			fileSave *fil;
			fil = new fileSave(NULL,arr,size);
			*w = (QWidget *)fil;
			*inter = (Interface*)fil;
		}
		if (rs==Init)
		{
			((fileSaveWork*)(inf->widgetAddr))->init();
			if (!((fileSaveWork*)(inf->ready))) *ready = true;
		}
		if (rs==Delete)
		{
			inf->removeArrows();
			inf->timer->stop();
			inf->form->close();
			fileSaveWork * f = (fileSaveWork *)inf->widgetAddr;
			delete f;
		}
		break;

	case Io:
		if (rs==Processing)
		{
			while (inProcess)
			{
				((ioWork*)(inf->widgetAddr))->Do();
			}
		}
		if (rs==getImage)
		{
			io *fil;
			fil = new io(NULL,NULL,NULL);
			inf = (Interface*)fil;
			*pix = inf->image();
			delete fil;
		}
		if (rs==newModule)
		{
			io *fil;
			fil = new io(NULL,arr,size);
			*w = (QWidget *)fil;
			*inter = (Interface*)fil;
		}
		if (rs==Init)
		{
			((ioWork*)(inf->widgetAddr))->init();
			if (!((ioWork*)(inf->ready))) *ready = true;
		}
		if (rs==Delete)
		{
			inf->removeArrows();
			inf->timer->stop();
			inf->form->close();
			ioWork * f = (ioWork *)inf->widgetAddr;
			delete f;
		}
		break;

	case bitGen:
		if (rs==Processing)
		{
			while (inProcess)
			{
				((bitGeneratorWork*)(inf->widgetAddr))->Do();
			}
		}
		if (rs==getImage)
		{
			bitGenerator *fil;
			fil = new bitGenerator(NULL,NULL,NULL);
			inf = (Interface*)fil;
			*pix = inf->image();
			delete fil;
		}
		if (rs==newModule)
		{
			bitGenerator *fil;
			fil = new bitGenerator(NULL,arr,size);
			*w = (QWidget *)fil;
			*inter = (Interface*)fil;
		}
		if (rs==Init)
		{
			((bitGeneratorWork*)(inf->widgetAddr))->init();
			if (!((bitGeneratorWork*)(inf->ready))) *ready = true;
		}
		if (rs==Delete)
		{
			inf->removeArrows();
			inf->timer->stop();
			inf->form->close();
			bitGeneratorWork * f = (bitGeneratorWork *)inf->widgetAddr;
			delete f;
		}
		break;

	case PSK2modulator:
		if (rs==Processing)
		{
			while (inProcess)
			{
				((psk2ModWork*)(inf->widgetAddr))->Do();
			}
		}
		if (rs==getImage)
		{
			psk2Mod *fil;
			fil = new psk2Mod(NULL,NULL,NULL);
			inf = (Interface*)fil;
			*pix = inf->image();
			delete fil;
		}
		if (rs==newModule)
		{
			psk2Mod *fil;
			fil = new psk2Mod(NULL,arr,size);
			*w = (QWidget *)fil;
			*inter = (Interface*)fil;
		}
		if (rs==Init)
		{
			((psk2ModWork*)(inf->widgetAddr))->init();
			if (!((psk2ModWork*)(inf->ready))) *ready = true;
		}
		if (rs==Delete)
		{
			inf->removeArrows();
			inf->timer->stop();
			inf->form->close();
			psk2ModWork * f = (psk2ModWork *)inf->widgetAddr;
			delete f;
		}
		break;


	case Summ:
		if (rs==Processing)
		{
			while (inProcess)
			{
				((summWork*)(inf->widgetAddr))->Do();
			}
		}
		if (rs==getImage)
		{
			summ *fil;
			fil = new summ(NULL,NULL,NULL);
			inf = (Interface*)fil;
			*pix = inf->image();
			delete fil;
		}
		if (rs==newModule)
		{
			summ *fil;
			fil = new summ(NULL,arr,size);
			*w = (QWidget *)fil;
			*inter = (Interface*)fil;
		}
		if (rs==Init)
		{
			((summWork*)(inf->widgetAddr))->init();
			if (!((summWork*)(inf->ready))) *ready = true;
		}
		if (rs==Delete)
		{
			inf->removeArrows();
			inf->timer->stop();
			inf->form->close();
			summWork * f = (summWork *)inf->widgetAddr;
			delete f;
		}
		break;


    case Xormod:
		if (rs==Processing)
		{
			while (inProcess)
			{
				((xorWork*)(inf->widgetAddr))->Do();
			}
		}
		if (rs==getImage)
		{
            xormod *fil;
            fil = new xormod(NULL,NULL,NULL);
			inf = (Interface*)fil;
			*pix = inf->image();
			delete fil;
		}
		if (rs==newModule)
		{
            xormod *fil;
            fil = new xormod(NULL,arr,size);
			*w = (QWidget *)fil;
			*inter = (Interface*)fil;
		}
		if (rs==Init)
		{
			((xorWork*)(inf->widgetAddr))->init();
			if (!((xorWork*)(inf->ready))) *ready = true;
		}
		if (rs==Delete)
		{
			inf->removeArrows();
			inf->timer->stop();
			inf->form->close();
			xorWork * f = (xorWork *)inf->widgetAddr;
			delete f;
		}
		break;


/**/
	case QPSKmodulator:
		if (rs==Processing)
		{
			while (inProcess)
			{
				((qpskModWork*)(inf->widgetAddr))->Do();	
			}
		}

		if (rs==getImage)
		{
			QPSKmod *fil;
			fil = new QPSKmod(NULL,NULL,NULL);
			inf = (Interface*)fil;
			*pix = inf->image();
			delete fil;
		}
		if (rs==newModule)
		{
			QPSKmod *fil;
			fil = new QPSKmod(NULL,arr,size);
			*w = (QWidget *)fil;
			*inter = (Interface*)fil;
		}
		if (rs==Init)
		{
			((qpskModWork*)(inf->widgetAddr))->init();
			if (!((qpskModWork*)(inf->ready))) *ready = true;
		}
		if (rs==Delete)
		{
			inf->removeArrows();
			inf->timer->stop();
			inf->form->close();
			qpskModWork * f = (qpskModWork *)inf->widgetAddr;
			delete f;
		}
		break;
		
/**/
    case test1mod:
        if (rs==Processing)
        {
            while (inProcess)
            {
                ((test1Work*)(inf->widgetAddr))->Do();
            }
        }
        if (rs==getImage)
        {
            test1 *fil;
            fil = new test1(NULL,NULL,NULL);
            inf = (Interface*)fil;
            *pix = inf->image();
            delete fil;
        }
        if (rs==newModule)
        {
            test1 *fil;
            fil = new test1(NULL,arr,size);
            *w = (QWidget *)fil;
            *inter = (Interface*)fil;
        }
        if (rs==Init)
        {
            ((test1Work*)(inf->widgetAddr))->init();
            if (!((test1Work*)(inf->ready))) *ready = true;
        }
        if (rs==Delete)
        {
            inf->removeArrows();
            inf->timer->stop();
            inf->form->close();
            xorWork * f = (xorWork *)inf->widgetAddr;
            delete f;
        }
        break;
/**/
	case IQDemodulator:
		if (rs==Processing)
		{
			while (inProcess)
			{
				((iqDemWork*)(inf->widgetAddr))->Do();
			}
		}
		if (rs==getImage)
		{
			IQdem *fil;
			fil = new IQdem(NULL,NULL,NULL);
			inf = (Interface*)fil;
			*pix = inf->image();
			delete fil;
		}
		if (rs==newModule)
		{
			IQdem *fil;
			fil = new IQdem(NULL,arr,size);
			*w = (QWidget *)fil;
			*inter = (Interface*)fil;
		}
		if (rs==Init)
		{
			((iqDemWork*)(inf->widgetAddr))->init();
			if (!((iqDemWork*)(inf->ready))) *ready = true;
		}
		if (rs==Delete)
		{
			inf->removeArrows();
			inf->timer->stop();
			inf->form->close();
			iqDemWork * f = (iqDemWork*)inf->widgetAddr;
			delete f;
		}
		break;
	}
}

void procThread::processStop(bool onlyStop)
{
    inProcess = false;
	if (!onlyStop)
	{
		for (int i=0;i<inf->arrows.size();i++)
		{
			inf->arrows.at(i)->queue->Lock();
		}
	}
    //usleep(50);
}
