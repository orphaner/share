/**
 * module : syntaxique
 * brief :
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date :
 * bug : Aucun dÅÈtectÅÈ Å‡ ce jour
 * 14h45 : soutenance
 */


/*------------------------------------------------------------------------------
  Fichiers include
  ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "hash.h"
#include "syntaxique.h"
#include "erreur.h"
#include "generation.h"
#include "lexical.h"
#include "librairie.h"
#include "typepredef.h"


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/
Arbre cur_localite = NULL;


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/**
 * param err : le numéÈro de l'erreur
 */
void erreur (Erreurs err)
{
   corrige = 1;
   erreur_mess (err);
}


/**
 * param t : le token é‡ tester
 * return : 0 ou 1
 */
char teste (Tokens t)
{
   if (token == t)
      next_token();
   else
   {
      switch (t)
      {
         case ID_TOKEN:
            erreur (ERR_ID );
            break;
         case NUM_TOKEN:
            erreur (ERR_NUM );
            break;
         case PAR_OUV_TOKEN:
            erreur (ERR_PAR_OUV );
            break;
         case PAR_FER_TOKEN:
            erreur (ERR_PAR_FER );
            break;
         case VIRG_TOKEN:
            erreur (ERR_VIRG );
            break;
         case PT_VIRG_TOKEN:
            erreur (ERR_PT_VIRG );
            break;
         case POINT_TOKEN:
            erreur (ERR_POINT );
            break;
         case DEUX_PT_TOKEN:
            erreur (ERR_DEUX_PT_TOKEN );
            break;
         case AFFEC_TOKEN:
            erreur (ERR_AFFEC );
            break;
         case BEGIN_TOKEN:
            erreur (ERR_BEGIN );
            break;
         case END_TOKEN:
            erreur (ERR_END );
            break;
         case IF_TOKEN:
            erreur (ERR_IF );
            break;
         case WHILE_TOKEN:
            erreur (ERR_WHILE );
            break;
         case THEN_TOKEN:
            erreur (ERR_THEN );
            break;
         case DO_TOKEN:
            erreur (ERR_DO );
            break;
         case WRITE_TOKEN:
            erreur (ERR_WRITE );
            break;
         case WRITELN_TOKEN:
            erreur (ERR_WRITELN );
            break;
         case READ_TOKEN:
            erreur (ERR_READ );
            break;
         case CONST_TOKEN:
            erreur (ERR_IN_DECL );
            break;
         case VAR_TOKEN:
            erreur (ERR_IN_DECL );
            break;
         case PROGRAM_TOKEN:
            erreur (ERR_PROGRAM );
            break;
         case FOR_TOKEN:
            erreur (ERR_FOR );
            break;
         case TO_TOKEN:
            erreur (ERR_TO );
            break;
         case DOWNTO_TOKEN:
            erreur (ERR_DOWNTO );
            break;
         case REPEAT_TOKEN:
            erreur (ERR_REPEAT );
            break;
         case UNTIL_TOKEN:
            erreur (ERR_UNTIL );
            break;
         case CASE_TOKEN:
            erreur (ERR_CASE );
            break;
         case OF_TOKEN:
            erreur (ERR_OF );
            break;

         default:/* ZARB */
            erreur (ERR_INCONNU );
            break;
      }
   }
   return 1;
}


/**
 * Ajoute un symbole dans la table des symboles
 * param t : le token attendu
 * param c : la calsse du symbole é‡ ajouter
 * return : un pointeur vers le noeud ajoutéÈ, NULL sinon
 */
Arbre teste_et_entre (Tokens t, Classes c, ...)
{
   Arbre add = NULL;
   Arbre pere = NULL;
   va_list varg;     /* Variable identifiant le prochain paraméËtre. */
   va_start (varg, c);

   if (token == t)
   {
      /* Ajout du symbole en fonction de sa classe */
      switch (c)
      {
         case CLASS_CHAMP:
            pere = va_arg (varg, Arbre);
            add =  hash_add_champ (&table,pere );
            break;
         default:
            add = hash_add (&table, c, cur_localite);
            break;
      }

      /* --> Ajout du symbole OK*/
      if (add)
      {
         if (token != ARRAY_TOKEN && token != RECORD_TOKEN)
            next_token ();
      }

      /* --> ERREUR : le symbole existe dÅÈjÅ‡ */
      else
      {
         erreur (ERR_DBL_ID);
         while (token != VIRG_TOKEN
                && token != PT_VIRG_TOKEN
                && token != EGAL_TOKEN)
            next_token ();
      }
   }
   else
      teste (t);

   return add;
}


/**
 * Recherche un symbole dans la table des symboles
 * param t : le token recherchéÈ
 * param c : la calsse du symbole é‡ rechercher
 * return : un pointeur vers le noeud trouvéÈ, NULL sinon
 */
Arbre teste_et_cherche (Tokens t, Classes c, ...)
{
   Arbre adresse = NULL;
   Arbre pere = NULL;
   va_list varg;     /* Variable identifiant le prochain paraméËtre. */
   va_start (varg, c);

   if (token == t)
   {
      /* Recherche du symbole en fonction de sa classe */
      switch (c)
      {
         case CLASS_CHAMP:
            pere = va_arg (varg, Arbre);
            adresse = hash_search_champ (&table, sym, pere);
            break;
         default:
            if (!cur_localite)
               adresse = hash_search (&table, sym, c);
            else
               adresse = hash_search_localite (&table, sym, c, cur_localite);
      }
      if (adresse)
      {
         next_token();
      }
      else
         erreur (ERR_NO_ID);
   }
   else
      teste (t);

   return adresse;
}


/*==============================================================================
  TRAME PRINCIPALE DU PROGRAMME
  ============================================================================*/
/**
 *
 */
