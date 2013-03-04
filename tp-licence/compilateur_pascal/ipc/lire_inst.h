#ifndef __LIRE_INST_H__
#define __LIRE_INST_H__

#include "lecture_fichier.h"
#include "../common/mnemonique.h"

/*------------------------------------------------------------------------------
  Constantes
  ----------------------------------------------------------------------------*/
#define MNEMO_OK     1
#define MNEMO_PARAM  2
#define MNEMO_ERREUR 3
#define MNEMO_CAR    4
#define MNEMO_STRING 5

#define CAR_COMMENT '#'
#define CAR_LIGNE   '\n'
#define CAR_TAB     '\t'
#define CAR_ESPACE  ' '
#define CAR_PLUS    '+'
#define CAR_MOINS   '-'
#define CAR_QUOTE   '\''
#define CAR_DQUOTE  '\"'



/*------------------------------------------------------------------------------
  Types personnalisés
  ----------------------------------------------------------------------------*/


typedef struct
{
      mnemoniques mn;
      int param;
}Instruction;


typedef struct
{
      Instruction *inst;
      long int pc;
      long int taille;
}Pile_exe;


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
int code_correct (int );
char trim (Fichier *);
char trim2 (Fichier *);
void suppr_commentaire (Fichier *);
int verif_fin_inst_ok (char *, Fichier *, long int);
int verif_apres_inst_ok (char *, Fichier *, long int);
int remplir_pile (Fichier , Pile_exe *);


int lire_entier (Fichier *);
void remplir_sauts (Fichier *, Pile_exe *);

#endif
