parent(pam, bob).
parent(tom, bob).
parent(tom, liz).
parent(bob, ann).
parent(bob, pat).
parent(pat, jim).

female(pam).
male(bob).
male(tom).
female(liz).
female(ann).
female(pat).
male(jim).

grandfather(X, Y) :-
    male(X), parent(X, Z), parent(Z, Y).

