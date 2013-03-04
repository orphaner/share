/**
 * module : interpreteur
 * brief : Interpr��tation du fichier de pcode
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date : F��vrier 2004
 * bug : Aucun d��tect�� �� ce jour
 *
 * Ici on simule l'��x��cution du fichier de pcode. Pour cel�� on utilise deux
 * piles : la pile des instructions et la pile des variables.
 * Chaque instruction de la pile d'instruction est interpr��t��e.
 *
 * Un mini d��bugeur est int��gr��. Il permet pour chaque instruction la
 * visualisation de :
 * - l'instruction courante
 * - l'instruction pr��c��dente
 * - l'instruction suivante
 * - le segment de donn��es
 * - le sommet et le sous somment de la pile
 */

/*------------------------------------------------------------------------------
  Fichiers include
  ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../common/mnemonique.h"
#include "erreur.h"
#include "pile_exe.h"
#include "pile_var.h"
#include "interpreteur.h"


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/

/* =============================================================================
   PCODE : OPERATIONS DE BASE
   ===========================================================================*/

/**
 * ADD : Additionne le sous-sommet de pile et le sommet, laisse le r��sultat au sommet.
 * param *p_var : la pile de variables
 */
void add (Pile_var *p_var)
{
   push_sommet_var(p_var, get_sous_sommet_var(*p_var) + pop_var(p_var) );
}


/**
 * SUB : Soustrait le sous-sommet de pile et le sommet, laisse le r��sultat au sommet.
 * param *p_var : la pile de variables
 */
void sub (Pile_var *p_var)
{
   push_sommet_var(p_var, get_sous_sommet_var(*p_var) - pop_var(p_var));
}


/**
 * MUL : Multiplie le sous-sommet de pile et le sommet, laisse le r��sultat au sommet.
 * param *p_var : la pile de variables
 */
void mul (Pile_var *p_var)
{
   push_sommet_var(p_var, get_sous_sommet_var(*p_var) * pop_var(p_var));
}


/**
 * DIV : Divise le sous-sommet de pile et le sommet, laisse le r��sultat au sommet.
 * param *p_var : la pile de variables
 */
void division (Pile_var *p_var)
{
   if ( !get_sommet_var(*p_var) )
      free_all (EXIT_INTER);

   push_sommet_var(p_var, get_sous_sommet_var(*p_var) / pop_var(p_var));
}


/* =============================================================================
   PCODE : FONCTIONS DE COMPARAISON
   ===========================================================================*/

/**
 * EQL : Laisse 1 au sommet de pile si sous-sommet == sommet, 0 sinon.
 * param *p_var : la pile de variables
 */
void eql (Pile_var *p_var)
{
   push_sommet_var(p_var, ( pop_var(p_var) == get_sommet_var(*p_var) ) ? 1 : 0 );
}


/**
 * NEQ : Laisse 1 au sommet de pile si sous-sommet != sommet, 0 sinon.
 * param *p_var : la pile de variables
 */
void neq (Pile_var *p_var)
{
   push_sommet_var(p_var, ( pop_var(p_var) != get_sommet_var(*p_var) ) ? 1 : 0 );
}


/**
 * GTR : Laisse 1 au sommet de pile si sous-sommet < sommet, 0 sinon.
 * param *p_var : la pile de variables
 */
void gtr (Pile_var *p_var)
{
   push_sommet_var(p_var, ( pop_var(p_var) < get_sommet_var(*p_var) ) ? 1 : 0 );
}


/**
 * LSS : Laisse 1 au sommet de pile si sous-sommet > sommet, 0 sinon.
 * param *p_var : la pile de variables
 */
void lss (Pile_var *p_var)
{
   push_sommet_var(p_var, ( pop_var(p_var) > get_sommet_var(*p_var) ) ? 1 : 0 );
}


/**
 * GEQ : Laisse 1 au sommet de pile si sous-sommet <= sommet, 0 sinon.
 * param *p_var : la pile de variables
 */
void geq (Pile_var *p_var)
{
   push_sommet_var(p_var, ( pop_var(p_var) <= get_sommet_var(*p_var) ) ? 1 : 0 );
}


/**
 * LEQ : Laisse 1 au sommet de pile si sous-sommet >= sommet, 0 sinon.
 * param *p_var : la pile de variables
 */
void leq (Pile_var *p_var)
{
   push_sommet_var(p_var, ( pop_var(p_var) >= get_sommet_var(*p_var) ) ? 1 : 0 );
}


/* =============================================================================
   PCODE : FONCTIONS D'AFFICHAGE
   ===========================================================================*/


