program toto;
var i,j,k : integer;

begin
   
   for i:=2 to 7 step 2 do
   begin
      writeln('i = ',i);
      for j:=0 to 3 do
      begin
         writeln('   j = ',j); 
         for k:=5 downto 3 do
         begin                  
            writeln('      k = ',k);
         end;
      end;
   end;
end.
