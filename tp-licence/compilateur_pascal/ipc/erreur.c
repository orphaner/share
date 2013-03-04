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

#include "lire_inst.h"
#include "pile_exe.h"
#include "pile_var.h"
#include "lecture_fichier.h"
#include "main.h"

#define MESSAGE_OK
#include "erreur.h"


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
void erreur (int code_erreur, long int num_ligne)
{
   fprintf (stderr, "Erreur(%ld): %s\n", num_ligne, ErreursMsg[code_erreur]);
}

/**
 *
 */
void free_all (int code_retour)
{
   if (code_retour == EXIT_OK)
      printf ("%s\n\n",ErreursMsg[code_retour]);
   else
      fprintf (stderr, "Erreur: %s\n", ErreursMsg[code_retour]);

   pile_exe_free (&p_exe);
   pile_var_free (&p_var);
   pile_var_free (&p_base);
   ferme_fichier (&fic);

   exit (code_retour);
}


/**
 *
 */
void usage (char *s, Erreurs code_retour, char *s2)
{
   printf(ErreursMsg[code_retour], s2);
   printf ("Usage: %s [-d] <nom_fichier_pcode>\n", s);
   exit (code_retour);
}
