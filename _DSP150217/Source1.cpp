/////////////////////////////////diagramscene
switch (myItemType)
{
case File:
	fileClass *fil;
	fil = new fileClass();
	el.proxy = (QWidget *)fil;
	el.inter = (Interface*)fil;
	break;
//////////////////////////////////mainwindow
	switch (type)
	{
	case File:
		fileClass *fil;
		fil = new fileClass();
		thisinf = (Interface*)fil;
		pix = thisinf->image();
		delete fil;
		break;
///////////////////////////////////thread
		switch (inf->type)
		{
		case File:
			while (inProcess)
			{
				((fileWork*)(inf->widgetAddr))->Do();
			}
			break;
/////////////////////////////////////manager
			switch (inf->type)
			{
			case File:
				((fileWork*)(inf->widgetAddr))->init();
				if (!((fileWork*)(inf->ready))) nonReady = true;
				break;
/////////////////////////////////////
