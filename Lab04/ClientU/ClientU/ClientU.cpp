#include <iostream>
#include "Winsock2.h"                
#include <string>
#include <stdexcept>
#include <sstream>
#pragma comment(lib, "WS2_32.lib")  

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

using namespace std;



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

// 1- позывной сервера 2- порт 3- указатель структуры параметровк сокета сервера
bool GetServer(char* name, short port, struct sockaddr* from, int* flen) { // отправка широковещательного запроса в локальную сеть всем узлам с позывным сервера
    SOCKET cC;

    try {
        if ((cC = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
            throw SetErrorMessageText("socket: ", WSAGetLastError());

        BOOL optval = TRUE;
        // установка режимов использования сокета / адресовать сообщение о поиске сервера всем компьютерам сети.
        if (setsockopt(cC, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval)) == SOCKET_ERROR) 
            throw SetErrorMessageText("setsockopt: ", WSAGetLastError());

        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(port);
        serv.sin_addr.s_addr = INADDR_BROADCAST;

        if (sendto(cC, name, strlen(name) + 1, 0, (sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR)
            throw SetErrorMessageText("sendto: ", WSAGetLastError());

        char ibuf[50];
        int libuf = recvfrom(cC, ibuf, sizeof(ibuf), 0, from, flen);
        if (libuf == SOCKET_ERROR) {
            closesocket(cC);
            throw SetErrorMessageText("recvfrom: ", WSAGetLastError());
        }

        if (strcmp(ibuf, name) == 0) {
            closesocket(cC);
            return true;
        }

        closesocket(cC);
        return false;
    }
    catch (string errorMsgText) {
        cout << errorMsgText << endl;
        if (cC != INVALID_SOCKET) closesocket(cC);
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

    if (GetServer(serverName, port, (sockaddr*)&from, &fromLen)) {
        cout << "[SERVER FOUND]: " << inet_ntoa(from.sin_addr) << endl;
        cout << "[SERVER IP]: " << inet_ntoa(((SOCKADDR_IN*)&from)->sin_addr) << endl;
        cout << "[SERVER PORT]: " << ntohs(((struct sockaddr_in*)&from)->sin_port) << endl;
    }
    else {
        cout << "[BAD RESPONSE]" << endl;
    }

    if (WSACleanup() == SOCKET_ERROR) {
        cout << "WSACleanup: ERROR " << WSAGetLastError() << endl;
        return 1;
    }

    system("pause");
    return 0;
}