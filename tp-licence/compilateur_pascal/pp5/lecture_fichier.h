#ifndef __LECTURE_FICHIER_H__
#define __LECTURE_FICHIER_H__


/*------------------------------------------------------------------------------
  Constantes
  ----------------------------------------------------------------------------*/
#define ERREUR      -1
#define FIN_FICHIER -5
#define BUFFER_FILE 1024
#define CAR_VIDE    -2


/*------------------------------------------------------------------------------
  Types personnalisés
  ----------------------------------------------------------------------------*/
/* Contient toutes les informations relatives à un fichier */
typedef struct
{
      int fd;
      char *nom_fichier;
      char buffer[BUFFER_FILE];
      int pos_buffer;
      int pos_fichier;
      char insere;
      unsigned int ligne;
      unsigned int colone;
}Fichier;


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
void affichage_buffer (Fichier);
void remplit_buffer (Fichier*);
Fichier ouvre_fichier_lecture (char *);
Fichier ouvre_fichier_ecriture (char *);
void ferme_fichier (Fichier *);
void supprime_fichier (Fichier *);
char lire_car (Fichier *);
int end_of_file (Fichier *);
void insere_car (Fichier *, char );
void ecrire_car (char, Fichier *);
void ecrire_string (char *, Fichier *);
void ecrire_entier (int, Fichier *);
void ecrit_buffer (Fichier *);
void vide_buffer (Fichier *);


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/


#endif
