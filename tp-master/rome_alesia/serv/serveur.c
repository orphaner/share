/**
 * module : serveur
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
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>

#include "erreur.h"
#include "memutils.h"
#include "net.h"
#include "sem.h"


/*------------------------------------------------------------------------------
  Constantes
  ----------------------------------------------------------------------------*/
#define SERVER_PORT   3000
#define NB_CARTES     3
#define TAILLE_PSEUDO 30
#define NB_CF         6.0


/*------------------------------------------------------------------------------
  Types personalisés
  ----------------------------------------------------------------------------*/
/* Liste des coups fourrés */
typedef enum
{
   CF_USED,
   CF_MOTIVATION7,
   CF_QUI_PERD_GAGNE,
   CF_MILIEU,
   CF_RESISTANCE,
   CF_FUITE,
   CF_DOUBLE_DOSE
}coups_fourres;


/* Description des coups fourrés */
static const char * cf_description [] =
{
   "Déjà utilisé",
   "Motivation : l'attaque est boostée de 7 points",
   "Qui perd gagne : le conflit avance dans le sens inverse",
   "Milieu : replace le conflit au milieu",
   "Résistance : si le conflit devait avancer vers moi, il reste en place",
   "Fuite : si je perd ce tour, les armées engagées ne sont pas retirées de ma réserve",
   "Double dose : si le conflit devait avancer, il avance de 2 cases"
};


/* Les équipes */
typedef enum
{
   NOBODY  = '0',
   ROME    = 'r',
   ALESIA  = 'a'
} teams;


/* Un maillon représentant un client*/
typedef struct _client
{
      /* Réseau */
      pthread_t thread_id;
      int sock;
      struct sockaddr_in cli_ad;

      /* Jeu */
      teams camp;
      int armees;
      coups_fourres cartes[NB_CARTES];

      /* Infos */
      char pseudo [TAILLE_PSEUDO];

      struct _game * cur_game;

      struct _client * next;
}client;


/* Pointeur sur un client*/
typedef client * ptr_client;


/* Maillon représentant une partie */
typedef struct _game
{
      int id;

      /* Les clients s'affrontant */
      ptr_client cli_rome;
      ptr_client cli_alesia;

      /* Valeurs init de la partie */
      int nb_cases;
      int nb_armees;

      /* Coup */
      int bet_rome, bet_alesia;
      coups_fourres card_rome, card_alesia;
      char got_res_rome, got_res_alesia;

      /* Lieu courant du combat */
      int combat;
      
      pthread_mutex_t mutex;

      int begin;

      struct _game * next;
}game;


/* Pointeur sur une partie */
typedef game * ptr_game;


/* Liste des commandes du serveur en entrée */
typedef enum
{
   CIN_PSEUDO      = '1',
   CIN_NEW_GAME    = '2',
   CIN_GET_LIST    = '3',
   CIN_JOIN_GAME   = '4',
   CIN_QUIT_GAME   = '5',
   CIN_QUIT_SERVER = '6',
   CIN_GET_PLAYER  = '7',
   CIN_GET_CARDS   = '8',
   CIN_GET_MY_GAME = '9',
   CIN_PLAY        = 'a',
   CIN_GET_RESULT  = 'b',
   CIN_CAN_PLAY    = 'c',
   CIN_GET_OTHER_IP = 'd'
}commandes_in;


/* Réponses possibles pour le client */
typedef enum
{
   RES_GAME_NOT_FOUND = '0',
   RES_GAME_JOIN_OK   = '1',
   RES_PSEUDO_USED    = '2',
   RES_PSEUDO_OK      = '3',
   RES_NOT_YET_RESULT = '4',
   RES_ROME_WIN       = '5',
   RES_ALESIA_WIN     = '6',
   RES_RESULT         = '7',
   RES_NO_RESULT      = '8',
   RES_CAN_PLAY       = '9',
   RES_CANT_PLAY      = 'a'
}result_out;


/* Liste des commandes du serveur en sortie */
typedef enum
{
   VIDE
}commandes_out;


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/
/* Liste des clients */
ptr_client client_list;