void program ()
{
   generer_com("Fichier P_CODE genere par pp1 ");
   generer_com("P_CODE file encoded by pp1 ");
   generer_com("Authors : Nicolas LASSALLE and Maxime GRYL \n");
   next_token();

   if (token == CONST_TOKEN || token == VAR_TOKEN || token == BEGIN_TOKEN)
   {
      erreur (ERR_PROGRAM);
      erreur (ERR_ID);
      erreur (ERR_PT_VIRG);
      block ();
   }
   else
   {
      teste (PROGRAM_TOKEN);
      if (token == CONST_TOKEN || token == VAR_TOKEN || token == BEGIN_TOKEN)
      {
         erreur (ERR_ID);
         erreur (ERR_PT_VIRG);
         block ();
      }
      else
      {
         teste_et_entre (ID_TOKEN, CLASS_PROG);
         teste (PT_VIRG_TOKEN);
         block ();
      }
   }


   if (token != POINT_TOKEN)
      erreur (ERR_POINT);
   if (!corrige)
   {
      printf("^^Programme correct^^\n\n");
      generer1 (HLT);
      /* vider_buffer (&fic_pcode); mis dans freeall en attendant */
      /* ecrire saut (&fic_pcode) */
      generer_com("Fichier genere sans erreurs connues");
   }
   else
      printf("\n~~Des erreurs ont ÅÈtÅÈ rencontrÅÈes~~\n\n");

}


/**
 *
 */
void block ()
{
   /* déÈclarations des const / var / types */
   do
   {
      while (token == CONST_TOKEN)
         consts (NULL);
      while (token == TYPE_TOKEN)
         types ();
      while (token == VAR_TOKEN)
         vars (NULL);
   } while (token == CONST_TOKEN || token == TYPE_TOKEN || token == VAR_TOKEN);

   generer2i (INT, table.offset);


   /* DéÈclaration des fonctions / procéÈdures */
   do
   {
      while (token == PROCEDURE_TOKEN)
         procs ();
      while (token == FUNCTION_TOKEN)
         funcs ();
   } while (token == PROCEDURE_TOKEN || token == FUNCTION_TOKEN);

   insts ();
}


/*==============================================================================
  PARTIE DÅ…CLARATION
  ============================================================================*/
/**
 * DÅÈclarations des constantes
 */
void consts (Arbre localite)
{
   Arbre new;

   teste (CONST_TOKEN);
   do
   {
      new = teste_et_entre (ID_TOKEN, CLASS_CONST);
      teste (EGAL_TOKEN);
      if (new)
      {
         new->adresse = expr_const();
         new->localite = localite;
      }
      else
         expr_const();
      teste (PT_VIRG_TOKEN);
   } while (token == ID_TOKEN);
}


/**
 * DÅÈclaration de toutes les variables
 */
int vars (Arbre localite)
{
   int total = 0;
   teste (VAR_TOKEN);

   do
   {
      total += decl (localite);
      teste (PT_VIRG_TOKEN);
   } while (token != BEGIN_TOKEN &&
            token != PROCEDURE_TOKEN &&
            token != FUNCTION_TOKEN
            /*token !=*/);
   return total;
}


/**
 * DÅÈclaration d'une ligne de variables (a, b : integer;)
 */
int decl (Arbre localite)
{
   Arbre *tab = NULL;
   Arbre le_type;
   int cpt = 0;
   int i = 0;
   int taille;
   int total = 0;

   tab = (Arbre *) realloc (tab, (cpt+1)*sizeof (Arbre));
   tab[cpt++] = teste_et_entre (ID_TOKEN, CLASS_VAR);

   while (token == VIRG_TOKEN)
   {
      next_token ();
      tab = (Arbre *) realloc (tab, (cpt+1)*sizeof (Arbre));
      tab[cpt++] = teste_et_entre (ID_TOKEN, CLASS_VAR);
   }

   teste (DEUX_PT_TOKEN);

   le_type = type ();

   if (le_type)
   {
      taille = le_type->taille;
      for (; i < cpt ; i++)
      {
         if (tab[i])
         {
            tab[i]->adresse = (localite) ? total : table.offset;
            tab[i]->taille = taille;
            total += taille;
            tab[i]->localite = localite;
            tab[i]->type_de_base = le_type;

            /* Mise é‡ jour de l'offset seulement si localitéÈ = globale*/
            if (!localite)
               table.offset += taille;
         }
      }
   }
   free (tab);
   return total;
}


/**
 * CréÈe un nouveau type anonyme
 * param t : ARRAY_TOKEN ou RECORD_TOKEN
 * return : pointeur vers le symbole de la table créÈéÈ. NULL sinon
 */
Arbre creer_anonyme (Tokens t)
{
   next_anonyme_string = itoc (next_anonyme);
   strcpy (sym, next_anonyme_string);
   nb_sym = strlen (next_anonyme_string)+1;
   next_anonyme++;
   return teste_et_entre (t, CLASS_TYPE);
}


/**
 * Recherche si le token courant est un type
 * CréÈe un type anonyme si néÈcessaire
 * return : pointeur vers le symbole de la table, NULL si non trouvÅÈ/créÈéÈ
 */
Arbre type ()
{
   Arbre new;

   /* Type anonyme tableau */
   if (token == ARRAY_TOKEN && (new = creer_anonyme(ARRAY_TOKEN)))
   {
      type_array (new);
      return new;
   }

   /* Type anonyme record */
   else if (token == RECORD_TOKEN && (new = creer_anonyme(RECORD_TOKEN)))
   {
      type_record (new);
      return new;
   }

   /* Type noméÈ (déÈfini par le programmeur) */
   return teste_et_cherche (ID_TOKEN, CLASS_TYPE);
}


/**
 * DÅÈclaration de tous les types
 */
void types ()
{
   if (token == TYPE_TOKEN)
   {
      teste (TYPE_TOKEN);
      do
      {
         typedecl ();
         teste (PT_VIRG_TOKEN);
      } while (token == ID_TOKEN);
   }
}


/**
 * DÅÈclaration d'un type
 */
void typedecl ()
{
   Arbre new;

   new = teste_et_entre (ID_TOKEN, CLASS_TYPE);
   teste (EGAL_TOKEN);

   if (new)
   {
      /* Reconaissance des tableaux */
      if (token == ARRAY_TOKEN)
         type_array (new);

      /* Reconaissance des record */
      else if (token == RECORD_TOKEN)
         type_record (new);

      /* Reconaissance des intervalles */
      else if (token == NUM_TOKEN
               || token == MOINS_TOKEN
               || token == PLUS_TOKEN
               || hash_search (&table, sym, CLASS_CONST))
         type_intervalle (new);
   }

   /* Double dÅÈclaration -> synchro sur ; */
   else
      while (token != PT_VIRG_TOKEN)
         next_token ();
}


