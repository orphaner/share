#ifndef __PILE_VAR_H__
#define __PILE_VAR_H__

/*------------------------------------------------------------------------------
  Constantes
  ----------------------------------------------------------------------------*/
#define BUFFER_PILE_VAR 1024
#define PILE_VAR_VIDE -1


/*------------------------------------------------------------------------------
  Types personnalisés
  ----------------------------------------------------------------------------*/
typedef struct
{
      int *mem;
      int sp;
      int segment;
}Pile_var;


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
Pile_var creer_pile_var ();
void push_var (Pile_var*, int);
void push_sommet_var (Pile_var*, int);
int pop_var (Pile_var*);
void pile_var_free (Pile_var*);

int get_sommet_var (Pile_var);
int get_sous_sommet_var (Pile_var);
int get_from_var(Pile_var, int);

void push_to_var(Pile_var*, int, int);
void inc_sp_var(Pile_var*, int);


#endif
