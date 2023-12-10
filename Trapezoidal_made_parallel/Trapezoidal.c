#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Forward Declaring the function we want to integrate 
double f(double x);


int main( int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number_of_segments>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]); // Number of segments
    // define lower bound of integral
    int a = 0; 
    // define the upper bound of integral
    int b = 20;
    // caluclate the step size
    double dx = (double)(b - a) / n;

    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
        // Check if n is divisible by size
    if (n % size != 0) {
        if (rank ==0) {
            fprintf(stderr, "The number of segments (%d) must be divisible by the number of processes (%d).\n", n, size);
        }
        MPI_Finalize();
        return 1;
    }
    int Np = n / size; // Number of segments per processor
    int a_local = a + (dx * rank * Np);
    double *loc_sol_buffer = (double*) calloc(1, sizeof(double));

    if (rank == 0 ) {
        double sum = 0.0;
        sum += f(a_local);
        for (int i = 1; i<=Np-1 ; ++i) {
            sum += 2*f(a_local + (i*dx));
        }
        loc_sol_buffer[0] = sum;
    }

    if (rank == size-1) {
        double sum = 0.0;
        sum += f(b);
        for (int i = 0; i<=Np-1 ; ++i) {
            sum += 2*f(a_local + (i*dx));
        }
        loc_sol_buffer[0] = sum;
    }
    
    else {
        double sum = 0.0;
        for (int i = 0; i<=Np-1 ; ++i) {
            sum += 2*f(a_local + (i*dx));
        }
        if( rank != size-1) {
            sum += f(a_local + (Np * dx)); 
        }
        loc_sol_buffer[0] = sum;
    }
    // Now we should perform reduction op
    // first declare region and rank where we must store the reduction result 
    double *sumRed  = (double*) calloc(1, sizeof(double));
    int msgroot = 0;

    MPI_Reduce(loc_sol_buffer, sumRed, 1, MPI_DOUBLE, MPI_SUM, msgroot, MPI_COMM_WORLD);

    if (rank == msgroot) {
        sumRed[0] = sumRed[0] * (dx/2); 
        printf("The Integral is %lf\n", sumRed[0]);
    }

    free(loc_sol_buffer);
    free(sumRed);

    MPI_Finalize();
    
    return 0;
}


double f(double x){
    // it is a simple function f(x) = x
    return pow(x,4)+pow(x,3)+pow(x,2)+pow(x,1);
}

