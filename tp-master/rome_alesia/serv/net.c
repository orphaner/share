/**
 * module : net
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
#include <sys/socket.h>  
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

#include "erreur.h"


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
  Redéfinition de fonctions pour gestion des erreurs
  ----------------------------------------------------------------------------*/
/**
 * cf man socket
 */
int xsocket (int domain, int type, int protocol)
{
   int sock;;
   if ( (sock = socket (domain, type, protocol)) < 0 )
      erreur_sys (ERR_SOCKET);
   return sock;
}


/**
 * cf man bind
 */
void xbind (int sockfd, struct sockaddr *xaddr, socklen_t addrlen)
{
   if (bind (sockfd,(struct sockaddr *) xaddr, addrlen ) < 0)
      erreur_sys (ERR_BIND);
}


/**
 * cf man listen
 */
void xlisten (int sock, int backlog)
{
   if (listen (sock, backlog) < 0)
      erreur_sys (ERR_LISTEN);
}


/**
 * cf man accept
 */
int xaccept (int sock, struct sockaddr *adresse, socklen_t *longueur)
{
   int res;
   if ( (res = accept (sock, adresse, longueur)) < 0 )
      erreur_sys (ERR_ACCEPT);
   return res;
}


/**
 * cf man connect
 */
void xconnect (int sockfd, struct sockaddr *serv_addr, socklen_t addrlen)
{
   if (connect (sockfd, serv_addr, addrlen) < 0)
      erreur_sys (ERR_CONNECT);
}


/*------------------------------------------------------------------------------
  Fonctions d'envoi / réception de messages
  ----------------------------------------------------------------------------*/
/**
 * cf man recv
 */
ssize_t xrecv (int fd, void *buf, size_t count, int flags)
{
   ssize_t taille;

   memset (buf, '\0', 512);
   taille = recv (fd, buf, count, flags);
   if (taille == -1)
      warning (WARN_RECV);

/*    printf("taille reçu: <%d>\n", taille); */

   return taille;
}


/**
 * cf man send
 */
int xsend (int s, void *msg, size_t len, int flags)
{
   int result;
   printf("envoi\n");

   if ( (result = send (s, msg, len, flags)) == -1 )
      warning (WARN_SEND);

/*    memset (msg, '\0', 512); */
   return result;
}


/**
 *
 */
int read_int (int sock, char *buffer)
{
   int i;
   xrecv (sock, buffer, 4, MSG_WAITALL);
   i = * ((int *) buffer);
   return ntohl (i);
}


/**
 *
 */
int read_char (int sock, char * buffer)
{
   char c;
   xrecv (sock, buffer, 1, 0);
   c = * ((char *) buffer);
   return c;
}


/**
 *
 */
int send_char (int sock, char c)
{
   char buffer[1];
   buffer[0] = c;
   return xsend (sock, buffer, 1, 0);
}


/**
 *
 */
int send_int (int sock, int i)
{
   char buffer[4];
   sprintf (buffer, "%d", i);
   return xsend (sock, buffer, 4, 0);
}
