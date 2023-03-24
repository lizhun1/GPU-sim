//must include cuda runtime to initial cuda device
#include "cuda_runtime.h"
#include "stdio.h"
#include <iostream>
//cuda kernel function
__global__ void add_kernel(float *result, float *operand_A, float *operand_B)
{
    int i=threadIdx.x;
    result[i]=operand_A[i]+operand_B[i];
}

__global__ void init_array(float *operand_A,float *operand_B)
{
    int i=threadIdx.x;
    operand_A[i]=2.0f;
    operand_B[i]=5.0f;
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
    float *a,*b,*x;
    //allocate unified memory
    cudaMallocManaged(&a,array_size*sizeof(float));
    cudaMallocManaged(&b,array_size*sizeof(float));
    cudaMallocManaged(&x,array_size*sizeof(float));
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