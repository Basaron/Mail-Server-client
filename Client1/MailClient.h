#ifndef _MAIL_CLIENT_H_
#define _MAIL_CLIENT_H_

#include "infoSerCli.h"

//class for the client - inherits the server-client interface 
class MailClient : public olc::net::client_interface<CustomMsgTypes>
{
private:
    std::string userId;

    //types of mails 
    enum class mailType : uint32_t
    {
        send,
        draft,
        forward,
    };
    
public:
    MailClient();
    ~MailClient();

    void updateInbox();
    void displayInbox();
    void displayFlagMails();
    void displaySentMails();
    void displayDraftMAils();
    void sendMail(mailType mT, char topicMail[]);
    void forwardMail(std::string mail);
    void replyMail(std::string mail);
    void deleteMail(std::string mail);
    void flagMail(std::string mail);
    bool createDraft();
    void createContact(uint32_t sendID);
    void accDccContact(bool verify, uint32_t contactID); //accept/decline contact
};


//constructor - makes the directories for the client 
MailClient::MailClient()
{
    mkdir("Inbox",0777);
    mkdir("Flag_Mails",0777);
    mkdir("Draft",0777);
    mkdir("Sent_Mails",0777);
}


MailClient::~MailClient()
{
}

void MailClient::sendMail(mailType mT, char topicMail[51]) //input: enum class type instance and maximum char array (the topic) of 50 characters 
{
    std::string mail;

    //switch statement for each of the possible types in the enum class 
    //this switch statement is used to get the content of an mail 
    switch (mT)
    {
    case mailType::send:
    {
        //converting the input char array to a string 
        std::stringstream tooString;
        tooString << topicMail;
        std::string fileName;
        tooString >> fileName;

        //open the desired file inside the send_mail directory
        //if the file is not there, then the file is made (however it is expected it is already there, since a topic is send as input) 
        std::ifstream mailFile ("Send_Mails/" + fileName + ".txt");
        getline (mailFile, mail); //getting the content/text in the mail 
        mailFile.close(); 
    }
    break;
    
    //the same happens for the rest of the cases - only difference is the directory 
    case mailType::forward:
    {   
    
        std::stringstream tooString;
        tooString << topicMail;
        std::string fileName;
        tooString >> fileName;

        std::ifstream mailFile ("Inbox/" + fileName);
        getline (mailFile, mail);
        mailFile.close();
    }
    break;

    case mailType::draft:
    {
        std::stringstream tooString;
        tooString << topicMail;
        std::string fileName;
        tooString >> fileName;

        std::ifstream mailFile ("Draft/" + fileName + ".txt");
        getline (mailFile, mail);
        mailFile.close();
    }
    break;

    default:
        std::cout << "error" << std::endl;
    break;
    }
    
    //variable that specifies which type of message is sent to the server (specified in infoSerCli.h) 
    olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::sendMail; //the type 


    //initializing a char array to 1000 zeros 
    char mailChar[mailSize] = {0};

    //for loops that inserts the input characters into the char array 
    std::strcpy(mailChar, mail.c_str()); //converting the mail string to the char array 
    for (int i = 0; i < mailSize; i++)
    {
        msg << mailChar[i]; //the text of the mail
    }

    for (int i = 0; i < 51; i++)
    {
        msg << topicMail[i]; //the topic of the mail 
    }
    

    //getting the topic of the mail 
    std::fstream content; //content is an txt file that holds all the topics of the mails 
	content.open("Send_Mails/content.txt", std::fstream::app); //app makes sure no overrides happen 
	content << topicMail << "\n";
    content.close();

    //sending the mail 
    uint32_t mailToUserID;
    std::cout << "Which user do you want to send to" << std::endl;
    std::cin >> mailToUserID; //getting the receiver ID 

    msg << mailToUserID; 
    
    Send(msg); //sending info to server

}


void MailClient::updateInbox()
{
    olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::updateInbox;

    Send(msg);
}


