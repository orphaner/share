/**
 * module : mnemonique
 * brief : 
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date : 
 * bug : Aucun d�tect� � ce jour
 *
 */


/*------------------------------------------------------------------------------
  Fichiers include
  ----------------------------------------------------------------------------*/
#include <stdio.h>

#include "mnemonique.h"


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/**
 * param mne : le code du menonique
 * return la chaine de caract�re correspondant au mn�monique sous forme int
 */
void revert_code (mnemoniques mne, char *inst)
{
   inst[0] = mne;
   inst[1] = (mne >> 8) & 255;
   inst[2] = mne >> 16;
   inst[3] = '\0';
}
