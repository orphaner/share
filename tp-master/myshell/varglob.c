/**
 * module : varglob
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
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "hash.h"
#include "varloc.h"
#include "varglob.h"
#include "erreur.h"
#include "common.h"
#include "dyn_string.h"
#include "memutils.h"


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/
/* Opérations sur sémaphores */
#define UP 1
#define DOWN -1

/* Taille de base de la table de hash*/
#define MAX_VAR_INIT 1009

/* Buffer de réallocation */
#define BUFFER_NOM    10240
#define BUFFER_VALEUR 10240
#define BUFFER_COL    (100 * 3)

/* Macros d'accès aux tailles des mémoires partagées */
#define TAILLE_INDICE infos_g[0]
#define TAILLE_NOM    infos_g[1]
#define TAILLE_VALEUR infos_g[2]
#define TAILLE_COL    infos_g[3]

#define NEXT_INDICE_NOM    infos_g[4]
#define NEXT_INDICE_VALEUR infos_g[5]

#define NB_MYSHELL_RUNNING infos_g[6]

/* Taille des mémoires partagées à l'initialisation pour créer les IPC */
#define IPC_TAILLE_INDICE_INIT sizeof (int)  * MAX_VAR_INIT
#define IPC_TAILLE_NOM_INIT    sizeof (char) * BUFFER_NOM
#define IPC_TAILLE_VALEUR_INIT sizeof (char) * BUFFER_VALEUR
#define IPC_TAILLE_COL_INIT    sizeof (int)  * BUFFER_COL
#define IPC_TAILLE_TAILLE      sizeof (int)  * 7

/* Taille des mémoires partagées en cours d'éxécution pr récupérer les IPC */
#define IPC_TAILLE_INDICE sizeof (int)  * TAILLE_INDICE
#define IPC_TAILLE_NOM    sizeof (char) * TAILLE_NOM
#define IPC_TAILLE_VALEUR sizeof (char) * TAILLE_VALEUR
#define IPC_TAILLE_COL    sizeof (int)  * TAILLE_COL

/* Accesseurs aux infos de la liste chainée des colisions */
#define COL_NOM(indice)     (col_g[(indice)])
#define COL_VALEUR(indice)  (col_g[(indice)+1])
#define COL_SUIVANT(indice) (col_g[(indice)+2])

/* Pour choper les clés */
#define KEY_MUTEX         'm'
#define KEY_INDICE_NOM    'i'
#define KEY_INDICE_VALEUR 't'
#define KEY_INDICE_COL    'c'
#define KEY_NOM           'h'
#define KEY_VALEUR        'e'
#define KEY_TAILLE        'l'
#define KEY_COL           'a'
#define KEY_PATH          "/usr/bin/less"
#define MASK_IPC 0674


/* Exclu mutuelle pr la section critique */
int mutex;

/* Contenu shm */
int  *indice_valeur_g;
int  *indice_nom_g;
int  *indice_col_g;
char *valeur_g;
char *nom_g;
int  *col_g;
int  *infos_g;

/* Id shm */
int shm_indice_valeur;
int shm_indice_nom;
int shm_indice_col;
int shm_valeur;
int shm_nom;
int shm_col;
int shm_infos;

/* Clés shm */
key_t mutex_k;
key_t indice_valeur_k;
key_t indice_nom_k;
key_t indice_col_k;
key_t valeur_k;
key_t nom_k;
key_t infos_k;
key_t col_k;

/* Tailles locales, permet de "réattacher" les segments si modif */
int taille_nom_l;
int taille_valeur_l;
int taille_col_l;

/* Les vraies variables d'environement */
extern char **environ;


/*------------------------------------------------------------------------------
  Protoypes de fonctions
  ----------------------------------------------------------------------------*/
void varglob_add_col_init (unsigned int cle);
int nom_egal (int indice_nom, char * nom, int taille_nom);
int varglob_cherche_liste (int cle, char * nom);



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
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/**
 * Libère le mutex !!! puis apelle erreur_sys classique.
 * param err : l'erreur à générer
 */
void varglob_erreur_sys (Erreurs err)
{
   V (mutex, 0);
   erreur_sys (err);
}


/**
 * Redimensionne le shm des noms si il est trop petit.
 */
void resize_nom ()
{
   char * copy = NULL;
   int taille_ini = TAILLE_NOM;

#ifdef DEBUG
   printf("Resize_nom\n");
#endif

   /* Copie locale */
   copy = my_malloc (sizeof (char) * taille_ini);
   memcpy (copy, nom_g, sizeof (char) * taille_ini);

   /* Augmentation taille */
   TAILLE_NOM = TAILLE_NOM + BUFFER_NOM;
   taille_nom_l = TAILLE_NOM;

   /* Suppression ancien shm */
   if (shmdt (nom_g) == -1)
      varglob_erreur_sys (ERR_DETACH_SHM_NOM);
   if (shmctl (shm_nom, IPC_RMID, NULL) == -1)
      varglob_erreur_sys (ERR_DETACH_SHM_NOM);

   /* Création du shm + grand */
   if ( (shm_nom = shmget (nom_k, IPC_TAILLE_NOM, IPC_CREAT | MASK_IPC)) == -1)
      varglob_erreur_sys (ERR_CREATE_SHM_NOM);

   if ( (nom_g = (char *) shmat (shm_nom, NULL, 0)) == NULL)
      varglob_erreur_sys (ERR_ATTACH_SHM_NOM);

   /* Copie locale dans nouveau shm */
   memcpy (nom_g, copy, sizeof (char) * taille_ini);

   free (copy);
}


