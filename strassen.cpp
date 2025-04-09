#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace chrono;

typedef vector<vector<int>> Matrix;


void add(const Matrix &A, const Matrix &B, Matrix &C, int n) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            C[i][j] = A[i][j] + B[i][j];
}

void subtract(const Matrix &A, const Matrix &B, Matrix &C, int n) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            C[i][j] = A[i][j] - B[i][j];
}

void multiplyNaive(const Matrix &A, const Matrix &B, Matrix &C, int n) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                C[i][j] += A[i][k] * B[k][j];
}

void strassen(const Matrix &A, const Matrix &B, Matrix &C, int n,int base) {

    if (n <=base) {
        multiplyNaive(A, B, C, n);
        return;
    }

    int k = n / 2;
    Matrix A11(k, vector<int>(k)), A12(k, vector<int>(k)), A21(k, vector<int>(k)), A22(k, vector<int>(k));
    Matrix B11(k, vector<int>(k)), B12(k, vector<int>(k)), B21(k, vector<int>(k)), B22(k, vector<int>(k));
    Matrix P1(k, vector<int>(k)), P2(k, vector<int>(k)), P3(k, vector<int>(k)), P4(k, vector<int>(k));
    Matrix P5(k, vector<int>(k)), P6(k, vector<int>(k)), P7(k, vector<int>(k));
    Matrix Temp1(k, vector<int>(k)), Temp2(k, vector<int>(k));

    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + k];
            A21[i][j] = A[i + k][j];
            A22[i][j] = A[i + k][j + k];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + k];
            B21[i][j] = B[i + k][j];
            B22[i][j] = B[i + k][j + k];
        }
    }

    add(A11, A22, Temp1, k);
    add(B11, B22, Temp2, k);
    strassen(Temp1, Temp2, P1, k,base);

    add(A21, A22, Temp1, k);
    strassen(Temp1, B11, P2, k,base); 

    subtract(B12, B22, Temp2, k);
    strassen(A11, Temp2, P3,k,base);

    subtract(B21, B11, Temp2, k);
    strassen(A22, Temp2, P4, k,base);

    add(A11, A12, Temp1, k);
    strassen(Temp1, B22, P5, k,base);

    subtract(A21, A11, Temp1, k);
    add(B11, B12, Temp2, k);
    strassen(Temp1, Temp2, P6, k,base); 

    subtract(A12, A22, Temp1, k); 
    add(B21, B22, Temp2, k);
    strassen(Temp1, Temp2, P7, k,base);

    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            C[i][j] = P1[i][j] + P4[i][j] - P5[i][j] + P7[i][j];
            C[i][j + k] = P3[i][j] + P5[i][j];
            C[i + k][j] = P2[i][j] + P4[i][j];
            C[i + k][j + k] = P1[i][j] - P2[i][j] + P3[i][j] + P6[i][j];
        }
    }
}

int main() {
    int n = 128;  // Matrix size (must be a power of 2) [2,4,8,16,32,64,128,256,512,1024,2048,4096]
    int base=n/2;  //base case will be changed for different test [n/2,n/4,n/8,n/16,n/32]
    Matrix A(n, vector<int>(n)), B(n, vector<int>(n)), C(n, vector<int>(n, 0));

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) 
        {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
        }

    auto start = high_resolution_clock::now();
    strassen(A, B, C, n,base);
    auto end = high_resolution_clock::now();

    auto durationMilli = duration_cast<milliseconds>(end - start);
    cout << "Time: " << durationMilli.count() << " milliseconds" << endl;

    for(vector<int>t:C)
    {
        for(int x:t)cout<<x<<" ";cout<<endl;
    }

    return 0;
}
