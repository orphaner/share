#ifndef __generation_h__
#define __generation_h__
/**
 * module : generation
 * brief :
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date :
 * bug : Aucun détecté à ce jour
 *
 */

#include "../common/mnemonique.h"
/*------------------------------------------------------------------------------
  Constantes
  ----------------------------------------------------------------------------*/
#define PILE_SAUT_BUFFER  255
#define PILE_SAUT_VIDE    -1

#define NB_SPACE_NUMEROTATION 5


/*------------------------------------------------------------------------------
  Types personnalisés
  ----------------------------------------------------------------------------*/
/* Un saut de pcode : le départ et l'arrivée */
typedef struct
{
      int depart;
      int arrivee;
} Saut;

/* Ensemble des sauts du programme pcode */
typedef struct
{
      Saut *liste;
      unsigned int nb;
}Pile_saut;


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
void pile_saut_init (Pile_saut *);
int pile_saut_push (Pile_saut *, int );
void pile_saut_update (Pile_saut *, unsigned int , int );
void pile_saut_to_file (Pile_saut *);
void pile_saut_free (Pile_saut *);
void generer1 (mnemoniques);
void generer2 (mnemoniques, char*);
void generer_com (char*);
void generer2i (mnemoniques , int );
void generer2c (mnemoniques , char );

void numeroter (int );


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/
Fichier fic_pcode; /* Le fichier de sortie */
Pile_saut sauts; /* La liste des sauts du pcode */
int ligne_pcode;


#endif
