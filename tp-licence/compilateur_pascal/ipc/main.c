/**
 * module : main
 * brief : Regroupe tous les modules et lance le programme
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date : Février 2004
 * bug : Aucun détecté à ce jour
 */


/*------------------------------------------------------------------------------
  Fichiers include
  ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../common/mnemonique.h"
#include "erreur.h"
#include "lecture_fichier.h"
#include "lire_inst.h"
#include "pile_exe.h"
#include "pile_var.h"
#include "interpreteur.h"
#include "main.h"


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/**
 *
 */
int main (int argc, char *argv[])
{
   char debug = 0;

   /* Nombre de paramètres KO */
   if (argc == 1)
   {
      usage (argv[0], EXIT_MISSING_PARAM, argv[0]);
   }

   /* Nombre de paramètres OK */
   else
   {
      if (argc == 2)
         fic = ouvre_fichier_lecture (argv[1]);
      else if (argc == 3)
      {
         if (!strcmp (argv[1], "-d"))
         {
            debug = 1;
            fic = ouvre_fichier_lecture (argv[2]);
         }
         else
         {
            usage (argv[0], EXIT_UNKNOW_PARAM, argv[1]);
         }
      }
      p_exe = creer_pile_exe ();
      p_var = creer_pile_var ();
      p_base = creer_pile_var ();

      if (!remplir_pile (fic, &p_exe))
      {
         pile_exe_ajust (&p_exe);
         execute (&p_var, &p_exe, &p_base, debug);
      }
      else
      {
         free_all (EXIT_PCODE);
      }
   }
   free_all (EXIT_OK);
   return EXIT_OK;
}
