#define CONC 1				// include lock functionality for conc_uf
#define DEBUG 1				// debug logs
#define DP 					// dyn parallelism flag (comment out to disable dyn parallelism)

#include "conc_uf.h"
#include "helpers.h"
#include "kruskal_kernels.h"

#include <thrust/sort.h>	// for thrust::sort_by_key()
#include <thrust/copy.h>	// print output

int main(int argc, char const *argv[])
{
	int v, e;
	cin >> v >> e;

	int *hfrom, *hto, *hwt, *dfrom, *dto, *dwt, *dwt_copy;
	hfrom = new int[e];
	hto = new int[e];
	hwt = new int[e];
	for (int i = 0; i < e; ++i)
	{
		cin >> hfrom[i] >> hto[i] >> hwt[i];
	}

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

	conc_uf *duf;
	cudaMalloc(&duf, sizeof(conc_uf));
	int *mincost;
	cudaHostAlloc(&mincost, sizeof(int), 0);
	edge *mst;
	cudaMalloc(&mst, (v - 1) * sizeof(edge));
	#if DEBUG
	PRINT_CUDA_ERR("Status after UF, MST edge list and mincost allocations");
	#endif

	kruskal_kernel_master<<<1, 1>>>(
		duf, v, e,
		dfrom, dto, dwt,
		mincost, mst
	);
	#ifndef DP
	cudaStream_t helperStream;
	cudaStreamCreate(&helperStream);
	int blocks = 1, threads = 32;
	kruskal_kernel_helpers<<<blocks, threads, 0, helperStream>>>(
		
	);
	#endif
	cudaDeviceSynchronize();
	#if DEBUG
	PRINT_CUDA_ERR("Status after kruskal_kernels");
	#endif

	cout << "\n\nKRUSKAL RESULTS:\n\n";
	cout << "Kruskal min cost = " << *mincost << endl;
	cout << "Kruskal edges chosen:" << endl;
	thrust::copy(mst, mst + (v - 1), ostream_iterator<edge>(cout, "\n"));

	delete [] hfrom;
	delete [] hto;
	delete [] hwt;
	return 0;
}