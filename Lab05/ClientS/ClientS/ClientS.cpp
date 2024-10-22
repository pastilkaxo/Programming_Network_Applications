#include <iostream>
#include "Winsock2.h"                // заголовок  WS2_32.dll
#include <string>
#include <stdexcept>
#include <sstream>
#pragma comment(lib, "WS2_32.lib    ")   // экспорт  WS2_32.dll

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

using namespace std;

string GetErrorMessageText(int ErrorCode);
string SetErrorMessageText(string ErrorMessage, int ErrorCode);

string GetErrorMessageText(int ErrorCode)
{
    string msgText;
    switch (ErrorCode)
    {
    case WSAEINTR:              msgText = "WSAEINTR";                   break;
    case WSAEACCES:             msgText = "WSAEACCES";                  break;
    case WSAEFAULT:             msgText = "WSAEFAULT";                  break;
    case WSAEINVAL:             msgText = "WSAEINVAL";                  break;
    case WSAEMFILE:             msgText = "WSAEMFILE";                  break;
    case WSAEWOULDBLOCK:        msgText = "WSAEWOULDBLOCK";             break;
    case WSAEINPROGRESS:        msgText = "WSAEINPROGRESS";             break;
    case WSAEALREADY:           msgText = "WSAEALREADY";                break;
    case WSAENOTSOCK:           msgText = "WSAENOTSOCK";                break;
    case WSAEDESTADDRREQ:       msgText = "WSAEDESTADDRREQ";            break;
    case WSAEMSGSIZE:           msgText = "WSAEMSGSIZE";                break;
    case WSAEPROTOTYPE:         msgText = "WSAEPROTOTYPE";              break;
    case WSAENOPROTOOPT:        msgText = "WSAENOPROTOOPT";             break;
    case WSAEPROTONOSUPPORT:    msgText = "WSAEPROTONOSUPPORT";         break;
    case WSAESOCKTNOSUPPORT:    msgText = "WSAESOCKTNOSUPPORT";         break;
    case WSAEPFNOSUPPORT:       msgText = "WSAEPFNOSUPPORT";            break;
    case WSAEAFNOSUPPORT:       msgText = "WSAEAFNOSUPPORT";            break;
    case WSAEADDRINUSE:         msgText = "WSAEADDRINUSE";              break;
    case WSAEADDRNOTAVAIL:      msgText = "WSAEADDRNOTAVAIL";           break;
    case WSAENETDOWN:           msgText = "WSAENETDOWN";                break;
    case WSAENETUNREACH:        msgText = "WSAENETUNREACH";             break;
    case WSAENETRESET:          msgText = "WSAENETRESET";               break;
    case WSAECONNABORTED:       msgText = "WSAECONNABORTED";            break;
    case WSAECONNRESET:         msgText = "WSAECONNRESET";              break;
    case WSAENOBUFS:            msgText = "WSAENOBUFS";                 break;
    case WSAEISCONN:            msgText = "WSAEISCONN";                 break;
    case WSAENOTCONN:           msgText = "WSAENOTCONN";                break;
    case WSAESHUTDOWN:          msgText = "WSAESHUTDOWN";               break;
    case WSAETIMEDOUT:          msgText = "WSAETIMEDOUT";               break;
    case WSAECONNREFUSED:       msgText = "WSAECONNREFUSED";            break;
    case WSAEHOSTDOWN:          msgText = "WSAEHOSTDOWN";               break;
    case WSAEHOSTUNREACH:       msgText = "WSAEHOSTUNREACH";            break;
    case WSAEPROCLIM:           msgText = "WSAEPROCLIM";                break;
    case WSASYSNOTREADY:        msgText = "WSASYSNOTREADY";             break;
    case WSAVERNOTSUPPORTED:    msgText = "WSAVERNOTSUPPORTED";         break;
    case WSANOTINITIALISED:     msgText = "WSANOTINITIALISED";          break;
    case WSAEDISCON:            msgText = "WSAEDISCON";                 break;
    case WSATYPE_NOT_FOUND:     msgText = "WSATYPE_NOT_FOUND";          break;
    case WSAHOST_NOT_FOUND:     msgText = "WSAHOST_NOT_FOUND";          break;
    case WSATRY_AGAIN:          msgText = "WSATRY_AGAIN";               break;
    case WSANO_RECOVERY:        msgText = "WSANO_RECOVERY";             break;
    case WSANO_DATA:            msgText = "WSANO_DATA";                 break;
    case WSA_INVALID_HANDLE:    msgText = "WSA_INVALID_HANDLE";         break;
    case WSA_INVALID_PARAMETER: msgText = "WSA_INVALID_PARAMETER";      break;
    case WSA_IO_INCOMPLETE:     msgText = "WSA_IO_INCOMPLETE";          break;
    case WSA_IO_PENDING:        msgText = "WSA_IO_PENDING";             break;
    case WSA_NOT_ENOUGH_MEMORY: msgText = "WSA_NOT_ENOUGH_MEMORY";      break;
    case WSA_OPERATION_ABORTED: msgText = "WSA_OPERATION_ABORTED";      break;
    default:msgText = "Неопознанная ошибка";
    }
    return msgText;
}

