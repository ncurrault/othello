DROPTABLETeamNames
=======

Nicholas did the original min-max algorithm (in the first week), and the bitboard
implementation (in the second week).

Irene did the original heuristic (in the first week), and the alpha-beta pruning
implementation (in the second week).

We also both worked together to fix bugs as needed, and improved the heuristic.

Some of the improvements we made were tuning our heuristic to focus on gaining
stable cells during the midgame. We also added a special case for the endgame to
check for a simple "has more cells" heuristic. Overall, we focused on a good
heuristic and being able to go to a high depth of the search tree by optimizing
through the bitboard and pruning. We think this strategy will work because our
AI did well in the mock tournament with the heuristic, and it is doing well in
tests against the example bots.