void MailClient::displayDraftMAils()
{
    std::fstream content;
	content.open("Draft/content.txt", std::fstream::in); //enters content.txt  
    std::string draft[50]; //maximum size of the mail 
    
    int i = 0;

    //while loop to go through the topics of content.txt. 
    while (std::getline(content, draft[i]))
    {   
        //assigning each topic to a number in an array - This is used so the user can select a topic and thus select a file
        std::cout << i << ": " << draft[i] << std::endl;
        i++;
    }

    //if we get here, then there is nothing in the folder
    if (i == 0)
    {
        std::cout << "The Draft folder is empty" << std::endl;
        content.close();

        std::cout << "Press any button to continue" << std::endl;
        std::cin.get();
        std::cin.get(); 
        return;
    }
    

    //getting the index number of the desired mail from the user
    int numMail;

    std::cout << "Choose a mail with a index number" << std::endl;
    std::cin >> numMail;

    //displaying the mail 
    std::string mailText;
    std::fstream mailToDisplay;
	mailToDisplay.open("Draft/" + draft[numMail] + ".txt", std::ios::in);
    std::cout << "Mail content:" << std::endl;
    while (std::getline(mailToDisplay, mailText))
    {
        std::cout << mailText << std::endl;
    }

    //options for the draft mails 
    std::cout << "1. Back" << std::endl;
    std::cout << "2. Send mail" << std::endl;

    int choser1 = 0;
    bool quit1 = false;
    std::cin >> choser1; //choosing an option 
    while (!quit1)
    {                    
        switch(choser1)
        {
            case 1:
                quit1 = true; //stops the while loop -> "1. back"
            break;

            case 2: //begins to send the mail 
            {
                char topicMail[51] = {0};
				std::strcpy(topicMail, draft[numMail].c_str());
                
                sendMail(mailType::draft ,topicMail);
                quit1 = true;
            }
            break;

            default:
                std::cout << "Wrong input try again" << std::endl;
                std::cin >> choser1;
            break;
        }
    }
}



void MailClient::displaySentMails()
{
    std::fstream content;
	content.open("Sent_Mails/content.txt", std::fstream::in); 
    std::string sentMails[50];

    int i = 0;
    
    //while loop to assign numbers to the topics 
    while (std::getline(content, sentMails[i]))
    {   
        std::cout << i << ": " << sentMails[i] << std::endl;
        i++;
    }

    if (i == 0)
    {
        std::cout << "The Sent_Mails folder is empty" << std::endl;
        content.close();
        std::cout << "Press any button to continue" << std::endl;
        std::cin.get();
        std::cin.get();
        return;
    }
    

    int numMail;

    std::cout << "Choose a mail with a index number" << std::endl;
    std::cin >> numMail;

    std::string mailText;
    std::fstream mailToDisplay;
	mailToDisplay.open("Send_Mails/" + sentMails[numMail] + ".txt", std::ios::in);
    std::cout << "Mail content:" << std::endl;
    while (std::getline(mailToDisplay, mailText))
    {
        std::cout << mailText << std::endl;
    }

    //since this is sent mails, nothing more can be done 
    std::cout << "Press any button to continue" << std::endl;
    std::cin.get();
    std::cin.get();
}


//same as before - only difference is the directory 
void MailClient::displayFlagMails()
{
    std::fstream content;
	content.open("Flag_Mails/content.txt", std::fstream::in);
    std::string flagMails[50];
    int i = 0;
    while (std::getline(content, flagMails[i]))
    {   
        std::cout << i << ": " << flagMails[i] << std::endl;
        i++;
    }

    if (i == 0)
    {
        std::cout << "The Flag_Mails folder is empty" << std::endl;
        content.close();
        std::cout << "Press any button to continue" << std::endl;
        std::cin.get();
        std::cin.get();
        return;
    }
    

    int numMail;

    std::cout << "Choose a mail with a index number" << std::endl;
    std::cin >> numMail;

    std::string mailText;
    std::fstream mailToDisplay;
	mailToDisplay.open("Flag_Mails/" + flagMails[numMail], std::ios::in);
    std::cout << "Mail content:" << std::endl;
    while (std::getline(mailToDisplay, mailText))
    {
        std::cout << mailText << std::endl;
    }

    std::cout << "Press any button to continue" << std::endl;
    std::cin.get();
    std::cin.get();
}



