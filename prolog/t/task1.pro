% Graph (please, modify this code for your case)
vertex(a).
vertex(b).
vertex(c).
vertex(d).
vertex(e).
edge(a, b, 3).
edge(a, c, 8).
edge(b, d, 0).
edge(c, d, 12).
edge(d, e, 9).

% Two-side edge
edge2(X, Y, C) :- edge(X, Y, C); edge(Y, X, C).

% Element X outside the set L
outside(X, L) :- member(X, L), !, fail.
outside(_, _).

% Search for simple path L from X to Y
path(X, Y, [X | L]) :- path(X, Y, [X], L).
path(X, X, _, []).
path(X, Y, L1, [P | L2]) :- edge2(X, P, _), outside(P, L1), path(P, Y, [P | L1], L2).

% Get all simple paths (multiple depth-first-search calls)
get_paths(X, Y, Z) :- findall(L, path(X, Y, L), Z).

% Count path cost
path_cost([], 0).
path_cost([_], 0).
path_cost([X, Y | L], C) :- edge2(X, Y, C1), path_cost([Y | L], C2), C is C1 + C2.

% Minimal simple path from X to Y
min_path(X, Y, L) :- get_paths(X, Y, Z), get_min_dist(Z, D), member(L, Z), path_cost(L, D).
get_min_dist([LE | LL], LA) :- path_cost(LE, CE), get_min_dist(LL, CE, LA).
get_min_dist([], C, C).
get_min_dist([LE | LL], CM, LA) :- path_cost(LE, CE), CE < CM, !, get_min_dist(LL, CE, LA).
get_min_dist([_ | LL], CM, LA) :- get_min_dist(LL, CM, LA).

% Add one vertex set to another
add_set([], X, X).
add_set([[X, _] | L], Y, Z) :- member([X, _], Y), !, add_set(L, Y, Z).
add_set([X | L], Y, Z) :- add_set(L, [X | Y], Z).

% Expand vertex set in breadth-first-search way
expand_vertex_set(SA, SB) :- expand_vertex_set(SA, SA, SB).
expand_vertex_set([], X, X).
expand_vertex_set([[V, Q] | SS], DS, AS) :- NQ is Q+1, findall([U, NQ], edge2(V, U, _), UL), add_set(UL, DS, NDS), expand_vertex_set(SS, NDS, AS).

% Breadth-first-search
bfs(V, S) :- bfs_state([[V, 0]], S).
bfs_state(S, S) :- expand_vertex_set(S, NS), S = NS, !.
bfs_state(S, ST) :- expand_vertex_set(S, NS), bfs_state(NS, ST).

% Get shortest path by BFS statement
bfs_state_get_path(S, I, F, F, [F]) :- !, member([F, I], S).
bfs_state_get_path(S, I, C, F, [C | P]) :- J is I+1, member([X, J], S), edge2(C, X, _), bfs_state_get_path(S, J, X, F, P).

% Get shortest path
short_path(X, Y, P) :- bfs(X, S), bfs_state_get_path(S, 0, X, Y, P).

% Insert value into sorted list on its place
inplace_insert([], X, [X]).
inplace_insert([H | L], X, [H | R]) :- H < X, !, inplace_insert(L, X, R).
inplace_insert([H | L], X, [X, H | L]).

% Sort
mysort([], R, R).
mysort([H | T], R, X) :- inplace_insert(X, H, NX), mysort(T, R, NX).
mysort(L1, L2) :- mysort(L1, L2, []).

% Checks if first set is a subset of the second set
subset([], _).
subset([H | T], L) :- member(H, L), !, subset(T, L).

% Checks if all vertixes are in set
all_vertixes_in_list(L) :- findall(X, vertex(X), V) , subset(V, L).

% Convert BFS state to the list of vertixes
convert_bfs_state_to_list([], []).
convert_bfs_state_to_list([[V, _] | S], [V | L]) :- convert_bfs_state_to_list(S, L).

% Check whiether graph is connected
is_connected :- vertex(X), !, bfs(X, S), convert_bfs_state_to_list(S, L), all_vertixes_in_list(L).

% Graph contains at least one cycle
cyclic :- edge(X, Y, _), get_paths(X, Y, L), L = [_,_|_], !.
