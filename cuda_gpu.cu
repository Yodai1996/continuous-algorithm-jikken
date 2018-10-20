#include <stdio.h>
#include <sys/time.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include <driver_types.h>
#define M 32 

__global__
void Calcu(float **od, float **ev)
{
  int i=blockIdx.x * blockDim.x + threadIdx.x;
  int j=blockIdx.y * blockDim.y + threadIdx.y;
        int count;
        float r=0.2;

        if(i<M && j<M){
        for(count=0;count<100;count+=2){
                od[i][j]=(1-4*r)*ev[i][j]+r*ev[i+1][j]+r*ev[i-1][j]+r*ev[i][j+1]+r*ev[i][j-1];
                __syncthreads();

                ev[i][j]=(1-4*r)*od[i][j]+r*od[i+1][j]+r*od[i-1][j]+r*od[i][j+1]+r*od[i][j-1];
                __syncthreads();
        }
}}


int main(void)
{
        struct timeval t0,t1;
        int i,j;
        float dx=1/M;
        float r=0.2;
        float even[M+1][M+1], odd[M+1][M+1];
        float time;
        size_t pitch;

        float **ev;
        float **od;

        for(i=0;i<M+1;i++)
                for(j=0;j<M+1;j++)
                        even[i][j]=1;

        for(i=0;i<M+1;i++){
                even[0][i]=0;
                even[M][i]=0;
                even[i][0]=0;
                even[i][M]=0;
        }
        for(i=0;i<M+1;i++){
                odd[0][i]=0;
                odd[M][i]=0;
                odd[i][0]=0;
                odd[i][M]=0;
        }

        cudaMallocPitch(ev, &pitch, (M+1)*sizeof(float), M+1);
        cudaMallocPitch(od, &pitch, (M+1)*sizeof(float), M+1);
   
        cudaMemcpy2D(ev ,pitch ,even ,pitch, (M+1)*sizeof(float), M+1, cudaMemcpyHostToDevice);
        cudaMemcpy2D(od ,pitch ,odd  ,pitch, (M+1)*sizeof(float), M+1, cudaMemcpyHostToDevice);

        dim3 threadPerBlock(32,32);
        dim3 numBlocks(M/threadPerBlock.x, M/threadPerBlock.y);

        gettimeofday(&t0, NULL);

        Calcu<<<numBlocks, threadPerBlock>>>(od , ev);

        cudaDeviceSynchronize();

        cudaMemcpy2D(even ,pitch ,ev ,pitch, (M+1)*sizeof(float), M+1, cudaMemcpyDeviceToHost);
        cudaMemcpy2D(odd  ,pitch ,od ,pitch, (M+1)*sizeof(float), M+1, cudaMemcpyDeviceToHost);

        gettimeofday(&t1, NULL);

        time = t1.tv_sec-t0.tv_sec + (t1.tv_usec - t0.tv_usec)*1.0e-6;
        printf("Elapsed time = %lf\n", time);
        printf("FLOPS = %lf\n", (M-1)*(M-1)*100*6/time);


        for(j=10;j>=0;j--){
                for(i=0;i<11;i++)
                        printf("%f ",even[i][j]);
                printf("\n");
        }

        return 0;
}