/* Liste des jeux */
ptr_game   game_list;

/* Socket du serveur */
int serv_sock;

/* Id de la prochaine partie */
int next_game_id;

/**/
pthread_mutex_t mutex_game;

/**/
pthread_mutex_t mutex_client;


/*------------------------------------------------------------------------------
  Fonctions de liste chainée des parties
  ----------------------------------------------------------------------------*/
/**
 * Désalloue la liste des parties
 */
void game_list_delete ()
{
   ptr_game tmp ;
   while (game_list != NULL)
   {
      tmp = game_list;
      game_list = game_list->next;
      free (tmp);
   }
}


/**
 *
 */
void game_raz (ptr_game g)
{
   g->bet_rome = g->bet_alesia = 0;
   g->card_rome = g->card_alesia = CF_USED;
   g->got_res_rome = g->got_res_alesia = 0;
}


/**
 * Rajoute un maillon pour un nouveau jeu en tête de liste
 * return : un pointeur vers le maillon alloué
 */
ptr_game game_list_get_new ()
{
   ptr_game new_game;

   P (mutex_game);

   new_game = my_malloc (sizeof (game));
   new_game->cli_rome = NULL;
   new_game->cli_alesia = NULL;
   new_game->next = game_list;
   new_game->id = next_game_id++;
   new_game->begin = 0;
   new_game->bet_rome = new_game->bet_alesia = 0;
   new_game->card_rome = new_game->card_alesia = CF_USED;
   new_game->got_res_rome = new_game->got_res_alesia = 0;
   pthread_mutex_init (&(new_game->mutex), NULL);
   game_list = new_game;

   V (mutex_game);

   return new_game;
}


/**
 *
 */
void game_list_suppr (ptr_game the_game)
{
   ptr_game tmp_g;
   ptr_game prev_g;
   int id;

   if (the_game == NULL) 
      return ;


   P (mutex_game);

   id = the_game->id;
   tmp_g = game_list;
   prev_g = tmp_g;

   /* Suppression en tête */
   if (tmp_g && tmp_g->id == id)
   {
      game_list = tmp_g->next;
      free (tmp_g);
   }

   /* Suppression ailleurs */
   else
   {
      while (tmp_g && tmp_g->id != id)
      {
         prev_g = tmp_g;
         tmp_g = tmp_g->next;
      }

      /* Le client était connecté à une partie !! */
      if (tmp_g)
      {
         prev_g->next = tmp_g->next;
         free (tmp_g);
      }
   }
   V (mutex_game);
}


/*------------------------------------------------------------------------------
  Fonctions de liste chainée des clients
  ----------------------------------------------------------------------------*/
/**
 * Désalloue la liste des clients
 */
void client_list_delete ()
{
   ptr_client tmp ;
   while (client_list != NULL)
   {
      tmp = client_list;
      client_list = client_list->next;
      printf("ferme socket client ds delete\n");
      close (tmp->sock);
      free (tmp);
   }
}


/**
 * Rajoute un maillon pour un nouveau client en tête de liste
 * return : un pointeur vers le maillon alloué
 */
ptr_client client_list_get_new ()
{
   ptr_client new_client;

   P (mutex_client);

   new_client = my_malloc (sizeof (client));
   new_client->next = client_list;
   new_client->cur_game = NULL;
   client_list = new_client;
   new_client->cur_game = 0;
   memset (new_client->pseudo, '\0', TAILLE_PSEUDO);

   V (mutex_client);

   return new_client;
}


/**
 *
 */
void client_list_suppr (ptr_client cli)
{
   ptr_client tmp_g;
   ptr_client prev_g;

   P (mutex_client);

   tmp_g = client_list;
   prev_g = tmp_g;

   /* Suppression en tête */
   if (tmp_g && tmp_g == cli)
   {
      client_list = tmp_g->next;
      printf("ferme socket client1\n");
      close (tmp_g->sock);
      free (tmp_g);
   }

   /* Suppression ailleurs */
   else
   {
      while (tmp_g && tmp_g != cli)
      {
         prev_g = tmp_g;
         tmp_g = tmp_g->next;
      }

      if (tmp_g)
      {
         prev_g->next = tmp_g->next;
         printf("ferme socket client2\n");
         close (tmp_g->sock);
         free (tmp_g);
      }
   }
   V (mutex_client);
}


