program proc1 (input, output);
  var x, y, r: integer;
  function f(): integer;
    var z: integer;
    begin
      z:= 1;
      write(z, x);
      x:= x - 1;
      f:= x + z;
    end
begin
  x:= 3;
  y:= 5;
  r:= f;
  write(r);
end.
