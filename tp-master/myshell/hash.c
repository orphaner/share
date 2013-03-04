/**
 * module : hash
 * brief : 
 * author : Maxime GRYL
 * author : Nicolas LASSALLE
 * date : 
 * bug : Aucun d�tect� � ce jour
 *
 */


/*------------------------------------------------------------------------------
  Fichiers include
  ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "hash.h"
#include "memutils.h"


/*------------------------------------------------------------------------------
  Variables globales
  ----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/*==============================================================================
  Fonctions de l'arbre binaire de recherche
  ============================================================================*/
/**
 * Cr�e un nouveau noeud 
 * param * nom :
 * param * valeur : 
 * return : un pointeur sur le noeud cr��
 */
Arbre abr_creer_noeud (char const * const nom, char const * const valeur)
{
   Arbre tmp = my_malloc (sizeof (Noeud));
   tmp->nom = my_malloc ((strlen(nom)+1) * sizeof (char));
   tmp->valeur = my_malloc ((strlen(valeur)+1) * sizeof (char));
   strcpy (tmp->nom, nom);
   strcpy (tmp->valeur, valeur);
   tmp->fg = tmp->fd = NULL;
   return tmp;
}


/**
 * Ajoute un identificateur dans la table
 * return : un pointeur vers le maillon cr��, null sinon
 */
Arbre abr_add (Arbre *abr, char * nom, char * valeur)
{
   Arbre parcours = *abr;
   Arbre prec;
   Arbre retour = NULL;
   char comp;

   /* Arbre nul, on ajoute en racine d'arbre */
   if (!*abr)
   {
      retour = *abr = abr_creer_noeud ( nom, valeur);
   }
   else
   {
      /* Sinon on recherche la bonne place dans l'arbre */
      prec = parcours;
      while (parcours)
      {

         prec = parcours;
         comp = strcmp (parcours->nom, nom);

         if (comp < 0)
         {
            parcours = parcours->fd;
         }

         else /* if (comp > 0) */
         {
            parcours = parcours->fg;
         }
      }

      comp = strcmp (prec->nom, nom);

      /* Ajout du nouveau noeud */
      if (comp < 0)
      {
         printf("comp < 0\n");
         retour = prec->fd = abr_creer_noeud (nom, valeur);
      }
      else
      {
         printf("comp else\n");
         retour = prec->fg = abr_creer_noeud (nom, valeur);
      }
   }

   return retour;
}


/**
 * Recherche un �l�ment dans un arbre
 * param abr : l'arbre dans lequel on recherche
 * param *s : la chaine de caract�re recherch�e
 * param c : la classe de la chaine de caract�re
 * param localite : la localite du symbole
 * return : un pointeur vers le maillon trouv�, NULL sinon
 */
Arbre abr_search (Arbre abr, char * nom)
{
   Arbre parcours = abr;
   char comp;

   while (parcours)
   {
      comp = strcmp (parcours->nom, nom);

      if (comp < 0)
         parcours = parcours->fd;

      else if (comp > 0)
         parcours = parcours->fg;

      /* Symbole de m�me nom, ... */
      else if (!comp)
         return parcours;
      
   }

   return NULL;
}


/**
 * D�salloue tous les espaces m�moires cr��s pour un arbre
 * param abr : l'arbre � d�saouller
 */
void abr_free (Arbre abr)
{
   if (abr)
   {
      free (abr->nom);
      free (abr->valeur);
      abr_free (abr->fg);
      abr_free (abr->fd);
      free (abr);
   }
}


/**
 * Affiche un arbre
 * param abr : l'arbre � afficher
 */
void abr_affiche (Arbre abr)
{
   if (abr)
   {
      printf("  - %s : %s\n", abr->nom, abr->valeur);
      abr_affiche (abr->fg);
      abr_affiche (abr->fd);
   }
}


/*==============================================================================
  Fonctions de la table de hash
  ============================================================================*/
/**
 * R�f�rence : Maitrise des algorithmes en C. Edition O'Reilly.
 * (Diverses corrections pour les unsigned afin d'avoir toujours un entier positif)
 * Donne une cl� de hash en fonction d'une chaine de caract�res
 * param *s : la chaine de caract�res
 * return : la cl� de hash de *s
 */
unsigned int hash (char *s)
{
   unsigned char *p = (unsigned char *) s;
   unsigned int res = 0;
   unsigned int tmp;

   while (*p)
   {
      res = (res << 4) + (*p);
      if ( (tmp = (res & 0xf0000000U)))
      {
         res = res ^ (tmp >> 24);
         res = res ^ tmp;
      }
      p++;
   }

   return res % MAX_TABLE;
}

/**
 */
unsigned int hash2 (char *s, unsigned int const max_table)
{
   unsigned char *p = (unsigned char *) s;
   unsigned int res = 0;
   unsigned int tmp;

   while (*p)
   {
      res = (res << 4) + (*p);
      if ( (tmp = (res & 0xf0000000U)))
      {
         res = res ^ (tmp >> 24);
         res = res ^ tmp;
      }
      p++;
   }

   return res % max_table;
}


