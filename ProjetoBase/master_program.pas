program exemplo12 (input, output);
  var x : integer;
  procedure p (var t : integer);
    label 100, 200;
    var s : integer;
    function f(z : integer): integer;
    begin (* f *)
      if z<0 then goto 200
        else if z=0 then f:=2
          else f:=f(z-2) *z+1
    end; (* f *)
  begin (* p *)
    100: s:=f(t) ; t:=s;
    if t<x then goto 100;
    200: x:=x-1
  end; (* p *)
  procedure r;
    procedure q;
      var y : integer;
      begin read(y); p(y) ; write(y) end; (* q *)
    begin q end; (* r *)
  begin read(x); r end.
