#ifndef __output_h__
#define __output_h__

/**
 * module : 
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


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/


extern void affiche_ps ();
extern void affiche_ps_stat (Stat_ps);
extern void affiche_ps_simple2 (Stat_ps stat);

extern void usage_ps ();
extern void erreur_bad_arg(char*);
extern void version_ps();


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/


#endif
