This implements a simple pipeline.

Code starts at address 0.
Each instruction is 4 byte
Data starts at address 1024*1024
Data is laid out in little endian form
Only integer register
All operations are on integer
32 bit registers
R0 always 0

Output:
Contents of each register, in sorted order
Contents of each address accessed in the program, in sorted order

Simulate whole program or first 1000 clock cycles whichever happens first
