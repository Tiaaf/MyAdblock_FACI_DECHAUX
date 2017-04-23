#include <stdlib.h> /* exit */
#include <string.h> /* memset */
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main (int argc, char* argv[]) {
	
	int clientSocket ;
	if ((clientSocket = socket(PF_INET, SOCK_STREAM, 0))<0) {
		perror ("cliecho: erreur socket\n") ;
		exit(1) ;
	}
	
	struct hostent *hp;	
	hp = (struct hostent *)gethostbyname(argv[1]);
	if(hp == NULL){
		fprintf(stderr, "cliecho: %s: %s non trouve dans in /etc/hosts ou dans le dns\n", argv[0], argv[1]);
	}
	
	/* Remplir la structure serv_addr avec l'adresse du serveur */
	struct sockaddr_in serv_addr ;
	bzero((char *)&serv_addr, sizeof(serv_addr)) ;
	//serv_addr.sin_family = AF_INET ;
	serv_addr.sin_family = hp->h_addrtype;
	serv_addr.sin_port = htons((ushort)atoi(argv[2])) ;
	//serv_addr.sin_addr.s_addr = inet_addr(argv[1]) ;
	serv_addr.sin_addr = *((struct in_addr *)(hp->h_addr)) ;
	
	/* Demande de connexion */
	if(connect (clientSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <0) {
		perror("cliecho: erreur connect") ;
		exit(1) ;
	}

	char buff[15];
	read(clientSocket, buff, 15);
	printf("%s", buff);
	exit(0);
}
