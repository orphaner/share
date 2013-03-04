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
  Fichiers include
  ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#define MESSAGE_OK
#include "erreur.h"


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/**
 *
 */
void erreur_sys (Erreurs code_retour)
{
   perror (ErreursMsg[code_retour]);
   exit (code_retour);
}
