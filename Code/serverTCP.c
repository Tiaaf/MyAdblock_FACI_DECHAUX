#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 1500
int str_echo (int sockfd) {
	int nrcv, nsnd;
	char msg[BUFSIZE];

	/*
	 * Attendre  le message envoye par le client 
	 */
	memset( (char*) msg, 0, sizeof(msg) );
	if ( (nrcv= read ( sockfd, msg, sizeof(msg)-1) ) < 0 )  {
		perror ("servmulti : : readn error on socket");
		exit (1);
	}
	msg[nrcv]='\0';
	printf ("servmulti :message recu=%s du processus %d nrcv = %d \n",msg,getpid(), nrcv);

	if ( (nsnd = write (sockfd, msg, nrcv) ) <0 ) {
		printf ("servmulti : writen error on socket");
		exit (1);
	}
	printf ("nsnd = %d \n", nsnd);
	return (nsnd);
} /* end of function */

int main (int argc, char* argv[]) {
	/* Ouvrir une socket socket STREAM) */
	int serverSocket ;
	if ((serverSocket = socket(PF_INET, SOCK_STREAM, 0))<0) {
		perror ("servecho: erreur socket\n") ;
		exit(1) ;
	}
	
	/* Lier l'adresse locale a la socket (bind) */
	struct sockaddr_in serv_addr ;
	memset (&serv_addr,0 , sizeof(serv_addr) ) ;
	serv_addr.sin_family = AF_INET ;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY) ;
	serv_addr.sin_port = htons(atoi(argv[1])) ;
	
	if (bind(serverSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0) {
		perror ("servecho: error bind\n") ;
		exit(1) ;
	}
	
	/* Parametrer le nombre de connexion "pending" */
	if (listen(serverSocket, SOMAXCONN)<0) {
		perror ("servecho: error listen\n") ;
		exit(1) ;
	}
	
	int dialogSocket ;
	int clilen ;
	struct sockaddr_in cli_addr ;
	for(;;) {
		/* Accept */
		clilen = sizeof(cli_addr) ;
		dialogSocket = accept(serverSocket, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen) ;
		if (dialogSocket<0) {
			perror ("servecho: error accept\n") ;
			exit(1) ;
		}
		switch( fork()) {
			case -1 : {
				perror("servecho: error fork\n") ;
				exit(1) ;
			}
			case 0 : {
				close (serverSocket) ;
				str_echo(dialogSocket) ;
				close(dialogSocket) ;
				exit(0) ;
			}
			default : {
				close(dialogSocket) ;
			}
		}
	}		
	exit(0) ;
}