/**
 * Reconaissance des tableaux
 * param new : le tableau que l'on est entrain de déÈclarer
 */
void type_array (Arbre new)
{
   int i = 0;
   int taille;
   Arbre le_type;

   new->type = TYPE_TABLEAU;
   new->TAB_NB_DIM = 0;
   new->TAB_TAB = NULL;

   teste (ARRAY_TOKEN);
   teste (CRO_OUV_TOKEN);
   dim (new);
   while (token == VIRG_TOKEN)
   {
      next_token ();
      dim (new);
   }
   teste (CRO_FER_TOKEN);
   teste (OF_TOKEN);

   le_type = type ();

   if (le_type)
   {
      new->type_de_base = le_type;
      taille = le_type->taille;
      new->taille = taille;

      for (; i < new->TAB_NB_DIM ; i++)
         new->taille *= new->TAB_TAILLE(i);
   }
}


/**
 * DÅÈclaration d'une dimension d'un tableau
 * param new : le tableau que l'on est entrain de déÈclarer
 */
void dim (Arbre new)
{
   int debut = 0;
   int fin = 0;
   int dim;
   Arbre pt = NULL;

   /* Mise é‡ jour de la dimension du tableau */
   dim = new->TAB_NB_DIM++;
   new->TAB_TAB = (Dim_tableau *)
      realloc (new->TAB_TAB, new->TAB_NB_DIM * sizeof (Dim_tableau));


   /* CAS 1 : A LA MAIN */
   /* Dimension du tableau déÈfinie é‡ la main [expr()..expr()] | [expr()]*/
   if (token == NUM_TOKEN  ||  hash_search_localite (&table, sym, CLASS_CONST, cur_localite) )
   {
      /* DéÈbut de l'intervalle du tableau ou bien pas d'intervalle */
      debut = expr_const ();

      /*La dim tableau est déÈfini par un déÈbut et une taille */
      if (token == PT_PT_TOKEN)
      {
         next_token ();
         fin = expr_const ();

         /* VéÈrif intervalle strictement croissant */
         if (debut >= fin)
            erreur (ERR_INTER_CROISS);
         else
         {
            new->TAB_INDICE_DEBUT (dim) = debut;
            new->TAB_TAILLE (dim) = fin - debut;
         }
      }

      /* Sinon le dim du tableau déÈfini par la taille */
      else
      {
         /* VéÈrif tableau non vide */
         if (debut == 0)
            erreur (ERR_INTER_CROISS);
         else
         {
            new->TAB_INDICE_DEBUT (dim) = 0;
            new->TAB_TAILLE (dim)= debut;
         }
      }
   }

   /* CAS2 : AVEC UN intervalle */
   /* Dim du tableau déÈfinie par un type intervalle */
   /* inter = 0..5 */
   /* toto = array[inter] */
   else if ( (pt = teste_et_cherche(ID_TOKEN, CLASS_TYPE)) &&
             pt->type == TYPE_INTER)
   {
      debut = new->TAB_INDICE_DEBUT (dim) = pt->INTER_DEBUT;
      new->TAB_TAILLE (dim) = pt->INTER_FIN - debut;
   }

}


/**
 * Reconaissance des intervalles
 * Intervalle de valeur entre debut et fin
 * Intervalle ne travaille que sur des types primitifs -> taille = 1
 * Le contenu d'une variable de type intervalle doit etre contenu ds cet intervalle
 * param new : l'intervalle que l'on est entrain de déÈclarer
 */
void type_intervalle (Arbre new)
{
   new->type = TYPE_INTER;
   new->taille = 1;

   /* DéÈbut de l'intervalle */
   new->INTER_DEBUT = expr_const();

   teste (PT_PT_TOKEN);

   /* Fin de l'intervalle */
   new->INTER_FIN = expr_const();


   /* VéÈrif d'intervalle strictement croissant */
   if (new->INTER_DEBUT >= new->INTER_FIN)
      erreur (ERR_INTER_CROISS);

}


/**
 * Reconaissance des record
 * param pere : le record que l'on est entrain de déÈclarer
 */
void type_record (Arbre pere)
{
   char ko;

   pere->type = TYPE_RECORD;
   pere->taille = 0;

   teste (RECORD_TOKEN);
   do
   {
      if ( !(ko = champs (pere)) )
         teste (PT_VIRG_TOKEN);
   } while (token == ID_TOKEN && !ko);

   /* Si il y a une erreur dans les champs, on synchronise sur END_TOKEN */
   if (ko)
   {
      erreur (ERR_IN_DECL_RECORD);
      while (token != END_TOKEN)
         next_token ();
   }

   teste (END_TOKEN);
}


/**
 * DÅÈclaration d'un champ d'un record
 * param pere : le record auquel on ajoute un champ
 * return : un pointeur vers le champ ajoutÅÈ, NULL si erreur
 */
char champs (Arbre pere)
{
   Arbre *tab = NULL;
   Arbre le_type;
   int cpt = 0;
   int i = 0;
   int taille;
   char ko = 0;

   /* Ajout du premier champ de record de la ligne */
   tab = (Arbre *) realloc (tab, (cpt+1)*sizeof (Arbre));
   if (! (tab[cpt++] = teste_et_entre (ID_TOKEN, CLASS_CHAMP, pere)))
   {
      erreur (ERR_IN_DECL_RECORD);
      ko = 1;
   }
   else
      tab[cpt-1]->type_info.champ.from_record = pere;

   /* Ajout des champs suivant {,champ} */
   while (token == VIRG_TOKEN)
   {
      next_token ();
      tab = (Arbre *) realloc (tab, (cpt+1)*sizeof (Arbre));
      if (! (tab[cpt++] = teste_et_entre (ID_TOKEN, CLASS_CHAMP, pere)))
      {
         erreur (ERR_IN_DECL_RECORD);
         ko = 1;
      }
      else
         tab[cpt-1]->type_info.champ.from_record = pere;
   }


   teste (DEUX_PT_TOKEN);

   le_type = type ();

   /* Mise é‡ jour des informations du record et de ses champs ssi tout est OK ! */
   if (le_type && !ko)
   {
      taille = le_type->taille;
      for (; i < cpt ; i++)
      {
         tab[i]->taille = taille;
         tab[i]->type_de_base = le_type;
         tab[i]->type_info.champ.decalage = pere->taille;
         pere->taille += taille;
      }
   }

   free (tab);

   return ko;
}


