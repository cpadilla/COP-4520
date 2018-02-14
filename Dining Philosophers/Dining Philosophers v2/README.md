Christofer Padilla
COP 4250
Dining Philosophers version 2

## Compiling

Make sure you have g++ installed on your system. Run the following command in your terminal:

```
g++ dp2.cpp
```

## Running the program

After compiling the program, an executable should be created (i.e. "a.exe"). Run this program from the command line.

## Proof of correctness

This version of the Dining Philosopher problem ensures that it never reaches a state where philosophers are deadlocked, that is, it is never the case that each philospher holds one chopstick and is stuck waiting for another to get the second chopstick. This is done by attempting to pick up the left chopstick first. If a philosopher can't, then he tries again later. If he picks up the left one, then he attempts to pick up the right chopstick. If he can, he eats. Otherwise, he puts down the left chopstick and then tries again later. This prevents any thread from locking a resource by giving it up and trying again later.