/**
 * Redimensionne le shm des valeurs si il est trop petit.
 */
void resize_valeur ()
{
   char * copy = NULL;
   int taille_ini = TAILLE_VALEUR;

#ifdef DEBUG
   printf("Resize_valeur\n");
#endif

   /* Copie locale */
   copy = my_malloc (sizeof (char) * taille_ini);
   memcpy (copy, valeur_g, sizeof (char) * taille_ini);

   /* Augmentation taille */
   TAILLE_VALEUR = TAILLE_VALEUR + BUFFER_VALEUR;
   taille_valeur_l = TAILLE_VALEUR;

   /* Suppression ancien shm */
   if (shmdt (valeur_g) == -1)
      varglob_erreur_sys (ERR_DETACH_SHM_VALEUR);

   if (shmctl (shm_valeur, IPC_RMID, NULL) == -1)
      varglob_erreur_sys (ERR_DETACH_SHM_VALEUR);

   /* Création du shm + grand */
   if ( (shm_valeur = shmget (valeur_k, IPC_TAILLE_VALEUR, IPC_CREAT | MASK_IPC)) == -1)
      varglob_erreur_sys (ERR_CREATE_SHM_VALEUR);

   if ( (valeur_g = (char *) shmat (shm_valeur, NULL, 0)) == NULL)
      varglob_erreur_sys (ERR_ATTACH_SHM_VALEUR);

   /* Copie locale dans nouveau shm */
   memcpy (valeur_g, copy, sizeof (char) * taille_ini);

   free (copy);
}


/**
 * Redimensionne le shm des listes des colisions si il est trop petit.
 */
void resize_col ()
{
   int * copy = NULL;
   int taille_ini = TAILLE_COL;

#ifdef DEBUG
   printf("Resize_col\n");
#endif

   /* Copie locale */
   copy = my_malloc (sizeof (int) * taille_ini);
   memcpy (copy, col_g, sizeof (int) * taille_ini);

   /* Augmentation taille */
   TAILLE_COL = TAILLE_COL + BUFFER_COL;
   taille_col_l = TAILLE_COL;

   /* Suppression ancien shm */
   if (shmdt (col_g) == -1)
      varglob_erreur_sys (ERR_DETACH_SHM_COL);

   if (shmctl (shm_col, IPC_RMID, NULL) == -1)
      varglob_erreur_sys (ERR_DETACH_SHM_COL);

   /* Création du shm + grand */
   if ( (shm_col = shmget (col_k, IPC_TAILLE_COL, IPC_CREAT | MASK_IPC)) == -1)
      varglob_erreur_sys (ERR_CREATE_SHM_COL);

   if ( (col_g = (int *) shmat (shm_col, NULL, 0)) == NULL)
      varglob_erreur_sys (ERR_ATTACH_SHM_COL);

   /* Copie locale dans nouveau shm */
   memcpy (col_g, copy, sizeof (int) * taille_ini);

   for (; taille_ini < TAILLE_COL ; taille_ini++)
      col_g[taille_ini] = -1;

   free (copy);
}


/*------------------------------------------------------------------------------
  Remplissage initial de la table
  ----------------------------------------------------------------------------*/
/**
 * Remplit la table de hash des variables globales
 * Appelé une seule fois par le premier myshell
 */
void varglob_remplir ()
{
   char ** env;
   char *c;
   char var[1024];
   char * valeur;
   int taille;
   int j;
   int indice_nom = 0;
   int indice_valeur = 0;
   unsigned int cle;


   /* Parcours de toutes les variables d'environement */
   for (env = environ ; *env != NULL ; ++env)
   {
      for (c = *env, taille = 0 ; *c != '=' ; *c++, taille++)
         var[taille] = *c;
      var [taille++] = '\0';

      /* Table de hash correcte (ie pas de collision) !! */
      cle = hash2 (var, TAILLE_INDICE);
#ifdef DEBUG
      printf("%d : %s\n", cle, var);
#endif
      if (indice_nom_g[cle] != -1)
      {
#ifdef DEBUG
         printf("Colision dans varglob_remplir\n");
#endif
         varglob_add_col_init (cle);
      }

      /* Insertion hashée direct */
      else
      {
         indice_nom_g[cle] = indice_nom;
         indice_valeur_g[cle] = indice_valeur;
      }

      /* Copie du nom */
      while (indice_nom + taille + 1 > TAILLE_NOM)
         resize_nom ();

      for (j = 0 ; j < taille ; j++, indice_nom++)
         nom_g[indice_nom] = var[j];
      nom_g[indice_nom++] = '\0';


      /* Copie de la valeur */
      valeur = getenv (var);
      taille = strlen (valeur);

      while (indice_valeur + taille + 1 > TAILLE_VALEUR)
         resize_valeur ();

      for (j = 0 ; j < taille ; j++, indice_valeur++)
         valeur_g[indice_valeur] = valeur[j];
      valeur_g[indice_valeur++] = '\0';

      NEXT_INDICE_NOM = indice_nom;
      NEXT_INDICE_VALEUR = indice_valeur;
   }
}


