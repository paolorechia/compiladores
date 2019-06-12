program proc1 (input, output);
  var x, y: integer;
  var m: boolean;
  procedure p;
    var z, w, k: integer;
    procedure q;
      var u: integer;
      begin
        read(u);
        write(u);
      end;
    begin
      x:= x - 1;
      if (x > 1) then
        p;
      y:= y * z;
    end;
begin
  read(x);
  p;
  write(x, y);
end.
