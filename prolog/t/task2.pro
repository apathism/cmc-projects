% SECOND PRACTICE TASK
% Koryabkin Ivan, CMC MSU, 2012

% range (Left, Right, List)
%   List of integer elements from Left to Right.
range(X, X, [X]).
range(L, R, [L | Range]) :-
	L < R,
	NewL is L+1,
	range(NewL, R, Range).

% is_valid_edge (Edge, N, M)
%   Checks whiether the edge is valid for N x M maze
%   (in assumption "edge is in ranges")
is_valid_edge([0, _, horisontal], _, _) :- !, fail.
is_valid_edge([_, 0, vertical], _, _) :- !, fail.
is_valid_edge([N, _, _], N, _) :- !, fail.
is_valid_edge([_, M, _], _, M) :- !, fail.
is_valid_edge([_, _, _], _, _).

% build_edges_list (N, M, List)
%   Builds list of edges for N x M maze
build_edges_list(N, M, List) :-
	findall(Edge, build_edge(N, M, Edge), List).
build_edge(N, M, [X, Y, P]) :-
	range(0, N, XR),
	range(0, M, YR),
	member(P, [vertical, horisontal]),
	member(X, XR),
	member(Y, YR),
	is_valid_edge([X, Y, P], N, M).

% build_disjoint_table (N, M, Table)
%    Builds system of disjoint sets in a table
build_disjoint_table(N, M, T) :- build_disjoint_table(N, M, 1, T).
build_disjoint_table(0, _, _, []).
build_disjoint_table(N, M, C, [R | T]) :-
	N > 0,
	NextRow is C + M,
	EndRow is NextRow - 1,
	range(C, EndRow, R),
	DecN is N-1,
	build_disjoint_table(DecN, M, NextRow, T).

% member (Element, Position, List)
%   Checks whiether Element is a memeber of the List on Position
member(E, 1, [E | _]).
member(E, P, [_ | T]) :- member(E, PX, T), P is PX + 1.
member(E, X, Y, T) :-
	member(R, X, T),
	member(E, Y, R).

% replace (OldElement, NewElement, OldList, NewList)
%   Replaces OldElement with NewElement in List
replace(_, _, [], []).
replace(E1, E2, [E1 | T1], [E2 | T2]) :- !, replace(E1, E2, T1, T2).
replace(E1, E2, [E | T1], [E | T2]) :- replace(E1, E2, T1, T2).
replace2d(_, _, [], []).
replace2d(E1, E2, [H1 | T1], [H2 | T2]) :-
	replace(E1, E2, H1, H2),
	replace2d(E1, E2, T1, T2).

% random_shuffle(List, NewList)
%   Shuffles the list
random_shuffle([], []).
random_shuffle([H | T], NewList) :- random_shuffle(T, 1, [H], NewList).
random_shuffle([], _, Result, Result).
random_shuffle([H | T], Length, List, Result) :-
	NewLength is Length + 1,
	random(0, NewLength, Position),
	insert(List, H, Position, NewList),
	random_shuffle(T, NewLength, NewList, Result).
insert(List, Element, 0, [Element | List]).
insert([H | T], Element, Position, [H | Result]) :-
	Position > 0,
	NewPosition is Position - 1,
	insert(T, Element, NewPosition, Result).

% add_edge(Table, Maze, Edge, ResultTable, ResultMaze)
%   Adds edge to disjoint table
add_edge(T, M, E, T, [E | M]) :-
	build_vertexes(E, X1, Y1, X2, Y2),
	member(C1, X1, Y1, T),
	member(C1, X2, Y2, T),
	!.
add_edge(T, M, E, RT, M) :-
	build_vertexes(E, X1, Y1, X2, Y2),
	member(C1, X1, Y1, T),
	member(C2, X2, Y2, T),
	replace2d(C1, C2, T, RT).
build_vertexes([X, Y, vertical], NX, Y, NX, NY) :- NX is X + 1, NY is Y + 1.
build_vertexes([X, Y, horisontal], X, NY, NX, NY) :- NX is X + 1, NY is Y + 1.

% kruskal (N, M, Maze)
%   Builds the Maze as a list of edges using kruskal algorithm
kruskal(N, M, Maze) :-
	build_disjoint_table(N, M, Disjoint),
	build_edges_list(N, M, TempEdges),
	random_shuffle(TempEdges, Edges),
	kruskal([], Disjoint, Edges, Maze).