/**
 * Retourne l'indice d'un emplacement vide ds le shm qui contient
 * la liste chainées des colisions.
 * La liste est agrandie si besoin est
 * return : indice appartenant col_g
 */
int varglob_find_free_space_col ()
{
   int i=0;
   while (i < TAILLE_COL && col_g[i] != -1)
      i += 3;

   if (i == TAILLE_COL)
      resize_col ();
   return i;
}


/**
 * Effectue le chainage pour l'ajout d'une variable globale en colision
 * dans la case "cle" de la table de hash
 * /!\ le nom et la valeur ne sont pas recopiés, seul le chainage est effectué !!!
 * param cle : la clé dans la table de hash de la variable globale
 */
void varglob_add_col_init (unsigned int cle)
{
   int indice_new_col = varglob_find_free_space_col();
   int deb = indice_col_g[cle];
   int i;

#ifdef DEBUG
   printf("Chainage d'un maillon pour ajout en collision!!\n");
#endif

   /* Création du maillon */
   COL_NOM(indice_new_col) = NEXT_INDICE_NOM;
   COL_VALEUR(indice_new_col) = NEXT_INDICE_VALEUR;
   COL_SUIVANT(indice_new_col) = -1;

   /* Insertion en tête */
   if (deb == -1)
   {
      indice_col_g[cle] = indice_new_col;
   }
   /* Insertion en dernière place */
   else
   {
      i = deb;
      while (COL_SUIVANT(i) != -1)
         i = COL_SUIVANT(i);
      COL_SUIVANT(i) = indice_new_col;
   }
}


/*------------------------------------------------------------------------------
  Ajout / Suppr / Modif des variables dans les listes chainés de colisions
  ----------------------------------------------------------------------------*/
/**
 * Modifie une variable globale stockée dans une liste chainée
 * param * valeur :  valeur de la variable globale
 * param indice_col : l'indice du maillon de la liste chainée qui contient la variable
 */
void varglob_modif_col (char * valeur, int indice_col)
{
   int indice_valeur;
   int i;
   int cpt = 0;
   int taille;

#ifdef DEBUG
   printf("Modification d'une variable dans collision\n");
#endif

   taille = strlen (valeur);
   indice_valeur = COL_VALEUR(indice_col);

   i = indice_valeur;
   while (valeur_g[i++] != '\0')
      cpt++;

   if (cpt < taille)
   {
#ifdef DEBUG
      printf("trop grand\n");
#endif
      indice_valeur = NEXT_INDICE_VALEUR;
      COL_VALEUR(indice_col) = indice_valeur;
      NEXT_INDICE_VALEUR += taille + 1;
      while (indice_valeur + taille + 1 > TAILLE_VALEUR)
         resize_valeur ();
   }

   for (i = 0 ; i < taille ; indice_valeur++, i++)
      valeur_g[indice_valeur] = valeur[i];
   valeur_g[indice_valeur] = '\0';
}


/**
 * Ajoute une variable globale dans la liste chainée de "cle"
 * param * nom : nom de la variable globale
 * param * valeur : valeur de la variable globale
 * param cle : clé dans la table de hash
 * param taille_nom : la longueur du nom
 */
void varglob_add_col (char * nom, char * valeur, unsigned int cle, int taille_nom)
{
   int taille;
   int j;

#ifdef DEBUG
   printf("Ajout d'une variable dans les colisions\n");
#endif

   varglob_add_col_init (cle);

   /* Copie du nom */
   while (NEXT_INDICE_NOM + taille_nom + 1 > TAILLE_NOM)
      resize_nom ();

   for (j = 0 ; j < taille_nom ; j++, NEXT_INDICE_NOM++)
      nom_g[NEXT_INDICE_NOM] = nom[j];
   nom_g[NEXT_INDICE_NOM++] = '\0';


   /* Copie de la valeur */
   taille = strlen (valeur);

   while (NEXT_INDICE_VALEUR + taille + 1 > TAILLE_VALEUR)
      resize_valeur ();

   for (j = 0 ; j < taille ; j++, NEXT_INDICE_VALEUR++)
      valeur_g[NEXT_INDICE_VALEUR] = valeur[j];
   valeur_g[NEXT_INDICE_VALEUR++] = '\0';
}


