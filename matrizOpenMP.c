#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void printMatrix(double mat[2][2], int n, int m){
    int i, j;
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            printf(" %.2f ", mat[i][j]);
        }
        printf("\n");
    }
}

void main(){

    int n = 2;
    int m = 3;

    double matriz1[n][m];
    matriz1[0][0] = 2.0; matriz1[0][1] = 3.0; matriz1[0][2] = 1.0; matriz1[1][0] = -1.0; matriz1[1][1] = 0.0; matriz1[1][2] = 2.0; 

    double matriz2[m][n];
    matriz2[0][0] = 1.0; matriz2[0][1] = -2.0; matriz2[1][0] = 1.0; matriz2[1][1] = 2.0; matriz2[2][0] = 3.0; matriz2[2][1] = 1.0;
    
    double matriz3[n][n];

    #pragma omp parallel 
    {
        int ID =  omp_get_thread_num();
        int i, j, k; 
        double sum = 0.0;
        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                for(k = 0; k < m; k++){
                    sum += matriz1[i][k]*matriz2[k][j];
                    printf("%.2f - %.2f \n", matriz1[i][k], matriz2[k][j]);
                }
                printf("%.2f\n", sum);
                matriz3[i][j] = sum;
                sum = 0;
            }
        }
        printf("Eu, thread %d finalizei minhas contas\n", ID);
    }

    printMatrix(matriz3, n, n);
    /*
    #pragma omp parallel
    {
        int ID =  omp_get_thread_num();
        printf("hello %d", ID);
        printf("world %d\n",ID);
    }
    */
}
