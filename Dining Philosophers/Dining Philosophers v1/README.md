Christofer Padilla
COP 4250
Dining Philosophers version 1

## Compiling

Make sure you have g++ installed on your system. Run the following command in your terminal:

```
g++ dp1.cpp
```

## Running the program

After compiling the program, an executable should be created (i.e. "a.exe"). Run this program from the command line.

## Proof of correctness

This version of the Dining Philosopher problem ensures that no two philosophers hold the same chopstick at the same time by using a mutex to lock a chopstick as soon as a philosopher picks it up. Each philosopher picks up the left chopstick, then the right chopstick to eat. This might cause a deadlock when all left or all right chopsticks are picked up by all philosophers at the same time.