/*==============================================================================
  Dé…CLARATION DES FONCTIONS / PROCé…DURES
  ============================================================================*/
/**
 *
 */
void paramdecl (Arbre pere)
{
   Arbre new;
   Arbre le_type;
   int dim = 0;

   /* Mise é‡ jour de la dimension du tableau des paraméËtres*/
   if (pere)
   {
      dim = pere->FUNC_NB_PARAM++;
      pere->FUNC_TAB_PARAM =
         (Param *) realloc (pere->FUNC_TAB_PARAM, pere->FUNC_NB_PARAM * sizeof (Param));
   }

   /* ParamÅËtre en donnÅÈe / rÅÈsultat */
   if (token == VAR_TOKEN)
   {
      next_token ();
      if (pere)
         pere->FUNC_VAR(dim) = 1;
      new = teste_et_entre (ID_TOKEN, CLASS_VAR);
   }

   /* ParamÅËtre en donnÅÈe */
   else
   {
      if (pere)
         pere->FUNC_VAR(dim) = 0;
      new = teste_et_entre (ID_TOKEN, CLASS_VAR);
   }
   teste (DEUX_PT_TOKEN);

   le_type = type ();

   if (new)
   {
      new->type_de_base = le_type;
      new->localite = pere;
      if (pere)
         pere->FUNC_PARAM(dim) = new->type_de_base;
   }
}


/**
 *
 */
void paramlist (Arbre pere)
{
   teste (PAR_OUV_TOKEN);
   if (token == ID_TOKEN || token == VAR_TOKEN)
   {
      paramdecl (pere);
      while (token == VIRG_TOKEN)
      {
         next_token ();
         paramdecl (pere);
      }
   }
   teste (PAR_FER_TOKEN);
}


/**
 *
 */
void procs ()
{
   Arbre new;
   int taille = 0;
   teste (PROCEDURE_TOKEN);

   if ( (new = teste_et_entre (ID_TOKEN, CLASS_PROCEDURE)) )
   {
      cur_localite = new;
      new->FUNC_NB_PARAM = 0;
      new->FUNC_TAB_PARAM = NULL;
   }
   paramlist (new);
   teste (PT_VIRG_TOKEN);

   if (token == CONST_TOKEN)
      consts (new);

   if (token == VAR_TOKEN)
      taille = vars (new);

   if (new)
      new->taille = taille;

   insts ();
   teste (PT_VIRG_TOKEN);
   cur_localite = NULL;
}


/**
 *
 */
void funcs ()
{
   Arbre new;
   Arbre type_retour;
   int taille = 0;

   teste (FUNCTION_TOKEN);
   if ( (new = teste_et_entre (ID_TOKEN, CLASS_FONCTION)) )
   {
      cur_localite = new;
      new->FUNC_NB_PARAM = 0;
      new->FUNC_TAB_PARAM = NULL;
   }
   paramlist (new);
   teste (DEUX_PT_TOKEN);

   /*type_retour = type ();*/
   /* Evite la créÈation d'un type anonyme pr rien ! */
   type_retour = teste_et_cherche (ID_TOKEN, CLASS_TYPE);

   /* retour seulement type de base */
   if (!EST_PRIMITIF(type_retour))
      erreur (ERR_FONCTION_TYPE_RETURN);

   if (new)
      new->type_de_base = type_retour;

   teste (PT_VIRG_TOKEN);

   if (token == CONST_TOKEN)
      consts (new);

   if (token == VAR_TOKEN)
      taille = vars (new);

   if (new)
      new->taille = taille;

   insts ();
   teste (PT_VIRG_TOKEN);
   cur_localite = NULL;
}


/**
 *
 */
void paramefflist (Arbre call)
{
   int i = 0;
   char c;
   Arbre param;

   teste (PAR_OUV_TOKEN);
   for (; i < call->FUNC_NB_PARAM ; i++)
   {
      /* ParamÅËtre boolÅÈen */
      if (token == TRUE_TOKEN || token == FALSE_TOKEN)
      {
         next_token ();
         if (call->FUNC_PARAM(i) != type_predef[PREDEF_BOOLEAN])
            erreur (ERR_TYPE_PARAM);
      }

      /* ParamÅËtre boolÅÈen */
      else if (token == QUOTE_TOKEN)
      {
         c = lire_car (&fichier_pascal);
         teste (QUOTE_TOKEN);
         teste (QUOTE_TOKEN);
         if (call->FUNC_PARAM(i) != type_predef[PREDEF_CHAR])
            erreur (ERR_TYPE_PARAM);
      }

      /* Expression entiÅËre */
      else  if ( (param = expr ()) )
      {
         if (param->type_de_base != call->FUNC_PARAM(i))
            erreur (ERR_TYPE_PARAM);

      }

      /* Pas de test de virgule si dernier paramÅËtre */
      if (i < call->FUNC_NB_PARAM - 1)
         teste (VIRG_TOKEN);
   }
   teste (PAR_FER_TOKEN);
}


/**
 *
 */
void appel ()
{
   Arbre call;
   if ( (call = teste_et_cherche (ID_TOKEN, CLASS_FONCTION | CLASS_PROCEDURE)) )
      paramefflist (call);
   else
   {
      next_token ();
      while (token != PAR_FER_TOKEN)
         next_token ();
   }
}


/*==============================================================================
  EXÅ…CUTION D'INSTRUCTIONS
  ============================================================================*/
/**
 *
 */
void insts ()
{
   teste (BEGIN_TOKEN);
   inst();
   while (token == PT_VIRG_TOKEN)
   {
      next_token ();
      inst ();
   }
   teste (END_TOKEN);
}


/**
 *
 */
