# TrabProgParalela

para compilar usando só o openMP:
export OMP_NUM_THREADS=2 <- n° de threads
gcc -o nm_executavel -fopenmp arquivo.c
./nm_executavel <- executar

para MPI:
mpicc arquivo.c -o nm_executavel <- compilar
mpirun -np 4 testempi <- para executar

Para MPI com openMP:
mpicc -openmp -fopenmp arquivo.c -o nm_executavel <- compilar
mpirun -np 2 ./testempi <-executar
