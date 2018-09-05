zip([], [], []).
zip([X], [X], []).
zip([X, Y | T], [X | T1], [Y | T2]) :-
    zip(T, T1, T2).
