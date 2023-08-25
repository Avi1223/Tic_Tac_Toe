#include<iostream>
#include<winsock2.h>
#include<conio.h>
#include <string>
#include<ws2tcpip.h>
using namespace std;

SOCKET clientSockfd;
int connectSock;

bool check(int a[],char element){
	if(a[0]==element&&(a[0]==a[4]&&a[0]==a[8])||(a[2]==a[4]&&a[2]==a[6]))
		return true;
	if(a[0]==element&&(a[0]==a[3]&&a[0]==a[6])||(a[1]==a[4]&&a[1]==a[7])||(a[2]==a[5]&&a[2]==a[8]))
		return true;
	for(int i=0;i<9;i=i+3){
		if(a[i]==element&&a[i]==a[i+1]&&a[i]==a[i+2])
			return true;
	}
	return false;
}

int filled(int a[]){
	int count=0;
	for(int i=0;i<9;i++){
		if(a[i]=='X'||a[i]=='O'){
			count++;
		}
	}
	if(count==9)
		return count;
	return count;
}

void display(int a[]){
	for(int i=0;i<9;i++){
		if(i%3==0&&i!=0){
			cout<<endl;
			cout<<"-----------\n";
		}
		if(a[i]==79||a[i]==88){
			cout<<" "<<static_cast<char>(a[i])<<" ";
		}	
		else
			cout<<" "<<a[i]<<" ";
		if(i==2||i==5||i==8)
			continue;
		cout<<"|";
	}
	cout<<endl;
}

int ClientSock(){
	//loading dll file
	WSADATA wsadata;
	int wsaErr=WSAStartup(MAKEWORD(2,2),&wsadata);
	if(wsaErr!=0){
		cout<<"Error Setting Up WinSock";
		return 1;
	}
	cout<<"Connection Established!"<<" Connection Status : "<<wsadata.szSystemStatus<<endl;
	//getaddrinfo
	addrinfo* result=nullptr;
	int status=getaddrinfo("address link here","port no here",nullptr,&result);   //put your host address and port here
	if (status != 0) {
        cerr<<"getaddrinfo error: "<<gai_strerror(status)<<endl;
        WSACleanup();
        return 1;    
	}
	//setting up socket
	clientSockfd=socket(result->ai_family,result->ai_socktype,result->ai_protocol);
	if(clientSockfd==INVALID_SOCKET){
		cout<<"Error Setting Up File Descriptor, "<<WSAGetLastError();
		return 1;
	}
	cout<<"File Descriptor Set Up."<<endl;
	//connecting
	connectSock=connect(clientSockfd,result->ai_addr,result->ai_addrlen);
	if(connectSock!=0){
		cout<<"Error Connecting, "<<WSAGetLastError();
		WSACleanup();
		freeaddrinfo(result);
		return 1;
	}
	cout<<"Connected."<<endl;
	return 0;
}

void input(int arr[],int val){
	cout<<"\nYour Turn - ";
	do{
		cin>>val;
		if(arr[val-1]=='X'||arr[val-1]=='O')
			cout<<"Invalid Input, Enter Again - ";
	}
	while(val>10&&val<0||arr[val-1]=='X'||arr[val-1]=='O');
	arr[val-1]=88;
}

bool winOrTie(int arr[]){
	system("cls");
	display(arr);
	if(check(arr,'X')){
		cout<<"\nPlayer 1 Wins!";
		closesocket(clientSockfd);
		WSACleanup();
		return true;
	}
	else if(check(arr,'0')){
		cout<<"\nPlayer 2 Wins!";
		closesocket(clientSockfd);
		WSACleanup();
		return true;
	}
	else if(filled(arr)==9){
		cout<<"\nGame Tied!";
		closesocket(clientSockfd);
		WSACleanup();
		return true;
	}
	return false;
}

int main(void){
	ClientSock();
	char playAgain;
	int arr[9]={1,2,3,4,5,6,7,8,9},val;
	while(true){
		if(winOrTie(arr)==1)
			break;
		input(arr,val);			
		int byte=send(clientSockfd,(char*)&arr,36,0);
		if(byte==SOCKET_ERROR){
			cout<<"Sending Error : "<<WSAGetLastError();
			return 1;
		}
		if(winOrTie(arr)==1)
			break;
		cout<<"\nPlayer 2's Turn."<<endl;
		byte=recv(clientSockfd,(char*)&arr,36,0);
		if(byte<0){
			cout<<"Recieving Error : "<<WSAGetLastError();
			return 1;
		}
	}
	getch();
	return 0;
}