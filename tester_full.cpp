#define CONC 1				// include lock functionality for conc_uf
#define DEBUG 1				// debug logs
#define DP 					// dyn parallelism flag (comment out to disable dyn parallelism)

#include "conc_uf.h"
#include "helpers.h"
#include "kruskal_kernels.h"

#include <thrust/sort.h>	// for thrust::sort_by_key()
#include <thrust/copy.h>	// print output

int v, e;
int *hfrom, *hto, *hwt, *dfrom, *dto, *dwt, *dwt_copy;

int *mincost;
edge *d_mst;
int *edge_type_helper;

void read_input()
{
	cin >> v >> e;

	hfrom = new int[e];
	hto = new int[e];
	hwt = new int[e];
	for (int i = 0; i < e; ++i)
	{
		cin >> hfrom[i] >> hto[i] >> hwt[i];
	}
}

void setup_device_arrays()
{
	int arrsize = e * sizeof(int);
	cudaMalloc(&dfrom, arrsize);
	cudaMalloc(&dto, arrsize);
	cudaMalloc(&dwt, arrsize);
	cudaMalloc(&dwt_copy, arrsize);
	#if DEBUG
	PRINT_CUDA_ERR("Status after array cudaMallocs");
	#endif

	cudaMemcpy(dfrom, hfrom, arrsize, cudaMemcpyHostToDevice);
	cudaMemcpy(dto, hto, arrsize, cudaMemcpyHostToDevice);
	cudaMemcpy(dwt, hwt, arrsize, cudaMemcpyHostToDevice);
	cudaMemcpy(dwt_copy, hwt, arrsize, cudaMemcpyHostToDevice);
	#if DEBUG
	PRINT_CUDA_ERR("Status after array cudaMemcpys");
	#endif

	thrust::sort_by_key(dwt, dwt + e, dfrom);
	thrust::sort_by_key(dwt_copy, dwt_copy + e, dto);
	#if DEBUG
	PRINT_CUDA_ERR("Status after thrust::sort_by_key calls");
	#endif
}

void setup_kernel_params()
{
	cudaHostAlloc(&mincost, sizeof(int), 0);
	cudaMalloc(&d_mst, (v - 1) * sizeof(edge));
	cudaMalloc(&edge_type_helper, e * sizeof(int));

	cudaMemset(edge_type_helper, 0, e);
	#if DEBUG
	PRINT_CUDA_ERR("Status after kernel param setup");
	#endif
}

void run_kernels()
{
	kruskal_kernel_master<<<1, 1>>>(
		v, e,
		dfrom, dto, dwt,
		mincost, d_mst,
		edge_type_helper
	);

	#ifndef DP
	cudaStream_t helperStream;
	cudaStreamCreate(&helperStream);
	int blocks, threads, partsize;
	partition(e, MAX_THREAD_LOAD, &blocks, &threads, &partsize);
	
	kruskal_kernel_helpers<<<blocks, threads, 0, helperStream>>>(
		dfrom, dto, dwt,
		partsize, e,
		edge_type_helper
	);
	#endif

	cudaDeviceSynchronize();
	#if DEBUG
	PRINT_CUDA_ERR("Status after kruskal_kernels");
	#endif
}

void print_results()
{
	cout << "\n\nKRUSKAL RESULTS:\n\n";
	cout << "Kruskal min cost = " << *mincost << endl;
	cout << "Kruskal edges chosen:" << endl;
	thrust::copy(d_mst, d_mst + (v - 1), ostream_iterator<edge>(cout, "\n"));
}

void cleanup()
{
	delete [] hfrom;
	delete [] hto;
	delete [] hwt;
}

int main(int argc, char const *argv[])
{
	read_input();
	setup_device_arrays();
	setup_kernel_params();
	run_kernels();
	print_results();
	cleanup();
	return 0;
}