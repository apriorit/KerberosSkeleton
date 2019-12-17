This project was implemented for an Apriorit site article to demonstrate how to implement Kerberos authentication via using the LSA Service API.  
There are unit-tests that allow you to study using of System API in debugging.  
There were not implemented the following functionality (instead it there is used mock functionality):  
- Client-server communication (sending messages to client/server, receiving messages from client/server).
- Parsing of the messages from client/server.
- Packing of the message into a request/response.  

With this project you can retrieve logon session by the Kerberos tickets. To do this, you need to do the following:
- Open KerberosSkeleton.sln file in VS 2015.  
- Build two projects ServerApp and ClientApp. If everything is successful, you will have two exe files (ServerApp.exe and ClientApp.exe).  
- Copy the ServerApp.exe to a Windows Server (e.g. Windows Server 2016).  
- Open the Command Line console and run the ServerApp.exe with one parameter -spn. It can look like this - ServerApp.exe -spn.  
- Copy the output string you will receive after the ServerApp.exe complete work. This the output string has to have following format "`host/<dnsHostName>.<dnsDomainName>@<dnsDomainName>".` 
- Copy the ClientApp.exe to a Windows Client (for example, Windows 10) that is in the same domain as the Windows Server.  
- Open Command Line console and run ClientApp.exe with two parameters:  
	- The first parameter is -spn.  
	- The second parameter is a string retrieved from the ServerApp.exe. It can look like this - "ClientApp.exe -spn `host/<dnsHostName>.<dnsDomainName>@<dnsDomainName>".`  
- If everything is successful, there are appeared two files next to the ClientApp.exe:  
	- krbtgtTicket.bin - it is the TGT ticket.  
	- serviceTicket.bin - it is the TGS ticket.  
- Copy the gotten tickets (krbtgtTicket .bin and serviceTicket .bin) to the Windows Server and place them next to the ServerApp.exe.  
- Open the Command Line console and run the ServerApp.exe with one -auth parameter. It can look like this - "ServerApp.exe -auth".  
- If the tickets are valid, you will receive the message "Success completed task of the authentication."  
- If the tickets are not valid, you will receive a message that begins with the text "Failed task of the authentication."

The main goal of the KerberosSkeleton project is:  
- Code demonstration for the Apriorit's site article.
- Ability to learn the implementation of Kerberos Authentication via debug.
- Provide you the kerberos skeleton project for your future Kerberos Authentication projects.  
The KerberosSkeleton project is licensed under the MIT license and can be freely used in your commercial or open source projects.  