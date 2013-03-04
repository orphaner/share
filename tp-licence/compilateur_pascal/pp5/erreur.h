/**
 * module : erreur
 * brief :
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date :
 * bug : Aucun détecté à ce jour
 *
 */

#ifndef __ERREUR_H__
#define __ERREUR_H__


/*------------------------------------------------------------------------------
  Constantes
  ----------------------------------------------------------------------------*/
typedef enum
{
   EXIT_OK,
   EXIT_PCODE,
   EXIT_VAR,
   EXIT_EXE,
   EXIT_INTER,
   EXIT_PARAM,
   EXIT_FIC_READ,
   EXIT_FIC_WRITE,
   EXIT_FIC_OPEN_R,
   EXIT_FIC_OPEN_W,
   EXIT_FIC_CLOSE,
   EXIT_FIC_ERASE,
   EXIT_PS_OVER,
   ERR_MAJ_SAUT,
   ERR_ID,
   ERR_PROGRAM,
   ERR_PAR_FER,
   ERR_END,
   ERR_PT_VIRG,
   ERR_EGAL,
   ERR_BEGIN,
   ERR_IN_DECL,
   ERR_VIRG,
   ERR_IN_CONST,
   ERR_AFFEC,
   ERR_THEN,
   ERR_DO,
   ERR_IN_EXPR,
   ERR_DBL_ID,
   ERR_NO_ID,
   ERR_NUM,
   ERR_NO_AFFEC,
   ERR_NUM_DEPASS,
   ERR_DIV_ZERO,
   ERR_POINT,
   ERR_PAR_OUV,
   ERR_WRITE,
   ERR_WRITELN,
   ERR_READ,
   ERR_FOR,
   ERR_TO,
   ERR_DOWNTO,
   ERR_REPEAT,
   ERR_UNTIL,
   ERR_INCONNU, /* ZARB */
   ERR_WHILE,
   ERR_IF,
   ERR_DEUX_PT_TOKEN,
   ERR_CASE,
   ERR_OF,
   ERR_IN_DECL_RECORD,
   ERR_INTER_CROISS,
   ERR_RECORD_WAITED,
   ERR_CHAMP_WAITED,
   ERR_COMP_BOOL,
   ERR_COMP_BOOL_TO_BOOL,
   ERR_TYPE_PARAM,
   ERR_CHAMP_ACCESS,
   ERR_BOOLEAN_WAITED,
   ERR_FONCTION_TYPE_RETURN
}Erreurs;


#ifdef MESSAGE_OK
static const char  *ErreursMsg[] =
{
   "Compilation terminée avec succès",
   "",
   "",
   "",
   "",
   "Erreur dans les paramètres",
   "Erreur à la lecture du fichier",
   "Erreur à l'écriture du fichier",
   "Erreur à l'ouverture du fichier en entrée",
   "Erreur à l'ouverture du fichier en sortie",
   "Erreur à la fermeture du fichier",
   "Erreur à la suppression du fichier",
   "Stack Overflow pile sauts",
   "Erreur lors de la mise à jour d'un saut",
   "identificateur attendu",
   "PROGRAM attendu",
   ") parenthèse fermante attendue",
   "END attendu",
   "; attendu",
   "= attendu",
   "begin attendu",
   "erreur dans la partie déclaration",
   ", attendue",
   "erreur dans une constante",
   ":= attendu",
   "then attendu",
   "do attendu",
   "erreur dans un facteur (expression erronée)",
   "identificateur déclaré deux fois",
   "identificateur non déclaré",
   "nombre attendu",
   "affectation non permise",
   "constante entière dépassant les limites",
   "division par zéro",
   ". attendu",
   "( parenthèse ouvrante attendue",
   "write attendu", /* zarb 2 */
   "writeln attendu",/* zarb 2 */
   "read attendu",/* zarb 2 */
   "for attendu",/* zarb 2 */
   "to attendu",
   "downto attendu",
   "repeat attendu",/* zarb 2 */
   "until attendu",
   "erreur inconu = dans ton cul :)",
   "while attendu", /* zarb 2 */
   "if attendu", /* zarb 2 */
   ": attendu",
   "case attendu", /*zarb*/
   "of attendu",
   "erreur dans la dÃ©claration d'un record",
   "erreur, intervalle non croissant",
   "variable de type record attendue",
   "variable de type champ attendue",
   "seuls les opérateurs '=' ou '<>' autorisés pour les booléens",
   "il faut comparer un boolean avec un boolean",
   "le type effectif du paramètre n'est pas celui attendu",
   "mauvais accès à un champ de record",
   "un booléen était attendu",
   "le retour de fonction doit être un type de base (char, integer, boolean)"
};
#undef MESSAGE_OK
#endif

/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
void free_all (Erreurs );
void usage (char *, Erreurs );
void erreur_mess (Erreurs );



/*------------------------------------------------------------------------------
  Constantes
  ----------------------------------------------------------------------------*/
char corrige; /* bool : 1 = le code pascal a ete corrigé. 0 sinon. */
/* empÃ¨che la poursuite de la génération si le code est corompu */


#endif
