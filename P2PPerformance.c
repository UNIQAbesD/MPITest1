#include<stdio.h>
#include<sys/time.h>
#include "mpi.h"

double second()
{
    struct  timeval tm;
    double t;
    gettimeofday(&tm,NULL);
    t=(double) (tm.tv_sec)+((double)(tm.tv_usec))/1.0e6;    
    return t;
}

int main(int argc,char *argv[])
{
    MPI_Init(&argc,&argv);
    
    int rank,size;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    double sentData=rank/10.0+1;
    double recvData;
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Send(&sentData, 1, MPI_DOUBLE, (rank+1)%size,100, MPI_COMM_WORLD);
    double start=second();
    MPI_Recv(&recvData, 1, MPI_DOUBLE, (rank+size-1)%size,100, MPI_COMM_WORLD,&status);
    double end=second();
    printf("%d's result sent:%lf recv:%lf time:%lf\n",rank,sentData,recvData,end-start);
    MPI_Finalize();

}