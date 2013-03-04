/**
 * module : lire_inst
 * brief : Lecture d'un fichier de pcode et remplissage de la pile d'.b��x��cution
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date : F��vrier 2004
 * bug : Aucun d��tect�� �� ce jour
 *
 * Ce fichier a pour but de lire un fichier de pcode et de remplir la
 * pile d'��x��cution. Elle v��rifie donc toute la validit�� syntaxique
 * du fichier pcode.
 * D.A��s qu'une erreur de syntaxe dans le fichier pcode est d��tect��e, on arr��te
 * d'empiler, mais on continue l'analyse du fichier afin d'afficher toutes les
 * erreurs du fichier.
 */


/*------------------------------------------------------------------------------
  Fichiers include
  ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#include "lire_inst.h"
#include "../common/mnemonique.h"
#include "erreur.h"
#include "pile_exe.h"



/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/**
 * V.b��rifie le mn��monique pass�� en param��tre
 * param cod : le code du mn��monique
 * return MNEMO_OK si le mn est correct et sans param��tre
 * return MNEMO_PARAM si le mn est correct et avec param��tre
 * return MNEMO_ERREUR si le mn est non reconnu
 */
int code_correct (int cod)
{
   int to_return; /* Le code de retour pr le return */

   switch (cod)
   {
      /* mn��moniques sans param��tres */
      case ADD:
      case SUB:
      case MUL:
      case DIV:
      case EQL:
      case NEQ:
      case GTR:
      case LSS:
      case GEQ:
      case LEQ:
      case PRN:
      case PRI:
      case PRB:
      case INN:
      case INC:
      case INB:
      case HLT:
      case STO:
      case LDV:
      case PRL:
      case CPA:
      case CPI:
      case CPJ:
      case DEL:
         to_return = MNEMO_OK;
         break;

         /* mn��moniques avec un param��tre de type entier*/
      case INT:
      case LDI:
      case LDA:
      case BRN:
      case BRR:
      case BZE:
      case BZR:
      case CAL:
      case RET:
         to_return = MNEMO_PARAM;
         break;

         /* mn��moniques avec un param��tre de type char */
      case PRC:
         to_return = MNEMO_CAR;
         break;

         /* mn��moniques avec un param��tre de type string */
      case PRS:
         to_return = MNEMO_STRING;
         break;

         /* mn��monique non reconnu */
      default:
         to_return = MNEMO_ERREUR;
         break;
   }

   return to_return;
}


/**
 * Passe tous les blancs (espaces et tabulations) et renvoi le dernier carat��re
 * non blanc. ��a ��vite de remettre ce carat��re dans le flux d'entr��e.
 * NB : cette fonction n'impose pas de rencontrer un blanc. D'o�� la fontion trim2 (+bas)
 * param *fic : le fichier �� lire
 * return Le premier caract��re non nul
 */
char trim (Fichier *fic)
{
   char c = lire_car (fic); /* Le caract��re lu */
   while ( c != FIN_FICHIER && ( c == CAR_ESPACE || c == CAR_TAB) )
      c = lire_car (fic);
   return c;
}


/**
 * Passe tous les blancs (espaces et tabulations). Cette fonction impose de
 * rencontrer 1+n blanc, avec 0 < n < +infini .
 * Le dernier caract��re non blanc lu est remi dans le flux d'entr��e.
 * param *fic : le fichier �� lire
 * return 1 si au moins un blanc est trouv��. 0 sinon.
 */
char trim2 (Fichier *fic)
{
   char c = lire_car (fic); /* Le caract��re lu */
   char retour;              /* Le code de retour pr le return */

   /* Aucun blanc n'est recontr�� -> erreur */
   if ( c != FIN_FICHIER && ( c != CAR_ESPACE && c != CAR_TAB))
      retour = 0;

   /* Un blanc est rencontr��. On passe tous les blancs suivants. */
   else
   {
      while ( c != FIN_FICHIER && ( c == CAR_ESPACE || c == CAR_TAB) )
         c = lire_car (fic);
      retour = 1;
   }

   /* Remise dans le flux du dernier caract��re non blanc */
   insere_car (fic, c);

   return retour;
}


