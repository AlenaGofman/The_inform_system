#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iptraffic.h"
#include "qtrus.h"
#include "types.h"
#include "struct_all.h"

#define rus(x) QString::fromLocal8Bit(x)
#define RUS(x) ((x).local8Bit())
#define MAX_PRODUCT_LEN 500 //maximum data size

static char buffer1[MAX_PRODUCT_LEN]; //buffer
static int cur_len = 0; //counter
struct ident_number d5; //pointer to structure, stream identifier

IPTrafficForm::IPTrafficForm(QWidget *parent, const char *name, WFlags fl)
{
	//stream identifier
	FILE *fp;
	if((fp = fopen("./etc/number_traffic", "r")) == NULL) //if such a file does not exist
	{
		fp = fopen("./etc/number_traffic", "w"); //create it
		d5.t = 1; //assign a value equal to one
		fwrite(&d5, sizeof(struct ident_number), 1, fp); //write the value to the file
	}
	else //if file exists
	{
		fread(&d5, sizeof(struct ident_number), 1, fp); //read the value from it
	}
	if(fp)
		fclose(fp); //close file
	EntryInfo(); //call the read function of this file
	return;
}

void IPTrafficForm::MainAddress()
{
	//Action of the save button
	connect((QObject *)saveButton, SIGNAL(clicked()), SLOT(save_struct()));
	//Action button
	connect((QObject *)sendButton, SIGNAL(clicked()), SLOT(delivery()));
	//Reset button action
	connect((QObject *)faultButton, SIGNAL(clicked()), SLOT(abolition()));
	return;
}

void IPTrafficForm::openTable()
{
	if(dataBox->currentText() == QString::fromLocal8Bit("Речь"))
	{
		audio *ad = &(((prim_IPtraffic *)buffer1)->a1); //pointer to nested sturcture speech
		
		AudioGroupBox->setEnabled(true);
		strncpy((char *)ad->audio, RUS(AudioListBox->currentText()), 40);
		VideoGroupBox->setEnabled(false);
		DataGroupBox->setEnabled(true);
	}
	
	if(dataBox->currentText() == QString::fromLocal8Bit("Видео"))
	{
		video *vd = &(((prim_IPtraffic *)buffer1)->v1); //pointer on the embedded machine video
		
		AudioGroupBox->setEnabled(false);
		VideoGroupBox->setEnabled(true);
		strncpy((char *)vd->video, RUS(VideoListBox->currentText()), 60);
		DataGroupBox->setEnabled(true);
	}
	
	if(dataBox->currentText() == QString::fromLocal8Bit("Данные"))
	{
		data *dd = &(((prim_IPtraffic *)buffer1)->dt); //pointer to nested data structure
		
		cur_len = LengthEdit->text().toInt(); //assign the entered data size
			if(cur_len > 1500) //if the data size exceeds 1500 bytes
		cur_len = 1500; //write that data size not exceeding 1500 bytes
			if(cur_len < 64) //if the data size is less than 46 bytes
		cur_len = 64; //write that data size not lower than 46 bytes
		int t = TempEdit->text().toInt();
		if(t>100)
			t = 100;
		if(t<1)
			t = 1;
		int i = IntervalEdit->text().toInt();
		if(i > 600) i = 600; if(i < 30) i = 30;
		
		AudioGroupBox->setEnabled(false);
		VideoGroupBox->setEnabled(false);
		DataGroupBox->setEnabled(true);
		dd->length_packet = cur_len; //package length
		dd->temp = t; //rate of issuance
		dd->interval = i; //dispensing interval
	}
	return;
}

int IPTrafficForm::abolition() //reset
{
	TempEdit->clear();
	LengthEdit->clear();
	IntervalEdit->clear();
	return 0;
}

int IPTrafficForm::delivery() //distribution of tasks
{
	OutInfo(); //function call OutInfo
	SendMessage(); //function call SendMessage
	return 0;
}

void IPTrafficForm::save_struct() //save
{
	if((matchIP(senderIP1->text()) && (matchIP(receiveIP1->text())))
	&&(matchIP(senderIP2->text()) && (matchIP(receiveIP2->text())))
	&&(matchIP(senderIP3->text()) && (matchIP(receiveIP3->text())))
	&&(matchIP(senderIP4->text()) && (matchIP(receiveIP4->text()))))
	{
		OutInfo();
		FILE *fp; 
		fp = fopen("./IP_data", "wb");
		fwrite(buffer1, sizeof(struct prim_IPtraffic), 1, fp);
		fclose(fp);
	} else {
		QMessageBox::warning(this, QString::fromLocal8Bit("Settings"), 
			QString::fromLocal8Bit("Wrong IP-address entered"), "Ok");
	}
	return;
}

