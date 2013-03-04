/**
 * module : list_fg
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
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "dyn_string.h"
#include "saisie.h"
#include "list_fg.h"
#include "memutils.h"


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/
list_fg list;


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/**
 * Initialise une liste à NULL
 * param list : la liste des tâches en background
 * return : la liste initialisée à NULL
 */
list_fg list_fg_init (list_fg list)
{
   list = NULL;
   return list;
}


/**
 * Supprime la liste des tâches en background
 * param list : la liste des tâches en background
 * return : une nouvelle liste initialisée à NULL
 */
list_fg list_fg_free (list_fg list)
{
   list_fg temp = list;
   while (temp != NULL)
   {
      temp = list->suivant;
      free (list);
      list = temp;
   }
   return list_fg_init (list);
}


/**
 * Supprime un pid de la liste des tâches en background
 * Affiche son numéro et son pid
 * param list : la liste des tâches en background
 * param pid : le pid à supprimer de la liste
 * return : la liste avec le maillon éventuellement supprimé
 */
list_fg list_fg_suppr (list_fg list, pid_t const pid)
{
   list_fg temp = list;
   list_fg prev = list;

   /* Suppression en tête */
   if (temp && temp->pid == pid)
   {
      printf ("[%d]+ %d\n", temp->pos, temp->pid);
      list = list->suivant;
      free (temp->nom_cmd);
      free (temp);
   }

   /* Suppression ailleurs */
   else
   {
      while (temp && temp->pid != pid)
      {
         prev = temp;
         temp = temp->suivant;
      }
      if (temp)
      {
         printf ("[%d]+ %d\n", temp->pos, temp->pid);
         prev->suivant = temp->suivant;
         free (temp->nom_cmd);
         free (temp);
      }
   }

   return list;
}


/**
 * Ajoute une tâche en background dans la liste
 * param list : la liste des tâches en background
 * param pid : le pid à ajouter dans la liste
 * return : la liste avec le maillon ajouté
 */
list_fg list_fg_add (list_fg list, pid_t const pid)
{
   int taille;
   char * chaine = saisie.chaine;
   list_fg l = my_malloc (sizeof (maillon));

   if (list == NULL)
      l->pos = 1;
   else
      l->pos = list->pos + 1;
   l->pid = pid;
   l->suivant = list;

   taille = strlen (chaine);
   l->nom_cmd = NULL;
   l->nom_cmd = my_malloc ((taille + 1) * sizeof (char));
   strcpy (l->nom_cmd, chaine);
   l->nom_cmd[taille] = '\0';

   list = l;

   printf ("[%d] %d\n", l->pos, l->pid);
   return list;
}


/**
 * Affiche la liste des tâches en background
 * param list : la liste des tâches en background à afficher
 */
void list_fg_aff (list_fg const list)
{  
   list_fg temp = list;
   while (temp != NULL)
   {
      printf ("[%d] -> %d\t%s\n", temp->pos, temp->pid, temp->nom_cmd);
      temp = temp->suivant;
   }
   printf ("\n");
}


/**
 * param list : la liste des tâches en background
 * param pos : le numéro de la tâche sur laquelle on veut un fg
 * return : le pid de la tache correspond au numéro ; -1 si non trouvé
 */
pid_t list_fg_get_pos (list_fg const list, int const pos)
{
   list_fg temp = list;
   while (temp)
   {
      if (temp->pos == pos)
         return temp->pid;
      temp = temp->suivant;
   }
   return -1;
}
