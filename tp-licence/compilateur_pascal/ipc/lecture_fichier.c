/**
 * module : lecture_fichier
 * brief : Lecture bufferisée d'un fichier quelconque
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date : Février 2004
 * bug : Aucun détecté à ce jour
 *
 * Lit le fichier en entrée par buffer. Dès que le buffer courant a été exploité,
 * on relit un buffer.
 * Ce module offre la possibilité de remettre un caractère dans le flux courant.
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
 * Remplit le buffer de caractères.
 * param *f : le fichier à lire
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
 * param *nom_fichier : la chaine de caractère correspondant au fichier à ouvrir
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
 * Ferme un fichier précédement ouvert.
 * param *f : le Fichier à fermer
 */
void ferme_fichier (Fichier *f)
{
   if (close (f->fd))
      free_all (EXIT_FIC_CLOSE);
}


/**
 * Retourne un caractère lu dans le buffer ou bien le caractère de remise.
 * param *f : Le fichier à lire
 * return : le caractère suivant dans le flux
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
 * Insère un caractère dans le flux de caractères.
 * *f : le fichier dans lequel on rajoute le caractère
 * c : le caractère à rajouter
 */
void insere_car (Fichier *f, char c)
{
   f->insere = c;
}
