program proc1 (input, output);
  var x, y: integer;
  procedure p(var t: integer, b: integer);
    var z, w, k: integer;
    begin
      x:= x - 1;
      y:= y * z;
      p(w, w);
      t:= x;
    end
begin
  read(x);
  p(x, y);
  write(x, y);
  write(t);
end.