void inst ()
{
   switch (token)
   {
      case ID_TOKEN:
         if ( (hash_search (&table, sym, CLASS_PROCEDURE)) )
            appel ();
         else
            affec ();
         break;
      case IF_TOKEN:
         si ();
         break;
      case WHILE_TOKEN:
         tantque ();
         break;
      case BEGIN_TOKEN:
         insts ();
         break;
      case WRITE_TOKEN:
         ecrire (0);
         break;
      case WRITELN_TOKEN:
         ecrire (1);
         break;
      case READ_TOKEN:
         lire ();
         break;
      case FOR_TOKEN:
         pour ();
         break;
      case REPEAT_TOKEN:
         repeat ();
         break;
      case CASE_TOKEN:
         cas ();
         break;
      default:
         break;
   }
}


/*==============================================================================
  ACCES AUX VARIABLES
  ============================================================================*/
/**
 *
 */
Arbre var2 (Arbre id)
{
   Arbre fils;
   int i, j, dim;

   if (!id)
      return id;

   /* AccéËs é‡ un champ de record */
   if (token == POINT_TOKEN)
   {
      next_token ();

      if ( (fils = teste_et_cherche (ID_TOKEN, CLASS_CHAMP, id->type_de_base)) )
      {
         if (fils->type_info.champ.decalage)
         {
            generer2i (LDI, fils->type_info.champ.decalage);
            generer1 (ADD);
         }
         return var2 (fils);
      }

      next_token();
      return var2 (fils);
   }

   /* AccéËs é‡ un tableau */
   else if (token == CRO_OUV_TOKEN)
   {
      next_token ();

      /* Tableau é‡ une dimension */
      if (id->type_de_base->TAB_NB_DIM == 1)
      {
         generer2i (LDI, id->type_de_base->type_de_base->taille);
         expr();
         generer1 (MUL);
      }

      /* Tableau é‡ plusieurs dimensions */
      else
      {
         for (i=0 ; i < id->type_de_base->TAB_NB_DIM-1 ; i++)
         {
            expr();

            dim = 1;
            for (j=i+1 ; j < id->type_de_base->TAB_NB_DIM ; j++)
               dim *= id->type_de_base->TAB_TAILLE(j);

            generer2i (LDI, dim);
            generer1 (MUL);
            generer1 (ADD);
            teste (VIRG_TOKEN);
         }
         expr();
         generer1 (ADD);
      }

      teste (CRO_FER_TOKEN);
      return var2 (id->type_de_base);
   }
   return id;
}


/**
 * Empile l'adresse de déÈpart de la variable, ensuite ajoute le déÈcalage
 * retourne le pointeur vers la variable.
 */
Arbre var (Classes c)
{
   Arbre id;
   if ((id = teste_et_cherche (ID_TOKEN, c)))
   {
      generer2i (LDA, id->adresse);
      generer_com ("var");

      id = var2 (id);

      if (id && !(id->classe & CLASS_CONST) && !EST_PRIMITIF(id->type_de_base))
         erreur (ERR_CHAMP_ACCESS);
   }
   return id;

   /* "déÈcalage" par rapport é‡ l'adresse initiale */
   /* si la recherche s'effectue sur une constante, var
      (id) ne renvoi rien. La valeur de la constante (son adresse) est qd meme empiléÈe.*/
}


/*==============================================================================
  AFFECTATIONS
  ============================================================================*/
/**
 *
 */
void affec ()
{
   Arbre adresse;
   char c;

   if (!cur_localite)
      adresse = var (CLASS_VAR);
   else
      adresse  = var (CLASS_VAR | CLASS_FONCTION);


   generer_com ("affectation_suite");

   if (adresse)
   {
      teste (AFFEC_TOKEN);

      /* Affectation d'un integer */
      if (adresse->type_de_base == type_predef[PREDEF_INTEGER])
      {
         expr ();
         generer1 (STO);
         generer_com ("FIN affectation");
      }

      /* Affectation d'un boolÅÈen */
      else if (adresse->type_de_base == type_predef[PREDEF_BOOLEAN])
      {
         /* TRUE */
         if (token == TRUE_TOKEN)
         {
            generer2i (LDI, 1);
         }

         /* FALSE */
         else if (token == FALSE_TOKEN)
         {
            generer2i (LDI, 0);
         }

         /* ERREUR */
         else
            erreur (ERR_BOOLEAN_WAITED);

         generer1 (STO);
         next_token ();
      }

      /* Affectation d'un char */
      else if (adresse->type_de_base == type_predef[PREDEF_CHAR])
      {
         c = lire_car (&fichier_pascal);
         teste (QUOTE_TOKEN);
         generer2i (LDI, (int)c);
         generer1 (STO);
         teste (QUOTE_TOKEN);
      }
   }
}


/*==============================================================================
  IF ELSE
  ============================================================================*/
/**
 *
 */
void si ()
{
   int sauv;

   teste (IF_TOKEN);
   cond ();

   generer2i (BZE, 0); /* 0 car incomplet */
   sauv = pile_saut_push (&sauts, ligne_pcode);

   teste (THEN_TOKEN);
   inst ();

   pile_saut_update (&sauts, sauv, ligne_pcode+1);
}


/*==============================================================================
  ENTRÅ…ES / SORTIES
  ============================================================================*/
/**
 *
 */
void ecrire (char ln)
{
   generer_com ("ecriture d'entier(s)");

   (ln) ? teste (WRITELN_TOKEN) : teste (WRITE_TOKEN);

   teste (PAR_OUV_TOKEN);
   mes ();
   while (token == VIRG_TOKEN)
   {
      next_token();
      mes ();
   }
   teste (PAR_FER_TOKEN);

   /* Retour Å‡ la ligne */
   if (ln)
      generer1 (PRL);

   generer_com ("FIN ecriture d'entier(s) \n");
}


/**
 *
 */
void mes ()
{
   char c;
   Arbre adresse;

   /* Affichage d'une chaine de caractÅËres */
   if (token == QUOTE_TOKEN)
   {
      while ( (c = lire_car_min ()) != CAR_QUOTE)
      {
         generer2c (PRC, c);
      }
      next_token ();
   }

   /* Affichage d'une expression */
   else
   {
      adresse = expr ();
      if (adresse)
      {
         /* Affichage d'un boolÅÈen */
         if (adresse->type_de_base == type_predef[PREDEF_BOOLEAN])
         {
            generer1 (PRB);
         }
         /* Affichage d'un char */
         else if (adresse->type_de_base == type_predef[PREDEF_CHAR])
         {
            generer1 (PRD);
         }
         else
            generer1 (PRN);
      }
      /* Affichage d'un entier */
      else
         generer1 (PRN);
   }
}


