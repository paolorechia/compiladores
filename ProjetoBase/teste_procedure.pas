program proc1 (input, output);
  var x, y: integer;
  procedure p;
    var z, w, k: integer;
    begin
      x:= x - 1;
      if (x > 1) then
        p;
      y:= y * z;
    end
  procedure q;
    var u: integer;
    begin
      u := 3 * 7;
      write(u);
    end
begin
  read(x);
  p;
  write(x, y);
end.
