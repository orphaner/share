#ifndef __list_fg_h__
#define __list_fg_h__
/**
 * module : list_fg
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
typedef struct _maillon
{
      pid_t pid;
      char * nom_cmd;
      int pos;
      struct _maillon * suivant;
} maillon;

typedef maillon * list_fg;



/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
extern list_fg list_fg_init (list_fg list);
extern list_fg list_fg_free (list_fg list);

extern list_fg list_fg_suppr (list_fg list, pid_t const pid);
extern list_fg list_fg_add (list_fg list, pid_t const pid);
extern void list_fg_aff (list_fg const list);
extern pid_t list_fg_get_pos (list_fg const list, int const pos);


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/
extern list_fg list;

#endif
