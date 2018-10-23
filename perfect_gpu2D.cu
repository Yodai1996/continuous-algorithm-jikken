#include <stdio.h>
#include <sys/time.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include <driver_types.h>
#define M 1024 

__global__
void Calcu(float *address, size_t pitch)
{
  //pitchの使い道がわからん。詰んだ。
  int i=blockIdx.x * blockDim.x + threadIdx.x;
  int j=blockIdx.y * blockDim.y + threadIdx.y;
  float up, down, left, right;
  
  float r=0.2;
  
  for(int count=0; count<100; count++){
    if(0<i && i<M && 0<j && j<M){
      float* row=(float*)((char*)address+j*pitch);
      float* up_row=(float*)((char*)address+(j-1)*pitch);
      float* down_row=(float*)((char*)address+(j+1)*pitch);
      up=up_row[i];
      down=down_row[i];
      left=row[i-1];
      right=row[i+1];
      __syncthreads();
      row[i]=(1-4*r)*row[i]+r*up+r*down+r*left+r*right;
      }
      __syncthreads();
  }
}


int main(void)
{
        struct timeval t0,t1;
        int i,j;
        float dx=1/M;
        float r=0.2;
        float array[M+1][M+1];
        float time;
        size_t pitch;

        float *address;

        for(i=0;i<M+1;i++)
                for(j=0;j<M+1;j++)
                        array[i][j]=1;

        for(i=0;i<M+1;i++){
                array[0][i]=0;
                array[M][i]=0;
                array[i][0]=0;
                array[i][M]=0;
        }

        cudaMallocPitch(&address, &pitch, (M+1)*sizeof(float), M+1);
   
        cudaMemcpy2D(address ,pitch ,array ,(M+1)*sizeof(float), (M+1)*sizeof(float), M+1, cudaMemcpyHostToDevice);

        dim3 threadPerBlock(32,32);
        dim3 numBlock(M/threadPerBlock.x, M/threadPerBlock.y);

        gettimeofday(&t0, NULL);

        Calcu<<<numBlock, threadPerBlock>>>(address, pitch);

        cudaDeviceSynchronize();

        cudaMemcpy2D(array ,(M+1)*sizeof(float) ,address ,pitch, (M+1)*sizeof(float), M+1, cudaMemcpyDeviceToHost);
        cudaFree(address);

        gettimeofday(&t1, NULL);

        time = t1.tv_sec-t0.tv_sec + (t1.tv_usec - t0.tv_usec)*1.0e-6;
        printf("Elapsed time = %lf\n", time);
        printf("FLOPS = %lf\n", (M-1)*(M-1)*100*6/time);


        for(j=10;j>=0;j--){
                for(i=0;i<11;i++)
                        printf("%f ",array[i][j]);
                printf("\n");
        }

        return 0;
}
