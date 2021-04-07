#include <bits/stdc++.h>
#include <mpi.h> 

using namespace std;

#define MASTER_TO_SLAVE_TAG 1 //tag for messages sent from master to slaves
#define SLAVE_TO_MASTER_TAG 4 //tag for messages sent from slaves to master


int main(int argc, char** argv){
    int size = 100; 
    MPI_Status status;
    MPI_Request request;
    int taskid,numTasks,offset,i,j,k,lowerBound,upperBound,portion;
    float A[size][size] , B[size][size],C[size][size];
    double mSecs;
    // Creating Matrices
    for (int i = 0 ; i < size; i++){
        for (int j = 0 ; j < size; j++){
            A[i][j] = ((float) rand()) / (float) RAND_MAX ;
            B[i][j] = ((float) rand()) / (float) RAND_MAX ;
            C[i][j] = 0 ;
        }
    }
    auto s = chrono::steady_clock::now();
    MPI_Init( &argc , &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
    
    if(taskid==0){
        for (i = 1; i < numTasks; i++){
            portion = size / (numTasks-1);
            lowerBound = (i-1)*portion;
            if ((i + 1 == numTasks) && (size % (numTasks-1) != 0))
                upperBound = size ;
            else 
                upperBound = lowerBound + portion;
            MPI_Isend(&lowerBound, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD, &request);
            MPI_Isend(&upperBound, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG + 1, MPI_COMM_WORLD, &request);
            MPI_Isend(&A[lowerBound][0], (upperBound - lowerBound) * size, MPI_FLOAT, i, MASTER_TO_SLAVE_TAG + 2, MPI_COMM_WORLD, &request);
        }
    }
    MPI_Bcast(&B, size*size, MPI_FLOAT, 0, MPI_COMM_WORLD);
    if (taskid > 0) {
        MPI_Recv(&lowerBound, 1, MPI_INT, 0, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&upperBound, 1, MPI_INT, 0, MASTER_TO_SLAVE_TAG + 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&A[lowerBound][0], (upperBound - lowerBound) * size, MPI_FLOAT, 0, MASTER_TO_SLAVE_TAG + 2, MPI_COMM_WORLD, &status);
        for (i = lowerBound; i < upperBound; i++) {
            for (j = 0; j < size; j++) {
                for (k = 0; k < size; k++) { 
                    C[i][j] =  C[i][j] +  A[i][k] * B[k][j];
                }
            }
        }
        MPI_Isend(&lowerBound, 1, MPI_INT, 0, SLAVE_TO_MASTER_TAG, MPI_COMM_WORLD, &request);
        MPI_Isend(&upperBound, 1, MPI_INT, 0, SLAVE_TO_MASTER_TAG + 1, MPI_COMM_WORLD, &request);
        MPI_Isend(&C[lowerBound][0], (upperBound - lowerBound) * size, MPI_FLOAT, 0, SLAVE_TO_MASTER_TAG + 2, MPI_COMM_WORLD, &request);
    }
    if (taskid == 0) {
        for (i = 1; i < numTasks; i++) {
            MPI_Recv(&lowerBound, 1, MPI_INT, i, SLAVE_TO_MASTER_TAG, MPI_COMM_WORLD, &status);
            MPI_Recv(&upperBound, 1, MPI_INT, i, SLAVE_TO_MASTER_TAG + 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&C[lowerBound][0], (upperBound - lowerBound) * size, MPI_FLOAT, i, SLAVE_TO_MASTER_TAG + 2, MPI_COMM_WORLD, &status);
        }
    }
    
    MPI_Finalize();
    auto e = chrono::steady_clock::now();
    auto diff = e - s;
    mSecs =chrono::duration <double, milli> (diff).count();
    cout << "Time Taken by Task " << taskid << " = " << mSecs << endl;
    return 0;
}