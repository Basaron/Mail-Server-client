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

int mailSize = 1000;

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
};



#endif
