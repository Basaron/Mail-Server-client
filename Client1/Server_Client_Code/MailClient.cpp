#include "MailClient.h"


int main()
{
    system("clear");

    MailClient c;
    c.Connect("127.0.0.1", 60000);

    
    int menu = -1;
    std::string userId;
    std::cout << "Welcome to BEAM Mail" << std::endl;
    std::cout << "Login to continue" << std::endl;
    std::cout <<"UserID:";
    std::getline(std::cin, userId);

    std::string mail;
    
    bool bQuit = false;
    while (!bQuit)
    {  

        if (c.IsConnected())
		{
			if (!c.Incoming().empty())
			{


				auto msg = c.Incoming().pop_front().msg;

				switch (msg.header.id)
				{
                    case CustomMsgTypes::ServerAccept:
                    {
                        // Server has 				
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
                        menu = -1;
                    }
                    break;

                    case CustomMsgTypes::sendMail:
                    {
                        char Message[60];
                        msg >> Message;
                        std::cout << Message << std::endl;

                        std::cin.get();
                        menu = -1;
                    }
                    break;

                    case CustomMsgTypes::updateInbox:
                    {
                        uint32_t numberOfMails;
                        msg >> numberOfMails;
                        
                        for (int i = numberOfMails - 10000; i > 0; i--)
                        {
                            char topic[51] = {0};

                            for (int i = 0; i < 51; i++)
                            {
                                msg >> topic[i];
                                
                            }
                            std::stringstream tooString;
                            for (int i = 50 ; i > 0; i--)
                            {
                                tooString << topic[i];
                                
                            }
                            std::string fileName = {0};
                            tooString >> fileName;
                            
                            std::fstream mailFile;
                            
                            mailFile.open("Inbox/" + fileName + ".txt", std::ios::out);

                            char mail[mailSize] = {0};
                            for (int i = 0; i < mailSize; i++)
                            {
                                msg >> mail[i];                             
                            }
                            
                            std::stringstream tooString2;
                            std::string mailstr;
                            for (int i = mailSize - 1; i > 0; i--)
                            {
                                tooString2 << mail[i];                                
                            }
                            
                            std::getline(tooString2, mailstr);
                            std::string sendPlz = mailstr.c_str();
                            mailFile  << sendPlz;

                            std::fstream contant;
			                contant.open("Inbox/content.txt", std::fstream::out);
			                contant << fileName.c_str() << "\n";

                            mailFile.close();
                        }
                        
                    }
                    break;

                    case CustomMsgTypes::replyMail:
                    {
                        
                    }
                    break;

                    case CustomMsgTypes::deleteMail:
                    {
                        
                    }
                    break;

                    case CustomMsgTypes::createContact:
                    {
                        char Message[50];
                        msg >> Message;
                        std::cout << Message << std::endl;

                        std::cin.get();
                        menu = -1;
                    }
                    break;

                    case CustomMsgTypes::accDccContact:
                    {
                        uint32_t contatorID;
                        int navNum = 0;
                        bool quit = false;

                        bool varifie;

                        msg >> contatorID;

                        std::cout << "\nThe user " << contatorID << " wants to contact you" << std::endl;;

                        std::cout << "1. Accept \n2. Decline \n";

                        std::cin >> navNum;

                        while (!quit)
                        {
                                
                            switch (navNum)
                            {
                            case 1:
                                varifie = true;
                                quit = true;
                                break;
                            
                            case 2:
                                
                                varifie = false;
                                quit = true;
                                break;

                            default:
                                std::cout << "Non-defined number please try again" << std::endl;
                                std::cin >> navNum;
                                break;
                            }
                        }
                        c.accDccContact(varifie, contatorID);
                    }
                    break;

				}
			}
		

            //system("clear"); 
            switch(menu)
            {
                case 0:
                    
                    
                break;

                case 1:
                {
                    std::cout << "Inbox" << std::endl;
                    c.updateInbox();
                    c.displayInbox();
                    
                    
                    std::cout << "\n1. See a new mail" << std::endl;
                    std::cout << "2. Back to menu" << std::endl;
                    
                    
                    int choser1 = 0;
                    bool quit1 = false;
                    std::cin >> choser1;
                    while (!quit1)
                    {                    
                        
                        switch(choser1)
                        {
                            case 1:
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
                    std::cout << "Send Mail" << std::endl;
                    c.createDraft();

                    menu = 0;                 
                    
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
                    std::cin.get();
                break;
                case 5:
                    std::cout << "Trash" << std::endl;
                    std::cin.get();
                break;

                default:
                    std::cout << "Select a destination" << std::endl;
                    std::cout << "1. Inbox" << std::endl;
                    std::cout << "2. Send Mails" << std::endl;
                    std::cout << "3. Contacts" << std::endl;
                    std::cout << "4. Flag Mails" << std::endl;
                    std::cout << "5. Trash" << std::endl;

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