/**
 * PRN : imprime le sommet, d��pile
 * param *p_var : la pile de variables
 */
void prn (Pile_var *p_var, char debug)
{
   if (debug)
      printf ("\n---Affichage : ");
   printf ("%d", pop_var(p_var));

   if (debug)
      printf ("\n");
}


/**
 * PRB : imprime 'vrai' si le sommet et != 0, 'faux' sinon. D��pile
 * param *p_var : la pile de variables
 */
void prb (Pile_var *p_var, char debug)
{
   if (debug)
      printf ("\n---Affichage : ");
   printf( (pop_var (p_var)) ? "VRAI" : "FAUX" );

   if (debug)
      printf ("\n");
}


/**
 * PRI : imprime le sommet, ne d��pile pas
 * param *p_var : la pile de variables
 */
void pri (Pile_var p_var, char debug)
{
   if (debug)
      printf ("\n---Affichage : ");
   printf ("%d", get_sommet_var(p_var));

   if (debug)
      printf ("\n");
}


/**
 * PRC : imprime le caractere pass�� en parametre de l'instruction
 * param *p_exe : la pile d'execution
 */
void prc (Pile_exe *p_exe, char debug)
{
   if (debug)
      printf ("\n---Affichage : ");
   printf ("%c", (char)get_param_exe(p_exe));

   if (debug)
      printf ("\n");
}


/**
 * PRD : imprime le caractere qui est au sommet de pile
 * param *p_exe : la pile d'execution
 */
void prd (Pile_var *p_var, char debug)
{
   if (debug)
      printf ("\n---Affichage : ");
   printf ("%c", (char) pop_var (p_var));

   if (debug)
      printf ("\n");
}


/**
 * PRL : imprime un retour �� la ligne
 */
void prl (char debug)
{
   if (debug)
      printf ("\n---Affichage : 'retour �� la ligne' \n");
   else
      printf ("\n");
}



/* =============================================================================
   PCODE : FONCTIONS D'ACCES A LA PILE
   ===========================================================================*/

/**
 * INN : Lit un entier, le stocke �� l'adresse trouv��e au sommet de pile, d��pile.
 * param *p_var : la pile de variables
 */
void inn (Pile_var *p_var)
{
   int total = 0;
   char lu, saisie = 0;

   for(;;)
   {
      lu = getchar();
      if ( isdigit (lu ) )
      {
         total = total * 10 + (lu - '0');
         saisie = 1;
      }
      else
      {
         if(lu != '\n' )
         {
            /*   fprintf (stderr, "Error: Saisie incorrecte. Arret du programme.\n"); */
            /*   free_all (EXIT_SAISIE); */
            printf (" -!-WARNING-!- Saisie incorrecte. Recommencez \n");
            total = 0;
            saisie = 0;
            while ( getchar() != '\n' );

         }
         else
         {
            if (!saisie)
            {
               /*  fprintf (stderr," -!-WARNING-!- Valeur non saisie. La valeur par d��fault est 0. \n"); */
               /*  free_all (EXIT_SAISIE); */
               printf (" -!-WARNING-!- Valeur non saisie. Recommencez \n");
               total = 0;
               saisie = 0;
            }
            else
            {
               push_to_var( p_var, pop_var(p_var), total);
               break;
            }
         }
      }
   }
}

/**
 * INC : Lit un caractere, le stocke �� l'adresse trouv��e au sommet de pile, d��pile.
 * param *p_var : la pile de variables
 */
void inc (Pile_var *p_var)
{
   char lu, suivant;

   for(;;)
   {
      lu = getchar();

      if(lu != '\n' )
      {
         suivant = getchar();
         if (suivant != '\n')
         {
            printf (" -!-WARNING-!- Un (1) seul caractere doit etre saisi. Recommencez \n");
            while ( getchar() != '\n' );
         }
         else
         {
            push_to_var( p_var, pop_var(p_var), lu);
            break;
         }
      }
      else
         printf (" -!-WARNING-!- Valeur non saisie. Recommencez \n");


   }
}


/**
 * INB : Lit un bool��en, le stocke �� l'adresse trouv��e au sommet de pile, d��pile.
 * param *p_var : la pile de variables
 * BOOLEAN est '1' ou '0'
 */
