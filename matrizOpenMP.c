#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

void printMatrix(double mat[SIZE][SIZE]){
    int i, j;

    printf("\n\nPrintando a matriz :D \n\n");

    for(i = 0; i < SIZE; i++){
        for(j = 0; j < SIZE; j++){
            printf(" %.2f ", mat[i][j]);
        }
        printf("\n");
    }
}

void inicializaMatrix(double mat[SIZE][SIZE]){

    int i, j;
    for(i = 0; i < SIZE; i++){
        for(j = 0; j < SIZE; j++){
            mat[i][j] = j + 1;
        }
    }
}

void main(){

    double matriz1[SIZE][SIZE];
    inicializaMatrix(matriz1);

    double matriz2[SIZE][SIZE];
    inicializaMatrix(matriz2);

    double matriz3[SIZE][SIZE];

    #pragma omp parallel 
    {
        int ID =  omp_get_thread_num();
        int i, j, k; 
        double sum = 0.0;
        for(i = 0; i < SIZE; i++){
            for(j = 0; j < SIZE; j++){
                for(k = 0; k < SIZE; k++){
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

    printMatrix(matriz3);
}
