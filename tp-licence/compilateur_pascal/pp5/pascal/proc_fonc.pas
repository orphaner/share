program testoffset;

const
   MAX = 100;
type
   complex     = record
                    re,im : integer;
                 end;     
   pile        = array [0..10] of complex;
   pile_int    = array [0..10] of integer;
   complexpile = record
                    p : array [0..2] of record
                           re,im : integer ;
                        end;
                    sommet  : integer;
                 end;
   
const
   TOOOOOO = 100;
var
   test1   : complex;
   i       : integer;
   mapile  : complexpile;
   mapile2 : complexpile;
   test    : array [0..2] of integer;
   test2   : array [0..2] of complex;
   ok      : boolean;
   c       : char;




function fucnspeciale (idd : integer ):integer; 
const MAX = 3;
var ddddeedede : integer;
begin
   write (MAX);
   idd:=45;
   idd:=4;
end;

procedure totii (idd :integer );
const MAX = 4;
var ddddededede : integer;
begin
   idd:=45;
   write ('tot');
end;

procedure totii2 (idd :boolean );
const MAX = 5;
var ddddededede : integer;
begin
   idd:=true;
   write ('tot');
end;

procedure totii3 (idd :boolean );
const MAX = 6;
var ddddededede : integer;
begin
   idd:=true;
   write ('tot');
end;

procedure totii4 (idd :boolean );
const MAX = 7;
var ddddededede : integer;
begin
   idd:=true;
   write ('tot');
end;

function fucnspeciale2 (idd :integer ):integer;
const MAX = 8;
var ddddeedede: integer;
begin
   write ('tot');
   ddddeedede:=45;
   idd:=4;
end;

function specialtest (c : char ,b:boolean,i:integer):integer;
const MAX =9;
begin
   i:=4;
   specialtest:=4;
end;

begin
   totii2(true);
   i:=10+fucnspeciale(i)+specialtest('t',false,1+5+8-8);
   i:=10+fucnspeciale(i)+specialtest(c,ok,i);
   test1.im:=4;
   i:=10;
   mapile.sommet := 0;
   mapile2.sommet := 0;
   mapile.p[2].re := 10;
   test[5]:=MAX+5+i;
   ok := true;
   c := 't';
   writeln(ok);
   read(ok, test2[1].re,c);
   writeln(ok);
   writeln(test2[1].re);
   writeln (i);
   

   writeln(1+5+8+8+7);
   while  true = ok do
   begin
      if i = 15 then
         ok:=false;
      writeln (i);
      i:=i+1;
   end;
   c:='z';
   if c <> 't' then
      writeln(ok);

   if 1+i=5+5+1  then
      writeln(ok);
   mapile.p[0].re :=3;
end.
