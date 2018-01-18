#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>

#include <netinet/tcp.h>
#include <arpa/inet.h>

#define PORT 65000

#define MAXSTRING 100

void view_ip()
{
 char s[256];
   struct hostent *host;
   struct in_addr **adr;
 
   if (!gethostname(s, 256))
     if ((host = gethostbyname(s)) != NULL)
      for (adr = (struct in_addr **)host->h_addr_list; *adr; adr++){
	printf("IP : %s, Nom : %s\n", inet_ntoa(**adr),host -> h_name);
	}
 }

/* Programme de recupération d'adresse mac */
/* Attention fonctionnel mais pas mis en forme dans le programme */
int getAdresseMAC(int socket){
    
    /* requet ARP puis enregistrement dans un .txt*/
    system("rm ~/arpp.txt");
    system("arp -e > ~/arpp.txt");
    
    fp = fopen("/home/workspace/arpp.txt", "r");
    if(fp==NULL){
        perror("Erreur d'ouverture de la table ARP");
        exit(0);
    }
    contenu_fp = (char *) malloc(1000 * sizeof(char));
    while(!feof(fp)) {
        fgets(contenu_fp, 256, fp);
        //recherche de l'adresse ip du client dans la table
        p=strstr(contenu_fp, inet_ntoa(adresseSocketClient.sin_addr));
        if(p != NULL){
            break;
        }
    }
    //Recuperation adresse mac
    mac = strtok(contenu_fp, " ");
    for(i=0; i<2; i++)
        mac = strtok(NULL, " ");
}

int main(int argc, char**argv){
	int hSocket,hSocketDiscute;
	
	struct hostent *infoHost,infoServer;
	struct sockaddr_in adresseSocketServeur, adresseSocketClient;
	unsigned int tailleSockaddr_in;

	char msgServeur[MAXSTRING],addMAC[6]; // msgClient[MAXSTRING],
char *msgClient;
	int nbreRecv;

	FILE* fp = NULL;
	fp= fopen("adressIP.b","a+t");
	
	/* Création de la socket */	
	hSocket = socket(AF_INET,SOCK_STREAM,0);
//hSocketDiscute = socket(AF_INET,SOCK_STREAM,0);
					//creation d'un socket : AF_INET : internet work (UDP, TCP)					//SOCK_DGRAM : protocole UDP

	if(hSocket == -1){
		perror("Erreur lors de la création de la socket !\n");
		exit(-1);
	}

	printf("Creation de la socket (%d)\n",hSocket);

	
	/* Préparation de la structure d adresse */	
	adresseSocketServeur.sin_family = AF_INET;
	adresseSocketServeur.sin_port = htons(PORT);
	//adresseSocketServeur.sin_addr.s_addr = inet_addr("192.168.89.49");
	adresseSocketServeur.sin_addr.s_addr = INADDR_ANY;
	bzero(&(adresseSocketServeur.sin_zero),8);

	unsigned int taille =sizeof(struct sockaddr);
    /* Reliage la socket à l'adresse du serveur */
	if(bind(hSocket, (struct sockaddr *)&adresseSocketServeur, sizeof(struct sockaddr)) == -1){
		perror("Error bind");
	}
	
/*
	view_ip();
*/
	int L;
    /* Attendre qu'un client se connect */
	if((L=listen(hSocket, 20)) == -1){
		perror("Erreur Listen");
		close(hSocket);
		exit(0);
	}
	printf("Listen : %d\n",L);
	taille = sizeof(struct sockaddr);

    /* Accepter la connexion du client */
	if( (hSocketDiscute=accept(hSocket, (struct sockaddr *)&adresseSocketClient,	(socklen_t *) &taille)) == -1){

			perror("Erreur accept");
			close(hSocket);
			close(hSocketDiscute);
			exit(0);
			
	}
	printf("Discute : %d \n",hSocketDiscute);
    msgClient = (char *) malloc(MAXSTRING * sizeof(char));
    printf("OK\n");
    /* Reception du message client */
    if(recv(hSocketDiscute, msgClient, 256, 0) == -1){
        perror("Erreur recv");
    }
    /* blocage par adresse ip */
    else {
     //fprintf(fp,"%s\n",inet_ntoa(adresseSocketClient.sin_addr));
     if(strcmp("192.168.89.105",inet_ntoa(adresseSocketClient.sin_addr)) == 0)
        printf("bloqué haha gotta you\n");
     else
        fprintf(stderr,"Received : %s\n",msgClient);
     }
        
    free(msgClient);
    /* Envoi d'une reponse */
    if(send(hSocketDiscute, "Ack", strlen("Ack"),0) == -1){
        perror("Erreur de send");
        //exit(-1);
    }
    printf("Send ok\n");
	close(hSocket);

	return(0);
}

