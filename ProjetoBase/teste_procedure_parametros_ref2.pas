program proc1 (input, output);
  var x, y: integer;
  procedure p(var t: integer, b: integer);
    var z, w, k: integer;
    begin
      x:= x - 1;
      y:= y * z;
      p(w, k);
      t:= t;
    end
begin
  read(x);
  p(x, y);
  write(x, y);
end.