/**
 *
 */
int client_doublon (ptr_client cli)
{
   ptr_client tmp;

   P (mutex_client);

   tmp = client_list;
   while (tmp)
   {
      if (tmp != cli && !strcmp (tmp->pseudo, cli->pseudo))
      {
         V (mutex_client);
         return 1;
      }
      tmp = tmp->next;
   }

   V (mutex_client);

   return 0;
}


/*------------------------------------------------------------------------------
  Autres fonctions
  ----------------------------------------------------------------------------*/
/**
 * Envoie une partie tmp à une client
 */
void send_game (int sock, char * buffer, ptr_game tmp)
{      
   teams camp;
   char * nom=NULL;
   if (tmp->cli_rome == NULL)
   {
      nom = tmp->cli_alesia->pseudo;
      camp = ROME;
   }
   else
   {
      nom = tmp->cli_rome->pseudo;
      camp = ALESIA;
   }
   sprintf (buffer, "%d:%d:%d:%c:%s", tmp->id, tmp->nb_armees, 
            tmp->nb_cases, camp, nom);
   printf ("jeu: %s\n", buffer);
   xsend (sock, buffer, 
          (TAILLE_PSEUDO + 4) * sizeof (char) + 3 * sizeof (int), 0);
}


/**
 * Calcule le résultat d'une manche
 */
void calcul_result (ptr_game g)
{
   int avance = 0;
   int boost_rome = 0;
   int boost_alesia = 0;


   /* Les cartes s'annulent ou pas de carte !! */
   if (g->card_rome == g->card_alesia)
   {
      if (g->bet_rome > g->bet_alesia)
         avance++;
      else if (g->bet_rome < g->bet_alesia)
         avance--;
      g->cli_rome->armees -= g->bet_rome;
      g->cli_alesia->armees -= g->bet_alesia;
   }

   /* Faut prendre en compte les cartes */
   else
   {

      /* Carte de motivation + 7 */
      if (g->card_rome == CF_MOTIVATION7)
         boost_rome = 7;
      if (g->card_alesia == CF_MOTIVATION7)
         boost_alesia = 7;

      /**************/
      /* Rome gagne */
      if (g->bet_rome + boost_rome  >  g->bet_alesia + boost_alesia)
      {
         avance++;

         /* Carte de double dose par rome */
         if (g->card_rome == CF_DOUBLE_DOSE)
            avance++;

         /* Carte de résistance */
         if (g->card_alesia == CF_RESISTANCE)
            avance = 0;
      }

      /****************/
      /* Alesia gagne (rome perd) */
      else if (g->bet_rome + boost_rome  <  g->bet_alesia + boost_alesia)
      {
         avance--;

         /* Carte de double dose par alesia */
         if (g->card_alesia == CF_DOUBLE_DOSE)
            avance--;

         /* Carte de résistance */
         if (g->card_rome == CF_RESISTANCE)
            avance = 0;
      }

      /* Carte de qui perd gagne */
      if (g->card_rome == CF_QUI_PERD_GAGNE || g->card_alesia == CF_QUI_PERD_GAGNE)
         avance = -avance;

      /* Carte de milieu */
      if (g->card_rome == CF_MILIEU || g->card_alesia == CF_MILIEU)
      {
         avance = 0;
         g->combat = g->nb_cases/2;
         g->combat++;
      }

      /* Retire les armées de rome */
      if (g->card_rome == CF_FUITE && avance < 0)
      {
         printf("fuite");
         g->cli_rome->armees -= 0;
      }
      else
      {
         printf("suppression ds armées de rome");
         g->cli_rome->armees -= g->bet_rome;
      }

      /* Retire les armées d'alésia */
      if (g->card_alesia == CF_FUITE && avance > 0)
      {
         printf("fuite");
         g->cli_alesia->armees -= 0;
      }
      else
      {
         printf("suppression ds armées d'alesia");
         g->cli_alesia->armees -= g->bet_alesia;
      }
   }
   g->combat += avance;
}


