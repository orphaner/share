/**
 * module : hash
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
#include <stdlib.h>
#include <string.h>


#include "syntaxique.h"
#include "typepredef.h"
#include "hash.h"


/*------------------------------------------------------------------------------
  Ecriture des fonctions
  ----------------------------------------------------------------------------*/
/*==============================================================================
  Fonctions de l'arbre binaire de recherche
  ============================================================================*/
/**
 * Crée un nouveau noeud avec sym et sa classe
 * param c : la classe de sym
 * param localite : la localite du symbole
 * return : un pointeur sur le noeud créé
 */
Arbre abr_creer_noeud (Classes c, int ladresse, Arbre localite)
{
   Arbre tmp = (Arbre) malloc (sizeof (Noeud));
   tmp->classe = c;
   tmp->nom = (char *) malloc (nb_sym * sizeof (char));
   tmp->adresse = ladresse;
   strcpy (tmp->nom, sym);
   tmp->fg = tmp->fd = NULL;
   tmp->localite = localite;
   return tmp;
}


/**
 * Ajoute un identificateur dans la table
 * param *abr : l'abre dans lequel on rajoute sym
 * param c : la classe de sym
 * param ladresse : l'adresse du symbole
 * param localite : la localite du symbole
 * return : un pointeur vers le maillon créé, null sinon
 */
Arbre abr_add (Arbre *abr, Classes c, int ladresse, Arbre localite)
{
   Arbre parcours = *abr;
   Arbre prec;
   Arbre retour = NULL;
   char comp;

   /* Arbre nul, on ajoute en racine d'arbre */
   if (!*abr)
   {
      retour = *abr = abr_creer_noeud (c, ladresse, localite);
   }
   else
   {
      /* Sinon on recherche la bonne place dans l'arbre */
      prec = parcours;
      while (parcours)
      {

         prec = parcours;
         comp = strcmp (parcours->nom, sym);

         if (comp < 0)
         {
            parcours = parcours->fd;
         }

         else /* if (comp > 0) */
         {
            parcours = parcours->fg;
         }
      }

      comp = strcmp (prec->nom, sym);

      /* Ajout du nouveau noeud */
      if (comp < 0)
         retour = prec->fd = abr_creer_noeud (c, ladresse, localite);

      else
         retour = prec->fg = abr_creer_noeud (c, ladresse, localite);
   }

   return retour;
}


/**
 * Recherche un élément dans un arbre
 * param abr : l'arbre dans lequel on recherche
 * param *s : la chaine de caractère recherchée
 * param c : la classe de la chaine de caractère
 * param localite : la localite du symbole
 * return : un pointeur vers le maillon trouvé, NULL sinon
 */
Arbre abr_search (Arbre abr, char *s, Classes c, Arbre localite)
{
   Arbre parcours = abr;
   char comp;

   while (parcours)
   {
      comp = strcmp (parcours->nom, s);

      if (comp < 0)
         parcours = parcours->fd;

      else if (comp > 0)
         parcours = parcours->fg;

      /* Symbole de même nom, ... */
      else if (!comp)
      {
         if ((parcours->classe & c) && (parcours->localite == localite))
            return parcours;

         parcours = parcours->fg;
      }
   }

   return NULL;
}


/**
 * Désalloue tous les espaces mémoires créés pour un arbre
 * param abr : l'arbre à désaouller
 */
void abr_free (Arbre abr)
{
   if (abr)
   {
      free (abr->nom);
      if (abr->type & TYPE_TABLEAU)
         free (abr->type_info.tableau.tab);
      if (abr->classe & (CLASS_PROCEDURE | CLASS_FONCTION))
         free (abr->FUNC_TAB_PARAM);
      abr_free (abr->fg);
      abr_free (abr->fd);
      free (abr);
   }
}


/**
 * Affiche un arbre
 * param abr : l'arbre à afficher
 */
void abr_affiche (Arbre abr)
{
   if (abr)
   {
      printf("  - %s ", abr->nom);
      if (!abr->localite)
         printf(" ; Global");
      else
         printf(" ; localite : %s", abr->localite->nom);
      if (abr->classe & (CLASS_VAR | CLASS_CONST) )
         printf(" ; adr: %d\n", abr->adresse);
      else
         printf("\n");
      abr_affiche (abr->fg);
      abr_affiche (abr->fd);
   }
}


/*==============================================================================
  Fonctions de la table de hash
  ============================================================================*/
/**
 * Référence : Maitrise des algorithmes en C. Edition O'Reilly.
 * (Diverses corrections pour les unsigned afin d'avoir toujours un entier positif)
 * Donne une clé de hash en fonction d'une chaine de caractères
 * param *s : la chaine de caractères
 * return : la clé de hash de *s
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
 * Retourne une nouvelle clé
 * param i : la clé initiale
 */
unsigned int hash_saut (unsigned int i)
{
   return (i+1) % MAX_TABLE;
}


