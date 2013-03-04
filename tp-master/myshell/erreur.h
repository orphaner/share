/**
 * module : erreur
 * brief :
 * author : Nicolas LASSALLE
 * date :
 * bug : Aucun détecté à ce jour
 *
 */

#ifndef __ERREUR_H__
#define __ERREUR_H__


/*------------------------------------------------------------------------------
  Constantes
  ----------------------------------------------------------------------------*/
typedef enum
{
   EXIT_ALL_OK,

   EXIT_NO_NEW_FORK,
   EXIT_CANT_EXEC_CMD,

   ERR_MALLOC,
   ERR_REALLOC,

   ERR_KEY_MUTEX,
   ERR_KEY_INDICE_NOM,
   ERR_KEY_INDICE_COL,
   ERR_KEY_INDICE_VALEUR,
   ERR_KEY_NOM,
   ERR_KEY_VALEUR,
   ERR_KEY_TAILLE,
   ERR_KEY_COL,

   ERR_CREATE_MUTEX,
   ERR_INIT_MUTEX,
   ERR_DELETE_MUTEX,

   ERR_CREATE_SHM_INFOS,
   ERR_ATTACH_SHM_INFOS,
   ERR_DETACH_SHM_INFOS,
   ERR_DELETE_SHM_INFOS,

   ERR_CREATE_SHM_INDICE_NOM,
   ERR_ATTACH_SHM_INDICE_NOM,
   ERR_DETACH_SHM_INDICE_NOM,
   ERR_DELETE_SHM_INDICE_NOM,

   ERR_CREATE_SHM_INDICE_VALEUR,
   ERR_ATTACH_SHM_INDICE_VALEUR,
   ERR_DETACH_SHM_INDICE_VALEUR,
   ERR_DELETE_SHM_INDICE_VALEUR,

   ERR_CREATE_SHM_INDICE_COL,
   ERR_ATTACH_SHM_INDICE_COL,
   ERR_DETACH_SHM_INDICE_COL,
   ERR_DELETE_SHM_INDICE_COL,

   ERR_CREATE_SHM_VALEUR,
   ERR_ATTACH_SHM_VALEUR,
   ERR_DETACH_SHM_VALEUR,
   ERR_DELETE_SHM_VALEUR,

   ERR_CREATE_SHM_NOM,
   ERR_ATTACH_SHM_NOM,
   ERR_DELETE_SHM_NOM,
   ERR_DETACH_SHM_NOM,

   ERR_CREATE_SHM_COL,
   ERR_ATTACH_SHM_COL,
   ERR_DELETE_SHM_COL,
   ERR_DETACH_SHM_COL
}Erreurs;


#ifdef MESSAGE_OK
static const char  *ErreursMsg[] =
{
   "",
   "Impossible de créer un nouveau processus",
   "Impossible d'éxécuter la commande",

   "Erreur de malloc",
   "Erreur de realloc",

   "Erreur de création de clé: mutex",
   "Erreur de création de clé: indice nom",
   "Erreur de création de clé: indice col",
   "Erreur de création de clé: indice valeur",
   "Erreur de création de clé: nom",
   "Erreur de création de clé: valeur",
   "Erreur de création de clé: taille",
   "Erreur de création de clé: col",

   "Impossible de créer le mutex",
   "Impossible d'initialiser le mutex",
   "Impossible de détruire le mutex",

   "Impossible de créer le shm des infos",
   "Impossible d'attacher le shm des infos",
   "Impossible de détacher le shm des infos",
   "Impossible de supprimer le shm des infos",

   "Impossible de créer le shm des indices des noms",
   "Impossible d'attacher le shm des indices des noms",
   "Impossible de détacher le shm des indices des noms",
   "Impossible de supprimer le shm des indices des noms",

   "Impossible de créer le shm des indices des valeurs",
   "Impossible d'attacher le shm des indices des valeurs",
   "Impossible de détacher le shm des indices des valeurs",
   "Impossible de supprimer le shm des indices des valeurs",

   "Impossible de créer le shm des indices des colisions",
   "Impossible d'attacher le shm des indices des colisions",
   "Impossible de détacher le shm des indices des colisions",
   "Impossible de supprimer le shm des indices des colisions",

   "Impossible de créer le shm des valeurs",
   "Impossible d'attacher le shm des valeurs",
   "Impossible de détacher le shm des valeurs",
   "Impossible de supprimer le shm des valeurs",

   "Impossible de créer le shm des noms",
   "Impossible d'attacher le shm des noms",
   "Impossible de détacher le shm des noms",
   "Impossible de supprimer le shm des noms",

   "Impossible de créer le shm des colisions",
   "Impossible d'attacher le shm des colisions",
   "Impossible de détacher le shm des colisions",
   "Impossible de supprimer le shm des colisions"
};
#undef MESSAGE_OK
#endif


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
void erreur_sys (Erreurs);



/*------------------------------------------------------------------------------
  Constantes
  ----------------------------------------------------------------------------*/


#endif
