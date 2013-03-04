/**
 * module : main
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
#include <string.h>
#include <getopt.h>

#include "lecture_fichier.h"
#include "syntaxique.h"
#include "lexical.h"
#include "hash.h"
#include "erreur.h"
#include "generation.h"
#include "librairie.h"


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/**
 *
 */
int main (int argc, char *argv[])
{
   char debug_tablesym = 0;
   char *nom_fichier_pascal;
   char *nom_fichier_pcode = "Gryl_Lassalle.pcode";
   double tps_debut, tps_fin;
   tps_debut = chrono();


   /* Nombre de paramètres KO */
   if (argc == 1)
   {
      usage (argv[0], EXIT_PARAM);
   }

   /* Nombre de paramètres OK */
   else
   {
      /* Option de voir la table des symboles */
      if (!strcmp (argv[1], "-tablesym"))
      {
         debug_tablesym = 1;
         nom_fichier_pascal = argv[2];
      }
      else

         nom_fichier_pascal = argv[1];
      fichier_pascal = ouvre_fichier_lecture (nom_fichier_pascal);


      /* mettre l'option de nom de fihier pcode en sortie */
      fic_pcode = ouvre_fichier_ecriture (nom_fichier_pcode);

      lex_init();
      hash_init (&table);
      pile_saut_init (&sauts);

      program();
      printf("offset: %d\n", table.offset);
      printf("ligne: %d\n", fichier_pascal.ligne);
      pile_saut_to_file (&sauts);

      if (debug_tablesym)
         hash_affiche(&table);
   }
   tps_fin=chrono();
   printf("Temps de compilation : %f ms\n",(tps_fin-tps_debut));
   free_all (EXIT_OK);
   exit (EXIT_OK);
}
