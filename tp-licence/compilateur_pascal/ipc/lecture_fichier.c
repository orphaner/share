/**
 * module : lecture_fichier
 * brief : Lecture bufferis�e d'un fichier quelconque
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date : F�vrier 2004
 * bug : Aucun d�tect� � ce jour
 *
 * Lit le fichier en entr�e par buffer. D�s que le buffer courant a �t� exploit�,
 * on relit un buffer.
 * Ce module offre la possibilit� de remettre un caract�re dans le flux courant.
 */


/*------------------------------------------------------------------------------
  Fichiers include
  ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "erreur.h"
#include "lecture_fichier.h"


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/**
 * Remplit le buffer de caract�res.
 * param *f : le fichier � lire
 */
void remplit_buffer (Fichier *f)
{
   f->pos_fichier = read (f->fd, f->buffer, BUFFER_FILE);
   if (f->pos_fichier == ERREUR)
      free_all (EXIT_FIC_READ);
   f->pos_buffer = 0;
   f->insere = CAR_VIDE;
}


/**
 * Ouvre un fichier en lecture.
 * param *nom_fichier : la chaine de caract�re correspondant au fichier � ouvrir
 * return : une structure Fichier qui contient le descripteur du fichier
 *          ainsi qu'un premier buffer remplit.
 */
Fichier ouvre_fichier_lecture (char *nom_fichier)
{
   Fichier f;
   f.fd = open (nom_fichier, O_RDONLY);
   if (f.fd == ERREUR)
      free_all (EXIT_FIC_OPEN_R);
   remplit_buffer (&f);
   return f;
}


/**
 * Ferme un fichier pr�c�dement ouvert.
 * param *f : le Fichier � fermer
 */
void ferme_fichier (Fichier *f)
{
   if (close (f->fd))
      free_all (EXIT_FIC_CLOSE);
}


/**
 * Retourne un caract�re lu dans le buffer ou bien le caract�re de remise.
 * param *f : Le fichier � lire
 * return : le caract�re suivant dans le flux
 */
char lire_car (Fichier *f)
{
   char c;
   if (f->insere != CAR_VIDE)
   {
      c = f->insere;
      f->insere = CAR_VIDE;
      return c;
   }
   else
   {
      if (f->pos_buffer == f->pos_fichier && f->pos_fichier == BUFFER_FILE)
         remplit_buffer(f);

      /* en fait, pos_fichier =1 -> 1 char */
      if (f->pos_buffer <  f->pos_fichier)
         /* pof_buffer =1 -> 2chars  (commence a 0) */
         return f->buffer[f->pos_buffer++];
   }
   return FIN_FICHIER;
}


/**
 * Ins�re un caract�re dans le flux de caract�res.
 * *f : le fichier dans lequel on rajoute le caract�re
 * c : le caract�re � rajouter
 */
void insere_car (Fichier *f, char c)
{
   f->insere = c;
}
