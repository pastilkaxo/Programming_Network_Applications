#include <iostream>
#include "Winsock2.h"                
#include <string>
#include <stdexcept>
#pragma comment(lib, "WS2_32.lib    ")   

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

using namespace std;

int countServers = 0;
string GetErrorMessageText(int ErrorCode);
string SetErrorMessageText(string ErrorMessage, int ErrorCode);
bool GetRequestFromClient(char* name, short port, sockaddr* from, int* flen);
bool  PutAnswerToClient(char* name, struct sockaddr* to, int* lto);
bool BroadcastServerCall(char* name, short port);

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
// name - позывной сервера from - указатель структуры параметры сокета клиента
bool GetRequestFromClient(char* name, short port, sockaddr* from, int* flen) { // обработка/ожидание запроса(позывной) клиентской программы
    SOCKET sS;
    try
    {
        if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMessageText("socket:", WSAGetLastError());

        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(port);
        serv.sin_addr.s_addr = INADDR_ANY;


        if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
            throw SetErrorMessageText("bind:", WSAGetLastError());

        char ibuf[1024];
        int libuf;

        while (true) {
            libuf = recvfrom(sS, ibuf, sizeof(ibuf) - 1, 0, from, flen); // ожидание запроса
            if (libuf == SOCKET_ERROR) {
                int error = WSAGetLastError();
                if (error == WSAETIMEDOUT) // состояние ожидания 
                    // return false;
                    continue;
                else
                    throw SetErrorMessageText("recvfrom:", error);
            }

            ibuf[libuf] = '\0';

            cout << "[MESSAGE]: " << ibuf << endl;

            if (strcmp(ibuf, name) == 0) { // сравнивает символы двух строк
                closesocket(sS);
                return true;
            }
            else {
                cout << "[BAD REQUEST]" << endl;
            }
        }
    }
    catch (string errorMsgText)
    {
        cout << errorMsgText << endl;
        if (sS != INVALID_SOCKET) closesocket(sS);
        return false;
    }
}
// name - позыной to - указатель структуры параметры  сокета клиента
bool PutAnswerToClient(char* name, struct sockaddr* to, int* lto) { // пересылка позывного сервера программе клиента или подтверждение сервером запроса клиента на установку соединения
    SOCKET sS;

    try {
        if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMessageText("socket:", WSAGetLastError());

        if (sendto(sS, name, strlen(name) + 1, 0, to, *lto) == SOCKET_ERROR)
            throw SetErrorMessageText("sendto:", WSAGetLastError());

        closesocket(sS);
        return true;
    }
    catch (string errorMsgText) {
        cout << errorMsgText << endl;
        if (sS != INVALID_SOCKET) closesocket(sS);
        return false;
    }
}

bool GetServer(char* call, short port, struct sockaddr* from, int* flen) {
    SOCKET cC;
    SOCKADDR_IN all;

    int timeout = 5000;

    int optval = 1;
    char buf[50];

    try {
        if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw  SetErrorMessageText("socket:", WSAGetLastError());

        if (setsockopt(cC, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR)
            throw  SetErrorMessageText("setsocketopt:", WSAGetLastError());

        if (setsockopt(cC, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(int)) == SOCKET_ERROR)
            throw  SetErrorMessageText("setsocketopt:", WSAGetLastError());

        all.sin_family = AF_INET;
        all.sin_port = htons(port);
        all.sin_addr.s_addr = INADDR_BROADCAST;

        int len;
        if (len = sendto(cC, call, strlen(call) + 1, NULL, (sockaddr*)&all, sizeof(all)) == SOCKET_ERROR)
            throw SetErrorMessageText("sendto:", WSAGetLastError());

        while (true)
        {
            if (recvfrom(cC, buf, sizeof(buf), NULL, from, flen) == SOCKET_ERROR)
                break;
            if (strcmp(call, buf) == 0) {
                countServers++;
                cout << "[SERVERS COUNT]:" << countServers << endl;
                hostent* shostname = gethostbyaddr((char*)&((SOCKADDR_IN*)from)->sin_addr, sizeof(SOCKADDR_IN), AF_INET);
                cout << "============================================\n";
                cout << "[SERVER IP]: " << inet_ntoa(((SOCKADDR_IN*)from)->sin_addr) << endl;
                cout << "[SERVER PORT]: " << ntohs(((struct sockaddr_in*)from)->sin_port) << endl;
                cout << "[HOSTNAME]: " << shostname->h_name << endl;
                cout << "============================================\n";
                closesocket(cC);
                exit(0);
                return true;
            }
            else {
                return false;
            }
        }


    }
    catch (string errorMsgText)
    {
        if (WSAGetLastError() == WSAETIMEDOUT) {
            cout << "[TIME IS OUT]: " << countServers << endl;
            if (closesocket(cC) == SOCKET_ERROR) throw SetErrorMessageText("closesocket: ", WSAGetLastError());
        }
        else throw SetErrorMessageText("GetServer:", WSAGetLastError());
    }
}

int main() {
    setlocale(LC_CTYPE, "ru");

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        cout << "WSAStartup: ERROR" << endl;
        return 1;
    }

    char server_name[] = "Hello";
    sockaddr_in from;
    memset(&from, 0, sizeof(from));
    int flen = sizeof(from);

   GetServer((char*)"Hello", 2000, (sockaddr*)&from, &flen);


    char hostname[50];

    if(gethostname(hostname,sizeof(hostname)) == SOCKET_ERROR)
        throw SetErrorMessageText("gethostname:", WSAGetLastError());

   
    cout << "\r[SERVER]: " << hostname << endl;
    cout << "\r[WAITING FOR CONNECTION]\n";


    while (true) {

        if (GetRequestFromClient(server_name, 2000, (sockaddr*)&from, &flen)) {
            hostent* client = gethostbyaddr((const char*)&from.sin_addr, 4, AF_INET);
            countServers++;
            cout << "============================================\n";
            cout << "[CONNECTED]" << endl;
            cout << "[CLIENT HOSTNAME]:" << client->h_name << endl;
            cout << "[CLIENT IP]:" << inet_ntoa(*(in_addr*)client->h_addr) << endl;
            cout << "[CLIENT PORT]: " << htons(from.sin_port) << endl;
            cout << "[SERVERS]:" << countServers << endl;

            if (PutAnswerToClient(server_name, (sockaddr*)&from, &flen)) {
                cout << "[SEND]:SUCCESS" << endl;
                cout << "============================================\n";
            }
            else {
                cout << "---> [SEND]:ERROR <---" << endl;
            }
        }
        else {
            cout << "[ERROR]" << endl;
            break;
        }

    }

    if (WSACleanup() == SOCKET_ERROR) {
        cout << "WSACleanup:ERROR" << endl;
        return 1;
    }

    return 0;
}

