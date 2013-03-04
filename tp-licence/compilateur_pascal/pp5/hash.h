#ifndef __hash_h__
#define __hash_h__
/**
 * module : hash
 * brief :
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date :
 * bug : Aucun d�tect� � ce jour
 *
 */


/*------------------------------------------------------------------------------
  Constantes
  ----------------------------------------------------------------------------*/
#define MAX_TABLE  1009U
#define MAX_SAUT   0
#define NOT_FOUND  -1
#define NO_INSERT  -1


/*------------------------------------------------------------------------------
  Macros fonctions
  ----------------------------------------------------------------------------*/
/* Acc�s aux infos des tableaux */
#define TAB_TAB type_info.tableau.tab
#define TAB_NB_DIM type_info.tableau.nb_dim
#define TAB_INDICE_DEBUT(dim) type_info.tableau.tab[(dim)].indice_debut
#define TAB_TAILLE(dim) type_info.tableau.tab[(dim)].taille

/* Acc�s aux infos des intervalles */
#define INTER_DEBUT type_info.intervalle.debut
#define INTER_FIN type_info.intervalle.fin

/* Acc�s aux infos des fonctions/proc�dures */
#define FUNC_RETOUR type_info.fonction.retour
#define FUNC_NB_PARAM type_info.fonction.nb_param
#define FUNC_TAB_PARAM type_info.fonction.params
#define FUNC_PARAM(dim) type_info.fonction.params[(dim)].param
#define FUNC_VAR(dim) type_info.fonction.params[(dim)].var


/*------------------------------------------------------------------------------
  Types personnalis�s
  ----------------------------------------------------------------------------*/
/* Enum�ration des classes possibles d'un identificateur */
typedef enum
{
   CLASS_PROG      = 1,
   CLASS_CONST     = 2,
   CLASS_VAR       = 4,
   CLASS_TYPE      = 8,
   CLASS_CHAMP     = 16,
   CLASS_ENS       = 32,
   CLASS_PROCEDURE = 64,
   CLASS_FONCTION  = 128
} Classes;

/* Enum�ration des types possibles d'une variable et/ou constante */
typedef enum
{
   TYPE_SCALAIRE    = 1,
   TYPE_TABLEAU     = 2,
   TYPE_RECORD      = 4,
   TYPE_CHAMP       = 8,
   TYPE_INTER       = 16,
   TYPE_BOOLEAN     = 32,
   TYPE_CHAR        = 64,
   TYPE_PARAMETRE   = 128
} Types;

/* Repr�sentation d'un param�tre des fonctions/proc�dures */
typedef struct
{
      struct _noeud *param;  /* Tableau de pointeur vers les types des param�tres */
      char var;              /* 0 si passage par valeur */
} Param;

/* Repr�sentation des fonctions/proc�dures */
typedef struct
{
      int nb_param;    /* Nb de param�tres */
      Param *params;   /* Liste des param�tres */
} Type_fonction;

/* Repr�sentation d'une dimension d'un tableau */
typedef struct
{
      int indice_debut;
      int taille;
} Dim_tableau;

/* Repr�sentation d'un tableau */
typedef struct
{
      int nb_dim;
      Dim_tableau *tab;
} Type_tableau;

/* Repr�sentation d'un champ d'un record */
typedef struct
{
      int decalage;
      struct _noeud *from_record;
} Type_champ;

/* Repr�sentation d'un intervalle */
typedef struct
{
      int debut;
      int fin;
} Type_intervalle;

/* Union pour la repr�sention des diff�rents types */
union Type_infos
{
      Type_tableau tableau;
      Type_champ champ;
      Type_intervalle intervalle;
      Type_fonction fonction;
};

/* Noeud d'un arbre */
typedef struct _noeud
{
      char *nom;
      int adresse;                /* Place occup�e danss la pile de var pcode */
      int taille;                 /* Taille occup�e en m�moire */

      Classes classe;             /* La classe (var | const | prog ...) */

      struct _noeud *type_de_base;/* Le type sur lequel le noeud se base
                                     par exemple un tableau d'entier */
      Types type;                 /* Le type (scalaire | tableau | record) */
      union Type_infos type_info; /* Repr�sentation d'un tableau ou d'un champ de record */

      struct _noeud *localite;    /* Localit� de la variable, NULL = globale */

      struct _noeud *fg, *fd;
} Noeud;

/* Arbre : pointeur sur un noeud */
typedef Noeud *Arbre;

/* Table de hash (table des symboles) */
typedef struct
{
      int offset;
      Arbre table[MAX_TABLE];
} Tablesym;


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
Arbre abr_creer_noeud (Classes, int, Arbre );
Arbre abr_add (Arbre *, Classes, int, Arbre );
Arbre abr_search (Arbre , char *, Classes, Arbre );
void abr_free (Arbre );
void abr_affiche (Arbre );


unsigned int hash (char *);
unsigned int hash_saut (unsigned int );
char symbole_egal (Arbre, Classes );
char symbole_egal2 (Arbre, Arbre);
Arbre hash_search (Tablesym *, char *, Classes );
Arbre hash_search_localite (Tablesym *t, char *s, Classes c, Arbre);
Arbre hash_add (Tablesym *, Classes, Arbre );
void hash_init (Tablesym *);
void hash_free (Tablesym *);
void hash_affiche (Tablesym *);


char symbole_egal_champ (Arbre , struct _noeud *);
Arbre hash_add_champ (Tablesym *, struct _noeud *);
Arbre abr_search_champ (Arbre abr, char *s, struct _noeud *pere);
Arbre hash_search_champ (Tablesym *t, char *s, struct _noeud *pere);


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/


#endif