string SetErrorMessageText(string ErrorMessage, int ErrorCode)
{
    return ErrorMessage + GetErrorMessageText(ErrorCode);
}


bool GetServerByName(const char* name,char* call,struct sockaddr* from,int*flen)  // пересылка позывного серверу адрес которого задан ввиде символического имени
{
    SOCKET sS;
    try {
            if ((sS = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
                throw SetErrorMessageText("socket: ", WSAGetLastError());
            hostent* host = gethostbyname(name);
            if(!host)
                throw SetErrorMessageText("gethostbyname: ", WSAGetLastError());
            cout << "[HOSTNAME]: " << host->h_name << endl;
            cout << "[HOST IP]: " << inet_ntoa(*(in_addr*)host->h_addr) << endl;

            SOCKADDR_IN serv;
            serv.sin_family = AF_INET;
            serv.sin_port = htons(2000);
            serv.sin_addr.s_addr = inet_addr(inet_ntoa(*(in_addr*)host->h_addr));
           // serv.sin_addr.s_addr = INADDR_LOOPBACK;

            char ibuf[50];
            int len = 0;

            if (sendto(sS, call, strlen(call) + 1, 0, (sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR)
                throw SetErrorMessageText("sendto: ", WSAGetLastError());

            int libuf = recvfrom(sS, ibuf, sizeof(ibuf), 0, from, flen);
            if (libuf == SOCKET_ERROR) {
                closesocket(sS);
                throw SetErrorMessageText("recvfrom: ", WSAGetLastError());
            }

            ibuf[libuf] = '\0';

            cout << "[MESSAGE]: " << ibuf << endl;


            if (strcmp(ibuf, name) == 0) {

                closesocket(sS);
                return true;
            }


            closesocket(sS);

            system("pause");

            return false;


    }
    catch (string errorMsgText) {
        cout << errorMsgText << endl;
        if (sS != INVALID_SOCKET) closesocket(sS);
        return false;
    }

}


int main() {
    setlocale(LC_CTYPE, "ru");

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        cout << "WSAStartup: ERROR " << WSAGetLastError() << endl;
        return 1;
    }

    char serverName[] = "Hello";
    short port = 2000;
    SOCKADDR_IN from;
    int fromLen = sizeof(from);

    GetServerByName("DESKTOP-2SJTH0H",serverName,(sockaddr*)&from,&fromLen);

    if (WSACleanup() == SOCKET_ERROR) {
        cout << "WSACleanup: ERROR " << WSAGetLastError() << endl;
        return 1;
    }

    return 0;
}