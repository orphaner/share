#ifndef __hash_h__
#define __hash_h__
/**
 * module : hash
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
#define MAX_TABLE  1009U
#define MAX_SAUT   10


/*------------------------------------------------------------------------------
  Types personnalisés
  ----------------------------------------------------------------------------*/

/* Noeud d'un arbre */
typedef struct _noeud
{
      char *nom;
      char * valeur;
      struct _noeud *fg, *fd;
} Noeud;

/* Arbre : pointeur sur un noeud */
typedef Noeud *Arbre;

/* Table de hash (table des symboles) */
typedef struct
{
      int taille;
      Arbre table[MAX_TABLE];
} Tablesym;



/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/

extern Arbre hash_search (Tablesym * t, char * nom);
extern Arbre hash_add (Tablesym * t, char * nom, char * valeur);
extern Arbre hash_modif (Tablesym * t, char * nom, char * valeur);
extern void hash_init (Tablesym *t);
extern void hash_free (Tablesym *t);
extern void hash_affiche (Tablesym *t);
extern unsigned int hash2 (char *s, unsigned int const max_table);



/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/


#endif
