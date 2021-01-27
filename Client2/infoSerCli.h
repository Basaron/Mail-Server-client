#ifndef _INFO_SER_CLI_H_
#define _INFO_SER_CLI_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <bits/stdc++.h> 
#include <sys/stat.h> 
#include <sys/types.h> 

#include "Server_Client_Code/olc_net.h"

int mailSize = 1000; //greatest amount of characters allowed 

//types of messages for between the server and client 
enum class CustomMsgTypes : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerMessage,
	sendMail,
	updateInbox,
	deleteMail,
	createContact,
	accDccContact,

	//for the client this is the functions used to tell the server which type of message is communicated
};



#endif
