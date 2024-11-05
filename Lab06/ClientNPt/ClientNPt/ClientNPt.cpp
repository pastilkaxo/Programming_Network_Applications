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

string SetPipeError(string text, int code) {
    return text + GetPipeError(code);
}

int main(int args, _TCHAR* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    HANDLE hPipe;
    try {
        HANDLE hPipe;
        try {
            if ((hPipe = CreateFile(
                TEXT("\\\\.\\pipe\\Tube"),
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL, OPEN_EXISTING, NULL,
                NULL
            )) == INVALID_HANDLE_VALUE) {
                throw SetPipeError("createfile:", GetLastError());
            }
            else {
                cout << "[CONNECTED TO SERVER]\n";
            }

            string obuf;
            DWORD  bytesRead = sizeof(obuf) - 1;
            char ibuf[60];
            int msgCount;
            DWORD state = PIPE_READMODE_MESSAGE;

            cout << "Кол-во сообщений: ";
            cin >> msgCount;
            cout << "<----------------------->\n";

            for (int i = 0; i < msgCount; i++) {
                obuf = "Hello from Vlad (" + to_string(i) + ")";
                Sleep(1000);
                // измененеие динамических характеристик канала
                SetNamedPipeHandleState(hPipe, &state, NULL, NULL);
                // запись и чтение данных именованного канала в одной функции за одну операцию
                TransactNamedPipe(hPipe,(LPDWORD)obuf.c_str(),sizeof(obuf)-1,ibuf, sizeof(obuf)-1,&bytesRead,NULL );

                ibuf[bytesRead] = '\0';

                cout << "[SERVER RESPONSE]: " << ibuf << endl;

            }

            system("pause");

            CloseHandle(hPipe);
        }
        catch (string ErrorPipeText) {
            cout << endl << ErrorPipeText;
        }

    }
    catch (string ErrorPipeText) {
        cout << endl << ErrorPipeText;
    }


    return 0;
}