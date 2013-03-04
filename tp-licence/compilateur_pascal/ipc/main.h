
/* Ces variables sont placées en globales, afin que l'appel à la fonction
 * free_all soit plus simple, car toutes les fonctions faisant appel à
 * free_all n'utilisent pas forcément les trois variables ci dessous.  */
Pile_exe p_exe; /* La pile des instructions */
Pile_var p_var; /* La pile des variables */

Pile_var p_base; /* La pile des variables (lifo, donc on utilise la structure de la pile des variables */

Fichier fic;    /* Le fichier contenant le pcode */
