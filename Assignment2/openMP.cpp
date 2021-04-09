#include <bits/stdc++.h>
#include <omp.h> 
using namespace std;

int size;

vector<vector<float>> calculation(vector<vector<float>>A , vector<vector<float>>B) {
    vector<vector<float>> C( size, vector<float> (size, 0));
    int N =  size;
    int i,j,k;
    #pragma omp parallel for schedule(dynamic) private(i,j,k) shared(A,B,C)
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            for (k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    float r;
    for (i=0;i<N;i++){
        #pragma omp parallel for schedule(dynamic) private(j,k,r) shared(C)
        for(j=i+1;j<N;j++){
            r = C[j][i]/C[i][i];
            for(k=0 ; k<N; k++){
                C[j][k] = C[j][k] - C[i][k]*r;  
            }
        }
    }
    return (C);
}

void print_matrix(vector<vector<float>>X){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            cout <<X[i][j] << "\t";

        }
        cout << endl;
    }
    cout << endl;
}


int main(int argc, char* argv[])
{
    std::cout << std::fixed;
    std::cout << std::setprecision(3);
    size = atoi(argv[1]);
    cout << "Size :"<<size<<endl<<endl;
    int threads;
    #pragma omp parallel shared(threads)
        threads = omp_get_num_threads();
        cout<<"Number of threads : " << threads<<endl;    

    cout<<endl;
    vector<vector<float>> A, B;
    // Creating random matrix A
    for (int j = 0 ; j < size;j++){
        vector <float> random ;
        for (int k = 0 ; k < size;k++){
            float randomNumber = ((float) rand()) / (float) RAND_MAX ;
            random.push_back(randomNumber);
        }
        A.push_back(random);
    }
        // Creating random matrix B
    for (int j = 0 ; j < size;j++){
        vector <float> random ;
        for (int k = 0 ; k < size;k++){
            float randomNumber = ((float) rand()) / (float) RAND_MAX ;
            random.push_back(randomNumber);
        }
        B.push_back(random);
    }
    auto s = chrono::steady_clock::now();
    vector<vector<float>> answer = calculation(A,B);
    auto e = chrono::steady_clock::now();
    auto diff = e - s;
    double mSecs =chrono::duration <double, milli> (diff).count();
    cout << "Upper Triangular Matrix :"<<endl;
    print_matrix(answer);
    cout<<endl;
    cout<<"Time in ms : "<<mSecs;
    cout<<endl;
    return 0;
}