#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv)
{
    int token_num;

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0)
    {
        token_num = 0;
        printf("Processo %d enviou o token %d\n", rank, token_num);
        MPI_Send(&token_num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&token_num, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d recebeu o token %d\n", rank, token_num);
    }
    else
    {
        MPI_Recv(&token_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d recebeu o token %d\n", rank, token_num);
        token_num++;
        printf("Processo %d enviou o token %d\n", rank, token_num);

        if (rank == size - 1)
            MPI_Send(&token_num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // Fecha o anel
        else
            MPI_Send(&token_num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
