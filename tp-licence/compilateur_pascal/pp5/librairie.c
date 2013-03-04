/**
 * module : librairie
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
 * param valeur : l'entier � transformer en chaine de caract�re
 * return : la chaine de caract�re correspondant � l'entier
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
 * return : la longueur de l'entier pass� en param�tre
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






