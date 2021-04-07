#include <bits/stdc++.h>
#include <omp.h> 
using namespace std;



vector<vector<float>> calculation(vector<vector<float>>A , vector<vector<float>>B) {
    vector<vector<float>> C ( A.size(), vector<float> (A.size(), 0));
    int N =  A.size();
    int i,j,k;
    #pragma omp parallel for schedule(static) private(i,j,k) shared(A,B,C)
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            for (k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    float r;
    for (i=0;i<N;i++){
        #pragma omp parallel for schedule(static) private(j,k,r) shared(C)
        for(j=i+1;j<N;j++){
            r = C[j][i]/C[i][i];
            for(k=0 ; k<N; k++){
                C[j][k] = C[j][k] - C[i][k]*r;  
            }
        }
    }
    return (C);
}


int main(int argc, char* argv[])
{
    int threads;
    #pragma omp parallel shared(threads)
        threads = omp_get_num_threads();
        cout << " Number of threads : " << threads<<endl;
    
    vector<int> sizes; 
    for (int i = 10 ; i <= 1000; i=i*10){
        sizes.push_back(i);
    }
    vector<float> time; 
    for (int i = 0; i < sizes.size(); i++ ){
        vector<vector<float>> A, B;
        // Creating random matrix A
        for (int j = 0 ; j < sizes[i];j++){
            vector <float> random ;
            for (int k = 0 ; k < sizes[i];k++){
                float randomNumber = ((float) rand()) / (float) RAND_MAX ;
                random.push_back(randomNumber);
            }
            A.push_back(random);
        }
        // Creating random matrix B
        for (int j = 0 ; j < sizes[i];j++){
            vector <float> random ;
            for (int k = 0 ; k < sizes[i];k++){
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
        time.push_back(mSecs);
    }

    cout<<"Sizes"<<"\t\t"<<"Time in ms"<<endl;
    for (int i =0 ; i<sizes.size();i++){
        cout<<sizes[i]<<"\t\t"<<time[i]<<endl; 
    }
    return 0;
}