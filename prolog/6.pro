insert(X, L, [X | L]).
insert(X, [H | T], [H | Z]) :- insert(X, T, Z).

perm([], []).
perm([H | T], B) :- perm(T, B1), insert(H, B1, B).