/**
 * Traite (ajout/suppr/modif) une variable globale dans
 * la liste chainée de "cle"
 * param cle : la clé dans la table de hash de la variable globale
 * param * nom : nom de la variable globale
 * param * valeur : valeur de la variable globale
 */
void varglob_set_col (unsigned int cle, char * nom, char * valeur)
{
   int start = indice_col_g[cle];
   int prev, tmp;
   int taille_nom = strlen(nom);
   int trouve=0;

   /* Suppresion d'une variable dans les colisions */
   if (valeur == NULL)
   {
      tmp = start;
      /* Suppression en tete */
      if (nom_egal (COL_NOM(tmp), nom, taille_nom))
      {
#ifdef DEBUG
         printf("Suppression en tete dans les colisions\n");
#endif
         indice_col_g[cle] = COL_SUIVANT(tmp);
         COL_NOM(tmp) = -1;
         COL_VALEUR(tmp) = -1;
         COL_SUIVANT(tmp) = -1;
      }
      /* Suppression ailleurs dans la liste */
      else
      {
         prev = tmp;
         while (!trouve && COL_SUIVANT(tmp) != -1)
         {
            if (nom_egal (COL_NOM(tmp), nom, taille_nom))
               trouve = 1;
            else
            {
               prev = tmp;
               tmp = COL_SUIVANT(tmp);
            }
         }
         if (nom_egal (COL_NOM(tmp), nom, taille_nom))
            trouve = 1;
         if (trouve)
         {

#ifdef DEBUG
            printf("Suppression ailleurs dans les colisions\n");
#endif
            COL_SUIVANT(prev) = COL_SUIVANT(tmp);
            COL_NOM(tmp) = -1;
            COL_VALEUR(tmp) = -1;
            COL_SUIVANT(tmp) = -1;
         }
      }
   }

   /* Ajout ou modif */
   else
   {
      tmp = start;

      /* Modification en tete */
      if (nom_egal (COL_NOM(tmp), nom, taille_nom))
      {
         varglob_modif_col (valeur, tmp);
      }

      else
      {
         prev = tmp;
         while (!trouve && COL_SUIVANT(tmp) != -1)
         {
            if (nom_egal (COL_NOM(tmp), nom, taille_nom))
               trouve = 1;
            else
            {
               prev = tmp;
               tmp = COL_SUIVANT(tmp);
            }
         }
         if (nom_egal (COL_NOM(tmp), nom, taille_nom))
            trouve = 1;
         /* Modification */
         if (trouve)
         {
            varglob_modif_col (valeur, tmp);
         }
         /* Ajout */
         else
         {
            varglob_add_col (nom, valeur, cle, taille_nom);
         }
      }
   }
}


/*------------------------------------------------------------------------------
  Ajout / Suppr / Modif des variables en hashage direct
  ----------------------------------------------------------------------------*/
/**
 * Ajoute une variable globale
 * param * nom : nom de la variable globale
 * param * valeur : la nouvelle valeur de la variable globale
 * param cle : la clé dans la table de hash de la variable globale
 */
void varglob_add (char * nom, char * valeur, unsigned int cle)
{
   int taille;
   int j;

#ifdef DEBUG
   printf("Ajout d'une variable\n");
#endif

   /* Copie du nom */
   indice_nom_g[cle] = NEXT_INDICE_NOM;
   taille = strlen (nom);

   while (NEXT_INDICE_NOM + taille + 1 > TAILLE_NOM)
      resize_nom ();

   for (j = 0 ; j < taille ; j++, NEXT_INDICE_NOM++)
      nom_g[NEXT_INDICE_NOM] = nom[j];
   nom_g[NEXT_INDICE_NOM++] = '\0';


   /* Copie du nom */
   indice_valeur_g[cle] = NEXT_INDICE_VALEUR;
   taille = strlen (valeur);

   while (NEXT_INDICE_VALEUR + taille + 1 > TAILLE_VALEUR)
      resize_valeur ();

   for (j = 0 ; j < taille ; j++, NEXT_INDICE_VALEUR++)
      valeur_g[NEXT_INDICE_VALEUR] = valeur[j];
   valeur_g[NEXT_INDICE_VALEUR++] = '\0';
}


/**
 * Supprime une variable globale
 * param cle : la clé dans la table de hash de la variable globale
 */
void varglob_suppr (unsigned int cle)
{
#ifdef DEBUG
   printf("Suppression d'une variable\n");
#endif
   indice_nom_g[cle] = -1;
   indice_valeur_g[cle] = -1;
}


/**
 * Modifie une variable globale
 * param * valeur : la nouvelle valeur de la variable globale
 * param cle : la clé dans la table de hash de la variable globale
 */
