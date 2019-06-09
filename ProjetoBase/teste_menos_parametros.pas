program proc1 (input, output);
  var x, y: integer;
  procedure p(t: integer, b: integer);
    var z, w, k: integer;
    begin
      x:= x - 1;
      if (x > 1) then
        p;
      y:= y * z;
    end
begin
  read(x);
  p(x);
  write(x, y);
end.
