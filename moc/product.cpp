#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "product.h"
#include "qtrus.h"
#include "types.h"
#include "struct_all.h"

#define MAX_PRODUCT_LEN 500

static char buffer1[MAX_PRODUCT_LEN];
static int cur_len = 0;

ProductForm::ProductForm(QWidget *parent, const char *name, WFlags fl)
{
	parent = parent;
	name = name;
	fl = fl;
	return;
}

void ProductForm::MainAddress()
{
	connect((QObject *)saveButton, SIGNAL(clicked()), SLOT(save_struct()));
	connect((QObject *)sendButton, SIGNAL(clicked()), SLOT(delivery()));
	connect((QObject *)faultButton, SIGNAL(clicked()), SLOT(abolition()));
	return;
}

int ProductForm::abolition()
{
	speedEdit->clear();
	lengthEdit->clear();
	timeoutEdit->clear();
	return 0;
}

int ProductForm::delivery()
{
	OutInfo();
	SendMessage();
	return 0;
}

void ProductForm::save_struct()
{
	OutInfo();
	FILE *fp;
	fp = fopen("./product", "wb");
	fwrite(buffer1, sizeof(struct prim_product), 1, fp);
	fclose(fp);
	return;
}

void ProductForm::OutInfo()
{
	prim_product *hdr = (prim_product *) buffer1; //buffer pointer
	
	//structure entry prim_product
	cur_len = lengthEdit->text().toInt(); //assign the entered data size
	if(cur_len > 1500) //while the size does not exceed  1500 byte
		cur_len = 1500; //write the size equal = 1500
	if(cur_len < 64) //if the size is smaller than 64 byte
		cur_len = 64; //write the size = 64 byte
	int t = speedEdit->text().toInt(); //assign the entered data size
	if(t > 100) t = 100; if(t < 1) t = 1; //if the data size is larger 1000 or smaller than 1, 
											//assign the minimum and maximum value to the variable
	int i = timeoutEdit->text().toInt();
	if(i > 600) i = 600; if(i < 30) i = 30; //if the value is greater than 60 seconds or less than 1 second, 
										//assign the minimum and maximum value to the variable
	hdr->V_prim = 0x02;
	hdr->V_serv = 0x01;
	hdr->V_zapr = 0x01;
	hdr->Retcode = 0x00;
	hdr->Speed = t;
	hdr->Length = cur_len;
	hdr->TimeOut = i;
	return;
}

int ProductForm::SendMessage()
{
	TIniFile f("data.ini");
	int nport = f.GetInt("[mainport]", "port2", 0);
	int nport2 = f.GetInt("[mainport]", "port", 0);
	
	u32 naddr = 0;
	u32 naddrR = 0;

	f.GetIP("[mainport]", "ipK", &naddr, 1);
	f.GetIP("[mainport]", "ipR", &naddrR, 1);
	
	static int nn = 0;
	printf("%d: port=%d, naddr=%8.8X\n", nn++, nport, naddr);
	printf("%d: port2 = %d, naddrR=%8.8X\n", nn++, nport2, naddrR);
	
	if(nport<=0)
		nport = 50000;
	if(nport2<=0)
		nport2 = 50000;
	
	struct sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_port = htons(nport);
	client.sin_addr.s_addr = naddr;
	
	OutInfo();
	if(sendto(sock, ::buffer1, sizeof(struct prim_product), 0, (struct sockaddr *)&client, sizeof(client)) < 0)
	{
		char sz[128];
		sprintf(sz, "Data transfer error(%d)\n%s", errno, strerror(errno));
		QMessageBox::information(this, rus("Error"), rus(sz), rus("ïË"));
		return -1;
	}
	QMessageBox::information(this, rus("Departure..."), rus("Task sent K1-K2"), rus("ïË"));
	
	struct sockaddr_in client2;
	client2.sin_family = AF_INET;
	client2.sin_port = htons(nport2);
	client2.sin_addr.s_addr = naddrR;
	
	OutInfo();
	if(sendto(sock, ::buffer1, sizeof(struct prim_product), 0, (struct sockaddr *)&client2, sizeof(client2)) < 0)
	{
		char sz[128];
		sprintf(sz, "Data transfer error(%d)\n%s", errno, strerror(errno));
		QMessageBox::information(this, rus("Error"), rus(sz), rus("ïË"));
		return -1;
	}
	QMessageBox::information(this, rus("Departure..."), rus("Task sent K0"), rus("ïË"));
	return 0;
}
