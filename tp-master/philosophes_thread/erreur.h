#ifndef __erreur_h__
#define __erreur_h__
/**
 * module : erreur
 * brief : 
 * author : Maxime GRYL
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
   ERR_PTHREAD_JOIN
}Erreurs;

#ifdef MESSAGE_OK
static const char  *ErreursMsg[] =
{
   "",
   "Erreur de malloc",
   "Erreur de realloc",
   "Impossible de créer un thread",
   "Impossible de récupérer le retour d'un thread"
};
#endif

/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
extern void erreur_sys (Erreurs);


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/


#endif
