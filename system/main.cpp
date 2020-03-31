#include<qapplication.h>
#include "mainform.h"

int sock = -1;
char buffer[500];
char buffer2[500];

int main(int argc, char *argv[])
{
	QApplication app(argc, argv); //class instance creation
	MainForm m;
	app.setMainWidget( &m ); //main application widget
	m.show();
	app.processEvents();

	//global socket
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
	{
		return -1;
	}
	
	//communication node address
	struct sockaddr_in server = {0};
	server.sin_family = AF_INET;
	server.sin_port = htons(9100);
	server.sin_addr.s_addr = 0;
	
	//bind socket to this address
	if(bind(sock, (struct sockaddr *) &server, sizeof(server)) < 0)
	{
		return -1;
	}
	int value = 1;
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &value, sizeof(value));
	
	return app.exec();
}
