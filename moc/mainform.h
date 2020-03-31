#ifndef MAINFORM_H
#define MAINFORM_H

#include<qapplication.h>
#include<qmenubar.h>
#include<qpopupmenu.h>
#include<qmessagebox.h>
#include<qvalidator.h>
#include<qvariant.h>
#include<qdialog.h>
#include<qtimer.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/un.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/time.h>
#include<qlistview.h>
#include<qlayout.h>
#include<qaction.h>
#include "mainaddressform.h"
#include "productivity.h"
#include "regimeform.h"

class MainForm: public mainaddress
{
	Q_OBJECT

public:
	MainForm(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
	~MainForm(){};

public slots:
	
private:

private slots:
	void connectToProduct();
	void connectToRegime();
	void connectToIPData();

protected slots:
	
protected:

};

#endif
