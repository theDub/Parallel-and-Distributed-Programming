To compile:

	Preconditions:
		1. Input file exists

		To create this, read the top of the matrixgen.c file
		and follow the instructions.
		
	Compiling and running main program:
	
		$ gcc -g -Wall -o main main.c lab1_IO.c -lpthread
		$ ./main [insert number of threads here]
		
		For example (post compilation):
		$ ./main 4 
		
		This command will run the main program with 4 threads.
		Make sure that the number of threads is a square number
		and also ensure that it is divisible by n^2, where n^2
		is the n X n array created and stored in the input file.
		
		*note that main depends on lab1_IO.c, lab1_IO.h and timer.here
		