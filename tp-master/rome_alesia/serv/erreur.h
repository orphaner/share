#ifndef __erreur_h__
#define __erreur_h__
/**
 * module : erreur
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
typedef enum
{
   EXIT_ALL_OK,
   ERR_MALLOC,
   ERR_REALLOC,

   ERR_PTHREAD_CREATE,
   ERR_PTHREAD_JOIN,

   ERR_SOCKET,
   ERR_BIND,
   ERR_LISTEN,
   ERR_ACCEPT,
   ERR_CONNECT,
   
   WARN_RECV,
   WARN_SEND
}Erreurs;

#ifdef MESSAGE_OK
static const char  *ErreursMsg[] =
{
   "",
   "Erreur de malloc",
   "Erreur de realloc",

   "Impossible de créer un thread",
   "Impossible de récupérer le retour d'un thread",

   "Erreur de socket",
   "Erreur de bind",
   "Erreur de listen",
   "Erreur de accept",
   "Erreur de connect",

   "Erreur de réception de message",
   "Erreur d'envoi de message"
};
#endif

/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
extern void erreur_sys (Erreurs);
extern void warning (Erreurs);


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/


#endif