/**
 * Calcule qui a gagné la partie
 */
int who_win (ptr_game g)
{

   /* Nul si X a plus d'armées et que Y en a pas assez pr aller au bout */
   if (g->cli_rome->armees == 0)
   {
      if (g->cli_alesia->armees < (g->combat))
         return 0;
      else
         return 2;
   }

   if (g->cli_alesia->armees == 0)
   {
      if (g->cli_rome->armees < (g->nb_cases - g->combat))
         return 0;
      else
         return 1;
   }

   /* Rome gagne */
   if (g->combat == g->nb_cases+1)
      return 1;


   /* Alesia gagne */
   if (g->combat == 0)
      return 2;

   return 3;
}


/**
 * Envoi le résultat calculé au joueur
 */
void send_result (ptr_client cli, char * buffer)
{
   int gagne;
   int carte;
   ptr_game g = cli->cur_game;


   if (cli->camp == ROME)
   {
      g->got_res_rome = 1;
      carte = cli->cur_game->card_alesia;
   }
   else
   {
      g->got_res_alesia = 1;
      carte = cli->cur_game->card_rome;
   }

   gagne = who_win (g);

   sprintf (buffer, "%d:%d:%d:%d:%d:%d:%d:%d:", gagne, g->combat, g->card_rome, 
            g->card_alesia, g->cli_rome->armees, g->cli_alesia->armees, g->bet_rome, 
            g->bet_alesia);

   printf("résultat: %s", buffer);
   xsend (cli->sock, buffer, 8*sizeof (char) + 8*sizeof (int), 0);
}


/*------------------------------------------------------------------------------
  Fonctions pour la communication avec les clients
  ----------------------------------------------------------------------------*/
/**
 * Envoi son pseudo au serveur
 */
void cmd_pseudo (ptr_client cli, char *buffer)
{
   int taille;
   printf ("demande de pseudo...\n");
   taille = read_int (cli->sock, buffer);
   xrecv (cli->sock, buffer, taille * sizeof (char), 0);
   memcpy (cli->pseudo, buffer, taille * sizeof (char));

   if (client_doublon (cli) == 1)
   {
      printf("Pseudo ERRONNE\n");
      send_char (cli->sock, RES_PSEUDO_USED);
   }
   else
   {
      printf("Pseudo OK\n");
      send_char (cli->sock, RES_PSEUDO_OK);
   }

   printf (" ...pseudo reçu : %s\n", cli->pseudo);
}


/**
 * Créer une nouvelle partie
 */
void cmd_new_game (ptr_client cli, char *buffer)
{
   ptr_game new_game;
   teams equipe;
   int armees;
   int cases;

   armees = read_int (cli->sock, buffer);
   cases = read_int (cli->sock, buffer);

   printf ("nouvelle partie... \n");
   new_game = game_list_get_new ();
   cli->cur_game = new_game;
   new_game->nb_armees = armees;
   new_game->nb_cases = cases;
   new_game->combat = new_game->nb_cases/2;
   new_game->combat++;
   equipe = read_char (cli->sock, buffer);

   if (equipe == ROME)
   {
      new_game->cli_rome = cli;
      cli->camp = ROME;
   }
   else if (equipe == ALESIA)
   {
      new_game->cli_alesia = cli;
      cli->camp = ALESIA;
   }

   printf(" ...nb_cases: <%d> && nb_armees: <%d> ; combat <%d>\n", 
          new_game->nb_cases, new_game->nb_armees, new_game->combat);
}


/**
 * Récupérer la liste des parties jouables
 */
void cmd_get_list (ptr_client cli, char *buffer)
{
   ptr_game tmp;
   printf ("Liste des parties...\n");

   P (mutex_game);
   tmp = game_list;

   while (tmp)
   {
      printf("begin <%d> -> %d",tmp->id, tmp->begin);
      if (tmp->begin == 0)
      {
         if (tmp->cli_rome == NULL || tmp->cli_alesia == NULL)
            send_game (cli->sock, buffer, tmp);
      }
      tmp = tmp->next;
   }
   V (mutex_game);
   send_char (cli->sock, '1');
}