/**
 * Vérifie que deux symboles sont strictements égaux, id+classe
 * param abr : un pointeur vers l'endroit où est stocké l'elt à vérifier
 * param c : la classe
 * return : 0 ou 1
 */
char symbole_egal (Arbre abr, Classes c)
{
   return (abr
           && !strcmp (abr->nom, sym)
           && (abr->classe & c)
           && !(abr->classe & CLASS_CHAMP)
           && !abr->localite);
}


/**
 * Vérifie si deux symboles ont le même ID
 * param abr : un pointeur vers l'endroit où est stocké l'elt à vérifier
 * param localite : la localite du symbole
 * return : 0 ou 1
 */
char symbole_egal2 (Arbre abr, Arbre localite)
{
   return (abr
           && !strcmp (abr->nom, sym)
           && !(abr->classe & CLASS_CHAMP)
           && (abr->localite == localite));
}


/**
 * Recherche un identificateur dans la table de hash
 * param *t : la table de hash
 * param *s : la chaine de caractère recherchée
 * param c : la classe de la chaine de caractère
 * return : un pointeur vers le maillon trouvé, NULL sinon
 */
Arbre hash_search (Tablesym *t, char *s, Classes c)
{
   unsigned int cle, jump;
   int i = 0;
   cle = jump = hash (sym);

   /* Hashage direct, c'est la fête ^^ */
   if (symbole_egal (t->table[cle], c))
   {
      return t->table[cle];
   }

   /* On se ballade dans les sauts */
   while (t->table[jump] && i++ < MAX_SAUT)
   {
      jump = hash_saut (jump);
      if (symbole_egal (t->table[jump], c))
      {
         return t->table[jump];
      }
   }

   /* Sinon on se ballade dans l'arbre */
   return abr_search (t->table[cle], s, c, NULL);
}


/**
 * Ajoute sym dans la table de hash
 * param *t : la table de hash
 * param c : la classe de la chaine de caractère
 * param localite : la localite du symbole
 * return : l'adresse de l'élément ajouté, NULL sinon (double déclaration)
 */
Arbre hash_add (Tablesym *t, Classes c, Arbre localite)
{
   unsigned int cle, jump;
   int i = 0;

   cle = jump = hash (sym);

   if (symbole_egal2 (t->table[cle], localite))
      return NULL;

   while (t->table[jump] && i++ < MAX_SAUT)
   {
      jump = hash_saut (jump);
      if (symbole_egal2 (t->table[jump], localite))
      {
         return NULL;
      }
   }

   return  (t->table[jump]) ?
      abr_add (&t->table[cle], c, ((c & CLASS_CONST) ? 0 : t->offset), localite )  :
      abr_add (&t->table[jump], c, ((c & CLASS_CONST) ? 0 : t->offset), localite);
}


/**
 * Initialise correctement la table de hash
 * -> tous les pointeurs vers les arbres à NULL
 * -> ajout des types prédéfinis
 * -> offset à zéro
 * param *t : la table de hash à initialiser
 */
void hash_init (Tablesym *t)
{
   unsigned int i = 0;
   t->offset = 0;
   for (; i < MAX_TABLE ; t->table[i++] = NULL);

   add_types_predefinis ();
}


/**
 * Désalloue l'espace mémoire occupé par une table de hash
 * param *t : la table de hash à désallouer
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
 * param *t : la table de hash à désallouer
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


/*==============================================================================
  Fonctions pour les champs des records
  ============================================================================*/
/**
 * Vérifie si deux champ sont identiques (même nom ds un même record)
 * param abr : un pointeur vers l'endroit où est stocké l'elt à vérifier
 * param pere : le record auquel on ajout un champ
 * return :
 */
char symbole_egal_champ (Arbre abr, struct _noeud *pere)
{
   return (abr
           && !strcmp (abr->nom, sym)
           && (abr->classe & CLASS_CHAMP)
           && abr->type_info.champ.from_record == pere);
}


/**
 * Ajoute un identificateur dans la table
 * param *abr : l'abre dans lequel on rajoute sym
 * param c : la classe de sym
 * param ladresse : l'adresse du symbole
 * param localite : la localite du symbole
 * return : un pointeur vers le maillon créé, null sinon
 */
Arbre abr_add_champ (Arbre *abr, Classes c, int ladresse, Arbre pere)
{
   Arbre parcours = *abr;
   Arbre prec;
   Arbre retour = NULL;
   char comp;

   /* Arbre nul, on ajoute en racine d'arbre */
   if (!*abr)
   {
      retour = *abr = abr_creer_noeud (c, ladresse, NULL);
   }
   else
   {
      /* Sinon on recherche la bonne place dans l'arbre */
      prec = parcours;
      while (parcours)
      {
         if (symbole_egal_champ (parcours, pere))
            return NULL;

         prec = parcours;
         comp = strcmp (parcours->nom, sym);

         if (comp < 0)
            parcours = parcours->fd;

         else
            parcours = parcours->fg;
      }

      /* Ajout du nouveau noeud */
      comp = strcmp (prec->nom, sym);
      if (comp < 0)
         retour = prec->fd = abr_creer_noeud (c, ladresse, NULL);

      else
         retour = prec->fg = abr_creer_noeud (c, ladresse, NULL);

   }

   return retour;
}

