/**
 * module : lexical
 * brief :
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date :
 * bug : Aucun détecté à ce jour
 *
 */


/*------------------------------------------------------------------------------
  Fichiers include
  ----------------------------------------------------------------------------*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lecture_fichier.h"
#include "librairie.h"
#include "lexical.h"

/*#define DEBUG 1*/

/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/
Tokens unget;
char tounget;


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/**
 *
 */


/**
 * Passe tous les blancs (espaces et tabulations). Cette fonction impose de
 * rencontrer 1+n blanc, avec 0 < n < +infini .
 * Le dernier caractère non blanc lu est remi dans le flux d'entrée.
 * param *fic : le fichier à lire
 * return 1 si au moins un blanc est trouvé. 0 sinon.
 */
int next_char ()
{
   char c = lire_car_min (); /* Le caractère lu */
   int retour;              /* Le code de retour pr le return */

   /* Aucun blanc n'est recontré -> erreur */
   if ( c != FIN_FICHIER && ( c != CAR_ESPACE && c != CAR_TAB && c != CAR_LIGNE))
      retour = 0;

   /* Un blanc est rencontré. On passe tous les blancs suivants. */
   else
   {
      while ( c != FIN_FICHIER && ( c == CAR_ESPACE || c == CAR_TAB || c == CAR_LIGNE) )
         c = lire_car_min ();
      retour = 1;
   }

   /* Remise dans le flux du dernier caractère non blanc */
   insere_car (&fichier_pascal, c);

   return retour;
}


/**
 * Permet de lire tous les caractères comme des minuscules
 * return : le caractère suivant en minucscule dans le fichier
 */
char lire_car_min ()
{
   /*char c = lire_car (&fichier_pascal);*/
   return tolower (lire_car (&fichier_pascal));
}


/**
 *
 */
Tokens identifiant (char c)
{
   /* Un espace est réservé dans SYM opur le caractere de terminaison */

   int i = 0;
   while (c != FIN_FICHIER && (isalnum(c) || c == CAR_UNDER))
   {
      if (i >= taille_sym)
      {
         sym = (char *) realloc (sym, (taille_sym + BUFFER_SYM) * sizeof(char));
         taille_sym += BUFFER_SYM;
      }
      sym[i++] = c;
      c = lire_car_min ();
   }
   if (i >= taille_sym)
   {
      sym = (char *) realloc (sym, (taille_sym + BUFFER_SYM) * sizeof(char));
      taille_sym += BUFFER_SYM;
   }
   sym[i++] = '\0';
   nb_sym = i;

   insere_car (&fichier_pascal, c);
   return ID_TOKEN;
}


/**
 *
 */
void fin_identifiant (char c, int i)
{
   while (c != FIN_FICHIER && (isalnum(c) || c == CAR_UNDER))
   {
      if (i >= taille_sym)
      {
         sym = (char *) realloc (sym, (taille_sym + BUFFER_SYM) * sizeof(char));
         taille_sym += BUFFER_SYM;
      }
      sym[i++] = c;
      c = lire_car_min ();
   }
   if (i >= taille_sym)
   {
      sym = (char *) realloc (sym, (taille_sym + BUFFER_SYM) * sizeof(char));
      taille_sym += BUFFER_SYM;
   }
   sym[i++] = '\0';
   nb_sym = i;

   insere_car (&fichier_pascal, c);
}


/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * A généraliser encore !!!!!!!!!!!!!!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */
int fin_token (char c, int code, int fin)
{
   int res = 0;
   int decal = 0;
   int i = 4;
   do
   {
      sym[i++] = c;
      res = res | (c << decal);
      decal += 8;
      c = lire_car_min ();
   } while (i < 8 && (isalnum(c) || c == CAR_UNDER));

   if ((res == code && fin == 1 && (!isalnum(c) && c != CAR_UNDER))
       ||(res == code && fin == 0))
   {
      insere_car(&fichier_pascal,c);
      return 1;
   }
   else
   {
      fin_identifiant(c,i);
      return 0;
   }
}


/**
 *
 */
int fin_token_uniq (char c, char lafin, int i, Tokens letoken)
{
   char cs;
   cs = lire_car_min ();
   if (c == lafin && (!isalnum (cs) && cs != CAR_UNDER))
   {
      insere_car (&fichier_pascal, cs);
      return letoken;
   }
   else
   {
      fin_identifiant (cs,i);
      return ID_TOKEN;
   }
}


/**
 *
 */