/**
 * Rejoindre une partie
 */
void cmd_join_game (ptr_client cli, char *buffer)
{
   int game_id;
   ptr_game tmp;
   int i;
   printf ("Rejoindre partie... ");

   xrecv (cli->sock, buffer, sizeof (int), 0);
   printf("Buffer: %s\n", buffer);
   sscanf (buffer, "%d", &game_id);

   P (mutex_game);
   tmp = game_list;
   while (tmp && game_id != tmp->id)
      tmp = tmp->next;

   /* Partie non trouvée */
   if (tmp == NULL)
   {
      printf("Partie non trouvée !!\n");
      send_char (cli->sock, RES_GAME_NOT_FOUND);
   }
   /* Partie trouvée */
   else
   {
      printf("Partie trouvée !!\n");
      if (tmp->cli_rome == NULL)
      {
         tmp->cli_rome = cli;
         cli->camp = ROME;
      }
      else
      {
         tmp->cli_alesia = cli;
         cli->camp = ALESIA;
      }
      tmp->cli_rome->armees = tmp->nb_armees;
      tmp->cli_alesia->armees = tmp->nb_armees;
      tmp->begin = 1;

      cli->cur_game = tmp;

      P (mutex_client);
      /* Distribution des cartes */
      for (i = 0 ; i < NB_CARTES ; i++)
      {
         tmp->cli_rome->cartes[i] = 1+(int) (NB_CF*rand()/(RAND_MAX+1.0));
         tmp->cli_alesia->cartes[i] = 1+(int) (NB_CF*rand()/(RAND_MAX+1.0));
      }
      V (mutex_client);

      send_char (cli->sock, RES_GAME_JOIN_OK);
   }
   V (mutex_game);
}


/**
 * Quitter une partie
 */
void cmd_quit_game (ptr_client cli, char *buffer)
{
   printf("Quitter partie... \n");
   if (cli->camp == ROME)
   {
      if (cli->cur_game)
         cli->cur_game->cli_rome = NULL;
   }
   else
   {
      if (cli->cur_game)
         cli->cur_game->cli_alesia = NULL;
   }
   if (cli->cur_game && cli->cur_game->cli_rome == NULL 
       && cli->cur_game->cli_alesia == NULL)
   {
      game_list_suppr (cli->cur_game);
   }
   cli->cur_game = NULL;
}


/**
 * Quitter le serveur
 */
void cmd_quit_server (ptr_client cli, char *buffer)
{
   printf("Quitter serveur...\n");

   cmd_quit_game (cli, buffer);
   client_list_suppr (cli);

   pthread_exit (0);
}


/**
 * Regarde si un client c'est connecté à sa partie créée
 */
void cmd_get_player (ptr_client cli, char *buffer)
{
   ptr_game tmp;

   printf("J'ai un copain ??...\n");

   P (mutex_game);
   tmp = cli->cur_game;

   if (tmp == NULL)
   {
      send_char (cli->sock, '0');
   }
   else
   {
      if (tmp->cli_rome == NULL || tmp->cli_alesia == NULL)
         send_char (cli->sock, '0');
      else
         send_char (cli->sock, '1');
   }
   V (mutex_game);
}


/**
 * Récupére les cartes distribuées
 */
void cmd_get_cards (ptr_client cli, char *buffer)
{
   int i;
   printf("Allez, file les cartes...\n");

   for (i = 0 ; i < NB_CARTES ; i++)
   {
      printf("%d:%s\n", cli->cartes[i], cf_description[cli->cartes[i]]);
      sprintf (buffer, "%d:%s", cli->cartes[i], cf_description[cli->cartes[i]]);
      xsend (cli->sock, buffer, 512, 0);
   }
}


/**
 * Donne les infos sur la partie rejointe
 */