/**
 * Cette fonction supprime tout les caract��res jusqu'�� ce qu'elle rencontre
 * le caract��re de fin de ligne '\n'. Le dernier caract��re lu ��tant '\n', on
 * a pas besoin de remettre de caract��re dans le flux.
 * param *fic : le fichier �� lire
 */
void suppr_commentaire (Fichier *fic)
{
   char c = lire_car (fic);
   while (c != CAR_LIGNE && c != FIN_FICHIER)
      c = lire_car (fic);
}


/**
 * Cette fonction sert lors de la lecture du mnemonique.
 * Chaque caract��re du mnemonique doit .A��tre alphab.b��tique.
 * Si ce n'est pas le cas, une erreur est affich��e car la fin de l'instruction
 * est attendue.
 * param c : le caract��re lu.
 * param *fic : le fichier �� lire
 * param num_ligne : le num��ro de ligne courant dans le fichier de pcode
 * return 1 si *c est alphab��tique, 0 sinon
 */
int verif_fin_inst_ok (char *c, Fichier *fic, long int num_ligne)
{
   if (!isalpha (*c))
   {
      erreur (ERR_END_INST, num_ligne);

      if (*c == CAR_LIGNE)
         *c = lire_car (fic);
      else
      {
         suppr_commentaire (fic);
         *c = lire_car (fic);
      }
      return 0;
   }
   return 1;
}


/**
 * Cette fontion a pour but de v��rifier que tous les caract��res suivant
 * une instruction sur une ligne sont correct. On ne peut rencontrer que
 * des blancs, une fin de ligne '\n' ou bien un commentaire '#'.
 * param c : le caract��re lu.
 * param *fic : le fichier �� lire
 * param num_ligne : le num��ro de ligne courant dans le fichier de pcode
 * return 1 si la fin de ligne est OK, 0 sinon
 */
int verif_apres_inst_ok (char *c, Fichier *fic, long int num_ligne)
{
   for (;;)
   {
      /* Si on a un espace / tab on continue de lire */
      if (*c == CAR_ESPACE || *c == CAR_TAB)
         *c = lire_car (fic);

      /* Si on a un commentaire, on le supprime ; Status OK*/
      else if (*c == CAR_COMMENT)
      {
         suppr_commentaire (fic);
         *c = lire_car (fic);
         return 1;
      }

      /* On est �� la fin du fichier : Status OK */
      else if (*c == FIN_FICHIER)
         return 1;

      /* On est �� la fin de la ligne : Status OK */
      else if (*c == CAR_LIGNE)
      {
         *c = lire_car (fic);
         return 1;
      }

      /* Une erreur est d��tect��e : On va �� la fin de la ligne ; Status KO */
      else
      {
         erreur (ERR_UNKNOW_CAR, num_ligne);
         suppr_commentaire (fic);
         *c = lire_car (fic);
         return 0;
      }
   }
}


/**
 * Fonction appel��e pour remplir la pile d'��x��cution. C'est elle
 * qui v��rifie la validit�� syntaxique du fichier pcode.
 * param fic : le fichier �� lire
 * param *p_exe : la pile d'��x��cution �� remplir
 * return : 0 si le fichier pcode est valide, 1 sinon
 */
