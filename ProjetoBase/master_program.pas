program exemplo12 (input, output);
  var x : integer;
  procedure p (var t : integer);
    label 100, 200;
    var s : integer;
    function f(z : integer): integer;
    begin (* f *)
      if z<0 then goto 200
        else if z=0 then f:=2
          else f:=f(z-2) 1 *z+1
    end; (* f *)
  begin (* p *)
    100: s:=f(t) 2 ; t:=s;
    if t<x then goto 100;
    200: x:=x-1
  end; (* p *)
  procedure r;
    procedure q;
      var y : integer;
      begin read(y); p(y) 3 ; write(y) end; (* q *)
    begin q 4 end; (* r *)
  begin read(x); r 5 end.
