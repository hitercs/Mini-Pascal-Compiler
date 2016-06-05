program quicksort(input, output);
var 
	array1, array2 : array[1..20] of integer;
	tmp1 : real;
	tmp2 : integer;
function abs(t : real; s : real) : real;
var 
	result : real;
begin 
	if (s > t)
	then
		result := s - t
	else
		result := t - s
end;
procedure test (  i : integer) ;
var 
	sum : integer;
begin
	sum := 1;
	while ( i < 5 )
	do
		sum  :=  sum + i
end;
begin
	tmp1 := 1;
	tmp2 := 2;
	abs(tmp1, tmp2);
	test ( 5 )
end