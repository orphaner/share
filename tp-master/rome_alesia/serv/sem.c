/**
 * module : semapore
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
#include <pthread.h>
#include "erreur.h"
#include "semaphore.h"


/*------------------------------------------------------------------------------
  Fonctions des sémaphores
  ----------------------------------------------------------------------------*/
/**
 * operation P sur un semaphore
 */
void P (pthread_mutex_t sem)
{
   pthread_mutex_lock (&sem);
}


/**
 * operation V sur un semaphore
 */
void V (pthread_mutex_t sem)
{
   pthread_mutex_unlock (&sem);
}
