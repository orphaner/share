/**
 * module : generation
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

#include "../common/mnemonique.h"
#include "lecture_fichier.h"
#include "erreur.h"
#include "librairie.h"
#include "generation.h"


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/
char commentaire = 1; /* 1 = commente les actions dans le fichier p_code (0 sinon)*/
char numerotation = 1; /* 1 = num�rote les lignes du fichier p_code (0 sinon)*/
char *com_numero = "   # " ; /* symbole plac� avant le num�ro de ligne */


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/* =============================================================================
   FONCTIONS DE GESTION DES SAUTS
   ===========================================================================*/
/**
 * Initialise la pile de sauts avec un premier buffer
 * param *sauts : La pile de sauts � initialiser
 */
void pile_saut_init (Pile_saut *sauts)
{
   sauts->liste = (Saut *) malloc (PILE_SAUT_BUFFER * sizeof (Saut));
   sauts->nb = 0;
   ligne_pcode = 0;
}


/**
 * Ajoute un nouveau saut que l'on devra mettre � jour ensuite
 * param *sauts : La pile de sauts dans laquelle on ajoute un saut en attente
 * return : la position du saut ins�rer dans la pile
 */
int pile_saut_push (Pile_saut *sauts, int depart)
{
   /* Teste si la pile n'est pas pleine */
   if ( !((++sauts->nb + 1 ) % PILE_SAUT_BUFFER) )
   {
      /* On r�allou avec taille= taille existante (=sp) + 1 taille buffer */
      sauts->liste = (Saut *) realloc (sauts->liste, ( (sauts->nb + 1) +  PILE_SAUT_BUFFER) * sizeof (Saut));

      /* Si realloc renvoi NULL alors la m�moire est satur�e */
      if (!sauts->liste)
         free_all (EXIT_PS_OVER);
   }
   /* Ajout du saut */
   sauts->liste[sauts->nb-1].depart = depart;

   return sauts->nb-1;
}


/**
 * Met � jour un saut conditionnel
 * param *sauts : La pile de sauts que l'on met � jour
 * param pos : Le saut � mettre � jour
 * param arrivee : La valeur du saut
 */
void pile_saut_update (Pile_saut *sauts, unsigned int pos, int arrivee)
{
   sauts->liste[pos].arrivee = arrivee;
}


/**
 * Ecrit la pile des sauts dans le fichier de sortie
 * param *sauts : La pile de sauts � �crire dans le fichier
 */
void pile_saut_to_file (Pile_saut *sauts)
{
   unsigned int i = 0;
   char *marqueur_saut_debut = "$J" ;

   ecrire_string(marqueur_saut_debut , &fic_pcode);
   ecrire_car ('\n', &fic_pcode);

   for ( ; i <sauts->nb ; i++)
   {
      ecrire_entier (sauts->liste[i].depart, &fic_pcode);
      ecrire_car (' ', &fic_pcode);
      ecrire_entier (sauts->liste[i].arrivee, &fic_pcode);
      ecrire_car ('\n', &fic_pcode);
   }
}


/**
 * D�salloue la m�moire utilis�e par la pile des sauts
 * param *sauts : La pile de sauts � �crire dans le fichier
 */
void pile_saut_free (Pile_saut *sauts)
{
   free (sauts->liste);
}



/* =============================================================================
   FONCTIONS DE GENERATION
   ===========================================================================*/
/**
 * Ecrit dans le fichier une instruction et passe a la ligne
 * param mne : le code du menonique
 */
void generer1 (mnemoniques mne)
{
   if (!corrige)
   {
      char inst[3];
      revert_code (mne, inst);
      ecrire_car (inst[0], &fic_pcode);
      ecrire_car (inst[1], &fic_pcode);
      ecrire_car (inst[2], &fic_pcode);

      if (numerotation)
         numeroter (0);

      ecrire_car ('\n', &fic_pcode);
      ligne_pcode++;
   }
}


/**
 * Ecrit dans le fichier un commentaire et passe � la ligne
 * param *param : la chaine de car du commentaire � �crire
 */
void generer_com (char* param)
{
   if (!corrige)
   {
      ecrire_car ('#', &fic_pcode);
      ecrire_car (' ', &fic_pcode);
      ecrire_string (param, &fic_pcode);
      ecrire_car ('\n', &fic_pcode);
   }
}


/**
 * Ecrit dans le fichier une instruction, un espace, son param�tre (un entier) et passe a la ligne
 * param mne : le code du mn�nonique
 * param param : le param�tre de l'instruction (un entier)
 */
void generer2i (mnemoniques mne, int param)
{
   if (!corrige)
   {
      char inst[3];

      revert_code (mne, inst);
      ecrire_car (inst[0], &fic_pcode);
      ecrire_car (inst[1], &fic_pcode);
      ecrire_car (inst[2], &fic_pcode);
      ecrire_car (' ', &fic_pcode);
      ecrire_entier (param, &fic_pcode);

      if (numerotation)
         numeroter ( longueur (param) + 1);

      ecrire_car ('\n', &fic_pcode);

      ligne_pcode++;
   }
}


/**
 * Ecrit dans le fichier une instruction, un espace, son param�tre (char) et passe a la ligne
 * param mne : le code du menonique
 * param param : le param�tre de l'instruction (char)
 */
void generer2c (mnemoniques mne, char param)
{
   if (!corrige)
   {
      char inst[3];
      revert_code (mne, inst);
      ecrire_car (inst[0], &fic_pcode);
      ecrire_car (inst[1], &fic_pcode);
      ecrire_car (inst[2], &fic_pcode);
      ecrire_car (' ', &fic_pcode);

      if (mne == PRC)
         ecrire_car ('\'', &fic_pcode);
      ecrire_car (param, &fic_pcode);
      if (mne == PRC)
         ecrire_car ('\'', &fic_pcode);

      if (numerotation)
         numeroter (2);

      ecrire_car ('\n', &fic_pcode);
      ligne_pcode++;
   }
}


/**
 *
 * param char_en_plus :
 */
void numeroter (int char_en_plus)
{
   char_en_plus = NB_SPACE_NUMEROTATION - char_en_plus;

   for (; char_en_plus > 0 ; char_en_plus--)
      ecrire_car (' ', &fic_pcode);

   ecrire_string (com_numero, &fic_pcode);
   ecrire_entier (ligne_pcode+1, &fic_pcode);
}
