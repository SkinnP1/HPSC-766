// OpenMP header 
#include <omp.h> 
#include <bits/stdc++.h>
using namespace std;


double trapezoid(double start, double end, int points, int threads){
    double h = (end-start)/points;
    double answer = cos(end)/2 + cos(start)/2;
    int i ;

    #pragma omp parallel for schedule(dynamic,threads)  default(none) private(i) shared(start,h,points,threads)  reduction(+:answer)
    for (i=1;i<points;i++){
        answer += cos(start + i*h);
    }

    return (answer*h);
}

int main(int argc, char* argv[]) 
{ 
    double s = omp_get_wtime();
    double pi = M_PI ;
    double start = -pi/2;
    double end = pi/2;
    int points = 2;
    int threads = 2;
    omp_set_num_threads(threads);
    double answer = trapezoid(start,end,points,threads);
    double e = omp_get_wtime();
    double diff = (e - s)*1000;
    cout<<"Points = "<<points<<endl;
    cout<<"Area =\t"<<answer<<endl;
    cout<<"Time =\t"<<diff<<endl;
    return 0;
} 