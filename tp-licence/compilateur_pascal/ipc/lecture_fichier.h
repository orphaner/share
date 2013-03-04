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
typedef struct
{
      int fd;
      char buffer[BUFFER_FILE];
      int pos_buffer;
      int pos_fichier;
      char insere;
}Fichier;


/*------------------------------------------------------------------------------
  Prototypes de fonctions
  ----------------------------------------------------------------------------*/
void remplit_buffer (Fichier*);
Fichier ouvre_fichier_lecture (char *);
void ferme_fichier (Fichier *);
char lire_car (Fichier *);
int end_of_file (Fichier *);
void insere_car (Fichier *, char );

#endif
