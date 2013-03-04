/**
 * module : erreur
 * brief :
 * author : Nicolas LASSALLE
 * date :
 * bug : Aucun d�tect� � ce jour
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
   "Impossible de cr�er un nouveau processus",
   "Impossible d'�x�cuter la commande",

   "Erreur de malloc",
   "Erreur de realloc",

   "Erreur de cr�ation de cl�: mutex",
   "Erreur de cr�ation de cl�: indice nom",
   "Erreur de cr�ation de cl�: indice col",
   "Erreur de cr�ation de cl�: indice valeur",
   "Erreur de cr�ation de cl�: nom",
   "Erreur de cr�ation de cl�: valeur",
   "Erreur de cr�ation de cl�: taille",
   "Erreur de cr�ation de cl�: col",

   "Impossible de cr�er le mutex",
   "Impossible d'initialiser le mutex",
   "Impossible de d�truire le mutex",

   "Impossible de cr�er le shm des infos",
   "Impossible d'attacher le shm des infos",
   "Impossible de d�tacher le shm des infos",
   "Impossible de supprimer le shm des infos",

   "Impossible de cr�er le shm des indices des noms",
   "Impossible d'attacher le shm des indices des noms",
   "Impossible de d�tacher le shm des indices des noms",
   "Impossible de supprimer le shm des indices des noms",

   "Impossible de cr�er le shm des indices des valeurs",
   "Impossible d'attacher le shm des indices des valeurs",
   "Impossible de d�tacher le shm des indices des valeurs",
   "Impossible de supprimer le shm des indices des valeurs",

   "Impossible de cr�er le shm des indices des colisions",
   "Impossible d'attacher le shm des indices des colisions",
   "Impossible de d�tacher le shm des indices des colisions",
   "Impossible de supprimer le shm des indices des colisions",

   "Impossible de cr�er le shm des valeurs",
   "Impossible d'attacher le shm des valeurs",
   "Impossible de d�tacher le shm des valeurs",
   "Impossible de supprimer le shm des valeurs",

   "Impossible de cr�er le shm des noms",
   "Impossible d'attacher le shm des noms",
   "Impossible de d�tacher le shm des noms",
   "Impossible de supprimer le shm des noms",

   "Impossible de cr�er le shm des colisions",
   "Impossible d'attacher le shm des colisions",
   "Impossible de d�tacher le shm des colisions",
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