void varglob_modif (char * valeur, unsigned int cle)
{
   int indice_valeur;
   int i;
   int cpt = 0;
   int taille;

#ifdef DEBUG
   printf("Modification d'une variable\n");
#endif

   taille = strlen (valeur);
   indice_valeur = indice_valeur_g[cle];

   i = indice_valeur;
   while (valeur_g[i++] != '\0')
      cpt++;

   if (cpt < taille)
   {
      indice_valeur = NEXT_INDICE_VALEUR;
      indice_valeur_g[cle] = indice_valeur;
      NEXT_INDICE_VALEUR += taille + 1;
      while (indice_valeur + taille + 1 > TAILLE_VALEUR)
         resize_valeur ();
   }

   for (i = 0 ; i < taille ; indice_valeur++, i++)
      valeur_g[indice_valeur] = valeur[i];
   valeur_g[indice_valeur] = '\0';
}


/**
 * Permet d'ajout, de modifier ou de supprimer une variable globale
 * param * nom : nom de la variable globale
 * param * valeur : valeur de la variable globale
 */
void varglob_set (char * nom, char * valeur)
{
   unsigned int cle;
   int i, j;
   char var[1024];

   cle = hash2 (nom, TAILLE_INDICE);

   /* Ajout de variable*/
   if (indice_nom_g[cle] == -1)
   {
      varglob_add (nom, valeur, cle);
   }
   else
   {
      i = indice_nom_g[cle];
      j = 0;
      while (nom_g[i] != '\0')
         var[j++] = nom_g[i++];
      var[j++] = '\0';

      /* Modification || Suppression de valeur (ie, même clé et même nom de var) */
      if (!strcmp (var, nom))
      {
         if (valeur == NULL)
            varglob_suppr (cle);
         else
            varglob_modif (valeur, cle);
      }

      /* Colision c'est la merde !! */
      else
      {
#ifdef DEBUG
         printf("Colision dans varglob_set\n");
#endif
         varglob_set_col (cle, nom, valeur);
      }
   }
}


/**
 * Vérifie si un segment de mémoire a été redimensioné.
 * Si redimensionné, on détache puis on rattache
 */
void varglob_verif_no_change ()
{
   if (taille_nom_l < TAILLE_NOM)
   {
#ifdef DEBUG
      printf("taille nom à changer\n");
#endif
      taille_nom_l = TAILLE_NOM;
      if (shmdt (nom_g) == -1)
         varglob_erreur_sys (ERR_DETACH_SHM_NOM);
      if ( (shm_nom = shmget (nom_k, IPC_TAILLE_NOM, 0)) == -1 )
         varglob_erreur_sys (ERR_CREATE_SHM_NOM);
      if ( (nom_g = (char *) shmat (shm_nom, NULL, 0)) == NULL)
         varglob_erreur_sys (ERR_ATTACH_SHM_NOM);
   }
   if (taille_valeur_l < TAILLE_VALEUR)
   {
#ifdef DEBUG
      printf("taille valeur à changer\n");
#endif
      taille_valeur_l = TAILLE_VALEUR;
      if (shmdt (valeur_g) == -1)
         varglob_erreur_sys (ERR_DETACH_SHM_VALEUR);
      if ( (shm_valeur = shmget (valeur_k, IPC_TAILLE_VALEUR, 0)) == -1 )
         varglob_erreur_sys (ERR_CREATE_SHM_VALEUR);
      if ( (valeur_g = (char *) shmat (shm_valeur, NULL, 0)) == NULL)
         varglob_erreur_sys (ERR_ATTACH_SHM_VALEUR);
   }
   if (taille_col_l < TAILLE_COL)
   {
#ifdef DEBUG
      printf("taille col à changer\n");
#endif
      taille_col_l = TAILLE_COL;
      if (shmdt (col_g) == -1)
         varglob_erreur_sys (ERR_DETACH_SHM_COL);
      if ( (shm_col = shmget (col_k, IPC_TAILLE_COL, 0)) == -1 )
         varglob_erreur_sys (ERR_CREATE_SHM_COL);
      if ( (col_g = (int *) shmat (shm_col, NULL, 0)) == NULL)
         varglob_erreur_sys (ERR_ATTACH_SHM_COL);
   }
}


/**
 * Initialise les variables globales
 */
