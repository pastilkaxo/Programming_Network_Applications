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


string SetErrorMsgText(string text, int code) {
    return text + GetErrorMsgText(code);
}


int main(int args, _TCHAR* argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    SOCKET sS;
    WSAData wsaData;

    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
            throw SetErrorMsgText("Startup: ", WSAGetLastError());
        }
        if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET) {
            throw SetErrorMsgText("socket:", WSAGetLastError());
        }

        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(2000);
        serv.sin_addr.s_addr = INADDR_ANY;


        if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR) {
            throw SetErrorMsgText("bind: ", WSAGetLastError());
        }

        // --->
        cout << "---------------Waiting----------------\n";
            while (true) {
                SOCKADDR_IN client;
                memset(&client, 0, sizeof(client));
                int lclnt = sizeof(client);
                char ibuf[50];
                int libuf = 0, lobuf = 0;


                while (true) {
                    if (libuf = recvfrom(sS, ibuf, sizeof(ibuf), NULL, (sockaddr*)&client, &lclnt) == SOCKET_ERROR) {
                        throw SetErrorMsgText("recv:", WSAGetLastError());
                    }
                    else if (strcmp(ibuf, "") == 0) {
                        cout << "[Client Disconected] : Empty String\n";
                        break;
                    }
                    else {
                        cout << "[Client Message]: " << inet_ntoa(client.sin_addr) << ":" << htons(client.sin_port) << "  " << ibuf << endl;

                    }


                    if ((lobuf = sendto(sS, ibuf, strlen(ibuf), NULL, (sockaddr*)&client, sizeof(client))) == SOCKET_ERROR) {
                        throw SetErrorMsgText("send: ", WSAGetLastError());
                    }


                    //if (libuf = recvfrom(sS, ibuf, sizeof(ibuf), NULL, (sockaddr*)&client, &lclnt) == SOCKET_ERROR) {
                    //    throw SetErrorMsgText("recv:", WSAGetLastError());
                    //}
                    //else {
                    //    cout << "[Client Message Edited]: " << inet_ntoa(client.sin_addr) << ":" << htons(client.sin_port) << "  " << ibuf << endl;

                    //}
                }
              
                cout << "[Client Disconected]\n";


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
