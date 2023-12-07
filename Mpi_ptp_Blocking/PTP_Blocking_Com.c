/* This Code Illustrates Basic Point to Point Communication between Two Rank Using MPI*/
// Some Essential Libraries to be used 
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    /* Always First initialize MPI Lib com systems*/
    MPI_Init(&argc, &argv);

    /*Find the index of the current operation*/
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Defining the size of the Message
    int N =10;
    // Defining the tag of the message, it is like a identity of the message 
    int n,tag = 999;

    // Suppose a rank '0'  is sending the message  to rank '1'
    if (rank == 0) {
        //  assign a region in heap for rank 0 of size 10 
        int *outBuffer = (int*) calloc( N , sizeof(int));
        //  Define the rank number for the receiving rank 
        int dest = 1;
        
        //  Storing some values in the Buffer which is to be sent to rank 1
        for (n=0 ; n<N; ++n){
            outBuffer[n] = n;
        }
        // MPI_SEND( BUFFER_NAME , AMOUNT OF DATA TO BE SENT , TYPE OF DATA BEING SENT ,  DESTINATION LOCATION , TAG OF THE DATA (Assigned before), THE WORLD/COLLECTION OF RANKS AMONGST WHICH COMMUNICATION TAKES PLACE)
        MPI_Send(outBuffer , N , MPI_INT, dest , tag, MPI_COMM_WORLD);
    }

    // Rank 1 will receive messages from RANK 0
    if(rank==1) { 
        // initiating a status variable defined by MPI_Status , used by MPI_Recv to ensure if the data is recieved
        MPI_Status status;

        // Define a Buffer Memory to store the data from the rank 0 
        int *inBuffer =  (int*) calloc( N ,  sizeof(int)); 
        // Define the source from which the data stream will be coming from 
        int source =0;
        // MPI_SEND( BUFFER_NAME , TYPE OF DATA RECEIVED ,  SOURCE LOCATION , TAG OF THE DATA (Assigned before), THE WORLD/COLLECTION OF RANKS AMONGST WHICH COMMUNICATION TAKES PLACE , STATUS OF THE RECEIVED MESSAGE)
        MPI_Recv(inBuffer,N , MPI_INT, source, tag, MPI_COMM_WORLD, &status);
        
        // DISPLAY THE RECEIVED MESSAGE
        for( n = 0 ; n<N ; ++n){
            printf("Received: inBuffer[%d]=%d \n",n,inBuffer[n]);
        }
    }

    // Shut down the MPI com, THIS IS VERY IMP
    MPI_Finalize();

    /*return as usual*/
    return 0;
}


// ----------------------- IGNORE THIS ------------------------------ //
    /*Find the total number of processes running in the world*/
    // int size;
    // MPI_Comm_size(MPI_COMM_WORLD, &size);

    // if( (rank%2 == 0)) {
    //     printf("MPI hello world from even process %d ! \n", rank);
    // }
    // else{
    //     printf("MPI hello world from odd process %d ! \n", rank);    
    // }
    /*Each rank print hello world message with rank and size info*/
    //printf("MPI hello world from %d of %d! \n", rank, size);

    /*Each rank will print the Hello message now*/
    // printf("HELLO TO THE WORLD OF MPI !\n");

    /*ALL Ranks must shutdown MPI Library Comms*/