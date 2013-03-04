#ifndef __PILE_EXE_H__
#define __PILE_EXE_H__


#include "lire_inst.h"


/*------------------------------------------------------------------------------
  Constantes
  ----------------------------------------------------------------------------*/
#define BUFFER_PILE_EXE 1024
#define PILE_EXE_VIDE -1


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
Pile_exe creer_pile_exe ();

void push_exe (Pile_exe*, Instruction);
mnemoniques get_mn_exe (Pile_exe*);
int get_param_exe (Pile_exe*);
void goto_abs_exe (Pile_exe*, int);
void goto_relatif_exe (Pile_exe*, int);
void goto_instruction_suivante (Pile_exe*);

void pile_exe_ajust (Pile_exe*);
void pile_exe_free (Pile_exe*);

void pile_exe_maj_saut (Pile_exe *, int , int );


#endif
