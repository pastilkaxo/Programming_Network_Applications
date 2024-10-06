
#include <iostream>
#include "WinSock2.h" // ws2_32.dll
#pragma comment(lib,"WS2_32.lib") // export
#include "tchar.h"
#pragma warning(disable : 4996)
#include "windows.h"
#include <thread>
#include <chrono>  
#include <ctime>
using namespace std;

string GetErrorMsgText(int code) {
    string msg;
    switch (code) {
    case WSAEINTR:          msg = "WSAEINTR - работа функции прервана";         break;
    case WSAEACCES:         msg = "WSAEACCES - доступ ограничен";        break;
    case WSASYSCALLFAILURE: msg = "WSASYSCALLFAILURE - ошибка системного вызова "; break;
    default:                msg = "***ERROR***";      break;

    }
    return msg;
}


string SetErrorMsgText(string text,int code) {
    return text + GetErrorMsgText(code);
}


int main(int args, _TCHAR* argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    try
    {
        clock_t t1, t2;
        SOCKET sS;
        WSAData wsaData;
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
            throw SetErrorMsgText("Startup: ", WSAGetLastError());
        }
        if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET) {
            throw SetErrorMsgText("socket:", WSAGetLastError());
        }

        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(2000);
        serv.sin_addr.s_addr = INADDR_ANY;


        if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR) {
            throw SetErrorMsgText("bind: " , WSAGetLastError());
        }



        while (true) {

            if (listen(sS, SOMAXCONN) == SOCKET_ERROR) {
                cout << WSAGetLastError();
                throw SetErrorMsgText("listen: ", WSAGetLastError());
                continue;
            }

            SOCKET cS;
            SOCKADDR_IN client;
            memset(&client, 0, sizeof(client));
            int lclnt = sizeof(client);
            char ibuf[1024];
            int libuf = 0, lobuf = 0;
            bool recieved = false;


            if ((cS = accept(sS, (sockaddr*)&client, &lclnt)) == INVALID_SOCKET) {
                throw  SetErrorMsgText("accept:", WSAGetLastError());
            }
            else {
                system("cls");
                cout << "============= Client connected ==========\n";
                cout << "IP: " << inet_ntoa(client.sin_addr) << endl
                    << "Port: " << htons(client.sin_port) << endl;
                t1 = clock();
                for (int i = 0; i < 1000; i++) {
                    if ((libuf = recv(cS, ibuf, sizeof(ibuf) - 1, 0)) == SOCKET_ERROR) {
                        cout << WSAGetLastError();
                        throw  SetErrorMsgText("recieve:", WSAGetLastError());
                    }
                    else if (libuf == 0) {
                        cout << "============= Client disconnected ==========\n";
                        break;
                    }
                    else {
                        ibuf[libuf] = '\0';
                        cout << ibuf;
                    }


                    if ((lobuf = send(cS, ibuf, strlen(ibuf), NULL)) == SOCKET_ERROR) {
                        throw SetErrorMsgText("send: ", WSAGetLastError());
                    }


                    if ((libuf = recv(cS, ibuf, sizeof(ibuf) - 1, 0)) == SOCKET_ERROR) {
                        cout << WSAGetLastError();
                        throw  SetErrorMsgText("recieve:", WSAGetLastError());
                    }
                    else {
                        ibuf[libuf] = '\0';
                        cout << ibuf;
                    }

                }
                t2 = clock();
  

                cout << "Затраченное время:" << (t2 - t1) / CLOCKS_PER_SEC; // +- 3700 секунд
            }
            cin.get();
            cout << "Waiting.....\n";
        }

        if (closesocket(sS) == SOCKET_ERROR) {
            throw SetErrorMsgText("closesocket: ", WSAGetLastError());
        }


        if (WSACleanup() == SOCKET_ERROR) {
            throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
        }
       
    }
    catch (string errMsg)
    {
        cout << "\n WSAGetLastError: " << errMsg;
    }



}
