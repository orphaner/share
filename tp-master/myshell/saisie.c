/**
 * module : saisie
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
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "common.h"
#include "memutils.h"
#include "dyn_string.h"
#include "saisie.h"
#include "hash.h"
#include "varloc.h"
#include "varglob.h"



/*------------------------------------------------------------------------------
  Types prédéfinis
  ----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
  Constantes
  ----------------------------------------------------------------------------*/
#define CAR_COMMENT    '#'
#define CAR_TAB        '\t'
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
#define CAR_AND        '&'
#define CAR_PIPE       '|'
#define CAR_INTEROG    '?'
#define CAR_ANTISLASH  '\\'
#define CAR_QUOTE      '\''
#define CAR_DQUOTE     '\"'
#define CAR_DOLLAR     '$'

#define LISTE_ECHAP_NB 3


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
void ajouter_un_argv (char * un_argv);


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/
static char const liste_echap [LISTE_ECHAP_NB] = {
   CAR_ANTISLASH,
   CAR_DQUOTE,
   CAR_QUOTE
};

dyn_string saisie;
list_inst linst;
list_inst ltemp;



/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/**
 *
 */
void inst_finalize ()
{
   if (ltemp->argc > 0)
   {
      ltemp->argv = my_realloc (ltemp->argv, (ltemp->argc+1) * sizeof (string));
      ltemp->argv[ltemp->argc] = NULL;
   }
}


void attach_new_inst (dyn_string *cur_argv)
{
   list_inst tmp = my_malloc (sizeof (inst));
   inst_finalize ();
   if (cur_argv->taille != 0)
   {
      ajouter_un_argv (dyn_string_final(cur_argv));
      dyn_string_free (cur_argv);
      dyn_string_init (cur_argv);
   }
   tmp->argc = 0;
   tmp->argv = NULL;
   tmp->redir = R_NONE;
   tmp->background = 0;
   ltemp->suivant = tmp;
   ltemp = tmp;
   tmp->suivant = NULL;
}

/**
 *
 */
void free_inst ()
{
   int i;
   list_inst temp = linst;

   /* Pour tous les maillons de la liste chainées*/
   while (temp != NULL)
   {
      temp = linst->suivant;
      for (i = 0; i < linst->argc ; i++)
      {
         free (linst->argv[i]);
         linst->argv[i] = NULL;
      }
      free (linst->argv);
      linst->argv = NULL;
      free (linst);
      linst = temp;
   }
   linst = NULL;
}


/**
 *
 */
int is_in_liste_echap (char const c)
{
   int i = 0;
   for (; i < LISTE_ECHAP_NB ; i++)
      if (c == liste_echap[i])
         return TRUE;
   return FALSE;
}


/**
 *
 */
