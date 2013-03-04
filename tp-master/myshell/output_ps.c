/**
 * module : myps
 * brief :
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date :
 * bug : le TTY affiche "pts/X" au lieu de "ttyX" pour les getty
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

#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>

#include "myps.h"
#include "output_ps.h"

#define TTY_CHARMASK 0xff

/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/

/** 
 * Affiche la valeur de la commande formatée
 */
void affiche_ps_cmd (char * cmd)
{
   unsigned int i;
   putchar(' ');
   for( i=1 ; i < strlen(cmd)-1 ; i++)
      putchar(cmd[i]);
}


/** 
 * Affiche la valeur de TIME formatée
 *   (donée en nombre de jiffies (1/100 de seconde))
 */
void affiche_ps_time (int tim)
{
   int hh,mm;
   
   tim /=100;
   hh = tim/3600;
   tim = tim%3600;
   mm = tim/60;
   tim = tim%60;
   
   printf ("%02d:%02d:%02d", hh,mm,tim  );
}


/** 
 * Affiche la valeur du tty formatée
 */
void affiche_ps_tty (int tty)
{
   if (tty&(TTY_CHARMASK))  
      printf ("pts/%-4d ", tty&(TTY_CHARMASK));
   else
      printf ("?        ");
}


/** 
 * Affiche la valeur du tty formatée
 */
void affiche_ps_user (uid_t const uid)
{
 struct passwd * user;
   if ( (user = getpwuid (uid)) )
      printf ("%s\t",user->pw_name);
}



/** 
 * Affiche l'entête avec le nom des colones demandées
 */
void affiche_ps ()
{  
   if (aff_ps & AFF_PID)  printf ("  PID ");
   if (aff_ps & AFF_TTY)  printf ("TTY     ");
   if (aff_ps & AFF_TIME) printf ("     TIME");
   if (aff_ps & AFF_CMD)  printf (" CMD ");

   printf ("\n");
}



/**
 * Affiche une ligne qui contient les valeurs demandées
 * param stat : contient les données necesaires a l'affichage
 */
void affiche_ps_stat (Stat_ps stat)
{
   if (aff_ps & AFF_PID)  printf ("%5d ", stat.pid);
   if (aff_ps & AFF_TTY)  affiche_ps_tty (stat.tty);
   if (aff_ps & AFF_TIME) affiche_ps_time (stat.utime + stat.stime);
   if (aff_ps & AFF_CMD)  affiche_ps_cmd (stat.comm);
   
   printf ("\n");
}



/**
 * Affiche l'aide
 */
void usage_ps ()
{
   printf ("************************\n");
   printf ("-A  all processes\n");
   printf ("-e  all processes\n");
   printf ("-p  by process ID\n");
   printf ("-t  by tty\n");
   printf ("t   by tty\n");

   printf ("\n");

   printf ("-All all processes\n");
   printf ("-V,V show version\n");
   printf ("--help display this help\n");
}


/**
 * Affiche un message d'erreur et l'aide
 * param err : le message
 */
void erreur_bad_arg (char* err)
{
   printf ("ERROR : %s \n", err);
   usage_ps ();
}


/**
 * Affiche la version de PS
 */
void version_ps()
{
   printf("myps version 1.1 \n");
}

