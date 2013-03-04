/**
 * module : client
 * brief : 
 * author : Nicolas LASSALLE
 * date : 
 * bug : Aucun détecté à ce jour
 *
 */


/*------------------------------------------------------------------------------
  Fichiers include
  ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>


typedef enum
{
   CIN_PSEUDO   = '1',
   CIN_NEW_GAME = '2'
}commandes_in;

/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/**
 *
 */
int main ()
{
   char pseudo[30];
   int cli_sock;
   struct sockaddr_in serv_addr;

   memset(pseudo,'\0',30);
   strcpy (pseudo, "mescouilles");

   cli_sock = socket (PF_INET, SOCK_STREAM, 0);
   
   serv_addr.sin_family =      AF_INET;
   serv_addr.sin_port =        htons (3000);
   serv_addr.sin_addr.s_addr = inet_addr ("127.0.0.1");
    
   connect (cli_sock, (struct sockaddr *)&serv_addr, sizeof (serv_addr));
   send (cli_sock, "1", sizeof(char), 0);

   send (cli_sock, pseudo, 30 * sizeof (char), 0);
   send (cli_sock, "2", sizeof(char), 0);

   memset(pseudo,'\0',30);
   snprintf (pseudo, 2*sizeof(int)+3*sizeof(char), "%d:%d:a", 10, 20);
   printf("pseudo[0] : %c\n",pseudo[0]);
   printf("pseudo[1] : %c\n",pseudo[1]);
   printf("pseudo[2] : %c\n",pseudo[2]);
   printf("pseudo[3] : %c\n",pseudo[3]);
   send (cli_sock, pseudo, 2*sizeof(int), 0);


   pause();

   exit (0);
}
