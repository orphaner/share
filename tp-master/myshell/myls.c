/**
 * module : myls
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
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>


#include "myls.h"
#include "memutils.h"


/*------------------------------------------------------------------------------
  Types personalisés
  ----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/
#define OPTION_ALL             1
#define OPTION_RECURSIVE       2
#define OPTION_HUMAN_READABLE  4
#define OPTION_DETAIL_LIST     8

unsigned int options_ls = 0; /* Masque des paramètres */

#define OPT_HELP (0-2)

static char const optstring[] = "aRlh";
static struct option const long_options[] =
{
   {"help",           no_argument, NULL, OPT_HELP}, 
   {"all",            no_argument, NULL, 'a'},
   {"recursive",      no_argument, NULL, 'R'},
   {"human-readable", no_argument, NULL, 'h'},
   {NULL, 0, NULL, 0}
};

#define PROGRAM_NAME "myls"
static const char * const usage_message =
   "Usage: %s [OPTION]... [FICHIER]...\n"
   "Afficher les informations au sujet des FICHIERS (du répertoire courant par défaut).\n"
   "\n"
   "  -a, --all            Afficher tous les fichiers des répertoires,  y compris les fichiers commençant par un '.'.\n"
   "  -R                   Afficher récursivement le contenu des sous-répertoires.\n"
   "  -h, --human-readable Ajouter une lettre indiquant l'unité de taille, comme M pour méga-octets.\n"
   "  -l                   Utiliser le format long d'affichage\n";


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/**
 * Affiche le type de l'objet
 */
void affiche_type (mode_t const mode)
{
   /* is it a regular file? */
   if (S_ISREG(mode)) printf("-");

   /* directory? */
   else if (S_ISDIR(mode))
      printf("d");

   /* character device? */
   else if (S_ISCHR(mode))
      printf("c");

   /* block device? */
   else if (S_ISBLK(mode))
      printf("b");

   /* fifo? */
   else if (S_ISFIFO(mode))
      printf("p");

   /* symbolic link? */
   else if (S_ISLNK(mode))
      printf("l");

   /* socket? */
   else if (S_ISSOCK(mode))
      printf("s");

   /* gnééé ? */
   else
      printf("?");
}


/**
 * Affiche les droits de l'objet
 * Sticky bits ??????????
 */
void affiche_droits (mode_t const mode)
{
   /* Droits pour l'utilisateur :*/
   (mode & S_IRUSR) ? printf("r") : printf("-");
   (mode & S_IWUSR) ? printf("w") : printf("-");
   (mode & S_IXUSR) ? printf("x") : printf("-");

   /* Droits pour le groupe :*/
   (mode & S_IRGRP) ? printf("r") : printf("-");
   (mode & S_IWGRP) ? printf("w") : printf("-");
   (mode & S_IXGRP) ? printf("x") : printf("-");

   /* Droits pour les autres :*/
   (mode & S_IROTH) ? printf("r") : printf("-");
   (mode & S_IWOTH) ? printf("w") : printf("-");
   (mode & S_IXOTH) ? printf("x") : printf("-");

   printf("\t");
}


/**
 * Affiche le nom du propriétaire de l'objet
 * param uid : uid du propriétaire dont on veut afficher le nom
 */
void affiche_user (uid_t const uid)
{
   struct passwd * user;
   if ( (user = getpwuid (uid)) )
      printf ("%s\t",user->pw_name);
}


/**
 * Affiche le nom du groupe de l'objet
 * param gid : gid du groupe dont on veut afficher le nom
 */
void affiche_groupe (gid_t const gid)
{
   struct group * groupe;
   if ( (groupe = getgrgid (gid)) )
      printf ("%s\t",groupe->gr_name);
}


/**
 * Affiche la date de modification
 * param date_modif : la date de modif que l'on veut afficher
 */
void affiche_date_modif (time_t const date_modif)
{
   struct tm *infotime;
   infotime = localtime (&date_modif);
   printf ( "%2d-%2d-%2d %2d:%2d ",
            infotime->tm_mday,
            infotime->tm_mon+1,
            infotime->tm_year+1900,
            infotime->tm_hour,
            infotime->tm_min );
}


/**
 * Lit la valeur du lien symbolique
 * param *filname : le path du lien dont on veut récupérer l'objet pointé
 * return : une chaine de caractère correspondant à l'objet pointé
 * /!\ A désaouller /!\
 * retun : NULL si le lien est erroné
 */
char * read_the_link (char const * filename)
{
   size_t buf_size = 128;
   char *buffer = NULL;
   ssize_t link_length;

   for (;;)
   {
      buffer = my_malloc (buf_size);
      link_length = readlink (filename, buffer, buf_size);

      if (link_length < 0)
      {
         free (buffer);
         return NULL;
      }

      if ((size_t) link_length < buf_size)
      {
         buffer[link_length] = 0;
         return buffer;
      }

      free (buffer);
      buf_size *= 2;
   }
}


/**
 * Affiche la taille d'un objet
 * param st_size : la taille en octets de l'objet
 */
void affiche_taille (off_t const st_size)
{
   int size = (int) st_size;
   if (options_ls & OPTION_HUMAN_READABLE)
   {
      if (size >= 1073741824) 
         printf ("%.1fG", (double)(size / 1073741824.0) );

      else if (size >= 1048576) 
         printf("%.1fM", (double)(size / 1048576.0) );

      else if (size >= 1024) 
         printf ("%.1fK", (double)(size / 1024.0 ) );

      else 
         printf ("%d", size);
   }
   else
      printf ("%d", size);
   printf ("\t");
}


/**
 * Affiche un objet
 * param *nom : le nom de l'objet
 * param *nom_long : le path complet de l'objet
 * params infos : le struct stat de stat (...)
 */
