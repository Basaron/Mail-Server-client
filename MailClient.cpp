#include "MailClient.h"


int main()
{
    system("clear"); //clear terminal 

    MailClient c; //instance of a client 
    c.Connect("127.0.0.1", 60000); //connecting the server

    //start screen for the terminal 
    int menu = -1; //default for the switch statement 
    std::string userId;
    std::cout << "Welcome to BEAM Mail" << std::endl;
    std::cout << "Login to continue" << std::endl;
    std::cout <<"UserID: ";
    std::getline(std::cin, userId);

    //variable
    std::string mail; 
    
    //active while loop as long as the user hasn't shut down the system  
    bool bQuit = false;
    while (!bQuit)
    {  
        //if client is connected to server
        if (c.IsConnected())
		{
            //if there are any messages for the clients in the queue then we go through all of them one by one 
			if (!c.Incoming().empty())
			{
				auto msg = c.Incoming().pop_front().msg; //gets the firs element in the queue

                //switch statement for the messages
				switch (msg.header.id)
				{
                    case CustomMsgTypes::ServerAccept:
                    {   
                        //server accepts connection 
                        std::cout << "Server Accepted Connection\n";
                    }
                    break;

                    case CustomMsgTypes::ServerMessage:
                    {
                        // Server has responded	
                        uint32_t clientID;
					    msg >> clientID;
                        std::cout << "Hello from [" << clientID << "]\n";
                        std::cin.get();
                        std::cin.get();
                        menu = -1;
                    }
                    break;

                    case CustomMsgTypes::sendMail:
                    {
                        //prints out the message sent from the server
                        char Message[60];
                        msg >> Message;
                        std::cout << Message << std::endl;

                        std::cout << "Press enter button to continue" << std::endl;
                        std::cin.get();
                        std::cin.get();
                        menu = -1;
                    }
                    break;

                    case CustomMsgTypes::updateInbox:
                    {
                        //updateInbox in the server sends the count as its output. So we insert the count in the variable 
                        uint32_t numberOfMails;
                        msg >> numberOfMails; 

                        //removes the current content.txt in the inbox
                        std::remove("Inbox/content.txt"); 

                        //-10000 is necessary using uint_32, so the count from the server would be 10000 + the actual number of mails 
                        for (int i = numberOfMails - 10000; i > 0; i--)
                        {
                            char topic[51] = {0};
                            
                            //inserting the topics into the array 
                            for (int i = 0; i < 51; i++)
                            {
                                msg >> topic[i];    
                            }

                            //sends the topic into toostring to convert the topics into string 
                            std::stringstream tooString;
                            for (int i = 50 ; i > 0; i--)
                            {
                                tooString << topic[i];
                                
                            }
                            //sending the topic into a filename 
                            std::string fileName = {0};
                            tooString >> fileName;
                            
                            //opens/creates the files with the topic
                            std::fstream mailFile;
                            mailFile.open("Inbox/" + fileName + ".txt", std::ios::out);

                            //get the content of each mail from the server
                            char mail[mailSize] = {0};
                            for (int i = 0; i < mailSize; i++)
                            {
                                msg >> mail[i];                             
                            }
                            
                            //inserting the mail content into a string conversion variable 
                            std::stringstream tooString2;
                            std::string mailstr;
                            for (int i = mailSize - 1; i > 0; i--)
                            {
                                tooString2 << mail[i];                                
                            }
                            
                            //getting the content of the mails and inserts into the mail 
                            std::getline(tooString2, mailstr);
                            std::string sendPlz = mailstr.c_str();
                            mailFile  << sendPlz;

                            //opens/creates the context.txt file and inserts the topics 
                            std::fstream content;
			                content.open("Inbox/content.txt", std::fstream::app);
			                content << fileName.c_str() << "\n";

                            mailFile.close();
                        }
                        
                    }
                    break;

                    case CustomMsgTypes::deleteMail:
                    {
                    }
                    break;

                    case CustomMsgTypes::createContact:
                    {
                        //prints the message received from the server
                        char Message[50];
                        msg >> Message;
                        std::cout << Message << std::endl;

                        std::cin.get();
                        menu = -1;
                    }
                    break;

                    case CustomMsgTypes::accDccContact:
                    {
                        uint32_t contactID;
                        int navNum = 0;
                        bool quit = false;
                        bool verify;

                        //gets the contactID
                        msg >> contactID;

                        std::cout << "\nThe user " << contactID << " wants to contact you" << std::endl;;
                        std::cout << "1. Accept \n2. Decline \n"; //options 
                        std::cin >> navNum; //get number from user for choice 

                        while (!quit)
                        {       
                            switch (navNum)
                            {
                            case 1:
                                verify = true;
                                quit = true;
                                break;
                            
                            case 2:
                                
                                verify = false;
                                quit = true;
                                break;

                            default:
                                std::cout << "Non-defined number please try again" << std::endl;
                                std::cin >> navNum;
                                break;
                            }
                        }
                        //calling the accept/decline contact
                        c.accDccContact(verify, contactID);
                    }
                    break;

				}
			}
		
            //switch statement to navigate through the mail interface
            system("clear"); 
            switch(menu)
            {   
                case 0:
                    
                break; //nothing in case 0 - used when the server is doing a task and we don't want to interrupt it  

                case 1:
                {
                    std::cout << "Inbox" << std::endl;
                    c.updateInbox(); //automatically updates the inbox       
                    
                    std::cout << "\n1. See mails" << std::endl;
                    std::cout << "2. Back to menu" << std::endl;
                    
                    //gets user input for option choice
                    int choser1 = 0;
                    bool quit1 = false;
                    std::cin >> choser1;
                    while (!quit1)
                    {                    
                        
                        switch(choser1)
                        {
                            case 1:                                
                                c.displayInbox();
                                quit1 = true;
                            break;

                            case 2:
                                menu = -1;
                                quit1 = true;
                            break;

                            default:
                                std::cout << "Wrong input try agin" << std::endl;
                                std::cin >> choser1;

                        }
                    }
                }
                break;

                case 2:
                {
                    std::cout << "Send a Mail" << std::endl;
                    std::cout << "\n1. Creat new mail to send" << std::endl;
                    std::cout << "2. Send a drafted mail" << std::endl;

                    int choser2 = 0;
                    bool quit1 = false;
                    std::cin >> choser2;
                    while (!quit1)
                    { 
                        switch (choser2)
                        {
                        case 1:
                        {
                            if(c.createDraft())
                            {
                                menu = -1;
                                quit1 = true;
                            }else
                            {
                                menu = 0;
                                quit1 = true;
                            }
                        }
                        break;
                        
                        case 2:

                        c.displayDraftMAils();

                        menu = -1;
                        quit1 = true;
                        break;
                        
                        default:
                            std::cout << "Wrong input try agin" << std::endl;
                            std::cin >> choser2;
                        break;
                        }
                    }
                    
                }
                break;

                case 3:
                    std::cout << "Contacts" << std::endl;
                    uint32_t contactID;
                    std::cout << "Enter the userID of the contact you want to add" << std::endl;
                    std::cin >>contactID;

                    c.createContact(contactID);

                    menu = 0; 

                break;

                case 4:
                    std::cout << "Flag Mails" << std::endl;

                    c.displayFlagMails();
                    
                    menu = -1;
                    std::cin.get();
                break;

                case 5:
                    std::cout << "Send Mails" << std::endl;
                    c.displaySentMails();

                    menu = -1;
                    std::cin.get();
                break;

                //start screen - this is what turns up when a user id has been entered
                default:
                    std::cout << "Select a destination" << std::endl;
                    std::cout << "1. Inbox" << std::endl;
                    std::cout << "2. Send a Mail" << std::endl;
                    std::cout << "3. Contacts" << std::endl;
                    std::cout << "4. Flag Mails" << std::endl;
                    std::cout << "5. Sendt mails" << std::endl;

                    std::cin >> menu;

            }
        }
		else
		{
			std::cout << "Server Down\n";
			bQuit = true;
		}
            
        
    }

}