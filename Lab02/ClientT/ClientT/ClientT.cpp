#include <iostream>
#include "WinSock2.h"
#include "tchar.h"
#include <string>
#pragma comment(lib,"WS2_32.lib")
#pragma warning(disable : 4996)

using namespace std;

string GetErrorMsgText(int code) {
	string msg;
	switch (code) {
	case WSAEINTR:          msg = "WSAEINTR - работа функции прервана";         break;
	case WSAEACCES:         msg = "WSAEACCES - доступ ограничен";        break;
	case WSASYSCALLFAILURE: msg = "WSASYSCALLFAILURE - ошибка системного вызова "; break;
	case WSAECONNREFUSED: msg = "WSAECONNREFUSED - в подключении отказано"; break;
	default:                msg = "***ERROR***";      break;

	}
	return msg;
}


string SetErrorMsgText(string msg, int code) {
	return msg + GetErrorMsgText(code);
}


int main(int args, _TCHAR* argv[])
{

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	try
	{
		WSAData wsaData;
		SOCKET cC;
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
			throw SetErrorMsgText("startup: ", WSAGetLastError());
		}

		if ((cC = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET) {
			throw SetErrorMsgText("socket: ", WSAGetLastError());
		}

		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);
		serv.sin_addr.s_addr = inet_addr("127.0.0.1");

		if (connect(cC,(sockaddr*)&serv,sizeof(serv))== SOCKET_ERROR) {  // 10061 
			throw SetErrorMsgText("connect: ", WSAGetLastError());
		}

		char ibuf[1024];
		char substr[1024];
		int libuf = 0;
		string obuf;
		int lobuf = 0;
		int msgCount;

		cout << "Введите колличетсво отправляемых сообщений:\n";
		cin >> msgCount;

		for (int i = 1; i <= msgCount; i++) {

			obuf = "Hello from Vlad " + to_string(i) + "\n";

			if ((lobuf = send(cC, obuf.c_str(), strlen(obuf.c_str()), NULL)) == SOCKET_ERROR) {
				throw SetErrorMsgText("send: ", WSAGetLastError());
			}


			if ((libuf = recv(cC, ibuf, sizeof(ibuf) - 1, 0)) == SOCKET_ERROR) {
				throw SetErrorMsgText("recv: ", WSAGetLastError());
			}


			ibuf[libuf] = '\0';
			cout << ibuf;

			int receivedNumber = i;

			if (sscanf(ibuf, "Hello from Vlad %d", &receivedNumber) == 1) {
				receivedNumber++;
				obuf = "E:Hello from Vlad " + to_string(receivedNumber) + "\n";

				if ((lobuf = send(cC, obuf.c_str(), strlen(obuf.c_str()), 0)) == SOCKET_ERROR) {
					throw SetErrorMsgText("send: ", WSAGetLastError());
				}
			}
	

		}


		
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

