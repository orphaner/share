/**
 * module : pile_exe
 * brief : Simulation d'une pile d'instructions
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date : Février 2004
 * bug : Aucun détecté à ce jour
 *
 * Ici on simule une pile d'éxécution. La pile est représentée sous forme
 * d'un tableau dynamique. Ce tableau est alloué par buffer afin d'optimiser
 * la rapidité d'éxécution du programme. Si à la fin du remplissage de la
 * pile il reste des cases allouées mais non affectées, on réduit la taille
 * du tableau par l'appel à la procédure "pile_exe_ajust"
 *
 * Toutes les opérations de base d'une pile telle que le pop, le push ont été
 * écrites. Celà permet un accès plus rapide à des opérations répétitives
 * lors de l'interprétation. Ces fonctions n'excluent pas l'accès direct aux
 * champs de la structure. Cependant il faudra faire attention aux dépassements
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
 * Créer une pile vide
 * return : une pile vide avec un premier buffer alloué
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
 * param *p_exe : la pile d'éxécution
 * param inst : l'instruction à rajouter au sommet de la pile
 */
void push_exe (Pile_exe *pile, Instruction inst)
{
   /* Teste si la pile n'est pas pleine */
   if ( (++pile->pc + 1 ) % BUFFER_PILE_EXE == 0)
   {
      pile->taille += BUFFER_PILE_EXE;

      /* On réallou avec taille = nb buffer existant + 1 taille buffer */
      pile->inst = (Instruction *) realloc (pile->inst, pile->taille * sizeof (Instruction));

      /* Si realloc renvoi NULL alors la mémoire est saturée */
      if (!pile->inst)
         free_all (EXIT_EXE_ENLARGE);
   }

   /* Ajout de l'instruction */
   pile->inst[pile->pc] = inst;
}


/**
 * Retourne le code de l'instruction au sommet de pile
 * param *pile : la pile d'éxécution
 * return : le mnémonqiue au sommet de la pile
 */
mnemoniques get_mn_exe (Pile_exe *pile)
{
   if (pile->pc <= PILE_EXE_VIDE || pile->pc >= pile->taille)
      free_all (EXIT_EXE_OVERFLOW);

   return pile->inst[pile->pc].mn;
}


/**
 * Retourne le paramètre de l'instruction au sommet de pile
 * param *pile : la pile d'éxécution
 * return : le paramètre du mnémonique au sommet de la pile
 */
int get_param_exe (Pile_exe *pile)
{
   if (pile->pc <= PILE_EXE_VIDE || pile->pc >= pile->taille)
      free_all (EXIT_EXE_OVERFLOW);

   return pile->inst[pile->pc].param;
}


/**
 * Place le pointeur d'instruction courante à l'adresse passée en paramètre
 * param *p_exe : la pile d'éxécution
 * param adresse : Indique l'endroit où l'on doit déplacer le pointeur d'instruction courante
 */
void goto_abs_exe (Pile_exe *pile, int adresse)
{
   if (adresse > pile->taille || adresse < PILE_EXE_VIDE )
      free_all (EXIT_EXE_OVERFLOW);

   /* - 1  pour prendre en compte le ++ de passage à l'instruction suivante */
   pile->pc = adresse - 1 ;
}


/**
 * Ajoute au pointeur d'instruction courante l'adresse passée en paramètre
 * param *p_exe : la pile d'éxécution
 * param adresse : à ajouter au pointeur d'instruction courante
 */
void goto_relatif_exe (Pile_exe *pile, int adresse)
{
   adresse += pile->pc;
   if (adresse > pile->taille || adresse < PILE_EXE_VIDE )
      free_all (EXIT_EXE_OVERFLOW);

   pile->pc = adresse - 1;
}


/**
 * Place le pointeur d'instruction courante à l'instruction suivante
 * param *p_exe : la pile d'éxécution
 */
void goto_instruction_suivante (Pile_exe *pile)
{
   if (pile->pc > pile->taille)
      free_all (EXIT_EXE_OVERFLOW);

   pile->pc++;
}


/**
 * Réajuste la pile à sa taille exacte (pas multiple de BUFFER_PILE_EXE)
 * Pile.taille devient le nombre total d'intructions
 * Pile.pc se place à la première instruction (début de pile=0)
 * param *pile : la pile d'éxécution
 */
void pile_exe_ajust (Pile_exe *pile)
{
   pile->taille = pile->pc + 1;
   pile->pc = 0;
   pile->inst = (Instruction *) realloc (pile->inst, pile->taille * sizeof(Instruction));
}


/**
 * Libère la mémoire de la pile d'instruction.
 * param *pile : la pile d'éxécution
 */
void pile_exe_free (Pile_exe *pile)
{
   /* Le realloc libère la mémoire de lui-meme */
   pile->inst = (Instruction *) realloc (pile->inst,0);
   pile->taille = 0;
   pile->pc = PILE_EXE_VIDE;
}


/**
 *
 * param *pile : la pile d'éxécution
 */
void pile_exe_maj_saut (Pile_exe *pile, int depart, int arrivee)
{
   /*if (depart < pile->taille)*/
   pile->inst[depart-1].param = arrivee-1;
}
