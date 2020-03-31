#ifndef REGIME_H
#define REGIME_H

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
#include"inifile.h"
#include"mainaddressform.h"
#include"regimeform.h"

class RegimeAddressForm: public regimeform
{
	Q_OBJECT

public:
	RegimeAddressForm(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
	~RegimeAddressForm(){};

private:
	RegimeAddressForm *form1;

private  slots:
	void OutInfo();
	int SendMessage();
	void OllSave();
	int onSend();

public slots:
	void MainAddress();
	void onClose();
};

#endif