void varglob_init ()
{
   int i = 0;


   /*-----------------------------------------------*/
   /* Initialisation des clés                       */
   /*-----------------------------------------------*/
   if ( (mutex_k = ftok (KEY_PATH, KEY_MUTEX)) == -1)
      erreur_sys (ERR_KEY_MUTEX);
#ifdef DEBUG
   printf("cle mutex : %d\n",mutex_k);
#endif

   if ( (indice_nom_k = ftok (KEY_PATH, KEY_INDICE_NOM)) == -1)
      erreur_sys (ERR_KEY_INDICE_NOM);

   if ( (indice_col_k = ftok (KEY_PATH, KEY_INDICE_COL)) == -1)
      erreur_sys (ERR_KEY_INDICE_COL);

   if ( (indice_valeur_k = ftok (KEY_PATH, KEY_INDICE_VALEUR)) == -1)
      erreur_sys (ERR_KEY_INDICE_VALEUR);

   if ( (nom_k = ftok (KEY_PATH, KEY_NOM)) == -1)
      erreur_sys (ERR_KEY_NOM);

   if ( (valeur_k = ftok (KEY_PATH, KEY_VALEUR)) == -1)
      erreur_sys (ERR_KEY_VALEUR);

   if ( (infos_k = ftok (KEY_PATH, KEY_TAILLE)) == -1)
      erreur_sys (ERR_KEY_TAILLE);

   if ( (col_k = ftok (KEY_PATH, KEY_COL)) == -1)
      erreur_sys (ERR_KEY_COL);


   /*-----------------------------------------------*/
   /* Initialisation du sémaphore mutex ; valeur = 1*/
   /*-----------------------------------------------*/
   if ( (mutex = semget (mutex_k, 1, IPC_CREAT | IPC_EXCL | MASK_IPC)) == -1)
   {
      if ( (mutex = semget (mutex_k, 1, 0)) == -1)
         erreur_sys (ERR_CREATE_MUTEX);
   }
   else
   {
      semctl (mutex, 0, SETVAL, 1);
#ifdef DEBUG
      printf("inittttt sémaphore\n");
#endif
      if (mutex == -1)
         erreur_sys (ERR_INIT_MUTEX);
   }

   P (mutex, 0);

   /*-----------------------------------------------*/
   /* shm : taille                                  */
   /*-----------------------------------------------*/
   if ( (shm_infos = shmget (infos_k, IPC_TAILLE_TAILLE, IPC_CREAT | MASK_IPC)) == -1)
   {
      i = 1;
      if ( (shm_infos = shmget (infos_k, IPC_TAILLE_TAILLE, 0)) == -1 )
         varglob_erreur_sys (ERR_CREATE_SHM_INFOS);
   }
   if ( (infos_g = (int *) shmat (shm_infos, NULL, 0)) == NULL)
      varglob_erreur_sys (ERR_ATTACH_SHM_INFOS);

   /*-----------------------------------------------*/
   /* shm : indice_nom                              */
   /*-----------------------------------------------*/
   if ( (shm_indice_nom = shmget (indice_nom_k, IPC_TAILLE_INDICE_INIT, IPC_CREAT | MASK_IPC)) == -1)
   {
      if ( (shm_indice_nom = shmget (indice_nom_k, IPC_TAILLE_INDICE, 0)) == -1 )
         varglob_erreur_sys (ERR_CREATE_SHM_INDICE_NOM);
   }
   if ( (indice_nom_g = (int *) shmat (shm_indice_nom, NULL, 0)) == NULL)
      varglob_erreur_sys (ERR_ATTACH_SHM_INDICE_NOM);


   /*-----------------------------------------------*/
   /* shm : indice_valeur                           */
   /*-----------------------------------------------*/
   if ( (shm_indice_valeur = shmget (indice_valeur_k, IPC_TAILLE_INDICE_INIT, IPC_CREAT | MASK_IPC)) == -1)
   {
      if ( (shm_indice_valeur = shmget (indice_valeur_k, IPC_TAILLE_INDICE, 0)) == -1 )
         varglob_erreur_sys (ERR_CREATE_SHM_INDICE_VALEUR);
   }
   if ( (indice_valeur_g = (int *) shmat (shm_indice_valeur, NULL, 0)) == NULL)
      varglob_erreur_sys (ERR_ATTACH_SHM_INDICE_VALEUR);


   /*-----------------------------------------------*/
   /* shm : indice_colision                         */
   /*-----------------------------------------------*/
   if ( (shm_indice_col = shmget (indice_col_k, IPC_TAILLE_INDICE_INIT, IPC_CREAT | MASK_IPC)) == -1)
   {
      if ( (shm_indice_col = shmget (indice_col_k, IPC_TAILLE_INDICE, 0)) == -1 )
         varglob_erreur_sys (ERR_CREATE_SHM_INDICE_COL);
   }
   if ( (indice_col_g = (int *) shmat (shm_indice_col, NULL, 0)) == NULL)
      varglob_erreur_sys (ERR_ATTACH_SHM_INDICE_COL);


   /*-----------------------------------------------*/
   /* shm : valeur                                  */
   /*-----------------------------------------------*/
   if ( (shm_valeur = shmget (valeur_k, IPC_TAILLE_VALEUR_INIT, IPC_CREAT | MASK_IPC)) == -1)
   {
      if ( (shm_valeur = shmget (valeur_k, IPC_TAILLE_VALEUR, 0)) == -1 )
         varglob_erreur_sys (ERR_CREATE_SHM_VALEUR);
   }
   if ( (valeur_g = (char *) shmat (shm_valeur, NULL, 0)) == NULL)
      varglob_erreur_sys (ERR_ATTACH_SHM_VALEUR);


   /*-----------------------------------------------*/
   /* shm : nom                                     */
   /*-----------------------------------------------*/
   if ( (shm_nom = shmget (nom_k, IPC_TAILLE_NOM_INIT, IPC_CREAT | MASK_IPC)) == -1)
   {
      if ( (shm_nom = shmget (nom_k, IPC_TAILLE_NOM, 0)) == -1 )
         varglob_erreur_sys (ERR_CREATE_SHM_NOM);
   }
   if ( (nom_g = (char *) shmat (shm_nom, NULL, 0)) == NULL)
      varglob_erreur_sys (ERR_ATTACH_SHM_NOM);


   /*-----------------------------------------------*/
   /* shm : col                                     */
   /*-----------------------------------------------*/
   if ( (shm_col = shmget (col_k, IPC_TAILLE_COL_INIT, IPC_CREAT | MASK_IPC)) == -1)
   {
      if ( (shm_col = shmget (col_k, IPC_TAILLE_COL, 0)) == -1 )
         varglob_erreur_sys (ERR_CREATE_SHM_COL);
   }
   if ( (col_g = (int *) shmat (shm_col, NULL, 0)) == NULL)
      varglob_erreur_sys (ERR_ATTACH_SHM_COL);

   /* Premier shell lancé */
   if (i == 0)
   {
#ifdef DEBUG
      printf("inittttt la table\n");
#endif

      TAILLE_INDICE = MAX_VAR_INIT;
      TAILLE_NOM    = BUFFER_NOM;
      TAILLE_VALEUR = BUFFER_VALEUR;
      TAILLE_COL    = BUFFER_COL;

      for (i = 0 ; i < MAX_VAR_INIT ; i++)
      {
         indice_nom_g[i] = -1;
         indice_valeur_g[i] = -1;
         indice_col_g[i] = -1;
      }
      for (i = 0 ; i < BUFFER_COL ; i++)
         col_g[i] = -1;

      NB_MYSHELL_RUNNING = 1;
      varglob_remplir ();
   }
   else
   {
      NB_MYSHELL_RUNNING++;
#ifdef DEBUG
      printf("myshell++\n");
#endif
   }

   taille_nom_l = TAILLE_NOM;
   taille_valeur_l = TAILLE_VALEUR;
   taille_col_l = TAILLE_COL;

#ifdef DEBUG
   printf("taille_nom_l: %d\n",taille_nom_l);
   printf("taille_valeur_l: %d\n",taille_valeur_l);
#endif

   V (mutex, 0);
}


