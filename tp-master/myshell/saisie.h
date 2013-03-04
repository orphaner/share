#ifndef __saisie_h__
#define __saisie_h__
/**
 * module : saisie
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


/*------------------------------------------------------------------------------
  Types personnalisés
  ----------------------------------------------------------------------------*/
typedef enum
{
   R_NONE,
   R_PIPE,
   R_SUP,
   R_DSUP,
   R_AND_AND,
   R_PT_VIRG,
   R_OR_OR
}redirections;

typedef string * argv_list;

typedef struct _inst
{
      argv_list argv;
      int argc;
      redirections redir;
      int background;
      struct _inst *suivant;
} inst;

typedef inst *list_inst;


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
extern void perform_saisie (string prompt);
extern void free_inst ();


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/
extern list_inst linst;
extern dyn_string saisie;

#endif