int remplir_pile (Fichier fic, Pile_exe *p_exe)
{
   int i = 0;                        /* Parcours de boucle */
   char inst[4];                     /* Stocke l'instruction courrante */
   char c;                           /* Stocke le caract��re courant */
   int lecode = 0;                   /* Stocke le code correspondant �� l'instrution lue */
   int arg = 0;                      /* Stocke le param��tre d'une instruction */
   unsigned long int num_ligne = 0;  /* Stocke le num��ro courant de ligne courant dans le fichier pcode */
   int err = 0;                      /* Flag qui indique si une erreur a ��t�� recontr��e ;
                                        Provoque l'arr��t de l'empilage, mais la lecture du fichier continu. */
   int inst_correct = 0;             /* Flag qui indique si l'instruction est correcte */
   int arg_lu = 0;                   /* Flag qui indique si l'argument a ��t�� lu */
   int arg_negatif = 0;              /* Flag qui indique si l'argument est n��gatif */
   Instruction mn;                   /* L'instruction que l'on va empiler */
   char arg_c;
   char prs_bool=0;
   char cs;
   char jump=0;

   inst[3] = '\0';
   c = trim (&fic);

   /* On parcours tout le fichier */
   while (c != FIN_FICHIER && !jump)
   {
      num_ligne++;
      inst_correct = 1;

      /* Ligne de commentaire  */
      if (c == CAR_COMMENT)
      {
         suppr_commentaire (&fic);
         c = trim (&fic);
      }

      /* Ligne vide */
      else if (c == CAR_LIGNE)
      {
         c = trim (&fic);
      }
      else
      {
         /* Lecture des 3 caract��res de l'instruction */
         i=0;
         while (inst_correct && i < 3)
         {
            inst[i] = toupper (c);
            if (verif_fin_inst_ok (&c, &fic, num_ligne) == 0)
            {
               inst_correct = 0;
               err = 1;
            }
            else if (i < 2)
               c = lire_car (&fic);
            i++;
         }

         /* Ici les 3 caract��res de l'instruction ont ��t��s lus correctement */
         /* On va v��rifier que c'est une instruction reconnue */
         /* Et ��ventuellement rechercher le param��tre */
         if (inst_correct)
         {

            /* On switche en fonction du code de l'instruction lue */
            lecode = code (inst[0],inst[1],inst[2]);
            switch (code_correct (lecode))
            {
               /* Mnemonique reconnu et sans param��tre */
               case MNEMO_OK:
                  c = lire_car (&fic);

                  if (!verif_apres_inst_ok (&c, &fic, num_ligne))
                     err = 1;
                  break;

                  /* Mnemonique reconnu et avec param��tre */
               case MNEMO_PARAM:

                  /* V��rification de l'espace apr��s le mn��monique */
                  if (!trim2 (&fic))
                  {
                     erreur (ERR_MISSING_SPACE, num_ligne);
                     err = 1;
                  }
                  else
                  {
                     c = trim (&fic);
                     /* -arg */
                     if (c == CAR_MOINS)
                     {
                        arg_negatif = 1;
                        c = lire_car (&fic);
                     }
                     /* +arg */
                     else if (c == CAR_PLUS)
                     {
                        c = lire_car (&fic);
                     }

                     while (isdigit (c))
                     {
                        arg = arg*10 + atol (&c);
                        arg_lu = 1;
                        c = lire_car (&fic);
                     }

                     if (!arg_lu)
                     {
                        err = 1;
                        erreur (ERR_MISSING_PARAM, num_ligne);
                     }
                     else if (arg_negatif)
                     {
                        arg = -arg;
                        arg_negatif = 0;
                     }

                     if (!verif_apres_inst_ok (&c, &fic, num_ligne))
                        err = 1;
                     else
                        mn.param = arg;

                     arg = 0;
                     arg_lu = 0;
                  }
                  break;

               case MNEMO_CAR:

                  /* V��rification de l'espace apr��s le mn��monique */
                  if (!trim2 (&fic))
                  {
                     erreur (ERR_MISSING_SPACE, num_ligne);
                     err = 1;
                  }
                  else
                  {
                     /* Lecture du caract��re */
                     c = lire_car (&fic);

                     /* V��rifie si pas fin de ligne ni de commentaire */
                     if (c == CAR_LIGNE || c == CAR_COMMENT)
                     {
                        err = 1;
                        erreur (ERR_MISSING_PARAM, num_ligne);
                        suppr_commentaire (&fic);
                        c = trim (&fic);
                     }
                     else
                     {
                        /* Quote ouvrante */
                        if (c != CAR_QUOTE)
                        {
                           erreur (ERR_MISSING_OPEN_QUOTE, num_ligne);
                           err = 1;
                           suppr_commentaire (&fic);
                           c = trim (&fic);
                        }
                        else
                        {
                           arg_c = lire_car (&fic);

                           /* Quote fermante */
                           if (c != CAR_QUOTE)
                           {
                              erreur (ERR_MISSING_CLOSE_QUOTE, num_ligne);
                              err = 1;
                              suppr_commentaire (&fic);
                              c = trim (&fic);
                           }
                           else
                           {
                              suppr_commentaire (&fic);
                              c = trim (&fic);
                              mn.param = (int) arg_c;
                           }
                        }
                     }
                     /* arg_lu = 0; */
                  }

                  break;

               case MNEMO_STRING:

                  /* V��rification de l'espace apr��s le mn��monique */
                  if (!trim2 (&fic))
                  {
                     erreur (ERR_MISSING_SPACE, num_ligne);
                     err = 1;
                  }
                  else
                  {
                     mn.mn = PRC;
                     c = lire_car (&fic);
                     if (c == CAR_LIGNE || c == CAR_COMMENT)
                     {
                        err = 1;
                        erreur (ERR_MISSING_PARAM, num_ligne);
                        suppr_commentaire (&fic);
                        c = trim (&fic);
                     }
                     else
                     {
                        if (c != CAR_DQUOTE)
                        {
                           erreur (ERR_MISSING_OPEN_DQUOTE, num_ligne);
                           err = 1;
                           suppr_commentaire (&fic);
                           c = trim (&fic);
                        }
                        else
                        {
                           c = lire_car (&fic);
                           while (c != CAR_LIGNE && c != CAR_COMMENT && c != CAR_DQUOTE)
                           {
                              mn.param = (int)c;
                              push_exe (p_exe, mn);
                              c = lire_car (&fic);
                           }
                           if (c != CAR_DQUOTE)
                           {
                              erreur (ERR_MISSING_CLOSE_DQUOTE, num_ligne);
                              err = 1;
                              suppr_commentaire (&fic);
                              c = trim (&fic);
                           }
                           else
                           {
                              suppr_commentaire (&fic);
                              c = trim (&fic);
                           }
                        }
                     }
                     prs_bool = 1;
                  }

                  break;

                  /* Mnemonique non reconnu */
               case MNEMO_ERREUR:
                  erreur (ERR_UNKNOW_MNEMO, num_ligne);
                  suppr_commentaire (&fic);
                  c = trim (&fic);
                  err = 1;
                  break;
            } /* Fin switch (lecode) */

            /* Si il n'y a pas d'erreur, on peut empiler l'instruction */
            if (!err && !prs_bool)
            {
               mn.mn = lecode;
               push_exe (p_exe, mn);
            }
            else
            {
               prs_bool = 0;
            }

         } /* Fin if (inst_correct) */
      } /* Fin v��rif diverses (lignes vides + commentaires) de l'instruction */


      /* V.A��rifie si on trouve le marqueur de d��but des sauts */
      if (c == '$')
      {
         cs = lire_car (&fic);
         if (cs == 'j' || cs == 'J')
         {
            jump = 1;
            suppr_commentaire (&fic);
         }
         else
            insere_car (&fic, cs);
      }

   } /* Fin du fichier */

   /* Si on a trouv�� le marqueur des sauts, on met �� jour les sauts */
   if (jump)
      remplir_sauts (&fic, p_exe);

   return err;
}


