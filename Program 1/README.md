Christofer Padilla
COP 4250
Program 1 Part 1

## Compiling

Make sure you have g++ installed on your system. Run the following command in your terminal:

```
g++ main.cpp
```

## Running the program

After compiling the program, an executable should be created (i.e. "a.exe"). Run this program from the command line. Once it's completed, it will write it's output to "primes.txt".

## Proof of correctness

In order to divide the work of computing primes evenly among multiple threads, each thread is assigned one integer at a time. When a thread is finished with testing an integer, it asks for another. The number is atomic, ensuring that each increment happens atomically so that no threads ever get the same number. This ensures mutual exclusion. The program uses a simple primality test to determine if a number is prime.