/**
 *
 */
void lire_une_var (Arbre adresse)
{
   if (adresse)
   {

      /* Lecture d'un entier */
      if (adresse->type_de_base == type_predef[PREDEF_INTEGER])
      {
         generer1 (INN);
      }
      /* Lecture d'un boolÅÈen */
      else if (adresse->type_de_base == type_predef[PREDEF_BOOLEAN])
      {
         generer1 (INB);
      }
      /* Lecture d'un char */
      else if (adresse->type_de_base == type_predef[PREDEF_CHAR])
      {
         generer1 (INC);
      }
   }
}


/**
 *
 */
void lire ()
{
   Arbre adresse = NULL;

   generer_com ("lecture d'entier(s) au clavier");
   teste (READ_TOKEN);
   teste (PAR_OUV_TOKEN);
   adresse = var (CLASS_VAR);

   lire_une_var (adresse);

   while (token == VIRG_TOKEN)
   {
      next_token ();
      adresse = var (CLASS_VAR);
      lire_une_var (adresse);
   }

   teste (PAR_FER_TOKEN);
   generer_com ("FIN lecture d'entier(s) au clavier \n");
}


/*==============================================================================
  CONDITION
  ============================================================================*/
/**
 * VÅÈrifie la fin d'une expression boolÅÈenne dans une condition
 * Return : l'opÅÈrateur de comparaison trouvÅÈ
 */
Tokens cond_fin_boolean (Arbre adresse)
{
   Tokens operateur;
   /* Bon opÅÈrateur de comparaison = ou <> seulement */
   if (token != EGAL_TOKEN && token != DIFF_TOKEN)
      erreur (ERR_COMP_BOOL);

   operateur = token;
   next_token ();

   /* VÅÈrification du 2ÅËme membre de l'expression de comparaison */
   /* -> TRUE ou FALSE */
   if (token == TRUE_TOKEN || token == FALSE_TOKEN)
   {
      generer2i (LDI, (token == TRUE_TOKEN) ? (1) : (0));
      next_token ();
   }

   /* -> comparaison avec une variable */
   else
   {
      adresse = expr ();

      if (adresse && adresse->type_de_base != type_predef[PREDEF_BOOLEAN])
         erreur (ERR_COMP_BOOL_TO_BOOL);
   }
   return operateur;
}


/**
 *
 */
void cond_lire_un_car ()
{
   generer2i (LDI, (int)lire_car (&fichier_pascal));
   teste (QUOTE_TOKEN);
   teste (QUOTE_TOKEN);
}


/**
 * VÅÈrifie que le token courant est un token de comparaison (= <> ..)
 * Return : 1 si token est de comparaison, 0 sinon
 */
char verif_token_comp ()
{
   return (token == EGAL_TOKEN ||
           token == DIFF_TOKEN ||
           token == INF_TOKEN ||
           token == SUP_TOKEN ||
           token == INF_EGAL_TOKEN ||
           token == SUP_EGAL_TOKEN);
}

/**
 *
 */
void cond ()
{
   Tokens operateur = FALSE_TOKEN;
   Arbre adresse = NULL;

   /* Expression boolÅÈenne qui commence par les tokens TRUE ou FALSE */
   if (token == FALSE_TOKEN || token == TRUE_TOKEN )
   {

      /* Empile true (1) ou false (0) */
      generer2i (LDI, (token == TRUE_TOKEN) ? (1) : (0));
      next_token ();
      operateur = cond_fin_boolean (adresse);
   }

   /* Comparaison de char qui commence par 'c' */
   else if (token == QUOTE_TOKEN)
   {
      cond_lire_un_car ();
      if (verif_token_comp())
      {
         operateur = token;
         next_token ();
      }
      cond_lire_un_car ();
   }


   /* Toutes les expressions qui commencent par des variables */
   else
   {
      adresse = expr ();

      if (adresse)
      {
         /* Expression boolÅÈenne qui commence par une variable BOOL */
         if (adresse->type_de_base == type_predef[PREDEF_BOOLEAN])
         {

            /* Bon opÅÈrateur de comparaison = ou <> seulement */
            if (token != EGAL_TOKEN && token != DIFF_TOKEN)
               erreur (ERR_COMP_BOOL);

            operateur = cond_fin_boolean (adresse);
         }

         /* Comparaison de char qui commence par une variable CHAR */
         else if (adresse->type_de_base == type_predef[PREDEF_CHAR])
         {
            if (verif_token_comp())
            {
               operateur = token;
               next_token ();
            }
            cond_lire_un_car ();
         }

         /* Comparaison normale expr() COND expr() sur des entiers */
         /* /!\ SI le 1er expr() commence par une variable */
         else
         {
            if (verif_token_comp())
            {
               operateur = token;
               next_token ();
               expr ();
            }
         }
      }

      /* Comparaison normale expr() COND expr() sur des entiers */
      /* /!\ SI le 1er expr() NE commence PAS par une variable */
      else
      {
         if (verif_token_comp())
         {
            operateur = token;
            next_token ();
            expr ();
         }
      }
   }

   /* GÅÈnÅÈration de l'opÅÈrateur qu'il faut !! */
   switch (operateur)
   {
      case EGAL_TOKEN:
         generer1 (EQL);
         break;
      case DIFF_TOKEN:
         generer1 (NEQ);
         break;
      case INF_TOKEN:
         generer1 (LSS);
         break;
      case SUP_TOKEN:
         generer1 (GTR);
         break;
      case INF_EGAL_TOKEN:
         generer1 (LEQ);
         break;
      case SUP_EGAL_TOKEN:
         generer1 (GEQ);
         break;
      default:
         break;
   }
}


/*==============================================================================
  EXPRESSIONS NON CALCULÅ…ES
  ============================================================================*/
/**
 *
 */