/**
 * Lit un entier dans le fichier
 * param fic : le fichier .b�� lire
 * return : l'entier lu
 */
int lire_entier (Fichier *fic)
{
   int valeur =0;
   char c;

   c = lire_car (fic);
   if (isdigit (c))
   {
      valeur = atoi(&c);
      c = lire_car (fic);
      while (isdigit (c))
      {
         valeur = (valeur*10) + atoi (&c);
         c = lire_car (fic);
      }

      /* Remise dans le flux du dernier caract.A��re non num��rique */
      insere_car (fic, c);
   }
   return valeur;
}


/**
 * Met �� jour tous les sauts du programme
 * param fic : le fichier .b�� lire
 * param *p_exe : la pile d'��x��cution o.A�� l'on doit modifier les sauts
 */
void remplir_sauts (Fichier *fic, Pile_exe *p_exe)
{
   int depart;
   int arrivee;
   char espace;
   char c;

   c = lire_car (fic);

   while (c != FIN_FICHIER)
   {
      insere_car (fic,c);
      depart = lire_entier (fic);
      espace = trim2 (fic);
      arrivee = lire_entier (fic);
      suppr_commentaire (fic);

      pile_exe_maj_saut (p_exe, depart, arrivee);
      c = lire_car (fic);
   }
}