void parse_saisie ()
{
   char c;
   char c2;
   Arbre result;
   char * str_var;
   dyn_string cur_argv;
   dyn_string variable;
   redirections redir;
   int p;

#ifdef DEBUG
   int i = 0;
   list_inst temp;
#endif

   redir = R_NONE;
   ltemp->argc = 0;
   ltemp->argv = NULL;


   dyn_string_init (&cur_argv);

   dyn_string_trim (&saisie);

   /* Parcours de tous les caracères de la chaine saisie */
   while ( (c = dyn_string_lire_car (&saisie)) != CAR_END_STRING)
   {
      switch (c)
      {
         case CAR_QUOTE:
            while ((c = dyn_string_lire_car (&saisie)) != CAR_END_STRING && c != CAR_QUOTE)
            {
               dyn_string_add_char (&cur_argv, c);
            }
            break;

         case CAR_DQUOTE:
            while ( (c = dyn_string_lire_car (&saisie)) != CAR_END_STRING && c != CAR_DQUOTE)
            {
               if (c != CAR_ANTISLASH)
                  dyn_string_add_char (&cur_argv, c);
               else
               {
                  if ( (c2 = dyn_string_lire_car (&saisie)) != CAR_END_STRING)
                  {
                     if (!is_in_liste_echap(c2))
                        dyn_string_add_char (&cur_argv, c);
                     dyn_string_add_char (&cur_argv, c2);
                  }
               }
            }
            break;

         case CAR_ANTISLASH:
            if ( (c2 = dyn_string_lire_car (&saisie)) != CAR_END_STRING )
               dyn_string_add_char (&cur_argv, c2);
            break;

         case CAR_SUP:
            if ( (c2 = dyn_string_lire_car (&saisie)) == CAR_SUP )
               ltemp->redir = R_DSUP;
            else
            {
               if (c2 != CAR_END_STRING)
                  dyn_string_remise_car (&saisie);
               ltemp->redir = R_SUP;
            }
            attach_new_inst (&cur_argv);
            break;

         case CAR_PIPE:
            if ( (c2 = dyn_string_lire_car (&saisie)) == CAR_PIPE )
            {
               ltemp->redir = R_OR_OR;
               if (c2 != CAR_END_STRING)
               {
                  dyn_string_trim (&saisie);
                  if ( (c2 = dyn_string_lire_car (&saisie)) != CAR_END_STRING)
                  {
                     dyn_string_remise_car (&saisie);
                     attach_new_inst (&cur_argv); 
                  }
               }
            }
            else
            {
               ltemp->redir = R_PIPE;
               if (c2 != CAR_END_STRING)
               {
                  dyn_string_trim (&saisie);
                  if ( (c2 = dyn_string_lire_car (&saisie)) != CAR_END_STRING)
                  {
                     dyn_string_remise_car (&saisie);
                     attach_new_inst (&cur_argv); 
                  }
                  dyn_string_remise_car (&saisie);
               }
            }
            break;

         case CAR_AND:
            if ( (c2 = dyn_string_lire_car (&saisie)) == CAR_AND)
            {
               ltemp->redir = R_AND_AND;
               if (c2 != CAR_END_STRING)
               {
                  dyn_string_trim (&saisie);
                  if ( (c2 = dyn_string_lire_car (&saisie)) != CAR_END_STRING)
                  {
                     dyn_string_remise_car (&saisie);
                     attach_new_inst (&cur_argv); 
                  }
               }
            }
            else
            {
               ltemp->background = TRUE;
               if (c2 != CAR_END_STRING)
               {
                  dyn_string_trim (&saisie);
                  if ( (c2 = dyn_string_lire_car (&saisie)) != CAR_END_STRING)
                  {
                     dyn_string_remise_car (&saisie);
                     attach_new_inst (&cur_argv); 
                  }
                  dyn_string_remise_car (&saisie);
               }
            }
            break;

         case CAR_PT_VIRG:
            ltemp->redir = R_PT_VIRG;
            attach_new_inst (&cur_argv);
            break;

         case CAR_END_STRING:
            break;

         case CAR_DOLLAR:

            c2 =  dyn_string_lire_car (&saisie);

            /* Si on a $? on transforme pas le ? pr les regex car $? == obtenir status ! */
            if (c2 != CAR_END_STRING && c2 == CAR_INTEROG)
            {
               dyn_string_add_char (&cur_argv, CAR_DOLLAR);
               dyn_string_add_char (&cur_argv, CAR_INTEROG);
            }
            /* Dollar double -> variable globale */
            else if (c2 != CAR_END_STRING && c2 == CAR_DOLLAR)
            {
               dyn_string_init (&variable);
               while ((c = dyn_string_lire_car (&saisie)) != CAR_END_STRING && c != CAR_ESPACE)
                  dyn_string_add_char (&variable, c);

               if (c != CAR_END_STRING)
                  dyn_string_remise_car (&saisie);

               if ( (str_var = varglob_get (dyn_string_final (&variable))) != NULL)
               {
                  p = 0;
                  while (str_var[p])
                  {
                     dyn_string_add_char (&cur_argv, str_var[p++]);
                  }
                  free (str_var);
               }
               dyn_string_free (&variable);
            }
            /* Dollar simple -> variable locale */
            else if (c2 != CAR_END_STRING)
            {
               dyn_string_init (&variable);
               dyn_string_add_char (&variable, c2);
               while ((c = dyn_string_lire_car (&saisie)) != CAR_END_STRING && c != CAR_ESPACE)
                  dyn_string_add_char (&variable, c);

               if (c != CAR_END_STRING)
                  dyn_string_remise_car (&saisie);

               if ( (result = hash_search (&varloc, dyn_string_final (&variable))) != NULL)
               {
                  str_var = result->valeur;
                  p = 0;
                  while (str_var[p])
                  {
                     dyn_string_add_char (&cur_argv, str_var[p++]);
                  }
               }
               dyn_string_free (&variable);
            }
            break;

         case CAR_EGAL:
            /* Cas où le = est collé au mot !!! */
            if (cur_argv.taille != 0)
            {
               ajouter_un_argv (dyn_string_final(&cur_argv));
               dyn_string_free (&cur_argv);
               dyn_string_init (&cur_argv);
            }
            /* Ajout du égal dans argv ! */
            dyn_string_add_char (&cur_argv, CAR_EGAL);
            ajouter_un_argv (dyn_string_final(&cur_argv));
            dyn_string_free (&cur_argv);
            dyn_string_init (&cur_argv);
            break;

         case CAR_ESPACE:
            if (cur_argv.taille != 0)
            {
               ajouter_un_argv (dyn_string_final(&cur_argv));
               dyn_string_free (&cur_argv);
               dyn_string_init (&cur_argv);
            }
            break;

         default:
            dyn_string_add_char (&cur_argv, c);
      }
   }

   /* On a un argv à ajouter en sortie de boucle !! */
   if (cur_argv.taille != 0)
   {
      ajouter_un_argv (dyn_string_final(&cur_argv));
   }

   dyn_string_free (&cur_argv);
   /* Ajout du marqueur de fin dans l'argv courant*/
   inst_finalize ();


#ifdef DEBUG
   printf("Sortie ::\n");
   temp = linst;
   while (temp)
   {
      ltemp = temp;
      printf("<<maillon>>\n");
      if (ltemp->argc > 0)
         for (i=0; i <= ltemp->argc; i++)
            printf("argv[%d] = %s\n", i, ltemp->argv[i]);
      else
         printf("argv = NULL\n");
      switch (ltemp->redir)
      {
         case R_PIPE:
            printf("|\n");
            break;
         case R_OR_OR:
            printf("||\n");
            break;
         case R_AND_AND:
            printf("&&\n");
            break;
         case R_SUP:
            printf(">\n");
            break;
         case R_DSUP:
            printf(">>\n");
            break;
         case R_PT_VIRG:
            printf(";\n");
            break;
         case R_NONE:
            break;
      }
      temp = temp->suivant;
   }
#endif
}


