Software Engineering 
====================

Contents of this file
---------------------
- Introduction
- Prerequisite
- Setup and Usage
- Authors and contributions

Introduction
-------------
This file contains information regarding the specifications on how to run the email server/client system. Including any installations or updates required to run the system
as it is intended to run.

Prerequisite
-------------
We assume these requisits are already installed:
- A Linux OS
If this is not the case we suggest getting Ubuntu 20.04. This can be done at:
https://ubuntu.com/download/desktop

Setup and Simulation
---------------------
In order to run the system om your computer, first make sure that you are booted on the Ubuntu OS, or you are running a virtual version similar thereof.  
Then open the github repository found at:  
https://github.com/Basaron/Mail-Server-client  
Download the entire repository and extract the content to your wanted directory. From here open a terminal from the Server1 folder and run the executable with the command:  
$ ./Server  
Once the server is running, exit the Server1, and open the Client1 folder. Open a terminal from this directory and run the executable with the following command:  
$ ./Client  
Repeat this step in the two other Client folders Client2 and Client3.  
Note that only 2 clients are needed for the system to work.  

Now everything should be up and running and the system can be navigated with both clients. To exit any running Clients or Servers highlight the corresponding terminal and press ctrl + c.  

If the above does not run the server and client executable, you must run a few installations. The required installations to run the system:  

 - g++  
 - ASIO Standalone  
 - pThreads  
 
 For help installing these please take a look at the references specified at the Installation Guide in the Final Report document.  

Once these installations have been made you will have to compile the system before running the executable files. To compile use the following commands:  
Clients:  
$ c++ -o Client MailClient.cpp -std=gnu++2a -pthread ./Client  

Server:  
$ c++ -o Server MailServer.cpp -std=gnu++2a -pthread ./Server  

Now you should be able to run the executable files with the commands:  
Server:  
$ ./Server  
Clients:  
$ ./Client

If any trouble occurs running the system or installing any dependencies please contact us at:  
au644652@post.au.dk  

Authors and contributions
--------------------------
Authors and contributors to this project are:

Name: Emil Chao Hu  
Email: au649481@post.au.dk  
StudyID: 201909692  

Name: Bastian Aaron Kramer  
Email: au644652@post.au.dk  
StudyID: 201908709  

Name: Magnus Trøjgård Tang  
Email: au638475@post.au.dk  
StudyID: 201904915 
