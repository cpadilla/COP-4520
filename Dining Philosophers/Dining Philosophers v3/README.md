Christofer Padilla
COP 4250
Dining Philosophers version 3

## Compiling

Make sure you have g++ installed on your system. Run the following command in your terminal:

```
g++ dp3.cpp
```

## Running the program

After compiling the program, an executable should be created (i.e. "a.exe"). Run this program from the command line.

## Proof of correctness

This version of the Dining Philosopher problem avoids starvation by using assymetric acquirement; even philosophers starting with the right chopstick, odd philosophers starting with the left chopstick. This solution is not completely fair, but it does ensure that no philosopher will starve.
