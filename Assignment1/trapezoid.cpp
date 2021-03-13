// OpenMP header 
#include <omp.h> 
#include <bits/stdc++.h>
using namespace std;


double trapezoid(double start, double end, int points, int threads){
    double h = (end-start)/points;
    double answer = cos(end)/2 + cos(start)/2;
    for (int i=1;i<points;i++){
        answer += cos(start + i*h);
    }
    return (answer*h);
}

int main(int argc, char* argv[]) 
{ 
    auto s = chrono::steady_clock::now();
    double pi = M_PI ;
    double start = -pi/2;
    double end = pi/2;
    int points = 10000;
    int threads = 1;
    double answer = trapezoid(start,end,points,threads);
    auto e = chrono::steady_clock::now();
    auto diff = e - s;
    double mSecs =chrono::duration <double, milli> (diff).count();
    cout<<"Points = "<<points<<endl;
    cout<<"Area =\t"<<answer<<endl;
    cout<<"Time =\t"<<mSecs<<endl;
    return 0;
} 