/**
 * module : philo_nico
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
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/wait.h>



/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/
#define ETAT_MANGE  0
#define ETAT_FAIM   1
#define ETAT_PENSE  2

#define GAUCHE (i+(nb_philos-1))%nb_philos
#define DROITE (i+1)%nb_philos

#define UP 1
#define DOWN -1



/* Pid du père créateur */
pid_t pid_pere;

/* Nombre de philosophes */
int nb_philos;

/* Exclu mutuelle pr la section critique */
int mutex;

/* Tableau de sémaphores pr chacun des philosophes */
int s;

/* Etat des philosophes */
int *etat;

/* La memoire partagee par les processus philosophes */
int shm_etat;  



/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
int philosophe (int i);
void prendre_fourchettes (int i);
void poser_fourchettes (int i);
void test (int i);
void penser ();
void manger ();


/*------------------------------------------------------------------------------
  Fonctions des sémaphores
  ----------------------------------------------------------------------------*/
/**
 * operation P sur un semaphore
 */
void P (int sem, int i)
{
   static struct sembuf op;
   op.sem_op = DOWN;
   op.sem_num = i;
   semop (sem, &op, 1);
}


/**
 * operation V sur un semaphore
 */
void V (int sem, int i)
{
   static struct sembuf op;
   op.sem_op = UP;
   op.sem_num = i;
   semop (sem, &op, 1);
}


/*------------------------------------------------------------------------------
  Fonctions des philosophes
  ----------------------------------------------------------------------------*/
/**
 *
 */
int philosophe (int i)
{
   printf ("Philosophe %d pense\n", i);
   penser ();
   prendre_fourchettes (i);
   manger ();
   poser_fourchettes (i);
   exit (EXIT_SUCCESS);
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
   printf ("   --> Philosophe %d a finit de manger\n", i);
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
   sleep (1 + rand () %10);
}


/**
 *
 */
void manger ()
{
   sleep (1 + rand () %10);
}


/*------------------------------------------------------------------------------
  Autres Fonctions
  ----------------------------------------------------------------------------*/
/**
 *
 */
void delete_mem ()
{
   int i;
   /* supprimer l'ensemble de semaphores */
   for (i = 0 ; i < nb_philos ; i++)
      semctl (s, i, IPC_RMID); 

   /* supprimer l'ensemble de semaphores */
   semctl (mutex, 0, IPC_RMID);

   /* supprimer le segment de memoire */
   shmdt (&shm_etat);
   shmctl (shm_etat, IPC_RMID, 0);
}


/**
 *
 */
void handler_fin (int sig) 
{
   if (getpid() == pid_pere)
   {
      printf("delete du père ds handler\n");
      delete_mem ();
   }
}


/*------------------------------------------------------------------------------
  main
  ----------------------------------------------------------------------------*/
/**
 *
 */
int main (int argc, char *argv[])
{
   int i;
   pid_t pid_philo;

   pid_pere = getpid ();

   /* Installation du handler de signal */
   signal (SIGINT, handler_fin);

   /* Récupération du nombre de philosophes */
   if (argc > 1)
      nb_philos = atoi (argv[1]);
   else
      nb_philos = 5;

   /* Initialisation du générateur de nombres aléatoires */
   srand (pid_pere);

   /* creation du segment de memoire */
   shm_etat = shmget (IPC_PRIVATE, nb_philos * sizeof (int), 0666);

   /* Attachement du segment en memoire comme un pointeur sur entier */
   etat = (int *) shmat (shm_etat, NULL, 0);

   /* Initialisation des états des processus */
   for (i = 0 ; i < nb_philos ; i++)
      etat[i] = ETAT_PENSE;

   /* Initialisation des sémaphores de chaque philosophe ; valeur = 0*/
   s = semget (IPC_PRIVATE, nb_philos, 0666);
   if (s == -1)
   {
      perror("semget 1");
      exit (EXIT_FAILURE);
   }
   for (i = 0 ; i < nb_philos ; i++)
   {
      if ((semctl (s, i, SETVAL, 0)) == -1)
      {
         perror ("semctl 1");
         exit (EXIT_FAILURE);
      }
   }

   /* Initialisation du sémaphore mutex ; valeur = 1*/
   mutex = semget (IPC_PRIVATE, 1, 0666);
   if (mutex == -1)
   {
      perror("semget 2");
      exit (EXIT_FAILURE);
   }
   if ((semctl (mutex, 0, SETVAL, 1)) == -1)
   {
      perror ("semctl 2");
      exit (EXIT_FAILURE);
   }

   /* Creation des PHILO processus philosophes */
   for (i = 0; i < nb_philos ; i++)
   {
      if ( (pid_philo = fork ()) == -1)
         perror ("fork");
      else if (pid_philo == 0)
         philosophe (i);
   }

   /* On attend la fin de chacun des philosophes */
   for (i = 0; i < nb_philos ; i++)
      wait (NULL);

   /* On libère le segment de mémoire partagé*/
   atexit (delete_mem);

   printf("tlm a finit\n");
   exit (EXIT_SUCCESS);
}