void IPTrafficForm::OutInfo() //structure entry
{
	openTable();
	prim_IPtraffic *hdr = (prim_IPtraffic *) buffer1; //structure pointer
	
	//structure entry prim_IPtraffic, selected fields
	strncpy((char*)hdr->zagolovokIP, "abcdefghijkl", 12); //IP header
	//sender IP address
	hdr->sourceIP1 = strtol(senderIP1->text(), NULL, 10);
	hdr->sourceIP2 = strtol(senderIP2->text(), NULL, 10);
	hdr->sourceIP3 = strtol(senderIP3->text(), NULL, 10);
	hdr->sourceIP4 = strtol(senderIP4->text(), NULL, 10);
	//recipient IP address
	hdr->destinationIP1 = strtol(receiveIP1->text(), NULL, 10);
	hdr->destinationIP2 = strtol(receiveIP2->text(), NULL, 10);
	hdr->destinationIP3 = strtol(receiveIP3->text(), NULL, 10);
	hdr->destinationIP4 = strtol(receiveIP4->text(), NULL, 10);
	strncpy((char*)hdr->zagolovokUDP, "mnopqrst", 8); //UDP header
	strncpy((char*)hdr->zagolovokRTP, "uvwxyz123456", 12); //RTP header
	strncpy((char*)hdr->type_stream, RUS(dataBox->currentText()), 8); //Type of traffic: speeach, video, data
	hdr->reserve = 1; //reserve
	hdr->id_stream = IdEdit->text().toInt(); //stream id
	strncpy((char*)hdr->id_packet, "00", 2); //serial number in this stream
	return;
}

bool IPTrafficForm::matchIP(const QString &s) //Function that determines whether the IP-address is entered correctly
{
	QRegExp r("[0-2]?[0-9]?[0-9]$");
	if(r.match(s)!=0)
	{
		return false;
	}
	else
	{
		int pos = 0;
		for (int i = 0; i < 4; i++) {
				int j;
			for (j = 1; j < 4; j++) {
				if (s[pos + j] == '.')
				{
					break;
				}
			}
			QString temp = s.mid(pos, j);
			pos += j + 1;
			
			int res = temp.toInt();
			if (res > 255) //if the value exceeds the number 255
				return false; //error
		}
		return true;
	}
}

int IPTrafficForm::SendMessage()
{
	TIniFile f("data.ini"); //the ini-file class is described in the folder system
	int i;
	int nport = f.GetInt("[mainport]", "port2", 0); //port
	u32 naddr = 0;
	f.GetIP("[mainport]", "ipK", &naddr, 1); //broadcast address
	
	int buf = strtol(LengthEdit->text(), NULL, 10); //recort number

	static int nn = 0;
	printf("%d: port=%d, naddr=%8.8X\n", nn++, nport, naddr);
	
	if(nport<=0)
		nport = 50000;
	
	//Create the address of the communication node
	struct sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_port = htons(nport);
	client.sin_addr.s_addr = naddr;
	
	//check filling cells
	if((matchIP(senderIP1->text()) && (matchIP(receiveIP1->text())))
	&&(matchIP(senderIP2->text()) && (matchIP(receiveIP2->text())))
	&&(matchIP(senderIP3->text()) && (matchIP(receiveIP3->text())))
	&&(matchIP(senderIP4->text()) && (matchIP(receiveIP4->text()))))
	{
		if(buf >= 1) //if the number of packets > 1
		{
			//send data
			OutInfo(); //function call OutInfo
			if(sendto(sock, buffer1, sizeof(struct prim_IPtraffic), 0, (struct sockaddr *)&client, sizeof(client)) < 0)
			{
				char sz[128];
				sprintf(sz, "Data transfer error(%d)\n%s", errno, strerror(errno));
				QMessageBox::information(this, rus("Error"), rus(sz), rus("Ок"));
				return -1;
			}
		
			buf--; //descrease the counter by 1
		
			for(i=0; i<buf; i++) //until the buf counter = 0
			{
				sleep(5); //set the timer between packets = 5 seconds
			
				if(sendto(sock, buffer1, sizeof(struct prim_IPtraffic), 0, (struct sockaddr *)&client, sizeof(client)) < 0)
				{
					char sz[128];
					sprintf(sz, "Data transfer error(%d)\n%s", errno, strerror(errno));
					QMessageBox::information(this, rus("Error"), rus(sz), rus("Ок"));
					return -1;
				}
			}
		
			d5.t++;
			{
				char sz[32];
				sprintf(sz, "%u", d5.t);
				IdEdit->setText(sz);
				open_struct();
			}
		
			//transfer result
			QMessageBox::information(this, rus("Departure..."), rus("Task sent"), rus("Ок"));
			return 0;
		}
	}
	else {
		QMessageBox::warning(this, QString::fromLocal8Bit("Settings"), 
			QString::fromLocal8Bit("Wrong IP-address entered"), "Ok");
	}
	return 0;
}

void IPTrafficForm::open_struct() //to stream id
{
	FILE *fp;
	int i = 0;
	fp = fopen("./etc/number_traffic", "w");
	if(fp)
	{
		fwrite(&d5, sizeof(struct ident_number), 1, fp); //re-write the file with the changed data
		fclose(fp);
	}
	return;
}

void IPTrafficForm::EntryInfo() //to stream id
{
	char sz[32];
	sprintf(sz, "%u", d5.t);
	IdEdit->setText(sz); //reads data from a file as a number
	return;
}
