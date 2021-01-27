#ifndef _MAIL_CLIENT_H_
#define _MAIL_CLIENT_H_

#include "infoSerCli.h"

//using namespace std;

class MailClient : public olc::net::client_interface<CustomMsgTypes>
{
private:
    std::string userId;

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
    void sendMail(mailType mT, char topicMail[]);
    void forwardMail(std::string mail);
    void replyMail();
    void deleteMail(std::string mail);
    void createDraft();
    void createContact(uint32_t sendID);
    void accDccContact(bool varifie, uint32_t contatorID);
};

MailClient::MailClient()
{
    mkdir("Inbox",0777);
    mkdir("Flag_Mails",0777);
    mkdir("Draft",0777);
    mkdir("Send_Mails",0777);
}

MailClient::~MailClient()
{
}

void MailClient::sendMail(mailType mT, char topicMail[51])
{
    std::string mail;
    switch (mT)
    {
    case mailType::send:
    {
        std::stringstream tooString;
        tooString << topicMail;
        std::string fileName;
        tooString >> fileName;

        std::ifstream mailFile ("Send_Mails/" + fileName + ".txt");
        getline (mailFile, mail);
        mailFile.close();
    }
    break;
    
    case mailType::forward:
    {
        std::stringstream tooString;
        tooString << topicMail;
        std::string fileName;
        tooString >> fileName;

        std::ifstream mailFile ("Inbox/" + fileName);
        getline (mailFile, mail);
        mailFile.close();
        std::cout << "hello" << std::endl;
    }
    default:
    std::cout << "error" << std::endl;
    break;
    }
    
    
    olc::net::message<CustomMsgTypes> msg;

    msg.header.id = CustomMsgTypes::sendMail;
    
    char mailChar[mailSize] = {0};

    std::strcpy(mailChar, mail.c_str());
    for (int i = 0; i < mailSize; i++)
    {
        msg << mailChar[i];
    }

    for (int i = 0; i < 51; i++)
    {
        msg << topicMail[i];
    }
    
    uint32_t mailToUserID;
    std::cout << "Witch user do you want to send to" << std::endl;
    std::cin >>mailToUserID;

    msg << mailToUserID;
    
    Send(msg);

}

void MailClient::updateInbox()
{
    olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::updateInbox;

    Send(msg);
}

void MailClient::displayInbox()
{
    std::fstream contant;
	contant.open("Inbox/content.txt", std::fstream::in);
    std::string inbox[50];
    int i = 0;
    while (std::getline(contant, inbox[i]))
    {   
        std::cout << i << ": " << inbox[i] << std::endl;
        i++;
    }

    int numMail;

    std::cout << "Chose a mail with a with the index number" << std::endl;
    std::cin >> numMail;

    std::string mailText;
    std::fstream mailToDisplay;
	mailToDisplay.open("Inbox/" + inbox[numMail], std::ios::in);
    
    while (std::getline(mailToDisplay, mailText))
    {
        std::cout << mailText << std::endl;
    }

    std::cout << "1. Back" << std::endl;
    std::cout << "2. Delet mail" << std::endl;
    std::cout << "3. Forward mail" << std::endl;

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

            default:
                std::cout << "Wrong input try agin" << std::endl;
                std::cin >> choser1;

        }
    }
    

    std::cin.get();
        
    contant.close();

}

void MailClient::forwardMail(std::string mail)
{
    char topic[51] = {0};
    std::strcpy(topic, mail.c_str());

    sendMail(mailType::forward, topic);

}

void MailClient::replyMail()
{

}

void MailClient::deleteMail(std::string mail)
{
   std::string deleter =  "Inbox/" + mail;  
    std::remove(deleter.c_str());

    std::fstream contant;
	contant.open("Inbox/content.txt");
    std::string inbox;
    std::fstream temp;
	temp.open("Inbox/temp.txt", std::fstream::out);
    while (std::getline(contant, inbox))
    {   
        inbox.replace(inbox.find(mail.c_str()),mail.length(),"");
        temp << inbox << std::endl;
    } 
    contant.close();
    temp.close();
    std::remove("Inbox/content.txt");
    std::rename("Inbox/temp.txt", "Inbox/content.txt");



}

void MailClient::createDraft()
{
    int navNum = 0;
    bool quit = false;
    bool save = false;
    bool send = false;
    
    std::string draftText;
    char topic[51] = {0};
    std::cin.get();
    std::cout << "write the topic of the Mail in one word with max 50 charicktors" << std::endl;
    std::cin >> topic;
    std::cin.get();

    std::stringstream tooString;
    tooString << topic;
   	std::string fileName;
	tooString >> fileName;

    std::cout << "write the mail"<< std::endl;;
    std::getline(std::cin, draftText);

    std::cout << std::endl << "1. Save Draft" << std::endl;
    std::cout << "2. Send Mail" << std::endl;
    std::cout << "3. Delete" << std::endl;

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
            break;
        }
    }
    
    if (save == true)
    {
        std::ofstream draftFil("Draft/" + fileName + ".txt");
        draftFil << draftText;
        draftFil.close();
    } else if (send == true)
    {
        std::ofstream draftFil("Send_Mails/" + fileName + ".txt");
        draftFil << draftText;
        draftFil.close();
        sendMail(mailType::send, topic);
        
    }

    return;

}

void MailClient::createContact(uint32_t contactID)
{
    olc::net::message<CustomMsgTypes> msg;

    msg.header.id = CustomMsgTypes::createContact;

    msg << contactID;

    Send(msg);

}

void MailClient::accDccContact(bool varifie, uint32_t contatorID)
{

    olc::net::message<CustomMsgTypes> msg1;
	msg1.header.id = CustomMsgTypes::accDccContact;
    msg1 << contatorID;
    msg1 << varifie;
    Send(msg1);
}


#endif
