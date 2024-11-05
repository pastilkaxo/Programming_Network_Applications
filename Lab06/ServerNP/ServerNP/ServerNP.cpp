#include <iostream>
#include "tchar.h"
#include "Windows.h"
#include <thread>
#include <chrono>
#include <ctime>
#include <string>
#pragma warning(disable : 4996)

using namespace std;


string GetPipeError(int code) {
    string msgText;
    switch (code)
    {
    case WSAEINTR:
        msgText = "Function interrupted";
        break;
    case WSAEACCES:
        msgText = "Permission denied";
        break;
    case WSAEFAULT:
        msgText = "Wrong address";
        break;
    case WSAEINVAL:
        msgText = "Wrong args";
        break;
    case WSAEMFILE:
        msgText = "Too many files have opened";
        break;
    case WSAEWOULDBLOCK:
        msgText = "Resource temporarily unavailable";
        break;
    case WSAEINPROGRESS:
        msgText = "Operation in progress";
        break;
    case WSAEALREADY:
        msgText = "Operation already in progress";
        break;
    case WSAENOTSOCK:
        msgText = "Wrong socket";
        break;
    case WSAEDESTADDRREQ:
        msgText = "Required adress location";
        break;
    case WSAEMSGSIZE:
        msgText = "Message is too long ";
        break;
    case WSAEPROTOTYPE:
        msgText = "Wrong protocol type to socket";
        break;
    case WSAENOPROTOOPT:
        msgText = "Error in protocol option";
        break;
    case WSAEPROTONOSUPPORT:
        msgText = "Protocol is not supported";
        break;
    case WSAESOCKTNOSUPPORT:
        msgText = "Socket type is not supported";
        break;
    case WSAEOPNOTSUPP:
        msgText = "Operation is not supported";
        break;
    case WSAEPFNOSUPPORT:
        msgText = "Protocol type is not supported";
        break;
    case WSAEAFNOSUPPORT:
        msgText = "Addresses type is not supported by protocol";
        break;
    case WSAEADDRINUSE:
        msgText = "Address is already used";
        break;
    case WSAEADDRNOTAVAIL:
        msgText = "Requested address cannot be used";
        break;
    case WSAENETDOWN:
        msgText = "Network disconnected";
        break;
    case WSAENETUNREACH:
        msgText = "Network is unttainable";
        break;
    case WSAENETRESET:
        msgText = "Network dropped the connection";
        break;
    case WSAECONNABORTED:
        msgText = "Connection aborted";
        break;
    case WSAECONNRESET:
        msgText = "Connection restored";
        break;
    case WSAENOBUFS:
        msgText = "Not enough memory for buffers";
        break;
    case WSAEISCONN:
        msgText = "Socket has already connected";
        break;
    case WSAENOTCONN:
        msgText = "Socket has not connected";
        break;
    case WSAESHUTDOWN:
        msgText = "Send cannot be performed: socket was shutdowned";
        break;
    case WSAETIMEDOUT:
        msgText = "Alloted time interval has ended";
        break;
    case WSAECONNREFUSED:
        msgText = "Connection was rejected";
        break;
    case WSAEHOSTDOWN:
        msgText = "Host was shotdowned";
        break;
    case WSAEHOSTUNREACH:
        msgText = "Has not way for host";
        break;
    case WSAEPROCLIM:
        msgText = "Too many processes";
        break;
    case WSASYSNOTREADY:
        msgText = "Network is unavailable";
        break;
    case WSAVERNOTSUPPORTED:
        msgText = "Version is not supported";
        break;
    case WSANOTINITIALISED:
        msgText = "WS2_32.dll is not initialised";
        break;
    case WSAEDISCON:
        msgText = "Connection in progress";
        break;
    case WSATYPE_NOT_FOUND:
        msgText = "Type is not found";
        break;
    case WSAHOST_NOT_FOUND:
        msgText = "Host is not found";
        break;
    case WSATRY_AGAIN:
        msgText = "Try again";
        break;
    case WSANO_RECOVERY:
        msgText = "Unknown error";
        break;
    case WSANO_DATA:
        msgText = "Has not data type";
        break;
    case WSAEINVALIDPROCTABLE:
        msgText = "Invalid provider";
        break;
    case WSAEINVALIDPROVIDER:
        msgText = "Error in provider version";
        break;
    case WSAEPROVIDERFAILEDINIT:
        msgText = "Failed provider initialization";
        break;
    case WSASYSCALLFAILURE:
        msgText = "Abnormal termination of a system call";
        break;
    default:
        msgText = "Unknown error";
        break;
    }
    return msgText;
}

string SetPipeError(string text,int code) {
	return text + GetPipeError(code);
}

int main(int args, _TCHAR* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
	HANDLE hPipe;
    clock_t t1, t2;

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;


    while (true) { 
        try {
            cout << "[WAITING FOR CLIENT]\n";

            // Создание именованного канала
            if ((hPipe = CreateNamedPipe(
                TEXT("\\\\.\\pipe\\Tube"),
                PIPE_ACCESS_DUPLEX,
                PIPE_TYPE_MESSAGE | PIPE_WAIT,
                1,
                0,
                0,
                INFINITE,
                &sa
            )) == INVALID_HANDLE_VALUE) {
                throw SetPipeError("CreateNamedPipe:", GetLastError());
            }


            // Ожидание подключения клиента
            if (!ConnectNamedPipe(hPipe, NULL)) {
                throw SetPipeError("ConnectNamedPipe:", GetLastError());
            }
            else {
                cout << "[CLIENT CONNECTED]\n";
                cout << "<------------------------>\n";
            }

            char ibuf[50],updbuf[60];
            DWORD bytesRead,bytesWrite;
            bool running = true;
            t1 = clock();
            while (running) {
                // Чтение сообщения от клиента
                if (ReadFile(hPipe, ibuf, sizeof(ibuf) - 1, &bytesRead, NULL)) {
                    Sleep(1000);
                    ibuf[bytesRead] = '\0';
                    cout << "[MESSAGE FROM CLIENT]: " << ibuf << endl;

                    string response = string(ibuf);
                    if (!WriteFile(hPipe, response.c_str(), response.size(), &bytesWrite, NULL)) {
                        throw SetPipeError("WriteFile:", GetLastError());
                    }

                    ReadFile(hPipe, updbuf, sizeof(updbuf) - 1, &bytesRead, NULL);
                    updbuf[bytesRead] = '\0';
                    cout << "UPD:[MESSAGE FROM CLIENT]: " << updbuf << endl;

                }
                else {
                    DWORD error = GetLastError();
                    if (error == ERROR_BROKEN_PIPE || error == ERROR_NO_DATA) {
                        cout << "[CLIENT DISCONNECTED]\n";
                        running = false;
                        break;
                    }
                }
            }

            t2 = clock();

            cout << "[TIME]: " << (t2 - t1) << " мс.\n";

            // Отключение клиента от именованного канала
            DisconnectNamedPipe(hPipe);
            CloseHandle(hPipe);

        }
        catch (string ErrorPipeText) {
            cout << endl << ErrorPipeText;
        }
    }

	return 0;
}