/**
 * Ajoute sym dans la table de hash
 * Ici sym est un champ de record
 * param *t : la table de hash
 * param pere : le record auquel on ajout un champ
 * return : l'adresse de l'élément ajouté, NULL sinon (double déclaration)
 */
Arbre hash_add_champ (Tablesym *t, struct _noeud *pere)
{
   unsigned int cle, jump;
   int i = 0;

   cle = jump = hash (sym);

   /* Hashage direct, c'est la fête ^^ */
   if (symbole_egal_champ (t->table[cle], pere))
   {
      return NULL;
   }

   /* On se ballade dans les sauts */
   while (t->table[jump] && i++ < MAX_SAUT)
   {
      jump = hash_saut (jump);
      if (symbole_egal_champ (t->table[jump], pere))
      {
         return NULL;
      }
   }

   /* Sinon on se ballade dans l'arbre */
   return (t->table[jump]) ?
      abr_add_champ (&t->table[cle], CLASS_CHAMP, 0, pere)  :
      abr_add_champ (&t->table[jump], CLASS_CHAMP, 0, pere);
}


/**
 * Recherche un élément dans un arbre
 * param abr : l'arbre dans lequel on recherche
 * param *s : la chaine de caractère recherchée
 * param pere : le record auquel on ajout un champ
 * return : un pointeur vers le maillon trouvé, NULL sinon
 */
Arbre abr_search_champ (Arbre abr, char *s, struct _noeud *pere)
{
   Arbre parcours = abr;
   char comp;

   while (parcours)
   {
      comp = strcmp (parcours->nom, s);

      if (comp < 0)
         parcours = parcours->fd;

      else
         parcours = parcours->fg;

      if  (symbole_egal_champ (parcours, pere))
         return parcours;

   }
   return NULL;
}


/**
 * Recherche un identificateur dans la table de hash
 * param *t : la table de hash
 * param *s : la chaine de caractère recherchée
 * param pere : le record auquel on ajout un champ
 * return : un pointeur vers le maillon trouvé, NULL sinon
 */
Arbre hash_search_champ (Tablesym *t, char *s, struct _noeud *pere)
{
   unsigned int cle, jump;
   int i = 0;
   cle = jump = hash (sym);

   /* Hashage direct, c'est la fête ^^ */
   if (symbole_egal_champ (t->table[cle], pere))
      return t->table[cle];

   /* On se ballade dans les sauts */
   while (t->table[jump] && i++ < MAX_SAUT)
   {
      jump = hash_saut (jump);
      if (symbole_egal_champ (t->table[jump], pere))
         return t->table[jump];
   }

   /* Sinon on se ballade dans l'arbre */
   return abr_search_champ (t->table[cle], s, pere);
}


/*==============================================================================
  Fonctions pour les accès aux variables locales
  ============================================================================*/
/**
 * Vérifie que deux symboles sont strictements égaux, id+classe+
 * param abr : un pointeur vers l'endroit où est stocké l'elt à vérifier
 * param c : la classe
 * param localite : la localite du symbole
 * return : 0 ou 1
 */
char symbole_egal_localite (Arbre abr, Classes c, Arbre localite)
{
   return (abr
           && !strcmp (abr->nom, sym)
           && (abr->classe & c)
           && !(abr->classe & CLASS_CHAMP)
           && (abr->localite == localite));
}


/**
 * Recherche un identificateur local ou global dans la table de hash
 * param *t : la table de hash
 * param *s : la chaine de caractère recherchée
 * param c : la classe de la chaine de caractère
 * param localite : la localite du symbole
 * return : un pointeur vers le maillon trouvé, NULL sinon
 */
Arbre hash_search_localite (Tablesym *t, char *s, Classes c, Arbre localite)
{
   unsigned int cle, jump;
   int i = 0;
   Arbre retour;
   cle = jump = hash (sym);

   /* Hashage direct, c'est la fête ^^ */
   if (symbole_egal_localite (t->table[cle], c, localite))
      return t->table[cle];

   /* On se ballade dans les sauts */
   while (t->table[jump] && i++ < MAX_SAUT)
   {
      jump = hash_saut (jump);
      if (symbole_egal_localite (t->table[jump], c,localite))
         return t->table[jump];
   }

   /* Sinon on se ballade dans l'arbre */
   retour = abr_search (t->table[cle], s, c, localite);

   return (retour) ? retour : hash_search (t, s,c);
}
