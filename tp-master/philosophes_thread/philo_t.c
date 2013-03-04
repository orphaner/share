/**
 * module : philo_t
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
#include <pthread.h>
#include <unistd.h>

#include "memutils.h"
#include "erreur.h"
#include "sem.h"
#include "philo_t.h"


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/
#define ETAT_MANGE  0
#define ETAT_FAIM   1
#define ETAT_PENSE  2

#define GAUCHE (i+(nb_philos-1))%nb_philos
#define DROITE (i+1)%nb_philos


/* Nombre de philosophes */
int nb_philos;

/* Exclu mutuelle pr la section critique */
pthread_mutex_t *mutex;

/* Tableau de sémaphores pr chacun des philosophes */
pthread_mutex_t *s;

/* Etat des philosophes */
int *etat;

/* Id des threads créés */
pthread_t * philos;


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
void * philosophe (void *arg);
void prendre_fourchettes (int i);
void poser_fourchettes (int i);
void test (int i);
void penser ();
void manger ();



/*------------------------------------------------------------------------------
  Fonctions des philosophes
  ----------------------------------------------------------------------------*/
/**
 *
 */
void * philosophe (void *arg)
{
   int i = (int)(int *)arg;
   printf ("Philosophe %d pense\n", i);
   penser ();
   prendre_fourchettes (i);
   manger ();
   poser_fourchettes (i);
   pthread_exit (0);
}


/**
 *
 */
void prendre_fourchettes (int i)
{
   P (mutex, 0);
   printf (" * Philosophe %d a faim\n", i);
   etat[i] = ETAT_FAIM;
   test (i);
   V (mutex, 0);
   P (s, i);
}


/**
 *
 */
void poser_fourchettes (int i)
{
   P (mutex, 0);
   printf ("      --> Philosophe %d a finit de manger\n", i);
   etat [i] = ETAT_PENSE;
   test (GAUCHE);
   test (DROITE);
   V (mutex, 0);
}


/**
 *
 */
void test (int i)
{
   if (etat[i] == ETAT_FAIM && etat[GAUCHE] != ETAT_MANGE && etat[DROITE] != ETAT_MANGE)
   {
      printf ("   --> Philosophe %d mange\n", i);
      etat [i] = ETAT_MANGE;
      V (s, i);
   }
}


/**
 *
 */
void penser ()
{
   sleep (1 + rand () %5);
}


/**
 *
 */
void manger ()
{
   sleep (1 + rand () %20);
}


/*------------------------------------------------------------------------------
  main
  ----------------------------------------------------------------------------*/
void delete_mem ()
{
   free (etat);
   free (mutex);
   free (s);
   free (philos);
}


/**
 *
 */
int main (int argc, char *argv[])
{
   int i;
   int retour;

   srand (getpid());
   atexit (delete_mem);


   /* Récupération du nombre de philosophes */
   if (argc > 1)
      nb_philos = atoi (argv[1]);
   else
      nb_philos = 5;

   etat = my_malloc (nb_philos * sizeof (int));
   mutex = my_malloc (sizeof (pthread_mutex_t));
   s = my_malloc (nb_philos * sizeof (pthread_mutex_t));
   philos = my_malloc (nb_philos * sizeof (pthread_t));


   pthread_mutex_init (&mutex[0], NULL);

   for (i = 0 ; i < nb_philos ; i++)
   {
      pthread_mutex_init (&s[i], NULL);
      P (s, i);
   }


   /* Initialisation des états des philosohpes */
   for (i = 0 ; i < nb_philos ; i++)
      etat[i] = ETAT_PENSE;


   /* Création des threads */
   for (i = 0 ; i < nb_philos ; i++)
      if (pthread_create (&philos[i], NULL, philosophe, (void *) i) < 0)
         erreur_sys (ERR_PTHREAD_CREATE);
   

   /* Code de retour des threads */
   for (i = 0 ; i < nb_philos ; i++)
      if (pthread_join (philos[i], &retour) != 0)
         erreur_sys (ERR_PTHREAD_JOIN);


   exit (EXIT_SUCCESS);
}
