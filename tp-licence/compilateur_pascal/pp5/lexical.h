/**
 * module : lexical
 * brief :
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date :
 * bug : Aucun détecté à ce jour
 *
 */

#ifndef __lexical_h__
#define __lexical_h__


/*------------------------------------------------------------------------------
  Fichiers include
  ----------------------------------------------------------------------------*/
#include "lecture_fichier.h"


/*------------------------------------------------------------------------------
  Constantes
  ----------------------------------------------------------------------------*/
#define CAR_COMMENT    '#'
#define CAR_LIGNE      '\n'
#define CAR_TAB        '\t'
#define CAR_ESPACE     ' '
#define CAR_PLUS       '+'
#define CAR_MOINS      '-'
#define CAR_MUL        '*'
#define CAR_DIV        '/'
#define CAR_EGAL       '='
#define CAR_INF        '<'
#define CAR_SUP        '>'
#define CAR_PAR_OUV    '('
#define CAR_PAR_FER    ')'
#define CAR_ACC_OUV    '{'
#define CAR_ACC_FER    '}'
#define CAR_CRO_OUV    '['
#define CAR_CRO_FER    ']'
#define CAR_POINT      '.'
#define CAR_PT_VIRG    ';'
#define CAR_VIRG       ','
#define CAR_2PT        ':'
#define CAR_UNDER      '_'
#define CAR_QUOTE      '\''

#define BUFFER_SYM      5


#define MN_D_PROGRAM   1735357040
#define MN_F_PROGRAM   7168370

#define MN_D_DOWNTO    1853321060
#define MN_F_DOWNTO    28532

#define MN_D_REPEAT    1701864818
#define MN_F_REPEAT    29793

#define MN_D_RECORD    1868785010
#define MN_F_RECORD    25714

#define MN_D_OTHERWISE 1701344367
#define MN_F_OTHERWISE 1936291698

#define MN_D_PROCEDURE 1668248176
#define MN_F_PROCEDURE 1920296037

#define MN_D_FUNCTION  1668183398
#define MN_F_FUNCTION  1852795252

#define MN_F_WRITELN   7236709

#define MN_D_CONST     1936617315
#define MN_D_BEGIN     1768383842
#define MN_D_WHILE     1818847351
#define MN_D_WRITE     1953067639
#define MN_D_UNTIL     1769238133
#define MN_D_ARRAY     1634890337
#define MN_D_FALSE     1936482662

#define MN_VAR         7496054
#define MN_END         6581861
#define MN_IF          26217
#define MN_THEN        1852139636
#define MN_DO          28516
#define MN_READ        1684104562
#define MN_FOR         7499622
#define MN_STEP        1885697139
#define MN_TO          28532
#define MN_CASE        1702060387
#define MN_ELSE        1702063205
#define MN_OF          26223
#define MN_TYPE        1701869940

#define MN_TRUE        1702195828


/*------------------------------------------------------------------------------
  Types personnalisés
  ----------------------------------------------------------------------------*/
typedef enum
{
   ID_TOKEN,
   NUM_TOKEN,
   PLUS_TOKEN,
   MOINS_TOKEN,
   MUL_TOKEN,
   DIV_TOKEN,
   EGAL_TOKEN,
   DIFF_TOKEN,
   INF_TOKEN,
   SUP_TOKEN,
   INF_EGAL_TOKEN,
   SUP_EGAL_TOKEN,
   PAR_OUV_TOKEN,
   PAR_FER_TOKEN,
   VIRG_TOKEN,
   PT_VIRG_TOKEN,
   POINT_TOKEN,
   AFFEC_TOKEN,
   BEGIN_TOKEN,
   END_TOKEN,
   IF_TOKEN,
   WHILE_TOKEN,
   THEN_TOKEN,
   DO_TOKEN,
   WRITE_TOKEN,
   WRITELN_TOKEN,
   READ_TOKEN,
   CONST_TOKEN,
   VAR_TOKEN,
   PROGRAM_TOKEN,
   TOKEN_INCONNU,
   FOR_TOKEN,
   TO_TOKEN,
   DOWNTO_TOKEN,
   REPEAT_TOKEN,
   UNTIL_TOKEN,
   STEP_TOKEN,
   CASE_TOKEN,
   OF_TOKEN,
   ELSE_TOKEN,
   OTHERWISE_TOKEN,
   PT_PT_TOKEN,
   DEUX_PT_TOKEN,
   QUOTE_TOKEN,
   CRO_OUV_TOKEN,
   CRO_FER_TOKEN,
   TYPE_TOKEN,
   ARRAY_TOKEN,
   RECORD_TOKEN,
   TRUE_TOKEN,
   FALSE_TOKEN,
   PROCEDURE_TOKEN,
   FUNCTION_TOKEN
}Tokens;


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
int next_char ();
void next_token ();
void lex_init ();
char lire_car_min ();
void unget_token (Tokens t);


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/
Tokens token;
char *sym;
int taille_sym;
int nb_sym;
int valeur;
Fichier fichier_pascal;

#endif
