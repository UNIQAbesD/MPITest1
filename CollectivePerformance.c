#include<stdio.h>
#include<sys/time.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"

double memset_n(void *buf,void *src,int dataSize,int aryLen)
{
    char *buf_c=(char *)buf;
    char *src_c=(char *)src;
    for(int i=0;i<aryLen;i++)
    {
        memcpy(buf_c+i*dataSize,src,dataSize);
    }
}

double second()
{
    struct  timeval tm;
    double t;
    gettimeofday(&tm,NULL);
    t=(double) (tm.tv_sec)+((double)(tm.tv_usec))/1.0e6;    
    return t;
}

double P2PPerformanceTest(int dataSize)
{
    int rank,size;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    float temp;
    temp=rank;
    int *sentData=malloc(sizeof(int)*dataSize);
    memset_n(sentData, &temp,sizeof(float),dataSize);

    temp=0;
    int *recvData=malloc(sizeof(int)*dataSize);
    memset_n(sentData, &temp,sizeof(float),dataSize);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Send(sentData, dataSize, MPI_INT, (rank+1)%size,100, MPI_COMM_WORLD);
    double start=second();
    MPI_Recv(recvData, dataSize, MPI_INT, (rank+size-1)%size,100, MPI_COMM_WORLD,&status);
    double end=second();
    //printf("%d's result(datanum:%d) time:%lf\n",rank,dataSize,end-start);
    return end-start;
}



int main(int argc,char *argv[])
{
    int dataNum=1000;
    int sampleNum=10;

    MPI_Init(&argc,&argv);
    int rank,size;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    
    double average=0;
    for(int i=0;i<sampleNum;i++)
    {
        average+=P2PPerformanceTest(dataNum);
    }
    average=average/sampleNum;
    printf("%d's result(datanum:%d) time:%lf\n",rank,dataNum,average);
    MPI_Finalize();

}