#include <stdio.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <iostream>;
using namespace::std;

__global__ void kernel(int a, int b, int* result_dev)
{
	// ...
	printf("hello from kernel\n");
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	*result_dev = a + b;
	printf("result_dev from kernel es %d\n", *result_dev);
}

extern "C" void launch_kernel()
{


	int* result_dev;
	int* result_host;

	result_host = (int*)malloc(sizeof(int));
	cudaMalloc(&result_dev, sizeof(int));


	std::cout << "hola" << endl;
	kernel << < 1, 2 >> > (50, 50, result_dev);

	cudaMemcpy(result_host, result_dev, sizeof(int), cudaMemcpyDeviceToHost);
	cout << *result_host << endl;


	printf("RUN CUDA KERNEL\n");
	//kernel << <1, 1 >> >();
}




