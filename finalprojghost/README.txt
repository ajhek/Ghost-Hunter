Code by Adyn Hek #101235873
Code is fully finished and should run without memory leaks.
There may be one occasional seg fault that I am unsure if I was able to entirely fix.
The program itself will simulate a game, where the hunters will move throughout a map trying to collect ghost evidence.
The ghost will also move around the map, leaving evidence and scaring hunters.
The hunters will win if they collect enough evidence to identify the ghost, and the ghost will win if all hunters are scared off.
Evidence left by the ghost is of 4 different types, although each ghost type can only leave three types. If hunters collect three types of unique ghostly data, they can identify the ghost and win the game.
If the ghost is in a room with a hunter, the hunter will build up fear points until they leave the room. If they reach max fear, they will flee the property in terror.
Compile with "make" and run with ./final
Hunters and ghosts will make decisions at random.
