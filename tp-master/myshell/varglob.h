#ifndef __varglob_h__
#define __varglob_h__
/**
 * module : varglob
 * brief : 
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date : 
 * bug : Aucun d�tect� � ce jour
 *
 */

/*------------------------------------------------------------------------------
  Constantes
  ----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
  Types personnalis�s
  ----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
extern void varglob_init ();
extern void mysetenv (int argc, char **argv);
extern int mysetenv2 (int argc, char ** argv);
extern char * varglob_get (char * nom);
extern void varglob_free ();


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/


#endif
