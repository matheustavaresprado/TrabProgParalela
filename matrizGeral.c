#include "mpi.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 3

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

int main (int argc, char *argv[]){

    int np, rank, dest, tag = 0, source, rc, count, iam = 0;
    MPI_Status status;

    int n = SIZE;
    int m = SIZE;


    double matriz1[n][m];
    matriz1[0][0] = 2.0; matriz1[0][1] = 3.0; matriz1[0][2] = 1.0;
    matriz1[1][0] = -1.0; matriz1[1][1] = 0.0; matriz1[1][2] = 2.0; 
    matriz1[2][0] = -1.0; matriz1[2][1] = 0.0; matriz1[2][2] = 2.0;

    double matriz2[m][n];
    matriz2[0][0] = 1.0; matriz2[0][1] = -2.0; matriz2[0][2] = -2.0;
    matriz2[1][0] = 1.0; matriz2[1][1] = 2.0; matriz2[1][2] = -2.0;
    matriz2[2][0] = 3.0; matriz2[2][1] = 1.0; matriz2[2][2] = -2.0;

    // descomentar abaixo para 100x100 e alterar SIZE
    // double matriz1[n][m];
    // inicializaMatrix(matriz1);

    // double matriz2[m][n];
    // inicializaMatrix(matriz2);
    
    double matriz3[SIZE][SIZE];

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int tamanho = n/np;

    #pragma omp parallel default(shared) private(iam, np) num_threads(2)
    {
        int i, j, k; 
        double sum;
        
        iam = omp_get_thread_num();
        for(i = (rank)*tamanho; i<((rank)*tamanho)+tamanho; i++){
            for(j = 0; j < n; j++){
                sum = 0;
                for(k = 0; k < m; k++){
                    sum += matriz1[i][k]*matriz2[k][j];
                    //printf("%.2f - %.2f \n", matriz1[i][k], matriz2[k][j]);
                }
                //printf("%.2f\n", sum);
                matriz3[i][j] = sum;
            }
        }
        printf("Eu, thread %d do processo %d finalizei minhas contas\n", iam, rank);
    }

    int i, j, k;
    double sum;
    if(rank == (np-1)){
        for(i = (rank)*tamanho; i<SIZE; i++){
            for(j = 0; j < n; j++){
                sum = 0;
                for(k = 0; k < m; k++){
                    sum += matriz1[i][k]*matriz2[k][j];
                }
                matriz3[i][j] = sum;
            }
        }
    }

    if(rank != 0){
        int i;

        if(rank == (np-1)){
            for(i = (rank)*tamanho; i<SIZE; i++){
                printf("Enviando linha %d para o processo 0.\n", i);
                MPI_Send(matriz3[i], n, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
            }
        }
        else{
            for(i = (rank)*tamanho; i<((rank)*tamanho)+tamanho; i++){
                printf("Enviando linha %d para o processo 0.\n", i);
                MPI_Send(matriz3[i], n, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
            }
        }
	}
	else{
        int i, j;
        for(i = 1; i<np; i++){
            if(i == (np-1)){
                for(j = (i)*tamanho; j<SIZE; j++){
                    printf("Recebendo a linha %d do processo %d.\n", j, i);
                    MPI_Recv(matriz3[j], n, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status);
                }
            }
            else{
                for(j = (i)*tamanho; j<((i)*tamanho)+tamanho; j++){
                    printf("Recebendo a linha %d do processo %d.\n", j, i);
                    MPI_Recv(matriz3[j], n, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status);
                }
            }
            
        }
        
        printMatrix(matriz3);
	}

    MPI_Finalize();
}