kruskal(Maze, _, [], Maze).
kruskal(Maze, Disjoint, [Edge | Edges], Result) :-
	add_edge(Disjoint, Maze, Edge, NewDisjoint, NewMaze),
	kruskal(NewMaze, NewDisjoint, Edges, Result).

% can_move(N, M, Maze, From, To)
%   Checks the availability of the move from From to To for DFS
can_move(_, _, _, _, [X, _]) :- X < 1, !, fail.
can_move(_, _, _, _, [_, Y]) :- Y < 1, !, fail.
can_move(N, _, _, _, [X, _]) :- X > N, !, fail.
can_move(_, M, _, _, [_, Y]) :- Y > M, !, fail.
can_move(_, _, Maze, [X, Y1], [X, Y2]) :-
	Y2 is Y1 + 1,
	XF is X - 1,
	(
		(member([XF, Y1, vertical], Maze), !, fail);
		true
	).
can_move(_, _, Maze, [X, Y1], [X, Y2]) :-
	Y2 is Y1 - 1,
	XF is X - 1,
	(
		(member([XF, Y2, vertical], Maze), !, fail);
		true
	).
can_move(_, _, Maze, [X1, Y], [X2, Y]) :-
	X2 is X1 + 1,
	YF is Y - 1,
	(
		(member([X1, YF, horisontal], Maze), !, fail);
		true
	).
can_move(_, _, Maze, [X1, Y], [X2, Y]) :-
	X2 is X1 - 1,
	YF is Y - 1,
	(
		(member([X2, YF, horisontal], Maze), !, fail);
		true
	).

% get_path(N, M, Maze, First, Last, Path)
%   Gets path in the constucted maze N x M
get_path(N, M, Maze, First, Last, Path) :- get_path(N, M, Maze, First, [-1, -1], Last, [], Path).
get_path(_, _, _, Current, _, Current, Path, [Current | Path]).
get_path(N, M, Maze, [CX, CY], [PX, PY], Last, Path, Answer) :-
	U is CX - 1,
	D is CX + 1,
	L is CY - 1,
	R is CY + 1,
	member([NX, NY], [[CX, L], [U, CY], [CX, R], [D, CY]]),
	[NX, NY] \= [PX, PY],
	can_move(N, M, Maze, [CX, CY], [NX, NY]),
	get_path(N, M, Maze, [NX, NY], [CX, CY], Last, [[CX, CY] | Path], Answer).
get_path_holes(N, M, Maze, Enterance, Finish, Path) :-
	NE is Enterance + 1,
	NF is Finish + 1,
	get_path(N, M, Maze, [NE, 1], [NF, M], Path).

% LET'S THE GRAPHICS BEGIN! >:)

% select_maze_size(N, M)
%   Prompts the size of the maze
select_maze_size(N, M) :-
	new(Dialog, dialog('Select maze size')),
	new(Height, int_item('Height', 10, low:=3, high:=30)),
	new(Width, int_item('Width', 10, low:=3, high:=30)),
	new(OkButton, button('OK', message(Dialog, return, 'OK'))),
	new(CancelButton, button('Cancel', message(Dialog, return, 'Cancel'))),
	send(Dialog, append, Height),
	send(Dialog, append, Width, next_row),
	send(Dialog, append, OkButton, next_row),
	send(Dialog, append, CancelButton, right),
	send(Dialog, default_button(OkButton)),
	get(Dialog, confirm, Value),
	(
		(
			get(Width, selection, N),
			get(Height, selection, M),
			!
		);
		true
	),
	free(Dialog),
	free(Height),
	free(Width),
	free(OkButton),
	free(CancelButton),
	atomic(N),
	atomic(M),
	Value = 'OK'.

% get_pixels_per_cell(Constant)
%   Fixed value for pixels per cell
get_pixels_per_cell(20).

% draw_maze_vertexes(Picture, Maze)
%   Draws the maze (all the vertexes and edges)
draw_maze_vertexes(_, []).
draw_maze_vertexes(Picture, [[X, Y, horisontal] | Maze]) :-
	get_pixels_per_cell(PPC),
	PX is X * PPC + PPC,
	PY1 is Y * PPC + PPC,
	PY2 is PY1 + PPC,
	send(Picture, display, line(start_y:=PX, start_x:=PY1, end_y:=PX, end_x:=PY2)),
	draw_maze_vertexes(Picture, Maze).
draw_maze_vertexes(Picture, [[X, Y, vertical] | Maze]) :-
	get_pixels_per_cell(PPC),
	PY is Y * PPC + PPC,
	PX1 is X * PPC + PPC,
	PX2 is PX1 + PPC,
	send(Picture, display, line(start_y:=PX1, start_x:=PY, end_y:=PX2, end_x:=PY)),
	draw_maze_vertexes(Picture, Maze).
