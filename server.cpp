#include <iostream>
#include <Winsock2.h>
#include <WS2tcpip.h>
#include <conio.h>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

SOCKET sockfd;
SOCKET acceptSock;
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

int ServerSock(){
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0){
		cout<<"WSA Start Error!"<<WSAGetLastError<<endl;
		closesocket(sockfd);
		WSACleanup();
		exit(1);
	}
	cout<<"WSA Loaded."<<endl;
	sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sockfd==INVALID_SOCKET){
		cout<<"Socket Start Error!"<<WSAGetLastError<<endl;
		closesocket(sockfd);
		WSACleanup();
		exit(1);
	}
	cout<<"Socket Created!"<<endl;
	sockaddr_in address;
    address.sin_addr.s_addr=inet_addr("127.0.0.1");
    address.sin_port=htons(9999);
    address.sin_family=AF_INET;
    if(bind(sockfd,(sockaddr*)&address,sizeof(address))==INVALID_SOCKET){
    	cout<<"Binding Error!"<<WSAGetLastError<<endl;
    	closesocket(sockfd);
    	WSACleanup();
		exit(1);
	}
	cout<<"Bind Successfull!"<<endl;
	if(listen(sockfd,1)!=0){
		cout<<"Listen Error!"<<WSAGetLastError<<endl;
		closesocket(sockfd);
    	WSACleanup();
		exit(1);
	}
	return listen(sockfd,1);
}

void Accept(){
	acceptSock=accept(sockfd,NULL,NULL);
	if(acceptSock==INVALID_SOCKET){
		cout<<"Accepting Error!"<<WSAGetLastError<<endl;
		closesocket(sockfd);
    	WSACleanup();
		exit(1);
	}
	cout<<"Connected."<<endl;
}

int input(int arr[],int val){
	cout<<"\nYour Turn - ";
		do{
			cin>>val;
			if(arr[val-1]=='X'||arr[val-1]=='O')
				cout<<"Invalid Input, Enter Again - ";
		}while(val>10&&val<0||arr[val-1]=='X'||arr[val-1]=='O');
		arr[val-1]=79;
}

bool winOrTie(int arr[]){
	system("cls");
	display(arr);
	if(check(arr,'X')){
		cout<<"\nPlayer 1 Wins!";
		closesocket(acceptSock);
		WSACleanup();
		return true;
	}
	else if(check(arr,'0')){
		cout<<"\nPlayer 2 Wins!";
		closesocket(sockfd);
		WSACleanup();
		return true;
	}
	else if(filled(arr)==9){
		cout<<"\nGame Tied!";
		closesocket(sockfd);
		WSACleanup();
		return true;
	}
	return false;
}

void playagain(char &playAgain){
	cout<<"\nPlay Again?(y/n) - "<<endl;
	do
		cin>>playAgain;
	while(playAgain!='y'||playAgain!='n');
	if(playAgain!='y'){
		closesocket(acceptSock);
		WSACleanup();
		exit(0);
	}
}
int main() {
	int listen=ServerSock();
	cout<<"listening..."<<endl;
	Accept();
	char playAgain;
//	do{
	int arr[9]={1,2,3,4,5,6,7,8,9},val;
	while(true){
		if(winOrTie(arr))
			break;
		cout<<"\nPlayer 1's Turn."<<endl;
		int byte=recv(acceptSock,(char*)&arr,36,0);
		if(byte<0){
			cout<<"Recieving Error : "<<WSAGetLastError();
			closesocket(acceptSock);
			WSACleanup();
			return 1;
		}
		if(winOrTie(arr))
			break;
		input(arr,val);
		byte=send(acceptSock,(char*)&arr,36,0);
		if(byte==SOCKET_ERROR){
			cout<<"Sending Error : "<<WSAGetLastError();
			closesocket(sockfd);
			WSACleanup();
			return 1;
		}
	}
//		playagain(playAgain);
//	}while(playAgain=='y');
    getch();
    return 0;
}
