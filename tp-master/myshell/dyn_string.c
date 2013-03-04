/**
 * module : dyn_string
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

#include "common.h"
#include "dyn_string.h"
#include "memutils.h"


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/**
 * Initialise une dyn_string AVEC allocation initiale
 * param s : la chaine de caractère dynamique à initialiser
 */
void dyn_string_init (dyn_string *s)
{
   s->nb_realloc = 16;
   s->taille = 0;
   s->parcours = 0;
   s->chaine = NULL;
   s->chaine = my_malloc (s->nb_realloc * sizeof(char));
}


/**
 * Initialise une dyn_string SANS allocation initiale
 * param s : la chaine de caractère dynamique à initialiser
 */
void dyn_string_init2 (dyn_string *s)
{
   s->nb_realloc = 0;
   s->taille = 0;
   s->parcours = 0;
   s->chaine = NULL;
}


/**
 * Désalloue une dyn_string 
 * param s : la chaine de caractère dynamique à désaouller
 */
void dyn_string_free (dyn_string *s)
{
   if (s->chaine != NULL)
      free (s->chaine);
}


/**
 * Affiche qques infos sur une dyn_string
 * param s : la chaine de caractère dynamique à afficher infos 
 */
void dyn_string_aff_infos (dyn_string s)
{
   printf("taille: %d\n", s.taille);
   printf("nb_realloc: %d",s.nb_realloc);
}


/**
 * Ajoute un caractère à la fin de la dyn_string. Réalloue si besoin.
 * param s : la chaine de caractère dynamique ds laquelle on ajoute
 */
void dyn_string_add_char (dyn_string *s, char const c)
{
   if (s->taille == s->nb_realloc)
   {
      s->nb_realloc *= 2;
      s->chaine = my_realloc (s->chaine, s->nb_realloc * sizeof (char));
   }
   s->chaine[s->taille++] = c;
}


/**
 * Finalise une dyn_string : caractère \0 + réallocation à la bonne taille !
 * param s : la chaine de caractère dynamique à finaliser
 * return : un pointeur vers la chaine de caractère de la dyn_string
 */
string dyn_string_final (dyn_string *s)
{
   if (s->taille == s->nb_realloc)
      s->chaine = my_realloc (s->chaine, (s->taille+1) * sizeof(char));
   s->chaine [s->taille++] = '\0';
   return s->chaine;
}


/**
 * Saisie une dyn_string
 * param s : la chaine de caractère dynamique
 */
void dyn_string_saisie (dyn_string *s)
{
   char c;
   while ( (c=getchar()) != CAR_LIGNE)
      dyn_string_add_char (s,c);
   dyn_string_final (s);
}


/**
 * Lit le caractère courant et avance dans le flux
 * param s : la chaine de caractère dynamique
 * return : le caractère lu, \0 CAR_END_STRING sinon
 */
char dyn_string_lire_car (dyn_string *s)
{
   return (s->parcours < s->taille) ? s->chaine[s->parcours++] : CAR_END_STRING;
}


/**
 * Lit le caractère courant et N'avance PAS dans le flux
 * param s : la chaine de caractère dynamique
 * return : le caractère lu, \0 CAR_END_STRING sinon
 */
char dyn_string_lire_car2 (dyn_string *s)
{
   return (s->parcours < s->taille) ? s->chaine[s->parcours] : CAR_END_STRING;
}


/**
 * Lit le dernier caractère lu si un caractère a déjà été lu
 * param s : la chaine de caractère dynamique à initialiser
 * return : le dernier caractère lu s'il existe, \0 CAR_END_STRING sinon
 */
char dyn_string_last_car (dyn_string *s)
{
   return (s->parcours > 0) ? s->chaine[s->parcours-1] : CAR_END_STRING;
}


/**
 * Remise d'un caractère dans le flux, au cas où on lit un caractère en trop 
 * param s : la chaine de caractère dynamique
 */
void dyn_string_remise_car (dyn_string *s)
{
   s->parcours--;
}


/**
 * Passe tous les espaces ds le flux de parcours à partir de la position courante
 * Le prochain caractère retourné sera le dernier caractère non nul.
 * param s : la chaine de caractère dynamique
 */
void dyn_string_trim (dyn_string  *s)
{
   char c;
   while ( (c = dyn_string_lire_car (s)) == CAR_ESPACE);
   if (c != CAR_ESPACE && c != CAR_END_STRING)
      dyn_string_remise_car (s);
}
