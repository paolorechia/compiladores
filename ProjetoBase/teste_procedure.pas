program proc1 (input, output);
  var x, y: integer;
  procedure p;
    begin
      x:= x - 1;
    end
begin
  read(x);
  p;
  write(x, y);
end.
