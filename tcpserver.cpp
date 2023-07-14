#include<iostream>
#include<WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")
// compiler directive to instruct few things to compiler
// here it is used to instruct the linker linker to include the "ws2_32.lib" library during the linking phase of the compliation

// this library contains the implementation of Winsock API(windows sockets) which is used to network programming on windows platform
// i.e it contains the functions and definitions necessary for creating and managing network sockets, performing network i/o operations and handling network protocols

// including it  makes it easier to complier and build network related applications without worrying about manually managing the library dependencies


using namespace std;

void main(){

	// initialize winsock 
	WSADATA wsData;

	WORD ver= MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsData);

	if(wsOK != 0){

		cerr << "can't initialize winsock! quitting"<<endl;
		return;

	}


	//create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);



	if(listening == INVALID_SOCKET){

		cerr << "can't create a socket ! quitting" <<endl;
		return;
	}
	// so  above we created a socket
	// albert once said, if you cant explain it easily , you havent understood it yet
	// so socket is just a number, it is this nebulous idea of "this is the reference number i am gonna use to access this port on this id address"





	// bind the socket to an ip address and port
	// so let us bind this socket with the ip address and port or port and ip address to socket
	sockaddr_in hint;

	hint.sin_family = AF_INET;

	hint.sin_port = htons(54000);

	hint.sin_addr.s_un.s_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint)); 

	// so we are taking the address of hint and type casting it as sockaddr datatype pointer
	// so now we have our socket bind to our networking code , so we can use that socket to reference the networking code so we can send things and receive things all inside that
	//socket 


	// tell winsock that this socket is for listening

	listen(listening, SOMAXCONN);

	// it doesnt accept any connections, it just marks it as for listening




	// wait for a connection
	sockaddr_in client;

	int clientsize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientsize);

	// this is the part where we are listening for connection and connnection happens and it happens on one particular port on the server and 54000 is our listening port.
	// the socket that actually returns is going to be this socket i.e some other random number

	char host[NI_MAXHOST];			// client's remote name
	char service[NI_MAXSERV];		// service ( i.e port) the client is connected on

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);   // same as memset(host, 0, NI_MAXSERV);

	// display the hostname or ip address of the client

	if(getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0){
		cout << host << "connected on port " <<service<<endl;		
	}
	else{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << "connected on port " << ntohs(client.sin_port) <<endl;
	}



	// close listening socket 

	closesocket(listening);



	// while loop: accept and echo message back to client
	char buf[4096];

	while(true){
		ZeroMemory(buf, 4096);

		// wait for client to send data

		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		
		if(bytesReceived == SOCKET_ERRO){

			cerr << "error in recv(). Quitting" <<endl;
			break;
		}

		if(bytesReceived == 0){

			cout << "Client disconnected "<<endl;
			break;

		}

		// echo message back to client
		send(clientSocket, buf, bytesReceived+1 , 0);
	}

	//close the socket
	closesocket(clientSocket);

	// cleanup winsock 
	WSACleanup();
}