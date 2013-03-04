/**
 * module : lecture_fichier
 * brief : Lecture bufferis��e d'un fichier quelconque
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date : F��vrier 2004
 * bug : Aucun d��tect�� �� ce jour
 *
 *
 * Lit le fichier en entr��e par buffer. D��s que le buffer courant a ��t�� exploit��,
 * on relit un buffer.
 * Ce module offre la possibilit�� de remettre un caract��re dans le flux courant.
 */


/*------------------------------------------------------------------------------
  Fichiers include
  ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#include "lexical.h"
#include "erreur.h"
#include "librairie.h"
#include "lecture_fichier.h"


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/


/* =============================================================================
   FONCTIONS DE LECTURE
   ===========================================================================*/
/**
 * Remplit le buffer de caract��res.
 * param *f : le fichier �� lire
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
 * param *nom_fichier : la chaine de caract��re correspondant au fichier �� ouvrir
 * return : une structure Fichier qui contient le descripteur du fichier
 *          ainsi qu'un premier buffer remplit.
 */
Fichier ouvre_fichier_lecture (char *nom_fichier)
{
   Fichier f;
   f.nom_fichier = (char *) malloc( (1+strlen(nom_fichier)) * sizeof(char));
   strcpy (f.nom_fichier, nom_fichier);
   f.fd = open (nom_fichier, O_RDONLY);
   if (f.fd == ERREUR)
      free_all (EXIT_FIC_OPEN_R);
   remplit_buffer (&f);
   f.ligne = f.colone = 1;
   return f;
}


/**
 * Retourne un caract��re lu dans le buffer ou bien le caract��re de remise.
 * param *f : Le fichier �� lire
 * return : le caract��re suivant dans le flux
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
      {
         /* pof_buffer =1 -> 2chars  (commence a 0) */

         /* Mise �� jour de ligne et colone courante */
         if (f->buffer[f->pos_buffer] == CAR_LIGNE)
         {
            f->ligne ++;
            f->colone = 0;
         }
         else
            f->colone++;
         return f->buffer[f->pos_buffer++];
      }
   }
   return FIN_FICHIER;
}


/**
 * Ins��re un caract��re dans le flux de caract��res.
 * *f : le fichier dans lequel on rajoute le caract��re
 * c : le caract��re �� rajouter
 */
void insere_car (Fichier *f, char c)
{
   f->insere = c;
}


/* =============================================================================
   FONCTIONS D'ECRITURE
   ===========================================================================*/
/**
 * Ouvre un fichier en ecriture.
 * param *nom_fichier : la chaine de caract��re correspondant au fichier �� ouvrir
 * return : une structure Fichier qui contient le descripteur du fichier
 */
Fichier ouvre_fichier_ecriture (char *nom_fichier)
{
   Fichier f;
   f.nom_fichier = (char *) malloc( (1+strlen(nom_fichier)) * sizeof(char));
   strcpy (f.nom_fichier, nom_fichier);
   f.fd = creat (nom_fichier, S_IRWXU);
   if (f.fd == ERREUR)
      free_all (EXIT_FIC_OPEN_W);
   f.pos_buffer = 0;
   f.pos_fichier = 0;
   return f;
}


/**
 * Ecrit un caract��res dans le buffer
 * Ecrit tout le buffer dans le fichier quand il est plein
 * param c : Le caract��re �� ecrire
 * param *f : Le fichier o�� ecrire
 */
void ecrire_car (char c, Fichier *f)
{
   f->buffer[f->pos_buffer++] = c;

   if (f->pos_buffer == BUFFER_FILE)
   {
      ecrit_buffer(f);
   }
}

/**
 * Ecrit tout le buffer de caract��res.
 * param *f : le fichier �� ecrire
 */
void ecrit_buffer (Fichier *f)
{
   f->pos_fichier = write (f->fd, f->buffer, BUFFER_FILE);
   if (f->pos_fichier == ERREUR)
      free_all (EXIT_FIC_WRITE);
   f->pos_buffer = 0;
}

/**
 * Vide le buffer (meme incomplet) dans le fichier.
 * param *f : le fichier �� ecrire
 */
void vide_buffer (Fichier *f)
{
   f->pos_fichier = write (f->fd, f->buffer, f->pos_buffer);
   if (f->pos_fichier == ERREUR)
      erreur_mess (EXIT_FIC_WRITE);
   f->pos_buffer = 0;
}


/**
 * Ecrit une chaine de caract��res dans le buffer
 * Utilise une boucle avec ecrire_car
 * param *s : La chaine de caract��res �� ecrire
 * param *f : Le fichier o�� ecrire
 */
void ecrire_string (char *s, Fichier *f)
{
   while (*s != '\0')
   {
      ecrire_car (*s, f);
      s++;
   }
}


/**
 * Ecrit un entier traduit en chaine de caract��res dans le buffer
 * Traduit l'entier en chaine de caract��res et utilise ecrire_string
 * param *entier : L'entier �� ecrire
 * param *f : Le fichier o�� ecrire
 */
void ecrire_entier (int entier, Fichier *f)
{
   ecrire_string (itoc(entier), f);
}


/**
 * Ferme un fichier pr��c��dement ouvert.
 * param *f : le Fichier �� fermer
 * n'appelle pas free_all car est appell�� par free_all
 */
void ferme_fichier (Fichier *f)
{
   if (close (f->fd))
      erreur_mess (EXIT_FIC_CLOSE);
}


/**
 * Supprime un fichier pr��c��dement ouvert et ferm��.
 * param *f : le Fichier �� suprimer.
 * n'appelle pas free_all car est appell�� par free_all
 */
void supprime_fichier (Fichier *f)
{
   if (unlink (f->nom_fichier) == -1 )
      erreur_mess (EXIT_FIC_ERASE);
}