void affiche_objet (char const * nom, char const * nom_long, struct stat const infos)
{
   char * link;

   /* Infos de l'objet */
   if (options_ls & OPTION_DETAIL_LIST)
   {
      affiche_type (infos.st_mode);
      affiche_droits (infos.st_mode);
      affiche_user (infos.st_uid);
      affiche_groupe (infos.st_gid);
      affiche_taille (infos.st_size);
      affiche_date_modif (infos.st_mtime);
   }

   /* Nom de l'objet */
   printf ("%s\t", nom);

   /* Si lien, nom de l'objet pointé */
   if (S_ISLNK (infos.st_mode) && (options_ls & OPTION_DETAIL_LIST)) 
   {
      if ( (link=read_the_link (nom_long)) )
      {
         printf (" -> %s", link);
         free (link);
      }
      else 
      {
         printf ("lien incorrect");
      }
   }

   if (options_ls & OPTION_DETAIL_LIST)
      printf ("\n");
}


/**
 * Parcours un répertoire "directory" pour afficher son contenu
 */
void parcours_rep (char const * directory)
{
   DIR * rep;
   struct dirent * objet = NULL;
   struct stat s;
   unsigned int i;
   unsigned int taille_file = 0;
   unsigned int lng = strlen (directory);
   unsigned int lng_total;

   char **file = NULL;
   char *str_stat = NULL;


   /* Tentative d'ouverture du répertoire */
   rep = opendir (directory);
   if (rep == NULL)
   {
      if (lstat (directory, &s) == 0)
      {
         affiche_objet (directory, directory, s);
         return ;
      }
      else
      {
         fprintf (stderr,"myls: %s: Aucun fichier ou répertoire de ce type\n", directory);
         exit (1);
      }
   }

   /* Si récursif, affichage du rép actuellement parcouru */
   if (options_ls & OPTION_RECURSIVE)
      printf ("\n%s:\n", directory);

   /* Parcours du contenu du répertoire */
   while ( (objet = readdir (rep)) )
   {
      lng_total = lng + strlen(objet->d_name) + 2;
      str_stat = my_malloc (lng_total * sizeof(char));
      snprintf (str_stat, lng_total, "%s/%s", directory, objet->d_name);

      if ( lstat (str_stat, &s) == -1 )
      {
         printf("myls: %s: Permission non accordée\n", str_stat);
         free (str_stat);
      }
      else
      {
         /* Le nom commence par . == fichier caché*/
         /* Si option a activée on affiche */
         if ( (objet->d_name[0] == '.' && (options_ls & OPTION_ALL)) || objet->d_name[0] != '.')
         {
            affiche_objet (objet->d_name, str_stat, s);
            
            /* Affichage récursif */
            /* Mise en file d'attente ! */
            if ((options_ls & OPTION_RECURSIVE) && S_ISDIR (s.st_mode) )
            {
               if ((options_ls & OPTION_ALL))
               {
                  if ( strcmp (objet->d_name, ".") && strcmp (objet->d_name, "..") )
                  {
                     file = my_realloc (file, (++taille_file) * sizeof (char *) );
                     file[taille_file-1] = str_stat;
                  }
               }
               else
               {
                  file = my_realloc (file, (++taille_file) * sizeof (char *) );
                  file[taille_file-1] = str_stat;
               }
            }
            else
            {
               free (str_stat);
            }
         }
         else
         {
            free (str_stat);
         }
      }

      str_stat = NULL;
      objet = NULL;
   }

   /* Fermeture du répertoire */
   closedir (rep);


   /* Parcours de la file d'attente des parcours récursifs ! */
   for (i=0 ; i < taille_file ; i++)
   {
      printf("-->%s\n",file[i]);
      parcours_rep (file[i]);
      free (file[i]);
   }

   free(file);

   if (!(options_ls & OPTION_DETAIL_LIST))
      printf("\n");
}


/**
 * Affiche comment se servir de myls !! 
 */
void usage ()
{
   printf(usage_message, PROGRAM_NAME);
   exit (0);
}


/**
 * La fonction à apeller pour se servir de myls ! (idem main !)
 * param *argv[] : tableau des arguments
 * param argc : nombre d'arguments
 */
int myls (int argc, char *argv[])
{
   int c;
   int diff;
   struct stat s;

   optind = 1;
   opterr = 1;
   optopt = 63;

   for (;;)
   {
      c = getopt_long (argc, argv, optstring, long_options, NULL);
      if (c == -1)
         break;

      switch (c)
      {
         case OPT_HELP:
            usage ();
            break;

         case 'a':
            options_ls |= OPTION_ALL;
            break;

         case 'l':
            options_ls |= OPTION_DETAIL_LIST;
            break;

         case 'R':
            options_ls |= OPTION_RECURSIVE;
            break;

         case 'h':
            options_ls |= OPTION_HUMAN_READABLE;
            break;

         case '?':
            break;
      }
   }


   diff = argc-optind;
   if (!diff)
   {
      parcours_rep (".");
   }
   else
   {
      while (optind < argc)
      {  
         if (diff > 1)
         {
            if ( lstat (argv[optind], &s) == -1 )
            {
               printf("myls: %s: Permission non accordée\n", argv[optind]);
            }
            else if (S_ISDIR(s.st_mode))
            {
               printf ("%s:\n", argv[optind]);
               parcours_rep (argv[optind++]);
               printf ("\n");
            }
            else
            {
               parcours_rep (argv[optind++]);
            }
         }
         else
         {
            parcours_rep (argv[optind++]);
         }
      }
   }
   if (!(options_ls & OPTION_DETAIL_LIST))
      printf("\n");
   exit (0);
}

