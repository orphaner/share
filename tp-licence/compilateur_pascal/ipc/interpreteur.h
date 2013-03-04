#ifndef __INTERPRETEUR_H__
#define __INTERPRETEUR_H__


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
void add (Pile_var*);
void sub (Pile_var*);
void mul (Pile_var*);
void division (Pile_var*);
void eql (Pile_var*);
void neq (Pile_var*);
void gtr (Pile_var*);
void lss (Pile_var*);
void geq (Pile_var*);
void leq (Pile_var*);

void inn (Pile_var*);
void inc (Pile_var*);
void inb (Pile_var*);
void inc_sp (Pile_var*, Pile_exe*);
void ldi (Pile_var*, Pile_exe*);
void lda (Pile_var*, Pile_exe*);
void ldv (Pile_var*);
void sto (Pile_var*);
void brn (Pile_exe *p_exe);
void brr (Pile_exe *p_exe);
void bze (Pile_var*, Pile_exe*);
void bzr (Pile_var*, Pile_exe*);

void cal (Pile_var*, Pile_exe*, Pile_var*);
void ret (Pile_var*, Pile_exe*, Pile_var*);

void cp (Pile_var*);
void cpj(Pile_var*);
void del(Pile_var*);

void prn (Pile_var *, char );
void prb (Pile_var *, char );
void pri (Pile_var , char );
void prc (Pile_exe *, char );
void prl (char );

void erreur_fatale_inter (char []);
void affiche_debug (Pile_var *, Pile_exe *);
void execute (Pile_var*, Pile_exe*, Pile_var*, char );


#endif