/**
 * Supprime tous les ipc's déclarés si le myshell courant est le dernier
 * Décrémente le nombre de myshell lancés sinon.
 */
void varglob_free ()
{
   if (NB_MYSHELL_RUNNING == 1)
   {
#ifdef DEBUG
      printf("Libération des IPC's\n");
#endif
      if (shmdt (infos_g) == -1)
         erreur_sys (ERR_DETACH_SHM_INFOS);
      if (shmctl (shm_infos, IPC_RMID, NULL) == -1)
         erreur_sys (ERR_DELETE_SHM_INFOS);

      if (shmdt (indice_nom_g) == -1)
         erreur_sys (ERR_DETACH_SHM_INDICE_NOM);
      if (shmctl (shm_indice_nom, IPC_RMID, NULL) == -1)
         erreur_sys (ERR_DELETE_SHM_INDICE_NOM);

      if (shmdt (indice_col_g) == -1)
         erreur_sys (ERR_DETACH_SHM_INDICE_COL);
      if (shmctl (shm_indice_col, IPC_RMID, NULL) == -1)
         erreur_sys (ERR_DELETE_SHM_INDICE_COL);

      if (shmdt (indice_valeur_g) == -1)
         erreur_sys (ERR_DETACH_SHM_INDICE_VALEUR);
      if (shmctl (shm_indice_valeur, IPC_RMID, NULL) == -1)
         erreur_sys (ERR_DELETE_SHM_INDICE_VALEUR);

      if (shmdt (nom_g) == -1)
         erreur_sys (ERR_DETACH_SHM_NOM);
      if (shmctl (shm_nom, IPC_RMID, NULL) == -1)
         erreur_sys (ERR_DELETE_SHM_NOM);

      if (shmdt (valeur_g) == -1)
         erreur_sys (ERR_DETACH_SHM_VALEUR);
      if (shmctl (shm_valeur, IPC_RMID, NULL) == -1)
         erreur_sys (ERR_DELETE_SHM_VALEUR);

      if (shmdt (col_g) == -1)
         erreur_sys (ERR_DETACH_SHM_COL);
      if (shmctl (shm_col, IPC_RMID, NULL) == -1)
         erreur_sys (ERR_DELETE_SHM_COL);

      if (semctl (mutex, 0, IPC_RMID, NULL) == -1)
         erreur_sys (ERR_DELETE_MUTEX);
   }
   else
   {
#ifdef DEBUG
      printf("myshell--\n");
#endif
      P (mutex, 0);
      NB_MYSHELL_RUNNING--;
      V (mutex, 0);
   }
}


/**
 * Affiche un couple nom=var
 * param indice_nom : l'indice du nom dans le shm contenant les noms
 * param indice_valeur : l'indice du nom dans le shm contenant les valeurs
 */
