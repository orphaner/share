/**
 * module : list_fg
 * brief : 
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date : 
 * bug : Aucun d�tect� � ce jour
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
 * Initialise une liste � NULL
 * param list : la liste des t�ches en background
 * return : la liste initialis�e � NULL
 */
list_fg list_fg_init (list_fg list)
{
   list = NULL;
   return list;
}


/**
 * Supprime la liste des t�ches en background
 * param list : la liste des t�ches en background
 * return : une nouvelle liste initialis�e � NULL
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
 * Supprime un pid de la liste des t�ches en background
 * Affiche son num�ro et son pid
 * param list : la liste des t�ches en background
 * param pid : le pid � supprimer de la liste
 * return : la liste avec le maillon �ventuellement supprim�
 */
list_fg list_fg_suppr (list_fg list, pid_t const pid)
{
   list_fg temp = list;
   list_fg prev = list;

   /* Suppression en t�te */
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
 * Ajoute une t�che en background dans la liste
 * param list : la liste des t�ches en background
 * param pid : le pid � ajouter dans la liste
 * return : la liste avec le maillon ajout�
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
 * Affiche la liste des t�ches en background
 * param list : la liste des t�ches en background � afficher
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
 * param list : la liste des t�ches en background
 * param pos : le num�ro de la t�che sur laquelle on veut un fg
 * return : le pid de la tache correspond au num�ro ; -1 si non trouv�
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
