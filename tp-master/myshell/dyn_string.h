#ifndef __dyn_string_h__
#define __dyn_string_h__

/**
 * module : dyn_string
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
/* Chaine de caractères dynamiques */
typedef struct
{
      string chaine;  /* La chaine de caractère en elle même */
      int taille;     /* La taille de la chaine de caractères */
      int nb_realloc; /* Le nombre de case allouées */
      int parcours;   /* Endroit dans le parcours de la chaine */
} dyn_string;


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
extern void dyn_string_init (dyn_string *s);
extern void dyn_string_init2 (dyn_string *s);
extern void dyn_string_free (dyn_string *s);
extern void dyn_string_aff_infos (dyn_string s);
extern void dyn_string_add_char (dyn_string *s, char const c);
extern string dyn_string_final (dyn_string *s);
extern void dyn_string_saisie (dyn_string *s);
extern char dyn_string_lire_car (dyn_string *s);
extern char dyn_string_lire_car2 (dyn_string *s);
extern char dyn_string_last_car (dyn_string *s);
extern void dyn_string_remise_car (dyn_string *s);
extern void dyn_string_trim (dyn_string  *s);


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/


#endif
