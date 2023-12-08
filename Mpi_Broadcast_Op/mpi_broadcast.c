#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc , char **argv){

MPI_Init(& argc, &argv);

int rank, size;
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
// Number of Messages you want to send
int Num_of_msg = 1;
// Alot a space to stire the output 
int *msgOut = (int*) malloc(Num_of_msg*sizeof(int));
// we define 0 to be the root or main source
int msgRoot = 0;

// lets define same value
if (rank == msgRoot) {
    msgOut[0] = 123;
}

// Broad Cast the message: 0 will send to 1, 1 sends to 3 while 0 send to 2, 2 send to 6 while 0 
// send to 4 and 1 send to 5 while 3 is also sending to 7. I know it is not clear draw a 
// tree on a paper to visualize it. But MPI handles all this internally all you need to know
// The following line Broad Casts the message from root to all other processes
MPI_Bcast(msgOut, Num_of_msg,MPI_INT, msgRoot,MPI_COMM_WORLD);

// Print the received message
    // Use index 0 as we only have one message
    printf("Yes, I am rank : %d and I have received the message which is: %d \n", rank, msgOut[0]);

// Free allocated memory and finalize MPI
free(msgOut);
MPI_Finalize();
}