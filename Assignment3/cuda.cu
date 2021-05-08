#include <bits/stdc++.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include <cuda.h>
using std::cout;
using namespace std;

__global__ void matrixMul(float *a, float *b, float *c, int N) {
  printf("%s","Hello World");
  int row = blockIdx.y * blockDim.y + threadIdx.y;
  int col = blockIdx.x * blockDim.x + threadIdx.x;
  if(row < N && col < N){
    c[row * N + col] = 0;
    for (int k = 0; k < N; k++) {
      c[row * N + col] += a[row * N + k] * b[k * N + col];
    }
    printf("%f",c[row * N + col]);
  }
}

void init_matrix(float *m, int N){
  for (int i=0;i<N*N;i++){
    m[i] = ((float) rand()) / (float) RAND_MAX ;
  }
}

void print_matrix(float * a, int N){
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++){
      cout << a[i*N+j]<<"\t";
    }
    cout<<"\n";
  }
}

void verify_result(float *a, float *b, float *c, int N){
    float tmp;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            tmp = 0;
            for(int k = 0; k < N; k++){
                tmp += a[i * N + k] * b[k * N + j];
            }
            assert(tmp == c[i * N + j]);
        }
    }
}

int main(int argc, char* argv[]) {

  int N = atoi(argv[1]);
  std::cout << std::fixed;
  std::cout << std::setprecision(5);
  // host memory
  float *host_A, *host_B , *host_C;
  host_A = new float[N*N];
  host_B = new float[N*N];
  host_C = new float[N*N];

  // Initialize matrices
  init_matrix(host_A,N);
  init_matrix(host_B,N);

  // Allocate device memory
  float *device_A, *device_B , *device_C;
  cudaMalloc(&device_A, N*N*sizeof(float));
  cudaMalloc(&device_B, N*N*sizeof(float));
  cudaMalloc(&device_C, N*N*sizeof(float));

  // Copy data to the device
  cudaMemcpy(device_A, host_A, N*N*sizeof(float), cudaMemcpyHostToDevice);
  cudaMemcpy(device_B, host_B, N*N*sizeof(float), cudaMemcpyHostToDevice);

  // Threads per CTA dimension
  int THREADS = 16;

  // Blocks per grid dimension (assumes THREADS divides N evenly)
  int BLOCKS = (N+THREADS-1)/THREADS;

  // Use dim3 structs for block  and grid dimensions
  dim3 threads(THREADS, THREADS);
  dim3 blocks(BLOCKS, BLOCKS);

  // Launch kernel
  auto s = chrono::steady_clock::now();
  matrixMul<<<blocks, threads>>>(device_A, device_B, device_C,N);
  auto e = chrono::steady_clock::now();
  auto diff = e - s;
  double mSecs =chrono::duration <double, milli> (diff).count();

  // Copy back to the host
  cudaMemcpy(host_C, device_C, N*N*sizeof(float), cudaMemcpyDeviceToHost);

  cout << "COMPLETED SUCCESSFULLY\n";
  verify_result(host_A, host_B, host_C, N);

  // Free memory on device
  cudaFree(device_A);
  cudaFree(device_B);
  cudaFree(device_C);

  cout <<"Size = "<<N;
  cout<<"\n \n";
  cout<<"A = \n";
  print_matrix(host_A,N);
  cout<<"\n";
  cout<<"B = \n";
  print_matrix(host_B,N);
  cout<<"\n";
  cout<<"C = \n";
  print_matrix(host_C,N);
  cout<<"\n";
  cout<<"Time in ms = "<<mSecs;
  cout<<"\n";
  return 0;
}