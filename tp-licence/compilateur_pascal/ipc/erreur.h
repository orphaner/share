#ifndef __ERREUR_H__
#define __ERREUR_H__


/*------------------------------------------------------------------------------
  Constantes
  ----------------------------------------------------------------------------*/

/* D�finition des codes de retour d'erreur */
typedef enum
{
   EXIT_OK,
   EXIT_PCODE,
   EXIT_VAR,
   EXIT_EXE_ENLARGE,
   EXIT_EXE_OVERFLOW,
   EXIT_VAR_ENLARGE,
   EXIT_VAR_ENLARGE_MEM,
   EXIT_VAR_UNDERFLOW,
   EXIT_VAR_OVERFLOW,
   EXIT_VAR_INVALID_ADDRESS,
   EXIT_VAR_EMPTY,
   EXIT_VAR_OUT,
   EXIT_INTER,
   EXIT_MISSING_PARAM,
   EXIT_UNKNOW_PARAM,
   EXIT_FIC_READ,
   EXIT_FIC_WRITE,
   EXIT_FIC_OPEN_R,
   EXIT_FIC_OPEN_W,
   EXIT_FIC_CLOSE,
   EXIT_FIC_ERASE,
   EXIT_PS_OVER,
   ERR_END_INST,
   ERR_UNKNOW_CAR,
   ERR_MISSING_SPACE,
   ERR_MISSING_PARAM,
   ERR_MISSING_OPEN_DQUOTE,
   ERR_MISSING_CLOSE_DQUOTE,
   ERR_MISSING_OPEN_QUOTE,
   ERR_MISSING_CLOSE_QUOTE,
   ERR_UNKNOW_MNEMO
}Erreurs;


#ifdef MESSAGE_OK
static const char  *ErreursMsg[] =
{
   "Interpr�tation termin�e avec succ�s",
   "Fichier de pcode non correct. Arret du programme.",
   "",
   "Stack Overflow. La pile instruction n'a pu etre agrandie",
   "Stack Overflow. D�passement de la pile d'instruction",
   "Stack Overflow. La pile d'ex�cution n'a pu etre agrandie",
   "Impossible d'incr�menter la zone m�moire de la pile",
   "Stack Underflow. D�passement de pile",
   "Stack Overflow. D�passement de pile",
   "Stack Overflow. Adresse invalide",
   "Stack Underflow. Pile vide",
   "Stack Underflow. Acc�s hors pile",
   "Interpr�teur: division par z�ro",
   "Missing parameter for %s\n",
   "Option '%s' non reconnue.\n",
   "Erreur � la lecture du fichier",
   "",
   "Erreur � l'ouverture du fichier en entr�e",
   "",
   "Erreur � la fermeture du fichier",
   "",
   "",
   "End of instruction waited",
   "Carat�re non reconnu en fin d'instruction",
   "Pas d'espace avant le param�tre",
   "Missing parameter",
   "missing open \"",
   "missing close \"",
   "missing open \'",
   "missing close \'",
   "Mn�monique non reconnu"
};
#undef MESSAGE_OK
#endif


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
void erreur (int code_erreur, long int num_ligne);
void free_all (int );
void usage (char *, Erreurs, char *);


#endif
