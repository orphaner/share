/**
 * module : interpréteur de commandes
 * brief :
 * author : Nicolas LASSALLE
 * date :
 * bug : Aucun détecté à ce jour
 *
 */



/*------------------------------------------------------------------------------
  Fichiers include
  ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h> 
 
#include "common.h"
#include "erreur.h"
#include "memutils.h"
#include "dyn_string.h"
#include "saisie.h"
#include "myls.h"
#include "myps.h"
#include "list_fg.h"
#include "hash.h"
#include "varloc.h"
#include "varglob.h"


/*------------------------------------------------------------------------------
  Constantes
  ----------------------------------------------------------------------------*/
#define R 0
#define W 1
#define TAILLE_HASH_CMDE 23U


/*------------------------------------------------------------------------------
  Types personalisés
  ----------------------------------------------------------------------------*/
typedef enum
{
   CMD_EXEC,
   CMD_EXIT,
   CMD_MYPWD,
   CMD_MYJOBS,
   CMD_MYFG,
   CMD_CD,
   CMD_SETENV,
   CMD_SET,
   CMD_MYLS,
   CMD_MYPS,
   CMD_STATUS
} commandes;


/*------------------------------------------------------------------------------
  Table de hash statique pour reconnaissance des tokens
  ----------------------------------------------------------------------------*/
static const char * commandes_nom[] =
{
   "",
   "",
   "",
   "",
   "myps",
   "cd",
   "",
   "set",
   "",
   "myls",
   "setenv",
   "",
   "exit",
   "",
   "",
   "myjobs",
   "myfg",
   "",
   "$?",
   "",
   "",
   "mypwd",
   "",
   ""
};

static const commandes commandes_token[] = 
{
   0,
   0,
   0,
   0,
   CMD_MYPS,
   CMD_CD,
   0,
   CMD_SET,
   0,
   CMD_MYLS,
   CMD_SETENV,
   0,
   CMD_EXIT,
   0,
   0,
   CMD_MYJOBS,
   CMD_MYFG,
   0,
   CMD_STATUS,
   0,
   0,
   CMD_MYPWD,
   0,
   0
};


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/
int last_status;
char has_last_status;
struct sigaction action;


/*------------------------------------------------------------------------------
  Pointeurs de fonctions pour exécution des commandes
  ----------------------------------------------------------------------------*/
typedef int (*pt_fct)(int, char **);
pt_fct pt_fct_myls;
pt_fct pt_fct_myps;
pt_fct pt_fct_mypwd;
pt_fct pt_fct_mysetenv;
pt_fct pt_fct_myset;
pt_fct pt_fct_myjobs;


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
void mycd (int argc, char **argv);
void myfg (int argc, char **argv);
char * get_pwd ();
int mypwd (int argc, char **argv);

void maj_status (int status);
void maj_status2 (int status);
void exec_cmd (list_inst temp);
void exec_cmd_interne (pt_fct const func, list_inst inst);
void execution (list_inst linst);

void myshell ();
void do_saisie ();
void fin_fils (int signal, const siginfo_t * ptr_info, void * v);
void sigusr1_action (int signal);


/*------------------------------------------------------------------------------
  Fonctions internes à MyShell
  ----------------------------------------------------------------------------*/
/**
 * Met à jour OLDPWD et PWD dans les variables locales
 */
void maj_pwd ()
{
   Arbre result;
   char * s;
   s = get_pwd ();
   printf("<%s>\n",s);
   if ( (result = hash_search (&varloc, "PWD")) != NULL)
   {
      if (hash_add (&varloc, "OLDPWD", result->valeur) == NULL)
         hash_modif (&varloc, "OLDPWD", result->valeur);
   }
   if (hash_add (&varloc, "PWD", s) == NULL)
      hash_modif (&varloc, "PWD", s);
   free (s);
}


/**
 * Change le répertoire de travail courant
 * param argc : nombre d'arguments
 * param **argv : tableau des arguments
 */
void mycd (int argc, char **argv)
{
   Arbre result;

   if (argc > 1)
   {
      if (chdir (argv[1]) == -1)
      {
         fprintf(stderr, "MyShell: mycd: %s: N'est pas un répertoire\n", argv[1]);
         maj_status2 (1);
      }
      else
      {
         maj_pwd ();
         maj_status2 (0);
      }
   }
   else
   {
      if ( (result = hash_search (&varloc, "HOME")) != NULL)
      {
         if (chdir (result->valeur) == -1)
         {
            fprintf(stderr, "MyShell: mycd: %s: N'est pas un répertoire\n", result->valeur);
            maj_status2 (1);
         }
         else
         {
            maj_pwd ();
            maj_status2 (0);
         }
      }
   }
   if (argc > 1 && !strcmp (argv[1], "--help"))
   {
      printf("MyShell: cd: usage: cd [dir]\n");
      maj_status2 (1);
   }
}