void varglob_affiche_uniq (int indice_nom, int indice_valeur)
{
   while (nom_g[indice_nom] != '\0')
      printf("%c", nom_g[indice_nom++]);

   printf("=");

   while (valeur_g[indice_valeur] != '\0')
      printf("%c", valeur_g[indice_valeur++]);

   printf("\n");
}


/**
 * Affiche la liste des variables globales
 */
void varglob_affiche ()
{
   int i,k;

   for (i = 0 ; i < MAX_VAR_INIT ; i++)
   {
      if (indice_nom_g[i] != -1)
      {
#ifdef DEBUG
         printf("clé: %d\n", i);
#endif
         varglob_affiche_uniq (indice_nom_g[i], indice_valeur_g[i]);

         /* Affichage de la liste des colisions */
         if (indice_col_g[i] != -1)
         {
#ifdef DEBUG
            printf("affichage des colisions\n");
#endif
            k = indice_col_g[i];
            varglob_affiche_uniq (COL_NOM(k), COL_VALEUR(k));
            while ( (k = COL_SUIVANT(k)) != -1)
            {
               varglob_affiche_uniq (COL_NOM(k), COL_VALEUR(k));
            }
         }
      }
   }
}


/**
 * Compare si le nom pointé par indice_nom == *nom
 * param indice_nom : l'indice du nom dans le shm contenant les noms
 * param * nom : nom de la variable globale
 * param taille_nom : la longueur du nom
 * return 0 si différent, 1 sinon
 */
int nom_egal (int indice_nom, char * nom, int taille_nom)
{
   int i = 0, egal = 1;

   while (egal && indice_nom < TAILLE_NOM && i < taille_nom)
      egal = (nom_g[indice_nom++] == nom[i++]);

   if (indice_nom == TAILLE_NOM && i < taille_nom)
      return 0;

   return egal;
}


/**
 * Recherche un nom de variable dans la liste chainée de la case "cle"
 * param cle : la clé dans la table de hash de la variable globale
 * param * nom : nom de la variable globale
 * return -1 si non trouvé, l'indice dans col_g si trouvé
 */
int varglob_cherche_liste (int cle, char * nom)
{
   int indice_col = indice_col_g[cle];
   int taille_nom = strlen (nom);

#ifdef DEBUG
   printf("%s ==", nom);
#endif

   do
   {
      if (nom_egal (COL_NOM(indice_col), nom, taille_nom))
         return indice_col;
      indice_col = COL_SUIVANT(indice_col);
   } while (COL_SUIVANT(indice_col) != -1);

   if (nom_egal (COL_NOM(indice_col), nom, taille_nom))
      return indice_col;

   return -1;
}


/**
 * Recherche une variable globale et renvoie sa valeur
 * param * nom : le nom de la variable globale
 * return : un char * de la valeur de la variable si trouvé ; NULL si non trouvé
 */
char * varglob_get (char * nom)
{
   unsigned int cle;
   int indice_valeur, indice_nom;
   dyn_string s_valeur, s_nom;
   char * final = NULL;
   char * final_nom;
   int res;

   dyn_string_init (&s_valeur);
   dyn_string_init (&s_nom);

   cle = hash2 (nom, TAILLE_INDICE);
   indice_valeur = indice_valeur_g [cle];
   indice_nom = indice_nom_g [cle];

   if (indice_nom != -1)
   {
      while (nom_g[indice_nom] != '\0')
         dyn_string_add_char (&s_nom, nom_g[indice_nom++]);
      final_nom =  dyn_string_final (&s_nom);

      /* Mattage dans les colisions */
      if (strcmp (final_nom, nom))
      {
         res = varglob_cherche_liste (cle, nom);
         if (res != -1)
            indice_valeur = COL_VALEUR(res);
         else
            indice_valeur = -1;
      }

      /* Si variable trouvée */
      if (indice_valeur != -1)
      {
         while (valeur_g[indice_valeur] != '\0')
            dyn_string_add_char (&s_valeur, valeur_g[indice_valeur++]);
         final = dyn_string_final (&s_valeur);
      }
      else
      {
         dyn_string_free (&s_valeur);
      }
   }
   else
   {
      dyn_string_free (&s_valeur);
   }
   dyn_string_free (&s_nom);
   return final;
}


/**
 * param argc : nombre d'arguments
 * param **argv : tableau des arguments
 */
void mysetenv (int argc, char **argv)
{
   P (mutex, 0);
   varglob_verif_no_change ();
   if (argc >= 3)
   {
      varglob_set (argv[1], argv[3]);
   }
   else
   {
      varglob_affiche ();
   }
   V (mutex, 0);
}


/**
 * Affiche la liste des varglob / A éxécuter dans un fork !
 * param argc : nombre d'arguments
 * param **argv : tableau des arguments
 */
int mysetenv2 (int argc, char ** argv)
{
   P (mutex, 0);
   varglob_verif_no_change ();
   varglob_affiche ();
   V (mutex, 0);
   exit (0);
}
