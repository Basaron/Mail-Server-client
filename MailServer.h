#ifndef _MAIL_SERVER_H_
#define _MAIL_SERVER_H_

#include "infoSerCli.h"

class MailServer : public olc::net::server_interface<CustomMsgTypes>
{
private:
    
public:
    MailServer(uint16_t nPort) : olc::net::server_interface<CustomMsgTypes>(nPort)
    {
		//mkdir("Server_Mails",0777);

    }
    
	//Server side functions
    void processSendMail();
    void createConnection();
    void getConnection();
    void getUsers();
	bool findClientAndSend(uint32_t clientID, olc::net::message<CustomMsgTypes>& msg);
	std::string checkContact(uint32_t myID, uint32_t userId);

protected:
	virtual bool OnClientConnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client)
	{
		//Accepts a client trying to connect
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerAccept;
		client->Send(msg);
		return true;
	}

	// Called when a client appears to have disconnected
	virtual void OnClientDisconnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client)
	{
		std::cout << "Removing client [" << client->GetID() << "]\n";
	}

	// Called when a message arrives
	virtual void OnMessage(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, olc::net::message<CustomMsgTypes>& msg)
	{
		//Switch statement to determine what to do depending on msg type as specified in infoSerCli.h
		switch (msg.header.id)
		{
		//in case of msg type updateInbox	
		case CustomMsgTypes::updateInbox:
		{
			//gets the client ID and converts to a string
			uint32_t myID = client->GetID();
			std::stringstream myIDs;
			myIDs << myID;
			std::string myIDstr;
			myIDs >> myIDstr;

			std::string contact;
			std::string c1;
			std::string c2;

			uint32_t count = 10000;
			
			//instance of a message
			olc::net::message<CustomMsgTypes> msg1;
			msg1.header.id = CustomMsgTypes::updateInbox;
			//Open the contact file
			std::fstream contactFile;
			contactFile.open("contacts.txt", std::ios::in);
			//Read the two contact id's of each line in the contact file
			while (std::getline(contactFile, contact))
			{
				c1 = contact.substr(0,5);
				c2 = contact.substr(6,5);

				//If myid matches an id in the file
				if (myIDstr == c1 || myIDstr == c2)
				{
					//contactID is the id of the contact between c1 and c2
					std::string contactID = contact.substr(12, 10);
					std::string mails;
					std::fstream mailsFile;
					//open the mail file between these two contacts
					mailsFile.open(contactID + "/mails.txt", std::fstream::in);
					
					
					while (std::getline(mailsFile, mails))
					{
						//if myID is not the first ID in the line
						std::string check;
						check = mails.substr(0,5);
						if(!(check == myIDstr))
						{
							//add the mails to this file
							std::fstream mailFile;
							mailFile.open(contactID + "/" + mails, std::ios::in);

							std::string mailstr;
							getline (mailFile, mailstr);

							//Copies the content of the mail sent into a file in the inbox folder
							char mailText[mailSize] = {0};
							std::strcpy(mailText, mailstr.c_str());				
							for (int i = 0; i < mailSize; i++)
							{
								
								msg1 << mailText[i];
								
							}
							
							char topicMail[51] = {0};
							std::strcpy(topicMail, mails.c_str());
													
							msg1 << topicMail;

							count = count + 1;
							mailFile.close();
						}
					}
					
					mailsFile.close();
					
				}

			}
			//If myID was the first id do nothing
			contactFile.close();
			
			msg1 << count;
			client->Send(msg1);

		}
		break;

		//message type is sendMail
		case CustomMsgTypes::sendMail:
		{	
			//get myID
			uint32_t myID = client->GetID();
			//the chosen id to send mail to
			uint32_t userID;
			msg >> userID;

			//Checks if a connection exists
			std::string connection = checkContact(myID, userID);

			//in case of no contact
			if(connection == "NoContact"){
								
				//specify msg type
				olc::net::message<CustomMsgTypes> msg;
				msg.header.id = CustomMsgTypes::sendMail;
				//char for error message and send error msg to client
				char error[60] = "No such contact is available, please check the entered ID";
				msg << error;

				client->Send(msg);

				break;

			}

			//get the msg for the topic
			char topic[51] = {0};
			//insert the characters
			for (int i = 0; i < 51; i++)
			{
				msg >> topic[i];
			}
			
			//Makes a string that holds the sender ID and the topic which will create the name of the mail file
			std::stringstream tooString;
			tooString  << myID << "_";
			for (int i = 50 ; i > 0; i--)
			{
				tooString << topic[i];
			}

			//string that gets the filename
			std::string fileName;
       		std::getline(tooString, fileName);
			
			//opens/creates the mail
			std::fstream mailFile;
			mailFile.open(connection + "/" + fileName  + ".txt", std::ios::out);

			//Get the mail content message
			char mail[mailSize] = {0};
			for (int i = 0; i < mailSize; i++)
			{
				msg >> mail[i];
			}
			
			//makes string that holds mail content
			std::stringstream tooString2;
			std::string mailstr;
			for (int i = mailSize - 1; i > 0; i--)
			{
				tooString2 << mail[i];
			}
			
			
       		std::getline(tooString2, mailstr);
			std::string sendPlz = mailstr.c_str();
			mailFile  << sendPlz;

			
			//inserts the mail into the mails.txt file
			std::fstream mailsFile;
			mailsFile.open(connection + "/mails.txt", std::fstream::app);
			mailsFile << fileName.c_str() << "\n";

			//sends the mail
			olc::net::message<CustomMsgTypes> msg;
			msg.header.id = CustomMsgTypes::sendMail;
			char message[60] = "The mail is sent";
			msg << message;

			client->Send(msg);

			mailFile.close();
			mailsFile.close();
			
		}
		break;
		
		//message type deleteMail
		case CustomMsgTypes::deleteMail:
		{
			//get the topic of the msg
			char topic[51] = {0};
			for (int i = 0; i < 51; i++)
			{
				msg >> topic[i];
			}

			//convert to string
			std::stringstream tooString;
			for (int i = 50 ; i > 0; i--)
			{
				tooString << topic[i];
			}

			std::string fileName;
       		std::getline(tooString, fileName);
			//get clientID
			uint32_t myID = client->GetID();
			//convert clientID to string
			std::stringstream myIDs;
			myIDs << myID;
			std::string myIDstr;
			myIDs >> myIDstr;

			std::string contact;
			std::string c1;
			std::string c2;
			//open the contact.txt file for writing
			std::fstream contactFile;
			contactFile.open("contacts.txt", std::ios::in);

			while (std::getline(contactFile, contact))
			{
				//initialize c1 and c2 to the two client id's in a contact
				c1 = contact.substr(0,5);
				c2 = contact.substr(6,5);

				//If myID is one of the c1 or c2
				if (myIDstr == c1 || myIDstr == c2)
				{
					//ID of the connection between c1 and c2
					std::string contactID = contact.substr(12, 10);
					std::string mails;
					std::fstream mailsFile;
					//open/create a file named by contactID 
					mailsFile.open(contactID + "/mails.txt", std::fstream::in);
					while (std::getline(mailsFile, mails))
					{	
						//compare filename and mail
						if (strcmp(fileName.c_str(), mails.c_str()) == 0)
						{
							//remove the string containing the mail
							std::string deletinator5000 = contactID + "/" + mails;
							std::remove(deletinator5000.c_str());
							
						}
						
					}

					mailsFile.close();
					//open the mails.txt file in contactID folder
					std::fstream content;
					content.open(contactID + "/mails.txt", std::fstream::in);
					std::string deletMail;
					std::fstream temp;
					//Open/create a temp of same file
					temp.open(contactID + "/temp.txt", std::fstream::out);
					
					//loop through the lines in the file
					while (std::getline(content, deletMail))
					{   
						//when the strings are not equal copy into temp file
						if (strcmp(fileName.c_str(), deletMail.c_str()) == 0)
						{

						}
						else
						{	
							temp << deletMail << std::endl;
						}
							
					} 

					content.close();
					temp.close();

					//delete the original file and rename the temp to orginal filename
					std::string destroyer9000 = contactID +"/mails.txt";
					std::string destroyer9000temp = contactID +"/temp.txt";
					std::remove(destroyer9000.c_str());
					std::rename(destroyer9000temp.c_str(), destroyer9000.c_str());	

				}
				contactFile.close();
			}

		}
		break;

		//message tyoe createContact
		case CustomMsgTypes::createContact:
		{
			
			uint32_t contactedID;
			uint32_t contactorID = client->GetID();
			char err[50] = "Contact does not exist or an error occured";
			char sent[50] = "Message has been sent";
			//contactedID gets msg
			msg >> contactedID;

			olc::net::message<CustomMsgTypes> msg1;
			msg1.header.id = CustomMsgTypes::accDccContact;
			//if the wanted contact exists send msg
			msg1 << contactorID;
			if(findClientAndSend(contactedID, msg1))
			{
				msg << sent;
				client->Send(msg);
			}else//if msg does not exist send error message
			{
				msg << err;
				client->Send(msg);
			}
			

		}
		break;
		//meesage type accDccContact
		case CustomMsgTypes::accDccContact:
		{

			//initialize varaibles
			bool verify;
			uint32_t contatorID;
			uint32_t contatedID = client->GetID();

			msg >> verify;
			msg >> contatorID;

			if (verify)
			{
				//if contact is acceptet open/create contacts.txt file and write the established contact
				std::fstream contactFile;
				contactFile.open("contacts.txt", std::fstream::app);
				contactFile <<  contatorID << " " << contatedID << " " << contatorID << contatedID << "\n";

				std::stringstream tooString;
				tooString << contatorID;
				tooString << contatedID;

				std::string dircektoryName;

				tooString >> dircektoryName;
				
				mkdir(dircektoryName.c_str(),0777);

				contactFile.close();
			}

		}//else contact is declined
		break;

		}
	}

};

