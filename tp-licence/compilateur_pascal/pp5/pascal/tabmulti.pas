program tabmulti;

var 
   test2    : array [2..4,3,4,5] of integer;
   test    : array [4,6,7] of integer;
   i,j,k,l : integer;

begin
   test2[3,2,3,4]:=4;
   i:=1;
   j:=1;
   k:=1;
   l:=7;
{    for i:= 0 to 2 do }
{    begin }
{       for j:=0 to 3 do }
{       begin }
         for k:=0 to 5 do
         begin
            write('k = ');
            writeln (k);
            for l:=0 to 6  do
            begin
{                write('i = '); }
{                write (i); }
{                write(' j = '); }
{                write (j); }
               write(' l = ');
               writeln (l);
               test[j,k,l] := l;
{                writeln(test[j,k,l]); }
            end;
         end;
{       end; }
{    end; }
   test[3,5,6]:=4;
   writeln( test[3,5,6]);
end.