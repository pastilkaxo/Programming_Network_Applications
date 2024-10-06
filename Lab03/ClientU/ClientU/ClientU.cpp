#include <iostream>
#include "WinSock2.h"
#include "tchar.h"
#include <string>
#pragma comment(lib,"WS2_32.lib")
#pragma warning(disable : 4996)
#include "windows.h"
#include <thread>
#include <chrono>  
#include <ctime>

using namespace std;

string GetErrorMsgText(int code) {
	string msgText;
	switch (code) {
	case WSAEINPROGRESS:msgText = "WSAEINPROGRESS"; break;
	case WSAENOTSOCK:msgText = "WSAENOCTSOCK"; break;
	case WSAEDESTADDRREQ:msgText = "WSAEDESTADDRREQ"; break;
	case WSAEMSGSIZE:msgText = "WSAEMSGSIZE"; break;
	case WSAEPROTOTYPE:msgText = "WSAEPROTOTYPE"; break;
	case WSAENOPROTOOPT:msgText = "WSAENOPROTOOPT"; break;
	case WSAEPROTONOSUPPORT:msgText = "WSAEPROTONOSUPPORT"; break;
	case WSAESOCKTNOSUPPORT:msgText = "WSAESOCKTNOSUPPORT"; break;
	case WSAEOPNOTSUPP:msgText = "WSAEOPNOTSUPP"; break;
	case WSAEPFNOSUPPORT:msgText = "WSAEPFNOSUPPORT"; break;
	case WSAEAFNOSUPPORT:msgText = "WSAEAFNOSUPPORT"; break;
	case WSAEADDRINUSE:msgText = "WSAEADDRINUSE"; break;
	case WSAEADDRNOTAVAIL:msgText = "WSAEADDRNOTAVAIL"; break;
	case WSAENETDOWN:msgText = "WSAENETDOWN"; break;
	case WSAENETUNREACH:msgText = "WSAENETUNREACH"; break;
	case WSAENETRESET:msgText = "WSAENETRESET"; break;
	case WSAECONNABORTED:msgText = "WSAECONNABORTED"; break;
	case WSAECONNRESET:msgText = "WSAECONNRESET"; break;
	case WSAENOBUFS:msgText = "WSAENOBUFS"; break;
	case WSAEISCONN:msgText = "WSAEISCONN"; break;
	case WSAENOTCONN:msgText = "WSAENOTCONN"; break;
	case WSAESHUTDOWN:msgText = "WSAESHUTDOWN"; break;
	case WSAETIMEDOUT:msgText = "WSAETIMEDOUT"; break;
	case WSAECONNREFUSED:msgText = "WSAECONNREFUSED"; break;
	case WSAEHOSTDOWN:msgText = "WSAEHOSTDOWN"; break;
	case WSAEHOSTUNREACH:msgText = "WSAEHOSTUNREACH"; break;
	case WSAEPROCLIM:msgText = "WSAEPROCLIM"; break;
	case WSASYSNOTREADY:msgText = "WSASYSNOTREADY"; break;
	case WSAVERNOTSUPPORTED:msgText = "WSAVERNOTSUPPORTED"; break;
	case WSANOTINITIALISED:msgText = "WSANOTINITIALISED"; break;
	case WSAEDISCON:msgText = "WSAEDISCON"; break;
	case WSATYPE_NOT_FOUND:msgText = "WSATYPE_NOT_FOUND"; break;
	case WSAHOST_NOT_FOUND:msgText = "WSAHOST_NOT_FOUND"; break;
	case WSATRY_AGAIN:msgText = "WSATRY_AGAIN"; break;
	case WSANO_RECOVERY:msgText = "WSANO_RECOVERY"; break;
	case WSANO_DATA:msgText = "WSANO_DATA"; break;
	case WSA_INVALID_HANDLE:msgText = "WSA_INVALID_HANDLE"; break;
	case WSA_INVALID_PARAMETER:msgText = "WSA_INVALID_PARAMETER"; break;
	case WSA_IO_INCOMPLETE:msgText = "WSA_IO_INCOMPLETE"; break;
	case WSA_IO_PENDING:msgText = "WSA_IO_PENDING"; break;
	case WSA_NOT_ENOUGH_MEMORY:msgText = "WSA_NOT_ENOUGH_MEMORY"; break;
	case WSA_OPERATION_ABORTED:msgText = "WSA_OPERATION_ABORTED"; break;
	case WSAEINVALIDPROCTABLE:msgText = "WSAEINVALIDPROCTABLE"; break;
	case WSAEINVALIDPROVIDER:msgText = "WSAEINVALIDPROVIDER"; break;
	case WSAEPROVIDERFAILEDINIT:msgText = "WSAEPROVIDERFAILEDINIT"; break;
	case WSASYSCALLFAILURE:msgText = "WSASYSCALLFAILURE"; break;	
	default:                msgText = "***ERROR***";      break;

	}
	return msgText;
}


string SetErrorMsgText(string msg, int code) {
	return msg + GetErrorMsgText(code);
}


int main(int args, _TCHAR* argv[])
{

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	WSAData wsaData;
	SOCKET cC;
	clock_t t1, t2;
	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
			throw SetErrorMsgText("startup: ", WSAGetLastError());
		}

		if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET) {
			throw SetErrorMsgText("socket: ", WSAGetLastError());
		}

		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);
		 //serv.sin_addr.s_addr = inet_addr("127.0.0.1");
		serv.sin_addr.s_addr = inet_addr("172.20.10.2");
		int lserv = sizeof(serv);

		char obuf[50];
		char ibuf[50];
		int libuf = 0;
		int lobuf = 0;
		int msgCount;
		char number[1024];

		cout << "Введите колличетсво отправляемых сообщений:\n";
		cin >> msgCount;

		t1 = clock();
		for (int i = 1; i <= msgCount; i++) {

			itoa(i, number, 10);
			strcpy(obuf, "Hello from Vlad ");
			strcat(obuf, number);


			if (i == 8) {
				Sleep(2000);
				continue;
			}

			if ((lobuf = sendto(cC, obuf, strlen(obuf) + 1, NULL, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
				throw SetErrorMsgText("sendto:", WSAGetLastError());



			if ((libuf = recvfrom(cC, ibuf, sizeof(ibuf), NULL, (sockaddr*)&serv, &lserv)) == SOCKET_ERROR)		
				throw SetErrorMsgText("recvfrom:", WSAGetLastError());

			ibuf[libuf] = '\0';
			cout << "[Server Message]: " << " " << ibuf << endl;


	/*		int receivedNumber = i;
			if (sscanf(ibuf, "Hello from Vlad %d", &receivedNumber) == 1) {
				receivedNumber++;
				string msg = "Hello from Vlad " + to_string(receivedNumber);
				strcpy(obuf, msg.c_str());
				if ((lobuf = sendto(cC, obuf, strlen(obuf) + 1, NULL, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
					throw SetErrorMsgText("sendto:", WSAGetLastError());
			}*/
		}
		t2 = clock();

		strcpy(obuf, "");
		if ((lobuf = sendto(cC, obuf, strlen(obuf) + 1, NULL, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
			throw SetErrorMsgText("sendto:", WSAGetLastError());


		cout << "[Затраченое время]: " << (t2 - t1)  << " мсек.\n";

		if (closesocket(cC) == SOCKET_ERROR) {
			throw SetErrorMsgText("closesocket: ", WSAGetLastError());
		}


		if (WSACleanup() == SOCKET_ERROR) {
			throw SetErrorMsgText("cleanup: ", WSAGetLastError());
		}
	}
	catch (string errMsg)
	{
		cout << errMsg << endl;
	}
}