//
bool MailServer::findClientAndSend(uint32_t clientID, olc::net::message<CustomMsgTypes>& msg)
{
		
	// Iterate through all clients in container
	for (auto& client : m_deqConnections)
	{
		// Check client is connected...
		if (client && client->IsConnected() && client->GetID() == clientID)
		{
			client->Send(msg);
			return true;
		}
	}

	return false;			
}

std::string MailServer::checkContact(uint32_t myID, uint32_t userID)
{
	//open/create contacts.txt file
	std::fstream contactFile;
	contactFile.open("contacts.txt", std::ios::in);
	
	//convert myID and userID to strings
	std::stringstream myIDs;
	myIDs << myID;
	std::string myIDstr;
	myIDs >> myIDstr;

	std::stringstream userIds;
	userIds << userID;
	std::string userIDstr;
	userIds >> userIDstr;


	std::string contact;
	std::string c1;
	std::string c2;
	//iterate every line in contacts.txt
	while (std::getline(contactFile, contact))
	{	//set c1 and c2 to the id's in the file
		c1 = contact.substr(0,5);
		c2 = contact.substr(6,5);
		//if c1 and c2 match myID and userID return combined contactID
		if ((myIDstr == c1 && userIDstr == c2)||(myIDstr == c2 && userIDstr == c1))
		{
			std::string contactID = contact.substr(12, 10);
			return contactID;
		}
		
	}
	//else return NoContact
	std::string NoContact = "NoContact";
	return NoContact;
}



#endif
