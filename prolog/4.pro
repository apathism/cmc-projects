length1([], 0).
length1([_ | T], L) :-
   length1(T, Lx), L is Lx + 1.
