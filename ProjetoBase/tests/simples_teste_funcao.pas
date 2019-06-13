program proc1 (input, output);
  var x, y, r: integer;
  function f(t: integer, b: integer): integer;
    var z: integer;
    begin
      z:= 1;
      write(t, b);
      x:= x - 1;
      f:= x + z;
    end
begin
  x:= 3;
  y:= 5;
  f(x, y); 
  write(r);
end.
