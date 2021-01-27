#include "MailServer.h"


int main()
{
	//server instance
    MailServer server(60000); 
	//Start server
	server.Start();
	while (1)
	{
		//Force server to respond to messages
		server.Update(-1, true);
	}
}
