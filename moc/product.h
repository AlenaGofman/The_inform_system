#ifndef PRODUCT_H
#define PRODUCT_H

#include<qvalidator.h>
#include<qtimer.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/un.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<qmessagebox.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/time.h>
#include<qapplication.h>
#include<qmenubar.h>
#include<qpopupmenu.h>
#include<qmessagebox.h>
#include<qvariant.h>
#include<qdialog.h>
#include<qlineedit.h>
#include "inifile.h"
#include "mainaddressform.h"
#include "productivity.h"

class ProductForm: public prodform
{
	Q_OBJECT

public:
	ProductForm(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
	~ProductForm(){};

private:
	ProductForm *form1;

private  slots:
	int abolition();
	int delivery();
	int SendMessage();
	void OutInfo();
	void save_struct();

public slots:
	void MainAddress();
};

#endif
