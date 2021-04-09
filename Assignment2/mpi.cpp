#include <bits/stdc++.h>
#include <mpi.h> 
using namespace std;




#define size 10


void print_matrix(float matrix[size][size]){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

void gaussian(float X[size][size]){
    float r;
    for (int i=0;i<size;i++){
        for(int j=i+1;j<size;j++){
            r = X[j][i]/X[i][i];
            for(int k=0 ; k<size; k++){
                X[j][k] = X[j][k] - X[i][k]*r;  
            }
        }
    }
}

int main(int argc, char** argv){

    std::cout << std::fixed;
    std::cout << std::setprecision(3);
    MPI_Status status;
    MPI_Request request;
    int taskid , numTasks;
    float mp;
    int p;
    int offset,i,j,k,lowerBound,upperBound,portion;
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
        cout << "Size : "<<size<<endl<<endl;
        cout<<"Number of processes : " <<numTasks<<endl<<endl;
        for (i = 1; i < numTasks; i++){
            portion = size / (numTasks-1);
            lowerBound = (i-1)*portion;
            if ((i + 1 == numTasks) && (size % (numTasks-1) != 0))
                upperBound = size ;
            else 
                upperBound = lowerBound + portion;
            MPI_Isend(&lowerBound, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request);
            MPI_Isend(&upperBound, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &request);
            MPI_Isend(&A[lowerBound][0], (upperBound - lowerBound) * size, MPI_FLOAT, i, 2, MPI_COMM_WORLD, &request);
        }
    }
    MPI_Bcast(&B, size*size, MPI_FLOAT, 0, MPI_COMM_WORLD);
    if (taskid > 0) {
        MPI_Recv(&lowerBound, 1, MPI_INT, 0, 0 , MPI_COMM_WORLD, &status);
        MPI_Recv(&upperBound, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&A[lowerBound][0], (upperBound - lowerBound) * size, MPI_FLOAT, 0, 2, MPI_COMM_WORLD, &status);
        for (i = lowerBound; i < upperBound; i++) {
            for (j = 0; j < size; j++) {
                for (k = 0; k < size; k++) { 
                    C[i][j] =  C[i][j] +  A[i][k] * B[k][j];
                }
            }
        }
        MPI_Isend(&lowerBound, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, &request);
        MPI_Isend(&upperBound, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &request);
        MPI_Isend(&C[lowerBound][0], (upperBound - lowerBound) * size, MPI_FLOAT, 0, 5, MPI_COMM_WORLD, &request);
    }
    if (taskid == 0) {
        for (i = 1; i < numTasks; i++) {
            MPI_Recv(&lowerBound, 1, MPI_INT, i, 3, MPI_COMM_WORLD, &status);
            MPI_Recv(&upperBound, 1, MPI_INT, i, 4, MPI_COMM_WORLD, &status);
            MPI_Recv(&C[lowerBound][0], (upperBound - lowerBound) * size, MPI_FLOAT, i, 5, MPI_COMM_WORLD, &status);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    for (k=0;k<size-1;k++)
 	{	
		MPI_Bcast(&C[k][0],size, MPI_FLOAT, 0, MPI_COMM_WORLD);
        if(taskid==0){
            for (p=1;p<numTasks;p++){
		  		for (i=k+1+p;i<size;i+=numTasks)
		  		{
				   MPI_Isend(&C[i], size, MPI_FLOAT, p, 8, MPI_COMM_WORLD, &request);
				   MPI_Wait(&request, &status);
		  		}
            }
			for (i=k+1 ; i<size ; i += numTasks)
			{
	  			mp = C[i][k] / C[k][k];
	  			for (j = k; j < size; j++)
	 			{
	   				C[i][j] -= C[k][j] * mp;
	 			}
			}
			for (p = 1; p < numTasks; p++)
			{
			  for (i = k + 1 + p; i < size; i += numTasks)
			  {
			    MPI_Recv(&C[i], size, MPI_FLOAT, p, 9, MPI_COMM_WORLD, &status);
			  }
			}
        }
		else
		{
			for (i = k + 1 + taskid; i < size; i += numTasks)
			{
				MPI_Recv(&C[i], size, MPI_FLOAT, 0, 8, MPI_COMM_WORLD, &status);		
				mp = C[i][k] / C[k][k];
				for (j = k; j < size; j++)
				{
				    C[i][j] -= C[k][j] * mp;
				}
				MPI_Isend(&C[i], size, MPI_FLOAT, 0, 9, MPI_COMM_WORLD, &request);						    
				MPI_Wait(&request, &status);		
			}
		}
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();
    if(taskid==0){
        cout << "Upper Triangular Matrix : " <<endl; 
        print_matrix(C);
        cout<<endl;
        auto e = chrono::steady_clock::now();
        auto diff = e - s;
        mSecs =chrono::duration <double, milli> (diff).count();
        cout << "Time Taken by Task in ms : " << mSecs << endl;
    }
    return 0;
}