void inb (Pile_var *p_var)
{
   char lu, suivant;

   for(;;)
   {
      lu = getchar();

      if(lu != '\n' )
      {
         suivant = getchar();
         if (suivant != '\n')
         {
            printf (" -!-WARNING-!- Un (1) seul caractere doit etre saisi : '1' ou '0'. Recommencez \n");
            while ( getchar() != '\n' );
         }
         else
         {
            if ( lu == '1' || lu == '0')
            {
               push_to_var( p_var, pop_var(p_var), lu - '0' );
               break;
            }
            else
               printf (" -!-WARNING-!-  Saisie incorrecte. vrai = '1' faux = '0'. Recommencez \n");
         }
      }
      else
         printf (" -!-WARNING-!- Valeur non saisie. vrai = '1' faux = '0'. Recommencez \n");


   }
}


/**
 * INT c : Incr��mente de la constante c le pointeur de pile (la constante c peut ��tre n��gative)
 * param *p_var : la pile de variables
 * param *p_exe : la pile d'��x��cution
 */
void inc_sp (Pile_var *p_var, Pile_exe *p_exe)
{
   inc_sp_var (p_var, get_param_exe(p_exe));
/*    assign_segment (p_var, get_param_exe(p_exe)); /\* Ne sert que pour le d��bugeur *\/ */
}


/**
 * LDI v : Empile la valeur v
 * param *p_var : la pile de variables
 * param *p_exe : la pile d'��x��cution
 */
void ldi (Pile_var *p_var, Pile_exe *p_exe)
{
   push_var (p_var, get_param_exe(p_exe));
}


/**
 * LDA a : Empile l'adresse a
 * param *p_var : la pile de variables
 * param *p_exe : la pile d'��x��cution
 */
void lda (Pile_var *p_var, Pile_exe *p_exe)
{
   push_var (p_var, get_param_exe(p_exe));
}


/**
 * LDI v : Empile la valeur v
 * param *p_var : la pile de variables
 */
void ldv (Pile_var *p_var)
{
   /* push la valeur a l'adresse r��cup��r��e en haut de la pile) */
   push_var(p_var, get_from_var (*p_var, pop_var (p_var) ) );
}


/**
 * STO : Stocke la valeur au sommet �� l'adresse indiqu��e par le sous-sommet, d��pile 2 fois.
 * param *p_var : la pile de variables
 */
void sto (Pile_var *p_var)
{
   push_to_var(p_var, pop_var (p_var), pop_var (p_var));
}


/* =============================================================================
   PCODE : BRANCHEMENT A L'INSTRUCTION i
   ===========================================================================*/

/**
 * BRN i : Branchement inconditionnel �� l'instruction i. Adressage absolu.
 * param *p_exe : la pile d'��x��cution
 */
void brn (Pile_exe *p_exe)
{
   goto_abs_exe (p_exe, get_param_exe(p_exe) );
}


/**
 * BRR : Branchement inconditionnel �� l'instruction i. Adressage relatif.
 * param *p_exe : la pile d'��x��cution
 */
void brr (Pile_exe *p_exe)
{
   goto_relatif_exe (p_exe, get_param_exe(p_exe) );
}


/**
 * BZE i : branchement �� l'instruction i si le sommet = 0, d��pile. Adressage absolu.
 * param *p_var : la pile de variables
 * param *p_exe : la pile d'��x��cution
 */
void bze (Pile_var *p_var, Pile_exe *p_exe)
{
   if (!pop_var(p_var))
      goto_abs_exe (p_exe, get_param_exe(p_exe) );
}


/**
 * BZR i : branchement �� l'instruction i si le sommet = 0, d��pile. Adressage relatif.
 * param *p_var : la pile de variables
 * param *p_exe : la pile d'��x��cution
 */
void bzr (Pile_var *p_var, Pile_exe *p_exe)
{
   if (!pop_var(p_var))
      goto_relatif_exe (p_exe, get_param_exe(p_exe) );
}


/* =============================================================================
   PCODE : PROCEDURES et FONCTIONS
   ===========================================================================*/


/**
 * CAL i : empile le compteur d'instructions et r��alise le branchement �� l'instruction i
 * param *p_var : la pile de variables
 * param *p_exe : la pile d'��x��cution
 */
void cal (Pile_var *p_var, Pile_exe *p_exe, Pile_var *p_base)
{
   /* empile l'adresse de l'instruction suivant l'appel */
   push_var (p_var, (*p_exe).pc + 1);

   /* sauvegarde de la base de la procedure sur la pile des vars */
   push_var (p_base, (*p_var).sp ) ;

   /* saut dans les instructions */
   goto_abs_exe (p_exe, get_param_exe(p_exe) );
   /* (*p_exe).pc = get_param_exe(p_exe) - 1 ;       */
}


