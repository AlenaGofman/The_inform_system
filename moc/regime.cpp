#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"types.h"
#include"regime.h"
#include"struct_all.h"

#define rus(x) QString::fromLocal8Bit(x)
#define MAX_MODE_LEN 500

static char buffer1[MAX_MODE_LEN];

RegimeAddressForm::RegimeAddressForm(QWidget *parent, const char *name, WFlags fl)
{
	parent = parent;
	name = name;
	fl = fl;
	return;
}

void RegimeAddressForm::MainAddress()
{
	connect((QObject *)sendButton, SIGNAL(clicked()), SLOT(onSend()));
	return;
}

void RegimeAddressForm::onClose()
{
	connect((QObject *)closeButton, SIGNAL(clicked()), SLOT(onClose()));
	return;
}

int RegimeAddressForm::onSend()
{
	OutInfo();
	OllSave();
	SendMessage();
	return 0;
}

void RegimeAddressForm::OutInfo()
{
	prim1_mode *hdr = (prim1_mode *) buffer1;
	hdr->V_prim= 0x01;
	hdr->V_serv= 0x01;
	hdr->V_zapr= 0x01;
	hdr->Retcode= 0x00;
	hdr->Mode_video = modelEdit->text().toInt();
	hdr->Number_form = numberEdit->text().toInt();
	return;
}

void RegimeAddressForm::OllSave()
{
	FILE *fp;
	fp = fopen("./regim", "wb");
	fwrite(buffer1, sizeof (struct prim1_mode), 1, fp);
	fclose(fp);
	return;
}

int RegimeAddressForm::SendMessage()
{
	TIniFile f("data.ini");
	int nport = f.GetInt("[mainport]", "port2", 0);
	u32 naddr = 0;
	f.GetIP("[mainport]", "ipR", &naddr, 1);
	
	static int nn = 0;
	printf("%d: port=%d, naddr=%8.8X\n", nn++, nport, naddr);
	
	if(nport<=0)
		nport = 50000;
	
	struct sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_port = htons(nport);
	client.sin_addr.s_addr = naddr;
	
	if(sendto(sock, buffer1, sizeof(struct prim1_mode), 0, (struct sockaddr *)&client, sizeof(client)) < 0)
	{
		char sz[128];
		sprintf(sz, "Data transfer error(%d)\n%s", errno, strerror(errno));
		QMessageBox::information(this, rus("Error"), rus(sz), rus("ïË"));
		return -1;
	}
	QMessageBox::information(this, rus("Departure..."), rus("Task sent"), rus("ïË"));
	return 0;
}
