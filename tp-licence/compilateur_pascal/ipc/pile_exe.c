/**
 * module : pile_exe
 * brief : Simulation d'une pile d'instructions
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date : F�vrier 2004
 * bug : Aucun d�tect� � ce jour
 *
 * Ici on simule une pile d'�x�cution. La pile est repr�sent�e sous forme
 * d'un tableau dynamique. Ce tableau est allou� par buffer afin d'optimiser
 * la rapidit� d'�x�cution du programme. Si � la fin du remplissage de la
 * pile il reste des cases allou�es mais non affect�es, on r�duit la taille
 * du tableau par l'appel � la proc�dure "pile_exe_ajust"
 *
 * Toutes les op�rations de base d'une pile telle que le pop, le push ont �t�
 * �crites. Cel� permet un acc�s plus rapide � des op�rations r�p�titives
 * lors de l'interpr�tation. Ces fonctions n'excluent pas l'acc�s direct aux
 * champs de la structure. Cependant il faudra faire attention aux d�passements
 * de tableaux.
 */


/*------------------------------------------------------------------------------
  Fichiers include
  ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include "../common/mnemonique.h"
#include "erreur.h"
#include "lire_inst.h"
#include "pile_exe.h"


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/**
 * Cr�er une pile vide
 * return : une pile vide avec un premier buffer allou�
 */
Pile_exe creer_pile_exe ()
{
   Pile_exe pile;
   pile.inst = (Instruction *) malloc (BUFFER_PILE_EXE * sizeof (Instruction));
   pile.pc = PILE_EXE_VIDE;
   pile.taille = BUFFER_PILE_EXE;
   return pile;
}


/**
 * Rajoute une instruction au sommet de pile
 * param *p_exe : la pile d'�x�cution
 * param inst : l'instruction � rajouter au sommet de la pile
 */
void push_exe (Pile_exe *pile, Instruction inst)
{
   /* Teste si la pile n'est pas pleine */
   if ( (++pile->pc + 1 ) % BUFFER_PILE_EXE == 0)
   {
      pile->taille += BUFFER_PILE_EXE;

      /* On r�allou avec taille = nb buffer existant + 1 taille buffer */
      pile->inst = (Instruction *) realloc (pile->inst, pile->taille * sizeof (Instruction));

      /* Si realloc renvoi NULL alors la m�moire est satur�e */
      if (!pile->inst)
         free_all (EXIT_EXE_ENLARGE);
   }

   /* Ajout de l'instruction */
   pile->inst[pile->pc] = inst;
}


/**
 * Retourne le code de l'instruction au sommet de pile
 * param *pile : la pile d'�x�cution
 * return : le mn�monqiue au sommet de la pile
 */
mnemoniques get_mn_exe (Pile_exe *pile)
{
   if (pile->pc <= PILE_EXE_VIDE || pile->pc >= pile->taille)
      free_all (EXIT_EXE_OVERFLOW);

   return pile->inst[pile->pc].mn;
}


/**
 * Retourne le param�tre de l'instruction au sommet de pile
 * param *pile : la pile d'�x�cution
 * return : le param�tre du mn�monique au sommet de la pile
 */
int get_param_exe (Pile_exe *pile)
{
   if (pile->pc <= PILE_EXE_VIDE || pile->pc >= pile->taille)
      free_all (EXIT_EXE_OVERFLOW);

   return pile->inst[pile->pc].param;
}


/**
 * Place le pointeur d'instruction courante � l'adresse pass�e en param�tre
 * param *p_exe : la pile d'�x�cution
 * param adresse : Indique l'endroit o� l'on doit d�placer le pointeur d'instruction courante
 */
void goto_abs_exe (Pile_exe *pile, int adresse)
{
   if (adresse > pile->taille || adresse < PILE_EXE_VIDE )
      free_all (EXIT_EXE_OVERFLOW);

   /* - 1  pour prendre en compte le ++ de passage � l'instruction suivante */
   pile->pc = adresse - 1 ;
}


/**
 * Ajoute au pointeur d'instruction courante l'adresse pass�e en param�tre
 * param *p_exe : la pile d'�x�cution
 * param adresse : � ajouter au pointeur d'instruction courante
 */
void goto_relatif_exe (Pile_exe *pile, int adresse)
{
   adresse += pile->pc;
   if (adresse > pile->taille || adresse < PILE_EXE_VIDE )
      free_all (EXIT_EXE_OVERFLOW);

   pile->pc = adresse - 1;
}


/**
 * Place le pointeur d'instruction courante � l'instruction suivante
 * param *p_exe : la pile d'�x�cution
 */
void goto_instruction_suivante (Pile_exe *pile)
{
   if (pile->pc > pile->taille)
      free_all (EXIT_EXE_OVERFLOW);

   pile->pc++;
}


/**
 * R�ajuste la pile � sa taille exacte (pas multiple de BUFFER_PILE_EXE)
 * Pile.taille devient le nombre total d'intructions
 * Pile.pc se place � la premi�re instruction (d�but de pile=0)
 * param *pile : la pile d'�x�cution
 */
void pile_exe_ajust (Pile_exe *pile)
{
   pile->taille = pile->pc + 1;
   pile->pc = 0;
   pile->inst = (Instruction *) realloc (pile->inst, pile->taille * sizeof(Instruction));
}


/**
 * Lib�re la m�moire de la pile d'instruction.
 * param *pile : la pile d'�x�cution
 */
void pile_exe_free (Pile_exe *pile)
{
   /* Le realloc lib�re la m�moire de lui-meme */
   pile->inst = (Instruction *) realloc (pile->inst,0);
   pile->taille = 0;
   pile->pc = PILE_EXE_VIDE;
}


/**
 *
 * param *pile : la pile d'�x�cution
 */
void pile_exe_maj_saut (Pile_exe *pile, int depart, int arrivee)
{
   /*if (depart < pile->taille)*/
   pile->inst[depart-1].param = arrivee-1;
}