Arbre expr ()
{
   Tokens op;
   Arbre id = term ();
   while (token == PLUS_TOKEN || token == MOINS_TOKEN)
   {
      op = token;
      next_token ();
      term();
      if (op == PLUS_TOKEN)
         generer1 (ADD);
      else
         generer1 (SUB);
   }
   return id;
}


/**
 *
 */
Arbre term ()
{
   Tokens op;
   Arbre id = fact ();
   while (token == MUL_TOKEN || token == DIV_TOKEN)
   {
      op = token;
      next_token ();
      fact ();
      if ( op == MUL_TOKEN )
         generer1 (MUL);
      else
         generer1 (DIV);
   }
   return id;
}


/**
 *
 */
Arbre fact ()
{
   Arbre adresse = NULL;

   int negation = 1 ;

   if (token == PLUS_TOKEN)
      next_token();

   else if (token == MOINS_TOKEN)
   {
      negation = -1 ;
      next_token();
   }

   if (token == ID_TOKEN)
   {

      /* ID = foncion */
      if ( (adresse = hash_search (&table, sym, CLASS_FONCTION)) )
      {
         appel ();
      }
      /* ID = variable */
      else
      {
         adresse = var (CLASS_VAR | CLASS_CONST);
      }


      if (!adresse)
         erreur (ERR_NO_ID); /* en fait syncro etc.... */
      else
      {
         if (adresse->classe != CLASS_CONST )
            generer1 (LDV);

         /* applique la néÈgation. */
         /* (ne peut etre appliquéÈ directement pour les CONST) */
         if (negation == -1)
         {
            generer2i (LDI, -1);
            generer1 (MUL);
         }
      }

   }


/* **************************** negatif :  + - */
   else if (token == NUM_TOKEN)
   {
      generer2i (LDI, valeur * negation);
      next_token ();
   }
   else
   {
      teste (PAR_OUV_TOKEN);
      expr ();
      teste (PAR_FER_TOKEN);
      /* s'il y a un moins devant l'expression, on la multiplie par -1 */
      if (negation == -1)
      {
         generer2i (LDI, -1);
         generer1 (MUL);
      }
   }
   return adresse;
}


/*==============================================================================
  EXPRESSIONS CALCULÅ…ES
  ============================================================================*/
/**
 * Pour les constantes on peut calculer l'expression numéÈrique qui leur est attribuéÈe
 * retour : la valeur de l'expression numéÈrique
 */
int expr_const ()
{
   int retour = term_const ();
   while (token == PLUS_TOKEN || token == MOINS_TOKEN)
   {
      if ( token == PLUS_TOKEN )
      {
         next_token ();
         retour += term_const ();
      }
      else
      {
         next_token ();
         retour -= term_const ();
      }
   }
   return retour;
}


/**
 *
 */
int term_const ()
{
   int retour = fact_const ();
   while (token == MUL_TOKEN || token == DIV_TOKEN)
   {
      if ( token == MUL_TOKEN )
      {
         next_token ();
         retour *= fact_const ();
      }
      else
      {
         next_token ();
         retour /= fact_const ();
      }
   }
   return retour;
}


/**
 *
 */
int fact_const ()
{
   Arbre adresse;
   int retour = 0;
   int negation = 1 ;

   if (token == PLUS_TOKEN)
      next_token();

   else
      if (token == MOINS_TOKEN)
      {
         negation = -1 ;
         next_token();
      }

   if (token == ID_TOKEN)
   {
      adresse = var (CLASS_CONST);
      if (!adresse)
         erreur (ERR_NO_ID); /* en fait syncro etc.... */
      else
         retour = adresse->adresse;
   }
   else if (token == NUM_TOKEN)
   {
      retour = valeur;
      next_token ();
   }
   else
   {
      teste (PAR_OUV_TOKEN);
      retour = expr_const ();
      teste (PAR_FER_TOKEN);
   }
   return negation * retour;
}


/*==============================================================================
  WHILE
  ============================================================================*/
/**
 *
 */
void tantque ()
{
   int debut = ligne_pcode;
   int sauv;

   teste (WHILE_TOKEN);
   cond ();

   generer2i (BZE, 0); /* 0 car incomplet */
   sauv = pile_saut_push (&sauts, ligne_pcode);

   teste (DO_TOKEN);
   inst ();

   generer2i (BRN, debut);
   pile_saut_update (&sauts, sauv, ligne_pcode+1);
}


/*==============================================================================
  FOR
  ============================================================================*/
/**
 * Le STEP doit absolument etre une valeur numéÈrique sans variable
 * ( calculable a partir de chiffre et/ou de constantes )
 * Aucun test n'est fait sur la modification de la variable de boucle dans la boucle
 * for u:=5 to 2 do    et
 * for u:=2 to 5 step 8 do    acceptéÈs.
 * Ya moyen de gerer la non-modification de la var de boucle :
 * en mettant un marqueur dans la table et tester ce marqueur dans les affectation
 * (ou bien la mettre classe=CONST pendant la duréÈe du for)...
 */
