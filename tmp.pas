program quicksort(input, output);
var 
	array1, array2 : array[1..20] of integer;
	tmp1 : real;
	tmp2 : real;
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
begin
	tmp1 := 1;
	tmp2 := 2;
	abs(s, t)
end