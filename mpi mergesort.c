#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define N 256

int main(int argc, char **argv){
 int myid, numproc;
 MPI_Init(&argc, &argv);
 MPI_Comm_size(MPI_COMM_WORLD, &numproc);
 MPI_Comm_rank(MPI_COMM_WORLD, &myid);

 int charge=N/128;
//this is representitive process
 if(myid==0){
  int buff[N];
  for(int i=0;i<N;i++)
    buff[i]=rand();

  for(int i=0;i<N;i++){
   MPI_Send(&send_number, 1, MPI_INT, myid+1, 0, MPI_COMM_WORLD);
  }
 }else{


  MPI_Recv(&recv_data, 1, MPI_INT, myid-1, 0, MPI_COMM_WORLD, NULL);

 }



 MPI_Barrier(MPI_COMM_WORLD);
 double t1=MPI_Wtime();



 if(myid%2==0){
  int send_number=myid*2;
  MPI_Send(&send_number, 1, MPI_INT, myid+1, 0, MPI_COMM_WORLD);
  printf("[%d] sends MPI_INT %d\n", myid, send_number);
}else{
  int recv_data;
  MPI_Recv(&recv_data, 1, MPI_INT, myid-1, 0, MPI_COMM_WORLD, NULL);
  printf("[%d] recives MPI_INT %d\n", myid, recv_data);
}

 MPI_Barrier(MPI_COMM_WORLD);
 double t2=MPI_Wtime();
 printf("%f", t2-t1);

 MPI_Finalize();
 return 0;
}