/**
 *
 */
void ajouter_un_argv (char * the_argv)
{
   glob_t globbuf;
   unsigned int i;

   /* On globe (pattern matching, tt bete en fait, suffit de trouver le bon man :) */
   glob (the_argv, GLOB_BRACE | GLOB_NOESCAPE | GLOB_NOCHECK | GLOB_MARK , NULL, &globbuf);

   /* Parcours des éventuels résultats */
   /* Si pas de résultat -> le motif */
   for (i=0 ; i < globbuf.gl_pathc ; i++)
   {
      ltemp->argc++;
      ltemp->argv = my_realloc (ltemp->argv, ltemp->argc * sizeof (string));
      ltemp->argv[ltemp->argc-1] = my_malloc ((strlen (globbuf.gl_pathv[i])+1) * sizeof (char));
      strcpy (ltemp->argv[ltemp->argc-1], globbuf.gl_pathv[i]);
   }

   globfree (&globbuf);
   dyn_string_trim (&saisie);
}


/**
 *
 */
void perform_saisie (string prompt)
{
   string line = NULL;
   int taille;

   free_inst ();
   dyn_string_free (&saisie);
   dyn_string_init2 (&saisie);
   linst = NULL;

   line = readline (prompt);

   if ( (taille = strlen(line)) > 0)
   {
      linst = my_malloc (sizeof (inst));
      linst->suivant = NULL;
      linst->redir = R_NONE;
      linst->background = 0;
      ltemp = linst;
      add_history (line);
      saisie.chaine = line;
      saisie.taille = taille;
      parse_saisie ();
   }
}
