#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define N 1024
#define P 128

int main(int argc, char **argv){
 int myid, numproc;
 MPI_Init(&argc, &argv);
 MPI_Comm_size(MPI_COMM_WORLD, &numproc);
 MPI_Comm_rank(MPI_COMM_WORLD, &myid);

//each process will have charge's number
 int charge=N/P;
 
//this is representitive process
 if(myid==0){
//make random N numbers
  int buff[N];
  for(int i=0;i<N;i++)
    buff[i]=rand();
//give each numbers to each process, include myid0 itself    
  for(int p=0;p<P;i++){
   MPI_Send(&buff[p*charge], charge, MPI_INT, p, 0, MPI_COMM_WORLD);
  }
 }
 
//each buffer recieve numbers 
 int recv_buff[charge];
 MPI_Recv(recv_buff, charge, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);
 
 
//begin to measure time 
 MPI_Barrier(MPI_COMM_WORLD);
 double t1=MPI_Wtime();

 
 //bubble sort
 for(int i=0;i<charge;i++){
  for(int j=i;j<charge;j++){
   if(j<i){
    int k=recv_buff[i];
    recv_buff[i]=recv_buff[j];
    recv_buff[j]=k;
   }
  }
 }
 
 
 
 
 
 
 


//finish to measure time 
 MPI_Barrier(MPI_COMM_WORLD);
 double t2=MPI_Wtime();
 printf("%f", t2-t1);

 MPI_Finalize();
 return 0;
}
