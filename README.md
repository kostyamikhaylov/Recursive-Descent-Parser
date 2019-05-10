# Recursive-Descent-Parser
For my own C-like language


Examples of what it can do:

./a.out {x=2\;}

./a.out {x=2\;new_line=100\;x=3\;}

./a.out {x=2\;if\(1\)s=3\;}

./a.out {x=2\;if\(1\){s=3\;d__=1\;}}

./a.out {x=2\;if\(2^3-8\){s=3\;d__=1\;}}

./a.out {x=2\;if\(p=2^3-9\){s=3\;d__=1\;}}

./a.out {x=2\;if\(p=3*3-9*\(24-23\)\){s=3\;d__=1\;}}



'\\' before brackets and semicolon is optional according OS;

all variables are stored in a string and are not overwritten for now.



Plans for the future:

Add variables declarations, comparisons;

Make it possible to use variables in expressions.

