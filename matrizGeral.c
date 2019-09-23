#include "mpi.h"
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

int main (int argc, char *argv[]){

    int np, rank, dest, tag = 0, source, rc, count, iam = 0;
    MPI_Status status;

    int n = 2;
    int m = 3;

    double matriz1[n][m];
    matriz1[0][0] = 2.0; matriz1[0][1] = 3.0; matriz1[0][2] = 1.0; matriz1[1][0] = -1.0; matriz1[1][1] = 0.0; matriz1[1][2] = 2.0; 

    double matriz2[m][n];
    matriz2[0][0] = 1.0; matriz2[0][1] = -2.0; matriz2[1][0] = 1.0; matriz2[1][1] = 2.0; matriz2[2][0] = 3.0; matriz2[2][1] = 1.0;
    
    double matriz3[n][n];

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int tamanho = n/np;

    #pragma omp parallel default(shared) private(iam, np)
    {
        int i, j, k; 
        double sum = 0.0;
        
        iam = omp_get_thread_num();
        for(i = rank; i < n; i+=tamanho){
            for(j = 0; j < n; j++){
                for(k = 0; k < m; k++){
                    sum += matriz1[i][k]*matriz2[k][j];
                    //printf("%.2f - %.2f \n", matriz1[i][k], matriz2[k][j]);
                }
                //printf("%.2f\n", sum);
                matriz3[i][j] = sum;
                sum = 0;
            }
        }
        printf("Eu, thread %d do processo %d finalizei minhas contas\n", iam, rank);
    }

    if(rank != 0){
        int i;
		for(i = rank; i < n; i = i+tamanho)
			MPI_Send(&matriz3[i][0], 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
	}
	else{
        int i, j;
		for(j = 1; j < tamanho; j++){
			for(i = j; i < n; i = i+tamanho){
				MPI_Recv(&matriz3[i][0], 1, MPI_INT, j, tag, MPI_COMM_WORLD, &status);
			}
		}
	}

    if(rank == 0){
        printMatrix(matriz3, n, n);
    }

    MPI_Finalize();
}