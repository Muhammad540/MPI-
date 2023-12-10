#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){

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

    MPI_Barrier(MPI_COMM_WORLD);
    
    // Reduce function in MPI
    MPI_Reduce(msgBuffer, msgReduced, number_of_Messages, MPI_INT, MPI_SUM, msgRoot, MPI_COMM_WORLD);
    // Broadcast the reduced message to all 
    MPI_Bcast(msgReduced,number_of_Messages,MPI_INT, msgRoot,MPI_COMM_WORLD);

    printf("Yes, I am rank : %d and I have received the message which is: %d \n", rank, msgReduced[0]);

    free(msgBuffer);
    free(msgReduced);

    MPI_Finalize();

    return 0;
}