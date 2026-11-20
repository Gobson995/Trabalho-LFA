program exemplo;

var
  total, i: integer;
  media: real;
  ativo: boolean;

function maior(a, b: integer): integer;
begin
  if a > b then
    Result := a
  else
    Result := b;
end;

procedure troca(var p, q: integer);
var
  aux: integer;
begin
  aux := p;
  p := q;
  q := aux;
end;

procedure calcula(n: integer; nome: string);
var
  k: integer;

  function dobro(x: integer): integer;
  begin
    Result := 2 * x;
  end;

begin
  for k := 1 to n do
    total := total + dobro(k);
  for k := n downto 1 do
    if k mod 2 = 0 then
      if k > 10 then total := total - 1 else total := total + 1;
  repeat
    n := n div 2;
  until (n <= 1) or not ativo;
  writeln('resultado de ', nome, ': ', total);
end;

begin
  total := 0;
  ativo := true;
  calcula(10, 'teste');
  media := total / 10.0;
  troca(total, i);
  i := -maior(total, i);
end.