/**
 * RET i : n��ttoie la pile des emplacements pour les paramettres des procedures
 * d��pile un num��ro d'instruction et place le pointeur d'instruction �� ce num��ro
 * param *p_var : la pile de variables
 * param *p_exe : la pile d'��x��cution
 */
void ret (Pile_var *p_var, Pile_exe *p_exe, Pile_var *p_base)
{
   /* retour a la base des calculs de la proc��dure */
   /* on enl��ve la diff��rence entre la base et l'��tat actuel */
   /* = ajout  base - SP */

   inc_sp_var (p_var, pop_var (p_base) -  (*p_var).sp  );

/*   printf("rerere %d \n", pop_var (p_base) -  (*p_var).sp ); */

   /* pointeur de pile moins le param (la taille des params de la proc)*/
   inc_sp_var (p_var, -get_param_exe(p_exe));

   /* retour a l'instruction suivant l'appel de la proc */
   goto_abs_exe (p_exe, pop_var (p_var));
}


/**
 * LDL i :
 * param *p_var : la pile de variables
 * param *p_exe : la pile d'��x��cution
 */
/* void ldl (Pile_var *p_var, Pile_exe *p_exe, Pile_var *p_base) */
/* { */
/*    /\* i = (base - sp) - adresse relative a la base de la var  *\/ */
/*    get_from_var ( *p_var, get_param_exe(p_exe)); */

/* } */


/* =============================================================================
   PCODE : AUTRES
   ===========================================================================*/


/**
 * CPA / CPI : Copie la valeur de l'adresse/entier en sommet de pile sur le sur-sommet
 * param *p_var : la pile de variables
 */
void cp (Pile_var *p_var)
{
   push_var (p_var, get_sommet_var(*p_var));
}


/**
 * CP2 : Copie la valeur de l'entier en sommet de pile sur le sur-sommet
 * param *p_var : la pile de variables
 */
void cpj (Pile_var *p_var)
{
   push_var (p_var, get_sous_sommet_var(*p_var));
}


/**
 * DL : Supprime l'entier du dessus de la pile (sert rarement (cf cas()))
 * param *p_var : la pile de variables
 */
void del (Pile_var *p_var)
{
   inc_sp_var (p_var, -1);
}



/* =============================================================================
   INTERPRETEUR : FONCTIONS PRINCIPALES
   ===========================================================================*/
/**
 * Affiche les informations suivantes en mode DEBUG :
 * - l'instruction courante
 * - l'instruction pr��c��dente
 * - l'instruction suivante
 * - le segment de donn��es
 * - le sommet et le sous somment de la pile
 * param *p_var : la pile de variables
 * param *p_exe : la pile d'��x��cution
 */
void affiche_debug (Pile_var *p_var, Pile_exe *p_exe)
{
   int i = PILE_VAR_VIDE; /* Compteur de boucle */
   char s_inst_debug[4]; /* Contient la chaine d'un mn��monique */

   printf ("Instruction num : %ld \n", p_exe->pc);

   /* Instruction pr��c��dente
      NB: Ne marche pas si l'instruction pr��c��dente est un saut */
   if (p_exe->pc >  PILE_EXE_VIDE + 1)
   {
      revert_code (p_exe->inst[p_exe->pc - 1].mn, s_inst_debug);
      printf (" * Instruction pr��c��dente : %s\n", s_inst_debug);
      if (code_correct(p_exe->inst[p_exe->pc - 1].mn) == MNEMO_PARAM)
         printf("   -> Param��tre entier : %d\n", p_exe->inst[p_exe->pc - 1].param);
      else if (code_correct(p_exe->inst[p_exe->pc - 1].mn) == MNEMO_CAR)
         printf("   -> Param��tre char : %c\n", (char) p_exe->inst[p_exe->pc - 1].param);
   }

   /* Instruction courrante */
   revert_code(get_mn_exe (p_exe), s_inst_debug);
   printf (" * Instruction courante : %s\n", s_inst_debug);
   if (code_correct(get_mn_exe (p_exe)) == MNEMO_PARAM)
      printf("   -> Param��tre entier : %d\n", get_param_exe(p_exe));
   else if (code_correct(p_exe->inst[p_exe->pc].mn) == MNEMO_CAR)
      printf("   -> Param��tre char : %c\n", (char) p_exe->inst[p_exe->pc].param);

   /* Instruction suivante */
   if (p_exe->pc < p_exe->taille-1)
   {
      revert_code (p_exe->inst[p_exe->pc + 1].mn, s_inst_debug);
      printf (" * Instruction suivante : %s\n", s_inst_debug);
      if (code_correct(p_exe->inst[p_exe->pc + 1].mn) == MNEMO_PARAM)
         printf("   -> Param��tre entier : %d\n", p_exe->inst[p_exe->pc + 1].param);
      else if (code_correct(p_exe->inst[p_exe->pc + 1].mn) == MNEMO_CAR)
         printf("   -> Param��tre char : %c\n", (char) p_exe->inst[p_exe->pc + 1].param);
   }

   /* Segment de donn��es */
   printf (" * Segment de donn��es :\n");
   if (p_var->segment == PILE_VAR_VIDE)
      printf("  - VIDE\n");
   else
      for ( ; i < p_var->segment  ; i++)
         printf ("   - adr(%d) = %d\n", i + 1, get_from_var (*p_var, i + 1));

   if (p_var->sp >  p_var->segment)
   {
      printf (" * Sommet Pile : A  %d \n", get_sommet_var (*p_var) );
      if (p_var->sp >  p_var->segment + 1)
         printf ("                 B  %d \n", get_sous_sommet_var (*p_var) );
      else
         printf ("                 B  $ Pas d'autres ��l��ments \n");
   }
   else
      printf (" * Pile vide \n");
}


