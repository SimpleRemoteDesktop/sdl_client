#include <cstdio>
# include "tcpSocketWrapper.h"
#include <stdio.h>


static SOCKET sock;

void SRD_Net_init(void)
{
#ifdef WIN32
	WSADATA wsa;
	int err = WSAStartup(MAKEWORD(2, 2), &wsa);
	if(err < 0)
	{
		puts("WSAStartup failed !");
	}
#endif
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET)
	{
		printf("ERROR WHILE CREATING SOCKET");
	}


}

void SRD_Net_connect(const char *hostname, int port) {
	struct hostent *hostinfo = NULL;
	SOCKADDR_IN sin = { 0 }; /* initialise la structure avec des 0 */

	hostinfo = gethostbyname(hostname); /* on récupère les informations de l'hôte auquel on veut se connecter */
	if (hostinfo == NULL) /* l'hôte n'existe pas */
	{
		printf ( "Unknown host %s.\n", hostname);
	}

	sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr; /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */
	sin.sin_port = htons(port); /* on utilise htons pour le port */
	sin.sin_family = AF_INET;

	if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		perror("connect()");
	}
}

int SRD_Net_send(void *data, int size) {
	return send(sock, (const char*) data, size, 0);

}

int SRD_Net_receive(char* buffer, int nbytes) {
	return recv(sock, buffer, sizeof buffer - 1, 0);
}

void SRD_Net_end(void)
{
	closesocket(sock);
#ifdef WIN32
	WSACleanup();
#endif
}


