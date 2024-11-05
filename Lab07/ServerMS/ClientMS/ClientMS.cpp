#include <iostream>
#include "tchar.h"
#include "Windows.h"
#include <thread>
#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#pragma warning(disable : 4996)


using namespace std;

string GetMailslotError(int code) {
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
    case ERROR_SEM_TIMEOUT:
        msgText = "----> [TIME IS OVER] <----";
        break;
    default:
        msgText = "Unknown error";
        break;
    }
    return msgText;
}

string SetMailslotError(string text, int code) {
    return text + GetMailslotError(code);
}


wstring StringToWString(const string& str) {
    return wstring(str.begin(), str.end());
}


int main() {

    setlocale(LC_ALL, "rus");
    HANDLE cH;
    DWORD wb;
    //char wbuf[] = "Hello from Vlad [MAILSLOT]";
    string wbuf;
    int count;
    clock_t t1, t2;
   //  vector<string> domains = { "\\\\user-iaibsc3f10\\mailslot\\Box","\\\\desktop-2sjth0h\\mailslot\\Box" };
    try {
      //  for (const auto& domain : domains) {
           // wstring wDomain = StringToWString(domain);
            if ((cH = CreateFile(
                TEXT("\\\\*\\mailslot\\Box"),
                GENERIC_WRITE | GENERIC_READ,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                NULL, NULL
            )) == INVALID_HANDLE_VALUE) {
                throw SetMailslotError("CreateFile:", GetLastError());
            }



            cout << "[MESSAGE COUNT]: ";
            cin >> count;
            cout << endl;
            t1 = clock();
            for (int i = 0; i < count; i++) {
                wbuf = "Hello from Vlad [" + to_string(i) + "]";
                Sleep(1000);
                if (!WriteFile(cH, wbuf.c_str(), sizeof(wbuf), &wb, NULL)) {
                    throw SetMailslotError("WriteFile:", GetLastError());
                }
                else {
                    cout << "[SEND]:SUCCESS\n";
                }
            }
            //
            //wbuf = "";

            //if (!WriteFile(cH, wbuf.c_str(), sizeof(wbuf), &wb, NULL)) {
            //    throw SetMailslotError("WriteFile:", GetLastError());
            //}
            //else {
            //    cout << "[SEND]: Empty String \n";
            //}

            t2 = clock();

            cout << "[TIME]:" << (t2 - t1) << " мс.\n";

            CloseHandle(cH);
        // }
       
    }
    catch (string errorMailslotText) {
        cout << endl << errorMailslotText;
    }

    system("pause");

    return 0;
}