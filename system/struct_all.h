#ifndef STRUCT_ALL_H
#define STRUCT_ALL_H

#define MAX 100
#define MAXL 500
#define MAX_DATA_LEN 1500
#define MAX_MODE_LEN 500
#define MAX_MODE_LEN2 500

#define R	4 
#define S	3
#define P	4
#define T	8

extern char buffer[MAX_MODE_LEN];
extern char buffer2[MAX_MODE_LEN2];
extern int sock;

struct prim1_mode
{
	unsigned char V_prim;
	unsigned char V_serv;
	unsigned char V_zapr;
	unsigned char Retcode;
	unsigned char Mode_video;
	unsigned char Number_form;
};

struct prim_conf
{
	unsigned char V_prim;
	unsigned char V_serv;
	unsigned char V_zapr;
	unsigned char Retcode;
	char Text[R][S][P][T];
	
};

struct prim_product
{
	unsigned char V_prim;
	unsigned char V_serv;
	unsigned char V_zapr;
	unsigned char Retcode;
	unsigned Speed;
	unsigned Length;
	unsigned TimeOut;
};

struct video
{
	unsigned char video[60];
};

struct audio
{
	unsigned char audio[40];
};

struct data
{
	unsigned length_packet;
	unsigned temp;
	unsigned interval;
};

struct prim_IPtraffic
{
	unsigned char zagolovokIP[12];
	unsigned char sourceIP1;
	unsigned char sourceIP2;
	unsigned char sourceIP3;
	unsigned char sourceIP4;
	unsigned char destinationIP1;
	unsigned char destinationIP2;
	unsigned char destinationIP3;
	unsigned char destinationIP4;
	unsigned char zagolovokUDP[8];
	unsigned char zagolovokRTP[12];
	unsigned char type_stream[8];
	unsigned char reserve;
	video v1;
	audio a1;
	data dt;
	unsigned id_stream;
	unsigned char id_packet[2];
};

struct ident_number
{
	u32 t;
};

struct Result
{
	unsigned char V_prim;
	unsigned char V_serv;
	unsigned char V_zapr;
	unsigned char Retcode;
	u32 Ident_traffic;
};

struct name_file
{
	unsigned char nm[3];
	char n[4];
};

#endif
