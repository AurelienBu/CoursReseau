#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <unistd.h>

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
      for (adr = (struct in_addr **)host->h_addr_list; *adr; adr++)
           printf("IP : %s, Nom : %s\n", inet_ntoa(**adr),host -> h_name);
 }
int getAdresseMAC(char* addMAC,int socket){
	
}

int main(int argc, char**argv){
	int hSocket;
	
	struct hostent *infoHost,infoServer;
	struct sockaddr_in adresseSocketServeur, adresseSocketClient;
	unsigned int tailleSockaddr_in;

	char msgClient[MAXSTRING],msgServeur[MAXSTRING],addMAC[6];
	int nbreRecv;

	FILE* fp = NULL;
	fp= fopen("adressMax.b","a+t");
	
	/* Création de la socket */	
	hSocket = socket(AF_INET,SOCK_DGRAM,0);
					//creation d'un socket : AF_INET : internet work (UDP, TCP)
					//SOCK_DGRAM : protocole UDP
	
	if(hSocket == -1){
		perror("Erreur lors de la création de la socket !\n");
		exit(-1);
	}
	else 
		printf("Creation de la socket \n");
	
	/* Préparation de la structure d adresse */	
	adresseSocketServeur.sin_family = AF_INET;
	adresseSocketServeur.sin_port = htons(PORT);
	//adresseSocketServeur.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&(adresseSocketServeur.sin_zero),8);

	unsigned int taille =sizeof(struct sockaddr_in);

	if(bind(hSocket, (struct sockaddr *)&adresseSocketServeur, sizeof(struct sockaddr_in)) == -1){
		perror("Error bind");
	}
	

	view_ip();
	
	/* Envoie du message vers serveur  */
	if( recvfrom(hSocket,
		     msgClient,
		     MAXSTRING,
		     0,
		     (struct sockaddr *)&adresseSocketClient,
		     &taille) == -1){

		perror("Erreur de send");
	}
	else {
		struct ifreq ifr;

		ifr.ifr_addr.sa_family = AF_INET;

		strcpy(ifr.ifr_name, "eno1");
		ioctl(hSocket,SIOCGIFHWADDR,&ifr);
		const unsigned char* mac = (unsigned char*) ifr.ifr_hwaddr.sa_data;
		printf("MAC  : %02X%02x%02x%02x%02x%02x \n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
		//fwrite(mac,6,1,fp);
		unsigned char macbad[6];
		fseek(fp,0,SEEK_SET);
		fread(macbad,6+1,1,fp);
		printf("FILE : %02X%02x%02x%02x%02x%02x \n",macbad[0],macbad[1],macbad[2],macbad[3],macbad[4],macbad[5]);

		int i,j=0;
		for(i=0;i<6;i++)
			if(mac[i] == macbad[i])
				j++;
			else
				i = 404;

		if(j==6)
			printf("pas afficher message !\n");
		else
			printf("%s\n",msgClient);
	}

	close(hSocket);
	

	return(0);
}