/**
 * Retourne une nouvelle cl�
 * param i : la cl� initiale
 */
unsigned int hash_saut (unsigned int i)
{
   return (i+1) % MAX_TABLE;
}


/**
 * V�rifie que deux symboles sont strictements �gaux
 * param abr : un pointeur vers l'endroit o� est stock� l'elt � v�rifier
 * param *nom : 
 * return : 0 ou 1
 */
char symbole_egal (Arbre abr, char * nom)
{
   return (abr && !strcmp (abr->nom, nom));
}


/**
 * Recherche un identificateur dans la table de hash
 * param *t : la table de hash
 * param *s : la chaine de caract�re recherch�e
 * param c : la classe de la chaine de caract�re
 * return : un pointeur vers le maillon trouv�, NULL sinon
 */
Arbre hash_search (Tablesym * t, char * nom)
{
   unsigned int cle, jump;
   int i = 0;
   cle = jump = hash (nom);

   /* Hashage direct, c'est la f�te ^^ */
   if (symbole_egal (t->table[cle], nom))
   {
      return t->table[cle];
   }

   /* On se ballade dans les sauts */
   while (t->table[jump] && i++ < MAX_SAUT)
   {
      jump = hash_saut (jump);
      if (symbole_egal (t->table[jump], nom))
      {
         return t->table[jump];
      }
   }

   /* Sinon on se ballade dans l'arbre */
   return abr_search (t->table[cle], nom);
}


/**
 * Ajoute sym dans la table de hash
 * param *t : la table de hash
 * param c : la classe de la chaine de caract�re
 * param localite : la localite du symbole
 * return : l'adresse de l'�l�ment ajout�, NULL sinon (double d�claration)
 */
Arbre hash_add (Tablesym * t, char * nom, char * valeur)
{
   unsigned int cle, jump;
   int i = 0;

   cle = jump = hash (nom);

   if (symbole_egal (t->table[cle], nom))
      return NULL;

   while (t->table[jump] && i++ < MAX_SAUT)
   {
      jump = hash_saut (jump);
      if (symbole_egal (t->table[jump], nom))
      {
         return NULL;
      }
   }

   return  (t->table[jump]) ?
      abr_add (&t->table[cle], nom, valeur) : abr_add (&t->table[jump], nom, valeur);
}


/**
 *
 */
void modif (Arbre abr, char * nom)
{
   free (abr->valeur);
   abr->valeur = my_malloc ((strlen (nom) +1) * sizeof (char));
   strcpy (abr->valeur, nom);
}


/**
 * Ajoute sym dans la table de hash
 * param *t : la table de hash
 * param c : la classe de la chaine de caract�re
 * param localite : la localite du symbole
 * return : l'adresse de l'�l�ment ajout�, NULL sinon (double d�claration)
 */
Arbre hash_modif (Tablesym * t, char * nom, char * valeur)
{
   unsigned int cle, jump;
   int i = 0;

   cle = jump = hash (nom);

   if (symbole_egal (t->table[cle], nom))
   {
      modif (t->table[cle], valeur);
      return t->table[cle];
   }

   while (t->table[jump] && i++ < MAX_SAUT)
   {
      jump = hash_saut (jump);
      if (symbole_egal (t->table[jump], nom))
      {
         modif (t->table[jump], valeur);
         return t->table[jump];
      }
   }

   return  (t->table[jump]) ?
      abr_add (&t->table[cle], nom, valeur) : abr_add (&t->table[jump], nom, valeur);
}


/**
 * Initialise correctement la table de hash
 * -> tous les pointeurs vers les arbres � NULL
 * -> ajout des types pr�d�finis
 * -> offset � z�ro
 * param *t : la table de hash � initialiser
 */
void hash_init (Tablesym *t)
{
   unsigned int i = 0;
   t->taille = MAX_TABLE;
   for (; i < MAX_TABLE ; t->table[i++] = NULL);

}


/**
 * D�salloue l'espace m�moire occup� par une table de hash
 * param *t : la table de hash � d�sallouer
 */
void hash_free (Tablesym *t)
{
   unsigned int i = 0;
   for (; i < MAX_TABLE ; i++)
   {
      if (t->table[i])
         abr_free (t->table[i]);
   }
}


/**
 * Affiche la table des symboles
 * param *t : la table de hash � d�sallouer
 */
void hash_affiche (Tablesym *t)
{
   unsigned int i = 0;
   for (; i < MAX_TABLE ; i++)
   {
      if (t->table[i])
      {
         printf("table[%d] :\n",i);
         abr_affiche (t->table[i]);
      }
   }
}

/* int main () */
/* { */
/*    Tablesym table; */
/*    hash_init (&table); */
/*    hash_add (&table, "toto", "prout"); */
/*    hash_add (&table, "tam�re", "suce des zours"); */
/*    hash_add (&table, "tota", "prout"); */
/*    hash_add (&table, "toti", "prout"); */
/*    hash_add (&table, "toto", "prout"); */
/*    hash_add (&table, "totl", "prout"); */
/*    hash_add (&table, "tote", "prout"); */
/*    hash_affiche (&table); */
/*    hash_free (&table); */
/* } */
