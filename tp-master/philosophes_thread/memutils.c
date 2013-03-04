/**
 * module : memutils
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
void * my_realloc (void * ptr, size_t const size)
{
   void * r = realloc (ptr, size);
   if (r == NULL)
      erreur_sys (ERR_REALLOC);
   return r;
}


/**
 *
 */
void *my_malloc (size_t const size)
{
   void * m = malloc (size);
   if (m == NULL)
      erreur_sys (ERR_MALLOC);
   return m;
}
