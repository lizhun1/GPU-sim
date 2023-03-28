//must include cuda runtime to initial cuda device
#include "cuda_runtime.h"
#include "stdio.h"
#include <iostream>
//cuda kernel function
__global__ void add_kernel(double *result, double *operand_A, double *operand_B)
{
    int i=threadIdx.x;
    result[i]=operand_A[i]+operand_B[i];
}

__global__ void init_array(double *operand_A,double *operand_B)
{
    int i=threadIdx.x;
    operand_A[i]=2.0f;
    operand_B[i]=5.0f;
}
__global__ void init_array_setp(double *operand_A,double *operand_B)
{
    int i=threadIdx.x;
    if(i<16){
        operand_A[i]=2.0f;
        operand_B[i]=5.0f;
    }
    else{
        operand_A[i]=0;
        operand_B[i]=0;
    }
    
}
__global__ void init_array_bra(double *operand_A,double *operand_B)
{
    int i=threadIdx.x;
    double a=1.0f;
    double b=2.0f;
    if(i==0){
       
        a=a+1;
        b=b+3;
        operand_A[i]=a;
        operand_B[i]=b;
    }
    else{
        b=b-4;
        a=a-2;
        operand_A[i]=a;
        operand_B[i]=b;

    }
    operand_A[i]=operand_A[i]+a;
    operand_B[i]=operand_B[i]+b;
    
}
// __global__ void init_array3(float operand_A,float *c)
// {
//     int i=threadIdx.x;
//     if(operand_A>2.0)
//         *c=operand_A;
//     else
//         *c=1.0;
// }

int main()
{   
    int array_size=10;
    double *a,*b,*x;
    //allocate unified memory
    cudaMallocManaged(&a,array_size*sizeof(double));
    cudaMallocManaged(&b,array_size*sizeof(double));
    cudaMallocManaged(&x,array_size*sizeof(double));
    //run kernel
    //platform("./file_list.txt")
    //platform.sim(string func_name,class cta,p1,p2)
    init_array<<<1,array_size>>>(a,b);
    add_kernel<<<1,array_size>>>(x,a,b);
    //wait for GPU
    cudaDeviceSynchronize();
    //Check for errors (all values should be 3.0f)
    float maxError = 0.0f;
    for (int i = 0; i < array_size; i++)
        maxError = fmax(maxError, fabs(x[i]-2.0f));
    std::cout << "done "<< maxError<<std::endl;
    //free memory
    cudaFree(x);
    cudaFree(a);
    cudaFree(b);

}