void cmd_get_my_game (ptr_client cli, char *buffer)
{
   printf("Donne ma partie...\n");
   if (cli->cur_game == NULL)
      send_char (cli->sock, '0');
   else
   {
      send_char (cli->sock, '1');
      send_game (cli->sock, buffer, cli->cur_game);
   }
}


/**
 * Joue un coup. Si les deux joueurs ont déjà joués, on calcule le résultat
 * de la manche dans la foulée
 */
void cmd_play (ptr_client cli, char *buffer)
{
   coups_fourres carte;
   int mise;

   printf("C'est joué...\n");

   /* Lecture du coup */
   carte = read_int (cli->sock, buffer);
   mise = read_int (cli->sock, buffer);
   printf ("coup reçu: %d - %d\n", carte, mise);

   /* On remplit les champs */
   if (cli->camp == ROME)
   {
      cli->cur_game->bet_rome = mise;
      cli->cur_game->card_rome = carte;
      cli->cur_game->got_res_rome = 0;
   }
   else
   {
      cli->cur_game->bet_alesia = mise;
      cli->cur_game->card_alesia = carte;
      cli->cur_game->got_res_alesia = 0;
   }

   /* Si les deux camps ont joués, on calcule direct le résultat */
   if (cli->cur_game->bet_rome > 0 && cli->cur_game->bet_alesia > 0)
      calcul_result (cli->cur_game);
}


/**
 * Envoi le score si les deux joueurs on joués
 */
void cmd_get_result (ptr_client cli, char *buffer)
{
   printf("Kikiagagné...\n");

   printf("paris: %d - %d", cli->cur_game->bet_rome, cli->cur_game->bet_alesia);

   /* qqn a pas encore joué, revenir plus tard */
   if (cli->cur_game->bet_rome == 0 || cli->cur_game->bet_alesia == 0)
   {
      send_char (cli->sock, RES_NO_RESULT);
   }

   /* tlm il a zoué */
   else
   {
      send_char (cli->sock, RES_RESULT);
      send_result (cli, buffer);
   }
}


/**
 * On peut jouer seulement si les deux joueurs ont récupérés les scores
 * sert à ne pas envoyer un nouveau coup sans qu'un des deux joueurs
 * n'est pas encore récupéré le score.
 */
void cmd_can_play (ptr_client cli, char *buffer)
{
   printf("Cay bon, jpeu jouer ??...\n");
   if (cli->cur_game->got_res_rome == 0 || cli->cur_game->got_res_alesia == 0)
   {
      send_char (cli->sock, RES_CANT_PLAY);
   }

   /* tlm il a eu son résultat */
   else
   {
      send_char (cli->sock, RES_CAN_PLAY);

      if (cli->camp == ROME)
         cli->cur_game->got_res_rome++;
      else if (cli->camp == ALESIA)
         cli->cur_game->got_res_alesia++;

      if (cli->cur_game->got_res_rome == 2 && cli->cur_game->got_res_alesia == 2)
         game_raz (cli->cur_game);
   }
}


/**
 * Donne à un joueur l'IP de l'autre joueur pour init le chat 
 */
void cmd_get_other_ip (ptr_client cli, char *buffer)
{
   char *s = NULL;
   int taille;

   printf ("allez, file l'ip de l'autre ! \n");

   if (cli->camp == ROME)
      s = inet_ntoa (cli->cur_game->cli_alesia->cli_ad.sin_addr);
   else if (cli->camp == ALESIA)
      s = inet_ntoa (cli->cur_game->cli_alesia->cli_ad.sin_addr);

   taille = strlen (s);
   sprintf (buffer, ":%d:", taille);
   xsend (cli->sock, buffer, 2*sizeof (char) + sizeof (int), 0);

   sprintf (buffer, "%s", s);
   xsend (cli->sock, buffer, taille, 0);
}


/**
 * Récupère les commandes des clients et exécute la fontion qui va bien
 * Si repère que le client s'est barré, cassos !
 */
