/**
 * module : varloc
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

#include "hash.h"
#include "varloc.h"


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/
/* Les variables d'environement de myshell */
Tablesym varloc;

/* Les vraies variables d'environement */
extern char **environ;


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/**
 *
 */
void varloc_remplir ()
{
   char ** env;
   char *c;
   char var[1024];
   int i;


   hash_init (&varloc);

   for (env = environ ; *env != NULL ; ++env)
   {
      for (c = *env, i = 0 ; *c != '=' ; *c++, i++)
         var[i] = *c;
      var [i] = '\0';

      hash_add (&varloc, var, getenv(var));
   }
}


/**
 * Affiche un arbre
 * param abr : l'arbre à afficher
 */
void varloc_abr_affiche (Arbre abr)
{
   if (abr)
   {
      printf("%s=%s\n", abr->nom, abr->valeur);
      varloc_abr_affiche (abr->fg);
      varloc_abr_affiche (abr->fd);
   }
}


/**
 * Affiche les variables locales 
 * param *t : la table de hash à afficher
 */
void varloc_affiche (Tablesym *t)
{
   unsigned int i = 0;
   for (; i < MAX_TABLE ; i++)
   {
      if (t->table[i])
      {
         varloc_abr_affiche (t->table[i]);
      }
   }
}


/**
 * Affecte une variable
 * param **argv : tableau des arguments
 * param argc : nombre d'arguments
 */
void myset (int argc, char **argv)
{
   /* Affecter une variable locale*/
   if (argc == 4)
   {
      if (hash_add (&varloc, argv[1], argv[3]) == NULL)
         hash_modif (&varloc, argv[1], argv[3]);
   }

   /* Affichage de la liste des variables locales */
   else
   {
      varloc_affiche (&varloc);
   }
}


/**
 * Affiche la liste des varloc / A éxécuter dans un fork !
 * param **argv : tableau des arguments
 * param argc : nombre d'arguments
 */
int myset2 (int argc, char **argv)
{
   varloc_affiche (&varloc);
   exit (0);
}