void MailClient::displayInbox()
{
    std::fstream content;
	content.open("Inbox/content.txt", std::fstream::in);
    std::string inbox[50];
    
    int i = 0;
    
    while (std::getline(content, inbox[i]))
    {   
        std::cout << i << ": " << inbox[i] << std::endl;
        i++;
    }

    if (i == 0)
    {
        std::cout << "The Inbox is empty" << std::endl;
        content.close();

        std::cout << "Press any button to continue" << std::endl;
        std::cin.get();
        std::cin.get();
        return;
    }
    
    int numMail;

    std::cout << "Choose a mail with a index number" << std::endl;
    std::cin >> numMail;

    std::string mailText;
    std::fstream mailToDisplay;
	mailToDisplay.open("Inbox/" + inbox[numMail], std::ios::in);
    std::cout << "Mail content:" << std::endl;
    
    while (std::getline(mailToDisplay, mailText))
    {
        std::cout << mailText << std::endl;
    }

    std::cout << "Press any button to continue" << std::endl;
    std::cin.get();
    std::cin.get();

    //options for the inbox folder
    std::cout << "1. Back" << std::endl;
    std::cout << "2. Delete mail" << std::endl;
    std::cout << "3. Forward mail" << std::endl;
    std::cout << "4. Reply mail" << std::endl;
    std::cout << "5. Flag mail" << std::endl;

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
                deleteMail(inbox[numMail]);
                quit1 = true;
            break;

            case 3:
                forwardMail(inbox[numMail]);
                quit1 = true;
            break;
            case 4:
                replyMail(inbox[numMail]);
                quit1 = true;
            break;
            case 5:
                flagMail(inbox[numMail]);
                quit1 = true;
            break;
            default:
                std::cout << "Wrong input try again" << std::endl;
                std::cin >> choser1;

        }
    }
    
    std::cout << "Press any button to continue" << std::endl;
    std::cin.get();        
    std::cin.get();        
    content.close();
}


//function to copy a mail from the inbox and insert into flag mails folder
void MailClient::flagMail(std::string mail) //input is topic
{
    char topic[51] = {0};
    std::strcpy(topic, mail.c_str()); //copies the content of the input topic string 

    //opens the file in the inbox
    std::fstream mailFile;
	mailFile.open("Inbox/" + mail, std::ios::in);

    //copies the content of the inbox mail and inserts into a string 
    std::string mailstr;
	getline(mailFile, mailstr);

    mailFile.close();

    //opens/creates the mail in the flag mails folder
    std::fstream mailFlag;
	mailFlag.open("Flag_Mails/" + mail, std::ios::out);

    mailFlag << mailstr; //inserting the content 
    mailFlag.close();

    //inserting the topic into content.txt
    std::fstream flagerMails;
	flagerMails.open("Flag_Mails/content.txt", std::ios::out);

    flagerMails << mail << std::endl;

    flagerMails.close();
}


void MailClient::forwardMail(std::string mail) //input is topic 
{
    char topic[51] = {0};
    std::strcpy(topic, mail.c_str()); //converting to char array to use it as input for sendMail function 

    //calls sendMail 
    sendMail(mailType::forward, topic);

}


void MailClient::replyMail(std::string mail)
{
    //getting the userID
    std::string idSendTo;
	idSendTo = mail.substr(0,5); 

    //converts the string to uint32_t
    std::uint32_t idReceiver = atoi(idSendTo.c_str()); 
    std::string text; //variable to hold the content of the mail 

    //getting the topic of the mail 
    char topic[51] = {0};
    std::cin.get();
    std::cout << "Write the topic of the mail in one word with maximum 40 characters" << std::endl; //possible to insert above
    std::cin >> topic;

    std::cout << "Press any button to continue" << std::endl;
    std::cin.get();
    std::cin.get();

    //gets the topic and sends it to filename 
    std::stringstream tooString; //converts any type to string (our case is char array)
    tooString << topic;
   	std::string fileName;
	tooString >> fileName;

    //getting the content of the mail 
    std::cout << "Write the mail:"<< std::endl;;
    std::getline(std::cin, text);

    //message type for server
    olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::sendMail;

    //for loops that inserts the input characters into the char array 
    char mailChar[mailSize] = {0};
    std::strcpy(mailChar, text.c_str());
    for (int i = 0; i < mailSize; i++)
    {
        msg << mailChar[i];
    }

    for (int i = 0; i < 51; i++)
    {
        msg << topic[i];
    }

    msg << idReceiver; //send mail back to the same ID 

    Send(msg);
}



