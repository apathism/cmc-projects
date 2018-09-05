gcd(0, X, X) :- !.
gcd(X, 0, X) :- !.
gcd(X, Y, R) :- !, Xn is mod(X, Y),
                   gcd(Y, Xn, R).
