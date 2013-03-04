program toto;
var i,j,k : integer;
   c,d, e      : char;
   b        :  boolean;

begin
   writeln('-=- Tests des Entrees sorties -=- ');
   writeln('-=- Pensez Žà vous trompez pour activer les messages d erreurs -=- ');
   
   i:= 1;
   j:= 4;

   writeln('i =', i, ' j =',j);
   
   writeln('-=- affichage de la somme de 2 variables : ');
   writeln('-#- writeln(j+i);');
   writeln(j+i);

   writeln('-=- saisie d entier');
   read(k);
   write('-=- affichage de la valeur saisie:');
   writeln(k);
   writeln('-=- affichage SANS retour a la ligne de 1 puis 2 puis 3:');
   write(1);
   write(2);
   write(3);
   writeln(' ');
   writeln('-=- affichage AVEC retour a la ligne de 1 puis 2 puis 3:');
   writeln(1);
   writeln(2);
   writeln(3);
   writeln(' ');
   writeln('-=- saisie de boulŽéen ');
   read(b);
   write('-=- affichage de la valeur saisie: ');
   writeln(b);
   
   writeln('-=- saisie d un char ');
   read(c);
   write('-=- affichage de la valeur saisie: ');
   writeln(c);

   writeln('-=- saisie de 3 char 1 par 1 ');
   read(c);
   read(d);
   read(e);
   write('-=- affichage des 3 char saisis: ');
   write(c);
   write(d);
   writeln(e);
   
end. 