void * client_thread (void *arg)
{
   ptr_client cli = (ptr_client)(int *)arg;
   char buffer[512];
   int nb_lu;
   char cmd;

   printf ("nouveau client : <%s>\n", inet_ntoa (cli->cli_ad.sin_addr));


   /* Boucle d'attente des messages du client courant */
   for (;;)
   {
      printf("Attente commande\n");
      nb_lu = xrecv (cli->sock, buffer, 1 * sizeof (char), 0); 
      if (nb_lu == 0) 
      {
         printf("On dirait que cassos !!\n");
         cmd_quit_server (cli, buffer);
      }
      cmd = buffer[0];
      printf("commande: %c\n", cmd);

      switch (cmd)
      {
         case CIN_PSEUDO:
            cmd_pseudo (cli, buffer);
            break;

         case CIN_NEW_GAME:
            cmd_new_game (cli, buffer);
            break;

         case CIN_GET_LIST:
            cmd_get_list (cli, buffer);
            break;

         case CIN_JOIN_GAME:
            cmd_join_game (cli, buffer);
            break;

         case CIN_QUIT_GAME:
            cmd_quit_game (cli, buffer);
            break;

         case CIN_QUIT_SERVER:
            cmd_quit_server (cli, buffer);
            break;

         case CIN_GET_PLAYER:
            cmd_get_player (cli, buffer);
            break;

         case CIN_GET_CARDS:
            cmd_get_cards (cli, buffer);
            break;

         case CIN_GET_MY_GAME:
            cmd_get_my_game (cli, buffer);
            break;

         case CIN_PLAY:
            cmd_play (cli, buffer);
            break;

         case CIN_GET_RESULT:
            cmd_get_result (cli, buffer);
            break;

         case CIN_CAN_PLAY:
            cmd_can_play (cli, buffer);
            break;

         case CIN_GET_OTHER_IP:
            cmd_get_other_ip (cli, buffer);
            break;
      }
   }

   pthread_exit (0);
}


/*------------------------------------------------------------------------------
  Fonctions du serveur
  ----------------------------------------------------------------------------*/
/**
 * Attend que les clients se connectent et cré un thread pour chacun
 * param serv_sock : la socket du serveur
 */
void wait_clients (int serv_sock)
{
   int cli_ad_lg;
   ptr_client cli;
   struct sockaddr_in fautcreerunstructsockaddr_inmemesionsensertpassinoncamarchepasmoipascomprendre;  

   /* Boucle d'attente de connection des clients */
   for (;;)
   {
      cli = client_list_get_new ();
      cli->sock = xaccept (serv_sock,
                           (struct sockaddr *) &(cli->cli_ad) ,
                           &cli_ad_lg);

      if (pthread_create (&(cli->thread_id), NULL,
                          client_thread, (void *) cli) < 0)
         erreur_sys (ERR_PTHREAD_CREATE);
   }
}


/**
 * Crée une socket pour le serveur
 * param port : le port de connection de la socket
 * return : un numéro de socket
 */
int create_server (int port)
{
   int sock;
   struct sockaddr_in serv_ad;

   serv_ad.sin_port = htons (port);
   serv_ad.sin_family = AF_INET;
   serv_ad.sin_addr.s_addr = htonl (INADDR_ANY);

   sock = xsocket (PF_INET, SOCK_STREAM, 0);
   xbind (sock,(struct sockaddr *) &serv_ad, sizeof (serv_ad));
   xlisten (sock, 5);

   return sock;
}


/**
 * Initialise le serveur
 */
void init_server ()
{
   client_list = NULL;
   game_list = NULL;
   srand (getpid ());
   next_game_id = 1;
   pthread_mutex_init (&mutex_game, NULL);
   pthread_mutex_init (&mutex_client, NULL);
}


/**
 * Libère la mémoire allouée
 */
void delete_mem ()
{
   printf("at exit\n");
   close (serv_sock);
   game_list_delete ();
   client_list_delete ();
}


/**
 *
 */
int main ()
{

   /*signal(SIGINT, delete_mem);*/
   atexit (delete_mem);

   init_server ();

   serv_sock = create_server (SERVER_PORT);
   wait_clients (serv_sock);

   exit (EXIT_ALL_OK);
}
