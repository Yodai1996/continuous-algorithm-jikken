#include <stdio.h>
#include <time.h>
#define M 10

int main(void)
{
	struct timeval t0,t1;
	int i,j,k,count;
	float r=0.2;
	float dx=1/M;
	float dt=r*dx*dx;
	float even[M+1][M+1], odd[M+1][M+1];
	float time;
	
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
	
	gettimeofday(&t0, NULL);
	
	for(count=0;count<100;count+=2){
		for(i=1;i<M;i++)
			for(j=1;j<M;j++){
				odd[i][j]=(1-4*r)*even[i][j]+r*even[i+1][j]+r*even[i-1][j]+r*even[i][j+1]+r*even[i][j-1];
			}
		
		for(i=1;i<M;i++)
			for(j=1;j<M;j++){
				even[i][j]=(1-4*r)*odd[i][j]+r*odd[i+1][j]+r*odd[i-1][j]+r*odd[i][j+1]+r*odd[i][j-1];
			}		
	}
	
	gettimeofday(&t1, NULL);
	time = t1.tv_sec-to.tv_sec + (t1.tv_usec - t0.tv_usec)*1.0e-6;
	printf("Elapsed time = %lf\n", time);
	printf("FLOPS = %lf\n", (M-1)*(M-1)*100*6/time);
	
	for(j=M;j>=0;j--){
		for(i=0;i<M+1;i++)
			printf("%0.3f ",even[i][j]);
		printf("\n");
	}
	
	return 0;
}
