#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <unistd.h>

#ifdef __unix__                    /* __unix__ is usually defined by compilers targeting Unix systems */

    #define OS_Windows 0
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

#elif defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or   64 bit Windows systems */

    #define OS_Windows 1
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
    #include <windows.h>
    #include <stdio.h>
    #include <tchar.h>

#endif
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
int getAdresseMAC(char* addMAC,int socket){
	
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

	if(bind(hSocket, (struct sockaddr *)&adresseSocketServeur, sizeof(struct sockaddr)) == -1){
		perror("Error bind");
	}
	
/*
	view_ip();
*/
	int L;
	if((L=listen(hSocket, 20)) == -1){
		perror("Erreur Listen");
		close(hSocket);
		exit(0);
	}
	printf("Listen : %d\n",L);
	taille = sizeof(struct sockaddr);

	if( (hSocketDiscute=accept(hSocket, (struct sockaddr *)&adresseSocketClient,	(socklen_t *) &taille)) == -1){

			perror("Erreur accept");
			close(hSocket);
			close(hSocketDiscute);
			exit(0);
			
	}
	printf("Discute : %d \n",hSocketDiscute);
					

				msgClient = (char *) malloc(MAXSTRING * sizeof(char));
				printf("OK\n"); 

				if(recv(hSocketDiscute, msgClient, 256, 0) == -1){
					perror("Erreur recv");
				}
				else
					fprintf(stderr,"Received : %s\n",msgClient);
		
				free(msgClient);
				
				if(send(hSocketDiscute, "Ack", strlen("Ack"),0) == -1){
			
					perror("Erreur de send");
					//exit(-1);
				}
				

				//else 
					printf("Send ok\n");
	


				close(hSocket);
				
	
		
	
	/* Envoie du message vers serveur  */

	/*else {
		fprintf(fp,"%s\n",inet_ntoa(adresseSocketClient.sin_addr));
		if(strcmp("192.168.89.105",inet_ntoa(adresseSocketClient.sin_addr)) == 0)
			printf("bloqué haha gotta you\n");
		else 
			printf("%s\n",msgClient);
	}*/

	close(hSocket);
	

	return(0);
}

