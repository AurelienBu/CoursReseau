#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <netinet/in.h>

#include <netinet/tcp.h>
#include <arpa/inet.h>

#define PORT 65000

#define MAXSTRING 100

int main(int argc, char**argv){
	int hSocket;
	
	struct hostent *infoHost,infoServer;
	struct sockaddr_in adresseSocketServeur, adresseSocketClient;
	unsigned int tailleSockaddr_in;

	char msgClient[MAXSTRING],msgServeur[MAXSTRING];
	int nbreRecv;

	/* Récupération des arguments */
	if(argc != 2){
		perror("Pas le bon nombre d'argument!");
		exit(-1);
	}
	
	/* Création de la socket */	
	hSocket = socket(AF_INET,SOCK_STREAM,0);
					//creation d'un socket : AF_INET : internet work (UDP, TCP)
					//SOCK_DGRAM : protocole UDP
	
	if(hSocket == -1){
		perror("Erreur lors de la création de la socket !\n");
		exit(-1);
	}
	else 
		printf("Creation de la socket\n");

	/* Préparation de la structure d adresse */	
	adresseSocketServeur.sin_family = AF_INET;
	adresseSocketServeur.sin_port = htons(PORT);
	adresseSocketServeur.sin_addr.s_addr= inet_addr(*++argv); //récuperation en stockage adresse serveur
//adresseSocketServeur.sin_addr.s_addr= inet_addr("192.168.89.118");
	printf("%s adresse IP \n",inet_ntoa(adresseSocketServeur.sin_addr));
	bzero(&(adresseSocketServeur.sin_zero),8);
	

	strcpy(msgClient,"Paola t'es une emmerdeuse ! \n");
	unsigned int taille =sizeof(struct sockaddr_in);
	
	/* Connexion au serveur TCP */
	if((connect(hSocket, (struct sockaddr *)&adresseSocketServeur, taille)) == -1)
	{
		perror("Connexion impossible");
	}

	/* Envoie du message vers serveur  */
	if(send(hSocket,
		msgClient,
		MAXSTRING,
		0) == -1){
			
			perror("Erreur de send");
		//exit(-1);
	}
	else 
		printf("Send ok\n");
	
	if(recv(hSocket,
		msgServeur,
		MAXSTRING,
		0) == -1){
			perror("Erreur recv");
	}
	else
		printf("Received : %s\n",msgServeur);

	close(hSocket);
	

	return(0);
}
