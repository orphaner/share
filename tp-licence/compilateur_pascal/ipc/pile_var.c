/**
 * module : pile_var
 * brief : Simulation d'une pile de variables
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date : F��vrier 2004
 * bug : Aucun d��tect�� �� ce jour
 *
 * Ici on simule une pile de variables. La pile est repr��sent��e sous forme
 * d'un tableau dynamique. Ce tableau est allou�� par buffer afin d'optimiser
 * la rapidit�� d'��x��cution du programme.
 *
 * Toutes les op��rations de base d'une pile telle que le pop, le push ont ��t��
 * ��crites. Cel�� permet un acc��s plus rapide �� des op��rations r��p��titives
 * lors de l'interpr��tation. Ces fonctions n'excluent pas l'acc��s direct aux
 * champs de la structure. Cependant il faudra faire attention aux d��passements
 * de tableaux.
 *
 * Ces d��passements de tableaux ��tait g��r�� selon les acc��s a la section  variables
 * ou a la section de calcul, avec l'impl��mentation des fonctions, et donc la mise
 * en place de variables locales en dehors de la section des variables, ces
 * protection n'ont plus toutes un sens et se sont r��duites.
 */


/*------------------------------------------------------------------------------
  Fichiers include
  ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include "erreur.h"
#include "lire_inst.h"
#include "pile_var.h"


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/**
 * Cr��er la pile �� vide avec allocation d'un premier buffer.
 * return : une pile vide avec un premier buffer allou��
 */
Pile_var creer_pile_var ()
{
   Pile_var p_var;
   p_var.mem = (int *) malloc (BUFFER_PILE_VAR * sizeof(int));
   p_var.segment = p_var.sp = PILE_VAR_VIDE;
   return p_var;
}


/**
 * Incr��mente le pointeur de pile (allocation si n��cessaire).
 * param *p_var : la pile de variables
 * param c : incr��ment du pointeur de pile
 */
void inc_sp_var (Pile_var *p_var, int c)
{
   /* Si l'ajout de c a SP fait d��passer SP de la pile on doit r��allouer */
   if ( ((p_var->sp + 1 ) % BUFFER_PILE_VAR) + c < BUFFER_PILE_VAR )
      p_var->mem = (int *) realloc (p_var->mem, ( ( ( (p_var->sp + 1) + c) / BUFFER_PILE_VAR)+ 1 ) * BUFFER_PILE_VAR  * sizeof(int));

   /* Si realloc renvoi NULL alors la m��moire est satur��e */
   if (!p_var->mem)
      free_all (EXIT_VAR_ENLARGE);

   p_var->sp += c;
}


/**
 * Ajoute une valeur en sommet de pile
 * param *p_var : la pile de variables
 * param element : l'��l��ment �� rajouter au sommet
 */
void push_var (Pile_var *p_var, int element)
{
   /* Teste si la pile n'est pas pleine */
   if ( !((++p_var->sp + 1 ) % BUFFER_PILE_VAR) )
   {
      /* On r��allou avec taille= taille existante (=sp) + 1 taille buffer */
      p_var->mem = (int *) realloc (p_var->mem, ( (p_var->sp + 1) + BUFFER_PILE_VAR) * sizeof(int));
      /* Si realloc renvoi NULL alors la m��moire est satur��e */
      if (!p_var->mem)
         free_all (EXIT_VAR_ENLARGE);
   }
   /* ajout de l'instruction */
   p_var->mem[p_var->sp] = element;
}


/**
 * REMPLACE la valeur au sommet
 * Utilis�� pour les r��sultats (des comparaisons / op��rations).
 * param *pile : la pile de variables
 * param element : l'��lement �� remplacer sur le sommet
 */
void push_sommet_var (Pile_var *p_var, int element)
{
   if (p_var->sp <= p_var->segment)
      free_all (EXIT_VAR_UNDERFLOW);

   /* Ajout de la valeur */
   p_var->mem[p_var->sp] = element;
}


/**
 * Ajoute une valeur �� l'adresse pass��e en param��tre de pile
 * param *p_var : la pile de variables
 * param adresse : l'adresse o�� l'on doit mettre la valeur
 * param element : l'��l��ment �� placer
 */
void push_to_var (Pile_var *p_var, int adresse, int element)
{
   if (adresse > p_var->sp || adresse <= PILE_VAR_VIDE)
      free_all (EXIT_VAR_INVALID_ADDRESS);
   p_var->mem[adresse] = element;
}


/**
 * Retourne la valeur de sommet de pile, d��pile
 * param *p_var : la pile de variables
 * return : le sommet de la pile
 */
int pop_var (Pile_var *p_var)
{
   if ( p_var->sp <= p_var->segment )
      free_all (EXIT_VAR_UNDERFLOW);

   /* Si la pile est vide a plus d'une taille de buffer (min 1 buffer), on la r��duit */
   if ( !(p_var->sp % BUFFER_PILE_VAR) && p_var->sp/BUFFER_PILE_VAR > 1)
      p_var->mem = (int *) realloc (p_var->mem, ((p_var->sp/BUFFER_PILE_VAR) - BUFFER_PILE_VAR) * sizeof(int));

   return p_var->mem[p_var->sp--];
}


/**
 * Retourne la valeur de sommet de pile, NE d��pile PAS
 * param p_var : la pile de variables
 * return : le sommet de la pile
 */
int get_sommet_var (Pile_var p_var)
{
   if ( p_var.sp <= p_var.segment )
      free_all (EXIT_VAR_OUT);

   return p_var.mem[p_var.sp];
}


/**
 * Retourne la valeur de SOUS-sommet de pile, NE d��pile PAS
 * param p_var : la pile de variables
 * return : le sous-sommet de la pile
 */
int get_sous_sommet_var (Pile_var p_var)
{
   /* evite l'acces au secteur de stokage des variables */
   if ( p_var.sp + 1 <= p_var.segment )
      free_all (EXIT_VAR_OUT);

   return p_var.mem[p_var.sp - 1 ];
}


/**
 * Retourne la valeur plac��e �� l'adresse pass��e en param��tre, NE d��pile PAS
 * param p_var : la pile de variables
 * return : la valeur de l'adresse pass�� en param��tre
 */
int get_from_var (Pile_var p_var, int adresse)
{
   if( adresse > p_var.sp || adresse <= PILE_VAR_VIDE )
      free_all (EXIT_VAR_INVALID_ADDRESS);

   return p_var.mem[adresse];
}


/**
 * Lib��re l'espace m��moire occup�� par la pile
 * param *pile : la pile de variables
 */
void pile_var_free (Pile_var *p_var)
{
   /* Le realloc lib��re la m��moire de lui-m��me */
   p_var->mem = (int *) realloc (p_var->mem,0);
}




/**
 * Assigne la variable segment dans la structure Pile_var.
 * Ne sert que pour le debugeur afin d'afficher le segment de donn��es.
 * param *p_var : la pile de variables
 * param i : la valeur �� mettre dans p->segment
 */
/* void assign_segment (Pile_var *p_var,  int i) */
/* { */
/*    p_var->segment = i - 1; */
/* } */
