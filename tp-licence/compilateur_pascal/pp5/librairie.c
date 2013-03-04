/**
 * module : librairie
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
#include <sys/time.h>

#include "librairie.h"


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/**
 * Transforme un entier en une chaine de caractere
 * param valeur : l'entier à transformer en chaine de caractère
 * return : la chaine de caractère correspondant à l'entier
 */
char *itoc (int valeur)
{
   sprintf(itoc_result,"%d", valeur);
   return itoc_result;
}


/**
 *
 */
double chrono ()
{
   struct timeval temps;
   gettimeofday(&temps, 0);
   return temps.tv_sec + temps.tv_usec * 1e-6;
}


/**
 * Calcule la longueur d'un entier
 * param entier : l'entier dont on doit calculer la longueur
 * return : la longueur de l'entier passé en paramètre
 */
int longueur (int entier)
{
   int retour = 0;

   if (entier <= 0)
   {
      retour++;
      entier *= -1 ;
   }

   while (entier !=0)
   {
      retour++;
      entier /= 10;
   }

   return retour;
}






