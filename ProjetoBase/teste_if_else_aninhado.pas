program exemplo75 (input, output);
var n: integer;
begin    
  n:= 10;
  if n > 0 then 
    begin
      if (n > 2) then
        n:= n + 1;
       else
        begin
          n:= n * 2;
          n:= n + 4;
        end;
      n:= n - 1;
    end;
   else
    n:= n / 3;
end.