Tokens reco_identifiant (char c)
{
   int res;
   int decal;
   int i;
   char cs;
   i=0;

   if (isalpha(c))
   {
      i = 0;
      res = 0;
      decal = 0;
      /* On lit au max les 4 premiers carac de l'ID */
      do
      {
         sym[i++] = c; /* Déjà init à 10car */
         res = res | (c << decal);
         decal += 8;
         c = lire_car_min ();
      } while (i < 4 && (isalnum(c) || c == '_'));


      /* On ne teste que si la taille de l'ID est supérieur à la taille mini d'un token */
      if (i > 1)
      {
         switch (res)
         {
            case MN_VAR:
               insere_car(&fichier_pascal,c);
               return VAR_TOKEN;
               break;
            case MN_END:
               insere_car(&fichier_pascal,c);
               return END_TOKEN;
               break;
            case MN_IF:
               insere_car(&fichier_pascal,c);
               return IF_TOKEN;
               break;
            case MN_THEN:
               insere_car(&fichier_pascal,c);
               return THEN_TOKEN;
               break;
            case MN_DO:
               insere_car(&fichier_pascal,c);
               return DO_TOKEN;
               break;
            case MN_READ:
               insere_car(&fichier_pascal,c);
               return READ_TOKEN;
               break;
            case MN_FOR:
               insere_car(&fichier_pascal,c);
               return FOR_TOKEN;
               break;
            case MN_TO:
               insere_car(&fichier_pascal,c);
               return TO_TOKEN;
               break;
            case MN_STEP:
               insere_car(&fichier_pascal,c);
               return STEP_TOKEN;
               break;
            case MN_CASE:
               insere_car(&fichier_pascal,c);
               return CASE_TOKEN;
               break;
            case MN_OF:
               insere_car(&fichier_pascal,c);
               return OF_TOKEN;
               break;
            case MN_ELSE:
               insere_car(&fichier_pascal,c);
               return ELSE_TOKEN;
               break;
            case MN_TYPE:
               insere_car(&fichier_pascal,c);
               return TYPE_TOKEN;
               break;
            case MN_TRUE:
               insere_car(&fichier_pascal,c);
               return TRUE_TOKEN;
               break;
            case MN_D_PROGRAM:
               return (fin_token (c, MN_F_PROGRAM, 1)) ? PROGRAM_TOKEN : ID_TOKEN;
               break;
            case MN_D_DOWNTO:
               return (fin_token (c, MN_F_DOWNTO, 1)) ? DOWNTO_TOKEN : ID_TOKEN;
               break;
            case MN_D_REPEAT:
               return (fin_token (c, MN_F_REPEAT, 1)) ? REPEAT_TOKEN : ID_TOKEN;
               break;
            case MN_D_RECORD:
               return (fin_token (c, MN_F_RECORD, 1)) ? RECORD_TOKEN : ID_TOKEN;
               break;
            case MN_D_FUNCTION :
               return (fin_token (c, MN_F_FUNCTION, 1)) ? FUNCTION_TOKEN : ID_TOKEN;
               break;
            case MN_D_OTHERWISE:
               if (fin_token (c, MN_F_OTHERWISE, 0))
               {
                  c=lire_car_min ();
                  return fin_token_uniq (c, 'e', i, OTHERWISE_TOKEN);
               }
               else
                  return ID_TOKEN;
               break;
            case MN_D_PROCEDURE:
               if (fin_token (c, MN_F_PROCEDURE, 0))
               {
                  c=lire_car_min ();
                  return fin_token_uniq (c, 'e', i, PROCEDURE_TOKEN);
               }
               else
                  return ID_TOKEN;
               break;
            case MN_D_CONST:
               return fin_token_uniq (c, 't', i, CONST_TOKEN);
               break;
            case MN_D_BEGIN:
               return fin_token_uniq (c, 'n', i, BEGIN_TOKEN);
               break;
            case MN_D_WHILE:
               return fin_token_uniq (c, 'e', i, WHILE_TOKEN);
               break;
            case MN_D_FALSE:
               return fin_token_uniq (c, 'e', i, FALSE_TOKEN);
               break;
            case MN_D_WRITE:
               cs = lire_car_min ();
               if (c == 'e' && (!isalnum (cs) && cs != CAR_UNDER))
               {
                  insere_car (&fichier_pascal, cs);
                  return WRITE_TOKEN;
               }
               insere_car (&fichier_pascal, cs);
               return (fin_token (c, MN_F_WRITELN, 1)) ? WRITELN_TOKEN : ID_TOKEN;
               break;
            case MN_D_UNTIL:
               return fin_token_uniq (c, 'l', i, UNTIL_TOKEN);
               break;
            case MN_D_ARRAY:
               return fin_token_uniq (c, 'y', i, ARRAY_TOKEN);
               break;
            default:
               fin_identifiant(c,i);
               return ID_TOKEN;
               break;
         }
      } /* if (i > 1) Un seul caractère lu */
      sym[1] = '\0';
      nb_sym = 2;
      insere_car (&fichier_pascal, c);
      return ID_TOKEN;
   }
   else
      return TOKEN_INCONNU;
}


