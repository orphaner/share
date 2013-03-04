/**
 * module : erreur
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

#define MESSAGE_OK
#include "erreur.h"
#include "lexical.h"
#include "syntaxique.h"
#include "hash.h"
#include "generation.h"
#include "lecture_fichier.h"


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/**
 * Provoque une erreur fatale :
 * - écrit le message d'erreur
 * - désalloue proprement toute la mémoire
 * - ferme s'il y a lieu les fichiers
 * - supprime le fichier p_code s'il y a uet des erreurs de compilation
 * - quitte le programme avec le code de retour correspondant
 * param code_retour : le code d'erreur
 */
void free_all (Erreurs code_retour)
{
   if (code_retour == EXIT_OK)
      printf ("%s\n\n",ErreursMsg[code_retour]);
   else
      fprintf (stderr, "(%u,%u) Erreur: %s\n",
               fichier_pascal.ligne, fichier_pascal.colone, ErreursMsg[code_retour]);

   free (sym);
/*    free (next_anonyme_string); */

   /* FICHIERS */
   if (code_retour != EXIT_FIC_OPEN_W)
   {
      if (!corrige)
         vide_buffer (&fic_pcode);
      ferme_fichier (&fic_pcode);
      if (corrige)
         supprime_fichier (&fic_pcode);
      free (fichier_pascal.nom_fichier);
      free (fic_pcode.nom_fichier);
   }
   if (code_retour != EXIT_FIC_OPEN_R)
      ferme_fichier (&fichier_pascal);

   hash_free (&table);
   pile_saut_free (&sauts);
   exit (code_retour);
}


/**
 * Apellée quand les paramètres du compilateur ne sont pas corrects
 * param *s : la chaine contenant le nom du programme
 * param code_retour : le code d'erreur
 */
void usage (char *s, Erreurs code_retour)
{
   printf ("%s\n",ErreursMsg[code_retour]);
   printf ("Usage: %s [options] <nom_fichier_pp5>\n", s);
   printf ("Les options possibles sont :\n");
   printf ("  -tablesym              :  affiche la table des symboles\n");
   printf ("  -o <nom_fichier_pcode> :  donne le nom <nom_fichier_pcode> au fichier généré\n");
   exit (code_retour);
}


/**
 * Affiche un message d'erreur
 * param err : le code d'erreur
 */
void erreur_mess (Erreurs err)
{
   fprintf (stderr, "%s(%u,%u) Error: %s \n",
            fichier_pascal.nom_fichier, fichier_pascal.ligne, fichier_pascal.colone, ErreursMsg[err]);
}