/**
 * La fonction d'interpr��tation du pcode, enfin ici :)
 * Boucle : Avance dans la pile d'instruction jusqu'�� HLT ou tant que la
 * pile d'instruction n'est pas vide.
 * Attribution des actions selon les instructions.
 * Modification de la pile des variables selon instructions.
 * Apr��s chaque instruction effectu��e on passe �� la suivante.
 * Appel �� affiche_info en mode d��bug
 * param *p_var : la pile de variables
 * param *p_exe : la pile d'��x��cution
 * param debug : Si = 1, provoque l'appel au mini-d��bugeur
 */
void execute (Pile_var *p_var, Pile_exe *p_exe, Pile_var *p_base, char debug)
{

   /* Tant que la pile d'��x��cution n'est pas vide */
   while (p_exe->pc != p_exe->taille)
   {

      /* Appel �� la fonction de d��bug  */
      if (debug)
      {
         affiche_debug (p_var, p_exe);
         getchar (); /* Pour attendre l'appui sur entr��e. Il doit exister mieux !! */
      }

      switch (get_mn_exe (p_exe))
      {
         case ADD:
            add (p_var);
            break;

         case SUB:
            sub (p_var);
            break;

         case MUL:
            mul (p_var);
            break;

         case DIV:
            division (p_var);
            break;

         case EQL:
            eql (p_var);
            break;

         case NEQ:
            neq (p_var);
            break;

         case GTR:
            gtr (p_var);
            break;

         case LSS:
            lss (p_var);
            break;

         case GEQ:
            geq (p_var);
            break;

         case LEQ:
            leq (p_var);
            break;

         case PRN:
            prn (p_var, debug);
            break;

         case PRI:
            pri (*p_var, debug);
            break;

         case PRC:
            prc (p_exe, debug);
            break;

         case PRD:
            prd (p_var, debug);
            break;

         case PRB:
            prb (p_var, debug);
            break;

         case PRL:
            prl (debug);
            break;

         case INN:
            inn (p_var);
            break;

         case INC:
            inc (p_var);
            break;

         case INB:
            inb (p_var);
            break;

         case INT:
            inc_sp (p_var, p_exe);
            break;

         case LDI:
            ldi (p_var, p_exe);
            break;

         case LDA:
            lda (p_var, p_exe);
            break;

         case LDV:
            ldv (p_var);
            break;

         case STO:
            sto (p_var);
            break;

         case BRN:
            brn (p_exe);
            break;

         case BRR:
            brr (p_exe);
            break;

         case BZE:
            bze (p_var, p_exe);
            break;

         case BZR:
            bzr (p_var, p_exe);
            break;

         case CAL:
            cal (p_var, p_exe, p_base);
            break;

         case RET:
            ret (p_var, p_exe, p_base);
            break;

         case CPA:
         case CPI:
            cp (p_var);
            break;

         case CPJ:
            cpj (p_var);
            break;

         case DEL:
            del (p_var);
            break;

         case HLT:
            printf ("\n");
            free_all (EXIT_OK);
            break;

            /* Il y a un default mais on rentre jamais dedans !! utile ???? */
         default:
            break;
      } /* Fin du switch */

      goto_instruction_suivante (p_exe);

   } /* Fin du while */

   /* Pas de free_all car mis dans le main */
}
