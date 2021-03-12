// OpenMP header 
#include <omp.h> 
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) 
{ 
    double pi = M_PI/2 ;
    vector <int> points;
    vector <double> t;
    vector <double> Area;
    for (int i=0;i<20;i++){
        points.push_back(100*(i+1));
    }
    double start = -pi;
    double end = pi;
    for (int p : points){
        auto s = chrono::steady_clock::now();
        double h = (end-start)/p;
        double answer = cos(start)/2;
        for (int i=1;i<p;i++){
            answer += cos(start + i*h);
        } 
        answer += cos(end)/2;
        Area.push_back(answer*h);
        auto e = chrono::steady_clock::now();
        auto diff = e - s;
        double mSecs =chrono::duration <double, milli> (diff).count();
        t.push_back(mSecs);
    }
    cout<<"Points\tArea\tTime\n"<<endl;
    for (int i=0;i<points.size();i++){
        cout<<points[i]<<"\t"<<Area[i]<<"\t"<<t[i]<<endl;
    }
    return 0;
} 
