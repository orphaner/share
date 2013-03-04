#ifndef __typepredef_h__
#define __typepredef_h__
/**
 * module : typepredef
 * brief :
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date :
 * bug : Aucun détecté à ce jour
 *
 */

/*------------------------------------------------------------------------------
  Constantes
  ----------------------------------------------------------------------------*/
#define EST_PRIMITIF(id) ((id) == type_predef[PREDEF_INTEGER] || (id) == type_predef[PREDEF_BOOLEAN] || (id) == type_predef[PREDEF_CHAR])

/*------------------------------------------------------------------------------
  Types personnalisés
  ----------------------------------------------------------------------------*/
typedef enum
{
   PREDEF_INTEGER = 0,
   PREDEF_CHAR    = 1,
   PREDEF_BOOLEAN = 2
}Predefs;


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
Arbre add_un_type_predefini (char *, int , int );
void add_types_predefinis ();


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/
Arbre type_predef[3];

#endif
