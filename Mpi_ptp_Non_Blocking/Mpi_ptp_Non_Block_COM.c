#include  <mpi.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc ,char  **argv){

// First and foremost initialize the MPI by passing in the value of main function
MPI_Init(&argc, &argv);

// Lets find the rank of each process; that sounded weird arent rank and process same thing , oh well..
int rank;
MPI_Comm_rank(MPI_COMM_WORLD, &rank);


// Lets assign the message size and the message tag
int N = 10; 
int tag = 999; 
// check if the rank is 0 
if (rank == 0 ) {
    int *outbuffer = (int*) calloc(N, sizeof(int));
    int dest = 1 ;

    for( int i=0 ; i< N ; ++i ){
        outbuffer[i] = i;
    }
    
    // Lets begin the main part NONBLOCKING SEND protocol to rank 1
    MPI_Request outRequest;
    // we are using Isend or immediate send, it doesnot wait for the receive mesg
    MPI_Isend(outbuffer, N,MPI_INT,dest, tag, MPI_COMM_WORLD, &outRequest);

    // Since this ia an immediate type of message send, we need to make sure that we 
    // donot corrupt the data in the out buffer cuz we donot actually know if the data has been 
    // successfully sent or not. We use MPI_Wait for this
    MPI_Status outStatus;
    MPI_Wait(&outRequest, &outStatus);
    }   

if (rank ==1 ) {
    int *inbuffer = (int*) calloc(N,sizeof(int));
    int source = 0;

    // lets initiate the receiving operation 
    MPI_Request inRequest;
    MPI_Irecv(inbuffer, N, MPI_INT,source, tag, MPI_COMM_WORLD, &inRequest);

    // Now lets perform the waiting operation
    MPI_Status inStatus;
    MPI_Wait(&inRequest, &inStatus);
    
    for (int i=0 ; i < N ; ++i) {
        printf(" This is the Received Data: %d \n", inbuffer[i]); 
        }
    }


// as always finalize the process
MPI_Finalize();
return 0;
}