draw_maze_borders(Picture, Enterance, Finish, N, M) :-
	get_pixels_per_cell(PPC),
	PX1 is PPC,
	PX2 is PPC * N + PX1,
	PY1 is PX1,
	PY2 is PPC * M + PY1,
	E1 is PX1 + Enterance * PPC,
	E2 is PX1 + (Enterance + 1) * PPC,
	F1 is PX1 + Finish * PPC,
	F2 is PX1 + (Finish + 1) * PPC,
	send(Picture, display, line(start_y:=PX1, start_x:=PY1, end_y:=PX1, end_x:=PY2)),
	send(Picture, display, line(start_y:=PX2, start_x:=PY1, end_y:=PX2, end_x:=PY2)),
	send(Picture, display, line(start_y:=PX1, start_x:=PY1, end_y:=E1, end_x:=PY1)),
	send(Picture, display, line(start_y:=E2, start_x:=PY1, end_y:=PX2, end_x:=PY1)),
	send(Picture, display, line(start_y:=PX1, start_x:=PY2, end_y:=F1, end_x:=PY2)),
	send(Picture, display, line(start_y:=F2, start_x:=PY2, end_y:=PX2, end_x:=PY2)).

% show_solve(Picture, SolvePath)
%   Show solution for the given maze, just event handler
show_solve(Picture, [[X, Y] | Path]) :-
	get_pixels_per_cell(PPC),
	PX is X * PPC + PPC / 2,
	PY1 is Y * PPC + PPC / 2,
	PY2 is PY1 + PPC,
	new(Line, line(start_y:=PX, start_x:=PY1, end_y:=PX, end_x:=PY2)),
	send(Line, colour, colour(red)),
	send(Picture, display, Line),
	show_solve_tail(Picture, [[X, Y] | Path]).
show_solve_tail(Picture, [[X, Y]]) :-
	get_pixels_per_cell(PPC),
	PX is X * PPC + PPC / 2,
	PY is Y * PPC + PPC / 2,
	new(Line, line(start_y:=PX, start_x:=PPC / 2, end_y:=PX, end_x:=PY)),
	send(Line, colour, colour(red)),
	send(Picture, display, Line).
show_solve_tail(Picture, [[X1, Y1], [X2, Y2] | Tail]) :-
	get_pixels_per_cell(PPC),
	PX1 is X1 * PPC + PPC / 2,
	PX2 is X2 * PPC + PPC / 2,
	PY1 is Y1 * PPC + PPC / 2,
	PY2 is Y2 * PPC + PPC / 2,
	new(Line, line(start_y:=PX1, start_x:=PY1, end_y:=PX2, end_x:=PY2)),
	send(Line, colour, colour(red)),
	send(Picture, display, Line),
	show_solve_tail(Picture, [[X2, Y2] | Tail]).

% draw_maze(N, M, Maze, Enterance, Finish, Path)
%   Draws maze window with all buttons
draw_maze(N, M, Maze, Enterance, Finish, Path) :-
	new(Dialog, dialog('Prolog Maze Generator')),
	get_pixels_per_cell(PPC),
	SY is (N + 2) * PPC,
	SX is (M + 2) * PPC,
	new(Picture, picture(size := size(SX, SY))),
	new(GenerateButton, button('Generate another one', message(Dialog, return, 'Generate'))),
	new(SolveButton, button('Solve', message(@prolog, show_solve, Picture, prolog(Path)))),
	new(ExitButton, button('Exit', message(Dialog, return, 'Exit'))),
	draw_maze_vertexes(Picture, Maze),
	draw_maze_borders(Picture, Enterance, Finish, N, M),
	send(Dialog, append, Picture),
	send(Dialog, append, GenerateButton, next_row),
	send(Dialog, append, SolveButton, right),
	send(Dialog, append, ExitButton, right),
	send(Picture, scrollbars, none),
	send(Dialog, open),
	get(Dialog, confirm, Value),
	free(Dialog),
	free(Picture),
	free(GenerateButton),
	free(SolveButton),
	free(ExitButton),
	Value = 'Generate'.

% main
%    Main predicate. Nothing to say :3
main :-
	select_maze_size(M, N),
	kruskal(N, M, Maze),
	random(0, N, Enterance),
	random(0, N, Finish),
	get_path_holes(N, M, Maze, Enterance, Finish, Path),
	draw_maze(N, M, Maze, Enterance, Finish, Path),
	main.