/**
 * Affiche la liste des tâches en background
 * param argc : nombre d'arguments
 * param **argv : tableau des arguments
 */
int myjobs (int argc, char **argv)
{
   list_fg_aff (list);
   exit (0);
}


/**
 * Redonne la main à une tâche exécutée en arrière plan
 * param **argv : tableau des arguments
 * param argc : nombre d'arguments
 */
void myfg (int argc, char **argv)
{
   pid_t pid_to_wait;
   int status;

   if (argc == 1)
   {
      if (list)
      {
         waitpid (list->pid, &status, WUNTRACED);
         maj_status2 (0);
      }
      else
      {
         fprintf(stderr, "MyShell: myfg: current: No such job\n");
         maj_status2 (1);
      }
   }
   else if (argc > 1)
   {
      if (!strcmp (argv[1], "--help"))
      {
         printf("MyShell: myfg: usage: myfg [job_spec]\n");
         maj_status2 (1);
      }
      else
      {
         pid_to_wait = list_fg_get_pos (list, atoi(argv[1]));
         if (pid_to_wait == -1)
         {
            fprintf(stderr, "MyShell: myfg: %s: No such job\n", argv[1]);
            maj_status2 (1);
         }
         else
         {
            waitpid (pid_to_wait, &status, WUNTRACED);
            maj_status2 (0);
         }
      }
   }
}


/**
 * Lit la valeur du path courant
 * Return : une chaine de caractère contenant le path courant
 * /!\ à désaouller /!\
 */
char * get_pwd ()
{
   size_t buf_size = 128;
   char * buffer = NULL;
   char * path_ok = NULL;

   for (;;)
   {
      buffer = my_malloc (buf_size);
      path_ok = getcwd (buffer, buf_size);

      if (path_ok == NULL)
      {
         free (buffer);
         buf_size *= 2;
      }
      else
      {
         return buffer;
      }
   }
}


/**
 * Hé merde, j'ai fait la fonction et c'est pas une fonction interne à bash;
 * brin, jla laisse !!
 * Affiche le path courant
 * param argc : nombre d'arguments
 * param **argv : tableau des arguments
 */
int mypwd (int argc, char **argv)
{
   char *pwd;
   if (argc > 1)
   {
      if (strcmp (argv[1], "--help"))
      {
         printf("myshell: pwd: %s: invalid option\n", argv[1]);
      }
      printf("pwd: usage: pwd\n");
      exit (1);
   }
   else
   {
      pwd = get_pwd ();
      printf("%s\n",pwd);
      free (pwd);
   }
   exit (0);
}


/*------------------------------------------------------------------------------
  Mise à jour des status
  ----------------------------------------------------------------------------*/
/**
 * Met à jour le status de la dernière commande terminée
 * param status : Le status récupéré
 */
void maj_status (int status)
{
   if (WIFEXITED (status))
   {
      if (WEXITSTATUS (status))
      {
         has_last_status = 1;
         last_status = WEXITSTATUS (status);
#ifdef DEBUG
         printf("maj status OK\n");
#endif
      }
      else
      {
#ifdef DEBUG
         printf("maj status KO\n");
#endif
         has_last_status = 1;
         last_status = 0;
      }
   }
   else
   {
#ifdef DEBUG
      printf("aucun last_status\n");
#endif
      has_last_status = 0;
   }
}


/**
 * Met à jour le status de la dernière commande terminée
 * param status : Le status à mettre
 */
void maj_status2 (int status)
{
   has_last_status = 1;
   last_status = status;
}


/*------------------------------------------------------------------------------
  Redirections vers les fichiers
  ----------------------------------------------------------------------------*/
/**
 * Ouvre un fichier en CREATE pour la redirection >
 * param nom_fichier : nom du fichier à ouvrir/créer
 */
int ouvre_fichier_ecriture (char *nom_fichier)
{
   int fd;
   fd = creat (nom_fichier, S_IRWXU);
   if (fd == ERREUR)
      printf("EXIT_FIC_OPEN_R\n");
   return fd;
}


/**
 * Ouvre un fichier en APPEND pour la redirection >>
 * param nom_fichier : nom du fichier à ouvrir/créer
 */
