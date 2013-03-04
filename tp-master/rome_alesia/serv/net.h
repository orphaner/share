#ifndef __net_h__
#define __net_h__
/**
 * module : net
 * brief : 
 * author : Nicolas LASSALLE
 * date : 
 * bug : Aucun détecté à ce jour
 *
 */

/*------------------------------------------------------------------------------
  Constantes
  ----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
  Types personnalisés
  ----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/

/* Redéfinition de fonctions pour gestion des erreurs */
extern int xsocket (int domain, int type, int protocol);
extern void xbind (int sockfd, struct sockaddr *addr, socklen_t addrlen);
extern void xlisten (int sock, int backlog);
extern int xaccept (int sock, struct sockaddr *adresse, socklen_t *longueur);
extern void xconnect (int sockfd, struct sockaddr *serv_addr, socklen_t addrlen);

/* Fonctions */
extern ssize_t xrecv (int fd, void *buf, size_t count, int flags);
extern int xsend (int s, void *msg, size_t len, int flags);

extern int read_int (int sock, char *buffer);
extern int read_char (int sock, char *buffer);

extern int send_char (int sock, char c);
extern int send_int (int sock, int i);



/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/


#endif
