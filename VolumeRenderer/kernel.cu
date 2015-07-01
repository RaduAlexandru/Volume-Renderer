#include <stdio.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <iostream>;
#include <vector>
#include <thrust\host_vector.h>
#include <thrust\device_vector.h>

using namespace::std;

__global__ void kernel()
{
	// ...
	/*printf("hello from kernel\n");
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	*result_dev = a + b;
	printf("result_dev from kernel es %d\n", *result_dev);*/



	/*int id = threadIdx.x % n;
	bool leaveLoop = false;
	while (!leaveLoop) {
		if (atomicExch(&(locks[id]), 1u) == 0u) {
			//critical section
			leaveLoop = true;
			atomicExch(&(locks[id]), 0u);
		}
	}*/





}

extern "C" void launch_kernel(std::vector<int > edgeTable, std::vector< std::vector<int> >triTable)
{


	/*int* result_dev;
	int* result_host;

	result_host = (int*)malloc(sizeof(int));
	cudaMalloc(&result_dev, sizeof(int));


	std::cout << "hola" << endl;
	kernel << < 1, 2 >> > (50, 50, result_dev);

	cudaMemcpy(result_host, result_dev, sizeof(int), cudaMemcpyDeviceToHost);
	cout << *result_host << endl;


	printf("RUN CUDA KERNEL\n");*/
	//kernel << <1, 1 >> >();

	
	

	
	//Copy to two tables to the device
	//thrust::device_vector<int> edgeTableD  ( edgeTable.begin(), edgeTable.end() );
	float* devPtr;
	size_t pitch;
	int width = 16;
	int height = 256;
	cudaMallocPitch((void**)&devPtr, &pitch, width*sizeof(int), height);

	printf("RUN CUDA KERNEL\n");
	//kernel << <1, 1 >> >(devPtr,pitch);
}




