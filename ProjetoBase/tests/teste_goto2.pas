program exemplo12 (input, output);
  var x : integer;
  procedure p (var t : integer);
    label 100, 200;
    var s : integer;
  begin (* p *)
    100: s:=t; t:=s;
    if t<x then goto 100;
    200: x:=x-1;
    if x > 5 then goto 200
    else x = 0;
  end; (* p *)
  begin read(x); p(x) end.
