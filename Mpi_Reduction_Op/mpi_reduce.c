#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main( int argc , char **argv) {
    MPI_Init(&argc, &argv);

    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int number_of_Messages = 1;
    int *msgBuffer = (int*) malloc(number_of_Messages*sizeof(int));
    msgBuffer[0] = rank;
    

    // perform the reduction operation
    int *msgReduced = (int*) malloc(number_of_Messages*sizeof(int)); 
    int msgRoot = 0;

    // Reduce function in MPI
    MPI_Reduce(msgBuffer, msgReduced, number_of_Messages, MPI_INT, MPI_SUM, msgRoot, MPI_COMM_WORLD);

    if (rank == msgRoot) {
        printf("The reduced sum of each rank index is %d\n", msgReduced[0]);
    }

    free(msgBuffer);
    free(msgReduced);

    MPI_Finalize();
    return 0;
}