void MailClient::deleteMail(std::string mail)
{
    //getting the file location and removes it  
    std::string deleter =  "Inbox/" + mail;  
    std::remove(deleter.c_str());

    //enters context.txt
    std::fstream content;
	content.open("Inbox/content.txt");
    
    //making an temporary file as it was not possible to specifically delete string in an txt file 
    std::string inbox;
    std::fstream temp;
	temp.open("Inbox/temp.txt", std::fstream::out);
    
    //copies everything from content.txt to temp.txt
    while (std::getline(content, inbox))
    {   
        if (mail != inbox) //will insert the mail into temp.txt as long as it is not the mail that has to be deleted
        {
            temp << inbox << std::endl;
        }
        
        
    } 
    content.close();
    temp.close();

    //deletes content.txt and renames temp.txt to content.txt 
    std::remove("Inbox/content.txt");
    std::rename("Inbox/temp.txt", "Inbox/content.txt");


    olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::deleteMail;
    
    char topicMail[51] = {0};
	std::strcpy(topicMail, mail.c_str());
													
    msg << topicMail;

    Send(msg);

}


//function to first make a file, which can either be a draft mail, sent or deleted
bool MailClient::createDraft()
{
    int navNum = 0;
    bool quit = false;
    bool save = false;
    bool send = false;

    //getting the topic    
    std::string draftText;
    char topic[51] = {0};
    std::cin.get();
    std::cout << "Write the topic of the mail in one word with maximum 40 characters" << std::endl;
    std::cin >> topic;

    std::cout << "Press any button to continue" << std::endl;
    std::cin.get();
    std::cin.get();

    //converting to string 
    std::stringstream tooString;
    tooString << topic;
   	std::string fileName;
	tooString >> fileName;

    //getting the content of the mail 
    std::cout << "Write the mail:" << std::endl;;
    std::getline(std::cin, draftText);
    
    //options 
    std::cout << std::endl << "1. Save Draft" << std::endl;
    std::cout << "2. Send Mail" << std::endl;
    std::cout << "3. Delete" << std::endl;

    //input option from the user 
    std::cin >> navNum;

    while (!quit)
    {       
        switch (navNum)
        {
        case 1:
            save = true;
            quit = true;
            break;
        
        case 2:
            send = true;
            quit = true;
            break;
        
        case 3:
            quit = true;
            break;

        default:
            std::cout << "Non-defined number please try again" << std::endl;
            std::cin >> navNum;
            std::cin.get();
            break;
        }
    }
    
    //inserting the mail into draft folder
    if (save == true) 
    {
        std::ofstream draftFil("Draft/" + fileName + ".txt");
        draftFil << draftText;
        draftFil.close();

        //inserting the topic into content.txt
        std::fstream content;
	    content.open("Draft/content.txt", std::fstream::app);
	    content << fileName << "\n";
        content.close();
    } 
    //sending the mail 
    else if (send == true)
    {
        std::ofstream draftFil("Send_Mails/" + fileName + ".txt"); //creating the file 
        draftFil << draftText;
        draftFil.close();
        sendMail(mailType::send, topic);

        return false;
        
    }
    return true;
}


void MailClient::createContact(uint32_t contactID)
{
    olc::net::message<CustomMsgTypes> msg;

    msg.header.id = CustomMsgTypes::createContact;

    msg << contactID; //input ID 

    Send(msg);

}


void MailClient::accDccContact(bool verify, uint32_t contactID)
{
    olc::net::message<CustomMsgTypes> msg1;
	msg1.header.id = CustomMsgTypes::accDccContact;
    msg1 << contactID;
    msg1 << verify; //accept or decline 
    Send(msg1);
}


#endif