int ouvre_fichier_ecriture_fin (char *nom_fichier)
{
   int fd;
   fd = open(nom_fichier, O_WRONLY | O_CREAT | O_APPEND, 0666);
   if (fd == ERREUR)
      printf("EXIT_FIC_OPEN_R\n");
   return fd;
}


/*------------------------------------------------------------------------------
  Fonctions d'éxécution des commandes
  ----------------------------------------------------------------------------*/
/**
 * param a[2] : un tube de communication
 * param temp : l'instruction courante
 */
void pipe_fils (int a[2], list_inst temp)
{
   int file;
   switch (temp->redir)
   {
      case R_PIPE : 
         close (a[R]);
         close (STDOUT_FILENO);
         dup   (a[W]);              
         break;

      case R_SUP :
         file = ouvre_fichier_ecriture (temp->suivant->argv[0]);
         close (STDOUT_FILENO);
         dup (file);
         break;

      case R_DSUP :
         file = ouvre_fichier_ecriture_fin (temp->suivant->argv[0]);
         close (STDOUT_FILENO);
         dup (file);
         break;

      default :
         break;
   }
}


/**
 * param a[2] : un tube de communication
 * param pid : le pid du fils
 * param temp : l'instruction courante
 */
void pere_exec (int a[2], pid_t pid, list_inst temp)
{
   int status;
   redirections red;
                
   if (temp->background == TRUE)
   {
      list = list_fg_add (list, pid);
      kill (pid, SIGUSR1);
   }

   if (temp->redir == R_PT_VIRG 
       || temp->redir ==  R_AND_AND 
       || temp->redir == R_OR_OR 
       || (temp->redir == R_NONE && temp->background == FALSE))
   {
      waitpid (pid, &status, 0);
      maj_status (status);
   }
   else
   {
      waitpid (pid, &status, WNOHANG);
   }
               
   red = temp->redir;         
   temp = temp->suivant;

   switch (red)
   {
      case R_PIPE : 
         close (a[W]);
         close (STDIN_FILENO);
         dup   (a[R]);
         break;
                 
         /* en cas de non-validation on ne poursuit pas l'execution */
      case R_AND_AND :
         if (last_status != 0)
            temp = NULL;          
         break;

      case R_OR_OR :  
         if (last_status == 0)
            temp = NULL;
         break;

         /* on passe le nom du fichier qui vient d'etre utilisé */
      case R_SUP :  
      case R_DSUP :
         if (temp != NULL)
            temp = temp->suivant;       
         break;

      default :
         break;
   }

   if (temp != NULL)
      execution (temp);
}

/**
 * Exécute une commande du système
 * param temp : l'instruction courante ; lancée dans un execvp
 */
void exec_cmd (list_inst temp)
{
   pid_t pid;
   int a[2];

   pipe (a);

   pid = fork();

   switch (pid)
   {
      /* Erreur de création */
      case -1:
         erreur_sys (EXIT_NO_NEW_FORK);
         break;

         /* Dans le fils */
      case 0:
         if (temp->background == TRUE)
            pause ();

         pipe_fils (a, temp);

         execvp (temp->argv[0], temp->argv);
         erreur_sys (EXIT_CANT_EXEC_CMD);
         break;

         /*Dans le père */
      default:
         pere_exec (a, pid, temp);
         break;
   }
}


/**
 * Exécute une commande interne à MyShell
 * param func : un pointeur de fontion vers la fonction à lancer !
 * param temp : l'instruction courante
 */
void exec_cmd_interne (pt_fct const func, list_inst temp)
{
   pid_t pid;
   int a[2];

   pipe (a);

   pid = fork();

   switch (pid)
   {
      /* Erreur de création */
      case -1:
         erreur_sys (EXIT_NO_NEW_FORK);
         break;

         /* Dans le fils */
      case 0:
         if (temp->background == TRUE)
            pause ();

         pipe_fils (a, temp);

         ((*func)(temp->argc, temp->argv));
         break;

         /*Dans le père */
      default:
         pere_exec (a, pid, temp);
         break;

   }
}


/**
 * param * cmde : le nom de la commande
 * return : le token associé à la commande
 */
commandes reco_commande (char * cmde)
{
   unsigned int cle = hash2 (cmde, TAILLE_HASH_CMDE);
   if (!strcmp (cmde, commandes_nom[cle]))
      return commandes_token[cle];
   return CMD_EXEC;
}


/**
 * Reconnaissance des commandes
 * param linst : l'instruction courante à lancer
 */
