/**
 * module : syntaxique
 * brief :
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date :
 * bug : Aucun d�tect� � ce jour
 *
 */

#ifndef __syntaxique_h__
#define __syntaxique_h__


/*------------------------------------------------------------------------------
  Fichiers include
  ----------------------------------------------------------------------------*/
#include "lexical.h"
#include "hash.h"
#include "erreur.h"


/*------------------------------------------------------------------------------
  Constantes
  ----------------------------------------------------------------------------*/
#define PAS_EXISTE 1
#define PAS_PAS 0


/*------------------------------------------------------------------------------
  Types personnalis�s
  ----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/

void erreur(Erreurs);
char teste (Tokens );
Arbre teste_et_entre (Tokens , Classes , ...);
Arbre teste_et_cherche (Tokens , Classes , ...);

void program ();
void block ();

/* D�claration des vars / const / types */
void consts (Arbre);
int vars (Arbre);
int decl (Arbre);
Arbre creer_anonyme (Tokens );
Arbre type ();
void types ();
void typedecl ();
void type_array (Arbre );
void dim (Arbre );
void type_intervalle (Arbre );
void type_record (Arbre );
char champs (Arbre );


/* Proc�dures */
void paramdecl ();
void paramlist ();
void procs ();
void funcs ();
void paramefflist (Arbre );
void appel ();

/* Ex�cution d'instructions */
void insts ();
void inst ();

/* Acc�s aux variables */
Arbre var2 (Arbre );
Arbre var (Classes );

/* Affectations */
void affec ();

/* if / else */
void si ();

/* Entr�es / sorties */
void ecrire (char );
void mes ();
void lire_une_var (Arbre );
void lire ();

/* Conditions */
Tokens cond_fin_boolean (Arbre );
void cond_lire_un_car ();
char verif_token_comp ();
void cond ();

/* Expressions non calcul�es */
Arbre expr ();
Arbre term ();
Arbre fact ();

/* Expressions calcul�es */
int expr_const ();
int term_const ();
int fact_const ();

/* While */
void tantque ();

/* For */
void pour ();
int pas ();

/* do .. while */
void repeat ();

/* switch */
void cas ();
void elsecase ();
void label ();
void nums ();



/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/
Tablesym table; /* D�clar� dans le syntaxique car la table des symboles n'a rien
                   � voir avec le lexical */
int next_anonyme;
char *next_anonyme_string;

#endif
