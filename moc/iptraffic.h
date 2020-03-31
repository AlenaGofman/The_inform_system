#ifndef IPTRAFFIC_H
#define IPTRAFFIC_H

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
#include<qcombobox.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/time.h>
#include<qapplication.h>
#include<qmenubar.h>
#include<qpopupmenu.h>
#include<qmessagebox.h>
#include<qregexp.h>
#include<qvariant.h>
#include<qdialog.h>
#include<qlineedit.h>
#include<qgroupbox.h>
#include<qlistbox.h>
#include "inifile.h"
#include "mainaddressform.h"
#include "IP_traffic.h"

class IPTrafficForm: public IPForm
{
	Q_OBJECT

public:
	IPTrafficForm(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
	~IPTrafficForm(){};

private:

private  slots:
	int abolition();
	int delivery();
	int SendMessage();
	void OutInfo();
	void save_struct();
	void openTable();
	void open_struct();
	void EntryInfo();

public slots:
	void MainAddress();
	bool matchIP(const QString &);
};

#endif
