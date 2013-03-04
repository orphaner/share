/**
 * module : pile_var
 * brief : Simulation d'une pile de variables
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date : Février 2004
 * bug : Aucun détecté à ce jour
 *
 * Ici on simule une pile de variables. La pile est représentée sous forme
 * d'un tableau dynamique. Ce tableau est alloué par buffer afin d'optimiser
 * la rapidité d'éxécution du programme.
 *
 * Toutes les opérations de base d'une pile telle que le pop, le push ont été
 * écrites. Celà permet un accès plus rapide à des opérations répétitives
 * lors de l'interprétation. Ces fonctions n'excluent pas l'accès direct aux
 * champs de la structure. Cependant il faudra faire attention aux dépassements
 * de tableaux.
 *
 * Ces dépassements de tableaux était géré selon les accés a la section  variables
 * ou a la section de calcul, avec l'implémentation des fonctions, et donc la mise
 * en place de variables locales en dehors de la section des variables, ces
 * protection n'ont plus toutes un sens et se sont réduites.
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
 * Créer la pile à vide avec allocation d'un premier buffer.
 * return : une pile vide avec un premier buffer alloué
 */
Pile_var creer_pile_var ()
{
   Pile_var p_var;
   p_var.mem = (int *) malloc (BUFFER_PILE_VAR * sizeof(int));
   p_var.segment = p_var.sp = PILE_VAR_VIDE;
   return p_var;
}


/**
 * Incrémente le pointeur de pile (allocation si nécessaire).
 * param *p_var : la pile de variables
 * param c : incrément du pointeur de pile
 */
void inc_sp_var (Pile_var *p_var, int c)
{
   /* Si l'ajout de c a SP fait dépasser SP de la pile on doit réallouer */
   if ( ((p_var->sp + 1 ) % BUFFER_PILE_VAR) + c < BUFFER_PILE_VAR )
      p_var->mem = (int *) realloc (p_var->mem, ( ( ( (p_var->sp + 1) + c) / BUFFER_PILE_VAR)+ 1 ) * BUFFER_PILE_VAR  * sizeof(int));

   /* Si realloc renvoi NULL alors la mémoire est saturée */
   if (!p_var->mem)
      free_all (EXIT_VAR_ENLARGE);

   p_var->sp += c;
}


/**
 * Ajoute une valeur en sommet de pile
 * param *p_var : la pile de variables
 * param element : l'élément à rajouter au sommet
 */
void push_var (Pile_var *p_var, int element)
{
   /* Teste si la pile n'est pas pleine */
   if ( !((++p_var->sp + 1 ) % BUFFER_PILE_VAR) )
   {
      /* On réallou avec taille= taille existante (=sp) + 1 taille buffer */
      p_var->mem = (int *) realloc (p_var->mem, ( (p_var->sp + 1) + BUFFER_PILE_VAR) * sizeof(int));
      /* Si realloc renvoi NULL alors la mémoire est saturée */
      if (!p_var->mem)
         free_all (EXIT_VAR_ENLARGE);
   }
   /* ajout de l'instruction */
   p_var->mem[p_var->sp] = element;
}


/**
 * REMPLACE la valeur au sommet
 * Utilisé pour les résultats (des comparaisons / opérations).
 * param *pile : la pile de variables
 * param element : l'élement à remplacer sur le sommet
 */
void push_sommet_var (Pile_var *p_var, int element)
{
   if (p_var->sp <= p_var->segment)
      free_all (EXIT_VAR_UNDERFLOW);

   /* Ajout de la valeur */
   p_var->mem[p_var->sp] = element;
}


/**
 * Ajoute une valeur à l'adresse passée en paramètre de pile
 * param *p_var : la pile de variables
 * param adresse : l'adresse où l'on doit mettre la valeur
 * param element : l'élément à placer
 */
void push_to_var (Pile_var *p_var, int adresse, int element)
{
   if (adresse > p_var->sp || adresse <= PILE_VAR_VIDE)
      free_all (EXIT_VAR_INVALID_ADDRESS);
   p_var->mem[adresse] = element;
}


/**
 * Retourne la valeur de sommet de pile, dépile
 * param *p_var : la pile de variables
 * return : le sommet de la pile
 */
int pop_var (Pile_var *p_var)
{
   if ( p_var->sp <= p_var->segment )
      free_all (EXIT_VAR_UNDERFLOW);

   /* Si la pile est vide a plus d'une taille de buffer (min 1 buffer), on la réduit */
   if ( !(p_var->sp % BUFFER_PILE_VAR) && p_var->sp/BUFFER_PILE_VAR > 1)
      p_var->mem = (int *) realloc (p_var->mem, ((p_var->sp/BUFFER_PILE_VAR) - BUFFER_PILE_VAR) * sizeof(int));

   return p_var->mem[p_var->sp--];
}


/**
 * Retourne la valeur de sommet de pile, NE dépile PAS
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
 * Retourne la valeur de SOUS-sommet de pile, NE dépile PAS
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
 * Retourne la valeur placée à l'adresse passée en paramètre, NE dépile PAS
 * param p_var : la pile de variables
 * return : la valeur de l'adresse passé en paramètre
 */
int get_from_var (Pile_var p_var, int adresse)
{
   if( adresse > p_var.sp || adresse <= PILE_VAR_VIDE )
      free_all (EXIT_VAR_INVALID_ADDRESS);

   return p_var.mem[adresse];
}


/**
 * Libère l'espace mémoire occupé par la pile
 * param *pile : la pile de variables
 */
void pile_var_free (Pile_var *p_var)
{
   /* Le realloc libère la mémoire de lui-même */
   p_var->mem = (int *) realloc (p_var->mem,0);
}




/**
 * Assigne la variable segment dans la structure Pile_var.
 * Ne sert que pour le debugeur afin d'afficher le segment de données.
 * param *p_var : la pile de variables
 * param i : la valeur à mettre dans p->segment
 */
/* void assign_segment (Pile_var *p_var,  int i) */
/* { */
/*    p_var->segment = i - 1; */
/* } */
