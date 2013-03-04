#ifndef __varloc_h__
#define __varloc_h__
/**
 * module : varloc
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
extern void varloc_remplir ();
extern void varloc_affiche (Tablesym *t);
extern void myset (int argc, char **argv);
extern int myset2 (int argc, char **argv);



/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/
extern Tablesym varloc;

#endif
