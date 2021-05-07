#include <bits/stdc++.h>
#define N 10
using std::cout;

__global__ void matrixMul(float *a, float *b, float *c) {
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

void init_matrix(float *m){
  for (int i=0;i<N*N;i++){
    m[i] = ((float) rand()) / (float) RAND_MAX ;
  }
}

void print_matrix(float * a){
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++){
      cout << a[i*N+j]<<"\t";
    }
    cout<<"\n";
  }
}

int main() {

  // host memory
  float *host_A, *host_B , *host_C;
  host_A = new float[N*N];
  host_B = new float[N*N];
  host_C = new float[N*N];

  // Initialize matrices
  init_matrix(host_A);
  init_matrix(host_B);

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
  matrixMul<<<blocks, threads>>>(device_A, device_B, device_C);

  // Copy back to the host
  cudaMemcpy(host_C, device_C, N*N*sizeof(float), cudaMemcpyDeviceToHost);

  cout << "COMPLETED SUCCESSFULLY\n";



  // Free memory on device
  cudaFree(device_A);
  cudaFree(device_B);
  cudaFree(device_C);

  // print_matrix(host_A);
  // print_matrix(host_B);
  // print_matrix(host_C);
  return 0;
}