void execution (list_inst linst)
{
   switch (reco_commande (linst->argv[0]) )
   {
      case CMD_EXEC:
         exec_cmd (linst);
         break;

      case CMD_EXIT:
         varglob_free ();
         exit (EXIT_ALL_OK);
         break;

      case CMD_MYPWD:
         exec_cmd_interne (pt_fct_mypwd, linst);
         break;

      case CMD_MYJOBS:
         exec_cmd_interne (pt_fct_myjobs, linst);
         break;

      case CMD_MYFG:
         myfg (linst->argc, linst->argv);
         break;

      case CMD_CD:
         mycd (linst->argc, linst->argv);
         break;

      case CMD_SETENV:
         if (linst->argc ==1)
            exec_cmd_interne (pt_fct_mysetenv, linst);
         else
            mysetenv (linst->argc, linst->argv);
         break;

      case CMD_SET:
         if (linst->argc ==1)
            exec_cmd_interne (pt_fct_myset, linst);
         else
            myset (linst->argc, linst->argv);
         break;

      case CMD_MYLS:
         exec_cmd_interne (pt_fct_myls, linst);
         break;

      case CMD_MYPS:
         exec_cmd_interne (pt_fct_myps, linst);
         break;

      case CMD_STATUS:
         if (has_last_status)
            printf("%d\n", last_status);
         else
            printf("\n");
         break;
      
   }
}


/**
 * Boucle infinie du programme.
 * Se charge de faire les saisies et d'exécuter les commandes de la saisie
 */
void myshell ()
{
   int oldout, oldin ;
   list_inst temp;

   do_saisie ();
 
   for (;;)
   {
      oldout = dup(STDOUT_FILENO);
      oldin = dup(STDIN_FILENO);
 
      temp = linst;
      execution (temp);

      close(STDOUT_FILENO);
      dup(oldout);
      close(STDIN_FILENO);
      dup(oldin);

      do_saisie ();
   }
}


/**
 * Affichage du prompt tant que rien n'a été tapé !
 */
void do_saisie ()
{
   do
   {
      perform_saisie ("myshell> ");
   } while (linst == NULL || linst->argc == 0);
}



/*------------------------------------------------------------------------------
  Fonctions associées à des signaux
  ----------------------------------------------------------------------------*/
/**
 * Action exécutée à la mort de tous les fils.
 * Sert à supprimer le pid du processus qui était dans la liste des processus
 * actuellement lancés.
 * Met à jour le status de la dernière commande terminée.
 * param signal : signal reçu
 * param * ptr_info :
 * param * v :
 */
void fin_fils (int signal, const siginfo_t * ptr_info, void * v)
{
   int status;
/*    waitpid (ptr_info->si_pid, &status, 0); */
/*    maj_status (ptr_info->si_status); */
/*    maj_status (&status); */
#ifdef DEBUG
   printf("fin_fil pid <%d>\n", ptr_info->si_pid); 
#endif
   list = list_fg_suppr (list, ptr_info->si_pid);
}


/**
 * Redéfinition de SIGUSR1
 * param signal : signal reçu
 */
void sigusr1_action (int signal)
{
}



/*------------------------------------------------------------------------------
  MAIN du programme
  ----------------------------------------------------------------------------*/
/**
 * Fonction de vidage mémoire apellée par atexit
 */
void delete_mem ()
{
   hash_free (&varloc);
   free_inst ();
/*   varglob_free ();*/
}


/**
 * Le main de MyShell :!)
 * param **argv : tableau des arguments
 * param argc : nombre d'arguments
 */
int main ()/* int argc, char *argv[]) */
{
   /* Vidage mémoire à la fin du programme */
   atexit (delete_mem);

   /* Initialisation des status */
   last_status = 0;
   has_last_status = 0;

   /* Remplit la liste des variables d'env locales*/
   varloc_remplir ();
   varglob_init ();

   /* Initialise la liste des processus en background */
   list = list_fg_init (list);

   /* Initialisation des pointeurs de fonctions */
   pt_fct_myls = &myls;
   pt_fct_myps = &myps;
   pt_fct_mypwd = &mypwd;
   pt_fct_mysetenv = &mysetenv2;
   pt_fct_myset = &myset2;
   pt_fct_myjobs = &myjobs;


   signal (SIGUSR1, sigusr1_action);

   /* Fonction à lancer à la mort des fils */
   sigemptyset (&action.sa_mask);
   sigaddset (&action.sa_mask, SIGCHLD);
   action.sa_handler = NULL;
   action.sa_sigaction = fin_fils;
   action.sa_flags = SA_SIGINFO;
   sigaction (SIGCHLD, &action, NULL);

   /* Lancement du shell */
   dyn_string_init2 (&saisie);
   myshell ();

   exit (EXIT_ALL_OK);
}
