#include <bits/stdc++.h>

using namespace std;

#define MASTER_TO_SLAVE_TAG 1 //tag for messages sent from master to slaves
#define SLAVE_TO_MASTER_TAG 4 //tag for messages sent from slaves to master
#define size 3

void print_matrix(int matrix[size][size]){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            cout << setprecision(3) << matrix[i][j] << "\t";

        }
        cout << endl;
    }
    cout << endl;
}

void change(int matrix[size][size]){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            matrix[i][j] += 1; 
        }
        cout << endl;
    }
    cout << endl;
}

int main(int argc, char** argv){
    int a[size][size] = {{1,2,3},{3,4,5},{5,6,7}};
    change(a);
    print_matrix(a);
    return 0 ;
}