/**
 *
 */
void unget_token (Tokens t)
{
   unget = t;
   tounget = 1;
}


/**
 *
 */
Tokens find_next_token ()
{
   char c, cs, fin;

   if (tounget)
   {
      printf("ungeted\n");
      tounget = 0;
      return unget;
   }

   next_char();
   c = lire_car_min ();

   if (isdigit (c))
   {
      valeur = atoii(c);
#if DEBUG
      printf("valeur: %d -> %d\n",valeur, atoii(c));
#endif
      c = lire_car_min ();
      while (isdigit (c))
      {
#if DEBUG
         printf("valeur: %d\n",valeur);
#endif
         valeur = valeur*10 + atoii (c);
         c = lire_car_min ();
      }

      /* Remise dans le flux du dernier caractère non numérique */
      insere_car (&fichier_pascal, c);
#if DEBUG
      printf("valeur: %d\n",valeur);
#endif
      return NUM_TOKEN;
   }

   switch (c)
   {
      case CAR_UNDER:
         return identifiant (c);
         /*return ID_TOKEN;*/
         break;

      case CAR_PLUS:
         return PLUS_TOKEN;
         break;

      case CAR_MOINS:
         return MOINS_TOKEN;
         break;

      case CAR_MUL:
         return MUL_TOKEN;
         break;

      case CAR_DIV:
         return DIV_TOKEN;
         break;

      case CAR_EGAL:
         return EGAL_TOKEN;
         break;

      case CAR_INF:
         c = lire_car_min ();
         switch (c)
         {
            case CAR_EGAL:
               return INF_EGAL_TOKEN;
               break;
            case CAR_SUP:
               return DIFF_TOKEN;
               break;
            default:
               insere_car (&fichier_pascal, c);
               return INF_TOKEN;
         }
         break;

      case CAR_SUP:
         c = lire_car_min ();
         switch (c)
         {
            case CAR_EGAL:
               return SUP_EGAL_TOKEN;
               break;
            default:
               insere_car (&fichier_pascal, c);
               return SUP_TOKEN;
         }
         break;

         /* Suppression des commentaires {...} */
      case CAR_ACC_OUV:
         while (lire_car_min () != CAR_ACC_FER);
         return find_next_token();
         break;

      case CAR_PAR_OUV:
         c = lire_car_min ();
         /* Suppression des commentaires (*...*) */
         if (c == CAR_MUL)
         {
            fin = 0;
            while (!fin)
            {
               c = lire_car_min ();
               if (c == CAR_MUL)
               {
                  cs = lire_car_min ();
                  if (cs == CAR_PAR_FER)
                     fin = 1;
               }
            }
            return find_next_token();
         }
         insere_car (&fichier_pascal, c);
         return PAR_OUV_TOKEN;
         break;

      case CAR_PAR_FER:
         return PAR_FER_TOKEN;
         break;

      case CAR_CRO_OUV:
         return CRO_OUV_TOKEN;
         break;

      case CAR_CRO_FER:
         return CRO_FER_TOKEN;
         break;

      case CAR_2PT:
         c = lire_car_min ();
         switch (c)
         {
            case CAR_EGAL:
               return AFFEC_TOKEN;
               break;
            default:
               insere_car (&fichier_pascal, c);
               return DEUX_PT_TOKEN;
         }
         break;

      case CAR_PT_VIRG:
         return PT_VIRG_TOKEN;
         break;

      case CAR_VIRG:
         return VIRG_TOKEN;
         break;

      case CAR_POINT:
         c = lire_car_min ();
         switch (c)
         {
            case CAR_POINT:
               return PT_PT_TOKEN;
               break;
            default:
               insere_car (&fichier_pascal, c);
               return POINT_TOKEN;
         }
         break;

      case CAR_QUOTE:
         return QUOTE_TOKEN;
         break;

      default:
         return reco_identifiant(c);
         break;
   }
   return TOKEN_INCONNU;
}


/**
 *
 */
void next_token ()
{
   token = find_next_token();
}


/**
 *
 */
void lex_init ()
{
   sym = NULL;
   sym = (char *) malloc ((2 * BUFFER_SYM) * sizeof(char));
   taille_sym = 2 * BUFFER_SYM;
   valeur = 0;
}
