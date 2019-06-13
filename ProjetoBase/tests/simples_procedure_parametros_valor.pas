program proc1 (input, output);
  var x, y: integer;
  procedure p(t: integer, b: integer);
    var z: integer;
    begin
      z:= 1;
      write(t, b);
      x:= x - 1;
      t:= x + z;
    end;
begin
  x:= 3;
  y:= 5;
  p(x, y); 
end.
