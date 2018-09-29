#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<limits.h>
#include<assert.h>

#define N 1024
#define P 128

int count=0;

int main(int argc, char **argv){
 int myid, numproc;
 MPI_Init(&argc, &argv);
 MPI_Comm_size(MPI_COMM_WORLD, &numproc);
 MPI_Comm_rank(MPI_COMM_WORLD, &myid);

//just confirm that numproc is same to P 
 assert(numproc==P);
//each process will have charge's number
 int charge=N/P;
//each buffer recieve numbers 
 int recv_buff[charge];
 int recv_left=INT_MIN;
 int recv_right=INT_MAX;
 
//this is representitive process
 if(myid==0){
//make random N numbers
  int buff[N];
  for(int i=0;i<N;i++)
    buff[i]=rand();
//give each numbers to each process, NOT include myid0 itself    
  for(int p=1;p<P;i++)
   MPI_Send(&buff[p*charge], charge, MPI_INT, p, 0, MPI_COMM_WORLD);
  for(int i=0;i<charge;i++)
   recv_buff[i]=buff[i];
 }else{
   MPI_Recv(recv_buff, charge, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);  
 }

//begin to measure time 
 MPI_Barrier(MPI_COMM_WORLD);
 double t1=MPI_Wtime();

//iteral
do{ 
 count=0;
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

//send and recive each values 
 if(myid==0){
   MPI_Send(&recv_buff[charge-1], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
   MPI_Recv(&recv_right, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, NULL);    
 }
 else if(myid%2==0){
   MPI_Send(&recv_buff[charge-1], 1, MPI_INT, myid+1, 0, MPI_COMM_WORLD);
   MPI_Send(&recv_buff[0], 1, MPI_INT, myid-1, 0, MPI_COMM_WORLD);  
   MPI_Recv(&recv_right, 1, MPI_INT, myid+1, 0, MPI_COMM_WORLD, NULL);    
   MPI_Recv(&recv_left, 1, MPI_INT, myid-1, 0, MPI_COMM_WORLD, NULL);    
 } 
 else if(myid==(numproc-1)){
   MPI_Recv(&recv_left, 1, MPI_INT, myid-1, 0, MPI_COMM_WORLD, NULL);    
   MPI_Send(&recv_buff[0], 1, MPI_INT, myid-1, 0, MPI_COMM_WORLD);  
 }
 else if(myid%2==1){
   MPI_Recv(&recv_left, 1, MPI_INT, myid-1, 0, MPI_COMM_WORLD, NULL);    
   MPI_Recv(&recv_right, 1, MPI_INT, myid+1, 0, MPI_COMM_WORLD, NULL);    
   MPI_Send(&recv_buff[0], 1, MPI_INT, myid-1, 0, MPI_COMM_WORLD);  
   MPI_Send(&recv_buff[charge-1], 1, MPI_INT, myid+1, 0, MPI_COMM_WORLD);
 } 
 
//compare and swap if needed
 if(recv_right<recv_buff[charge-1]){
  recv_buff[charge-1]=recv_right;
  count++;
 }
 if(recv_left>recv_buff[0]){
  recv_buff[0]=recv_left;
  count++;
 }
 //wait for all processes
 MPI_Barrier(MPI_COMM_WORLD); 
}while(count!=0); 

//finish to measure time 
 MPI_Barrier(MPI_COMM_WORLD);
 double t2=MPI_Wtime();
 
 //printing and so on
 if(myid==0){
  for(int i=0;i<charge;i++)
   buff[i]=recv_buff[i];  
  for(int p=1;p<P;i++)
   MPI_Recv(&buff[p*charge], charge, MPI_INT, p, 0, MPI_COMM_WORLD, NULL); 
  for(int i=0;i<N;i++)
   printf("%d\n", buff[i]);
  printf("%f", t2-t1);
 }else{
   MPI_Send(recv_buff, charge, MPI_INT, 0, 0, MPI_COMM_WORLD);  
 }
 
 MPI_Finalize();
 return 0;
}
