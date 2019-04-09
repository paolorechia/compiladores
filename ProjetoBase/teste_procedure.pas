program proc1 (input, output);
  var x, y: integer;
  procedure p;
    var z: integer;
    begin
      x:= x - 1;
      if (x > 1) then
        p;
      y:= y * z;
    end
begin
  read(x);
  p;
  write(x, y);
end.
