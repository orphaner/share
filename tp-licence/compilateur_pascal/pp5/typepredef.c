/**
 * module : typepredef
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
#include <string.h>

#include "hash.h"
#include "lexical.h"
#include "syntaxique.h" /* Pr la var de table de hash !! */
#include "typepredef.h"


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/*==============================================================================
  Fonctions pour les types pr�d�finis
  ============================================================================*/
/**
 * Hashage d'un type pr�d�fini d�fini par intervalle (ex: char 0->255)
 * param letype : le type pr�d�fini � ajouter
 * param borne_inf : valeur minimum que peut prendre le scalaire
 * param borne_sup : valeur maximum que peut prendre le scalaire
 * return : le noeud cr��
 */
Arbre add_un_type_predefini (char *le_type, int borne_inf, int borne_sup)
{
   Arbre new;
   strcpy (sym, le_type); /* nopb pr sym, init � 10cases :) */
   nb_sym = strlen(le_type)+1;
   new = hash_add (&table, CLASS_TYPE, NULL);
   new->taille = 1;
   new->type = TYPE_SCALAIRE;
   new->INTER_DEBUT = borne_inf;
   new->INTER_FIN   = borne_sup;
   return new;
}


/**
 * Hashage des types pr�d�finis
 */
void add_types_predefinis ()
{
   type_predef[PREDEF_INTEGER] = add_un_type_predefini ("integer", -2^32, 2^32);
   type_predef[PREDEF_CHAR]    = add_un_type_predefini ("char", 0, 255);
   type_predef[PREDEF_BOOLEAN] = add_un_type_predefini ("boolean", 0, 1);
}