void pour ()
{
   Arbre adresse;
   char down = 0; /* booleen : incrementation ou decrementation */
   int ad_debut;  /* adresse du mnemonique de saut */
   int ad_jump;   /* adresse d'arrivee du saut */
   int le_pas;    /* sauvegardera la valeur du pas */

   generer_com("debut du for");

   teste (FOR_TOKEN);
   /* adresse = teste_et_cherche (ID_TOKEN, CLASS_VAR); */
   adresse = var (CLASS_VAR);
   if (!adresse)
      erreur (ERR_NO_ID); /* en fait syncro etc.... */


   /* sauvegarde de l'adresse de la variable de boucle */
   generer1(CPA);

   /* initialisation de la variable de boucle */
   teste (AFFEC_TOKEN);
   expr ();
   generer1 (STO);

   /* teste TO / DOWNTO */
   if (token == TO_TOKEN)
      next_token ();
   else if (token == DOWNTO_TOKEN)
   {
      next_token ();
      down = 1;
   }
   else
      erreur (ERR_TO);


   ad_debut = ligne_pcode;
   /* condition d'arret */
   generer1(CPA);  /* on sauvegarde l'adresse pour les boucles suivantes*/
   generer1 (LDV);
   expr ();
   (down) ?  generer1 (GEQ) :  generer1 (LEQ);  /* comparaison */
   generer2i (BZE, 0);
   ad_jump = pile_saut_push (&sauts, ligne_pcode);

   le_pas = pas ();
   teste (DO_TOKEN);
   generer_com("instructions dans le for");
   inst ();

   generer_com("*Fin instructions dans le for");

   /* incrementation de la variable */
   generer1(CPA);  /* on sauvegarde l'adresse pour la MAJ de la variable*/
   generer1(CPA);  /* on sauvegarde l'adresse pour la condition d'arret*/
   generer1 (LDV);
   generer2i (LDI, le_pas);
   (down) ? generer1 (SUB) : generer1 (ADD);

   generer1(STO);  /* MAJ de la variable de boucle */

   generer2i (BRN, ad_debut);

   generer_com("apres le BRN du for");
   pile_saut_update (&sauts, ad_jump, ligne_pcode+1);

   /* apres la boucle la variable est é‡ +STEP (ou-STEP) de la valeur de sortie */
   /* ( si GTR/LSS alors on ne passera pas dans la boucle pour la derniere valeur)*/
   /* si GEQ/LEQ alors on passera mais la valeur est INC une fois de trop */
   generer1(CPA);
   generer1 (LDV);
   generer2i (LDI, le_pas);
   (down) ? generer1 (ADD) : generer1 (SUB);
   generer1 (STO);
}



/**
 * retour : la valeur du STEP
 * par defaut le STEP est a 1
 */
int pas ()
{
   if (token == STEP_TOKEN)
   {
      next_token ();
      return expr_const ();
   }
   else
      return 1;
}


/*==============================================================================
  DO .. WHILE
  ============================================================================*/
/**
 *
 */
void repeat ()
{
   int ligne = ligne_pcode;
   teste (REPEAT_TOKEN);
   inst();
   while (token == PT_VIRG_TOKEN)
   {
      next_token ();
      inst ();
   }
   teste (UNTIL_TOKEN);
   cond ();
   generer2i (BZR, 2);
   generer2i (BRN, ligne);
}


/*==============================================================================
  SWITCH
  ============================================================================*/
/**
 *
 */
void cas ()
{
   char nb_test = 0; /* indique s'il y a 1 ou plusieurs valeur testee pour 1 cas */
   int ad_jump; /* adresse d'arrivee du saut */

   teste (CASE_TOKEN);
   /* adresse = teste_et_cherche (ID_TOKEN, CLASS_VAR); */
   var (CLASS_VAR);
   generer1 (LDV);

   teste (OF_TOKEN);

   do
   {
      label (&nb_test);

      generer2i (BZE, 0);
      ad_jump = pile_saut_push (&sauts, ligne_pcode);
      teste (DEUX_PT_TOKEN);
      inst ();
      pile_saut_update (&sauts, ad_jump, ligne_pcode+1);

      teste (PT_VIRG_TOKEN);
      nb_test = 0;
   }
   while (token != ELSE_TOKEN &&
          token != OTHERWISE_TOKEN &&
          token != END_TOKEN);

   elsecase ();
   teste (END_TOKEN);
}


/**
 *
 */
void elsecase ()
{
   /*   if (token == ELSE_TOKEN || token == OTHERWISE_TOKEN) */
/*    { */
/*        generer2i (LDA, adresse); */
/*         generer1 (LDV); */
/*           next_token (); */
/*          inst (); */
/*    } */
   generer1 (DEL); /* vire la valeur de sauvegarde */
}


/**
 * label va comparer les difféÈrentes valeurs é‡ tester a la varible du case
 * Le resultat de ces tests serontadditionnéÈs : Si il est difféÈrent de 0
 * alors au moins une expression valide le test : on JUMP dans les insts a effectuer.
 *
 */
void label (char *nb_test)
{
   if(*nb_test == 0)
   {
      generer1 (CPI); /* une fois pour sauvegarde la valeur pour les autres comparaisons */
      /*  generer1 (CPI);*/ /* une fois pour pouvoir supromer cette valeur de la pile (add+sub=-1valeur) */
   }
   else
      generer1 (CPJ); /* pour passéÈ 'en dessous' du resultat d'autres tests */

   /* on empile la valeur é‡ tester en sommet de pile */
   nums();

   if (token == PT_PT_TOKEN)
   {
      next_token ();
      generer1 (GEQ);

      generer1 (CPJ); /*chope la valeur a testéÈe en dessous du resultat du 1éÈ test */

      /* empilage et comparaison de la 2eme borne */
      nums ();
      generer1 (LEQ);

      /* si on est dans l'interval on a 1 et 1 */
      generer1 (ADD); /* pour avoir resultat des 2 a la fois */
      generer2i (LDI, 2 );
      generer1 (EQL);
      /* il reste sur la pile 0 si OK ( 1+1 == 2). 1 si FALSE */

      /* ici il reste sur la pile : sommet = resultat, ss-somet = la valeur */
   }
   else
      if (token == VIRG_TOKEN)
      {
         generer1 (EQL);

         next_token ();

         if (*nb_test != 0)
            generer1 (ADD);
         *nb_test = 1;

         label (nb_test);
      }
      else /* derniere valeur a tester */
      {
         generer1 (EQL);

         if (*nb_test != 0)
            generer1 (ADD);
         /* *nb_test = 1;  */

         /* il y a 2 fois la meme valeur en sous et sous-sous sommet de pile */
         /*   generer1 (ADD);
              generer1 (SUB); */
         /* ADD+SUB = éÈlimination de ces 2 valeurs de la pile. */
      }
}


/**
 * place la valeur a comparer en sommet de pile
 */
void nums ()
{
/*   Arbre adresse;*/
   if (token == ID_TOKEN)
   {
      /* adresse = teste_et_cherche (ID_TOKEN, CLASS_CONST); */
      var (CLASS_CONST );  /* et CLASS-VAR non mais !! */
/*       //LDV */
/*       //return adresse->adresse; */
   }
   else
   {
      expr();
      /*      //  LDI valeur; // si c'est pas mi en ahut de pile */
/*       /\* hep, normalement valeur est avant, a caus su nexttoken dans teste *\/ */
/*       //return valeur ; */
   }
}

