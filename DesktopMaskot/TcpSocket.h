#pragma once
#include "myfunc.h"
class GmailTcp {
	SOCKET sock;
	WSADATA wsaData;
	SOCKET sockListen;
public:
	GmailTcp() {
		if(WSAStartup(WINSOCK_VERSION, &wsaData))
			MessageBox(NULL, TEXT("TCP ERROR"), TEXT("start failed"), MB_OK | MB_ICONINFORMATION);
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(20000);
		addr.sin_addr.S_un.S_addr = INADDR_ANY;
		sockListen = socket(addr.sin_family, SOCK_STREAM, 0);

		if (sockListen == INVALID_SOCKET)
			MessageBox(NULL, TEXT("TCP ERROR"),TEXT("socket failed"), MB_OK | MB_ICONINFORMATION);

		if (bind(sockListen, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
			MessageBox(NULL, TEXT("TCP ERROR"), TEXT("bindエラー"), MB_OK | MB_ICONINFORMATION);
		std::thread cT(&GmailTcp::wateTcp,this);
		cT.detach();
	}
	~GmailTcp() {
		closesocket(sock);
		WSACleanup();
	}
	void wateTcp() {
		while (1) {
			if (listen(sockListen, 5) == SOCKET_ERROR)
			{
				MessageBox(NULL, TEXT("TCP ERROR"), TEXT("listenエラー"), MB_OK | MB_ICONINFORMATION);
			}
			struct sockaddr_in client;
			int clientlen = sizeof(client);
			char buf[256];
			memset(buf, 0, sizeof(buf));
			sock = accept(sockListen, (struct sockaddr*)&client, &clientlen);
			recv(sock, buf, sizeof(buf), 0);
			printf("%s\n", buf);
			if (!strcmp(buf, "update"))
				printf("ok");
			char ret[] = u8"日本語";
			send(sock, ret, sizeof(ret), 0);
		}
	}

};