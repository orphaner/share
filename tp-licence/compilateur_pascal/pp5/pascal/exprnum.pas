program toto;
var i,j,k,l : integer;
   c      : char;
   b      : boolean;
          
begin
   writeln('-=- Tests des Expresions numŽériques -=- ');

   i := 5;
   j := 10;
   k := 100;

   {+ - * / }
   l := i+ j*5*2 - k - 10/2 ;    { l = 0}
   writeln(l);

   { () }
   l := 2 * ( 1 + 1) * (2 - 1 ) ; { l = 4}
   writeln(l);

   { -+ +- -( +( }
   l := -l + -(+4 * -2 --i ++3) ; { l = -4}
   writeln(l);

   {erreurs Žà la compilation : (exemples) }
   
{   l := +++3; } 
   

   
end.