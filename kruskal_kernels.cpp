#include "kruskal_kernels.h"

/*
 * Kruskal kernel implementation uses the algo in this paper:
 * https://pdfs.semanticscholar.org/a19c/f7ef198fdc76c9a6e32a4bd9653d672a78f0.pdf
 * [Anastasios Katsigiannis, Nikos Anastopoulos, Konstantinos Nikas, and Nectarios Koziris -
 *  An approach to parallelize Kruskal’s algorithm using Helper Threads]
 *
 * The sole master thread is placed in its own kernel, and the helpers in another.
 * Both execute concurrently.
 */
__device__ conc_uf *duf;
__device__ volatile int master_index = -1;		// -1 = not yet started

__global__
void kruskal_kernel_master(
	int v, int e,
	int *dfrom, int *dto, int *dwt,
	int *mincost, edge *mst,
	int *edge_type_helper
)
{
	*mincost = 0;
	duf = new conc_uf(v);

	#ifdef DP
	int blocks, threads, partsize;
	partition(e, MAX_THREAD_LOAD, &blocks, &threads, &partsize);
	kruskal_kernel_helpers<<<blocks, threads>>>(
		dfrom, dto, dwt,
		partsize, e,
		edge_type_helper
	);
	#endif

	for (master_index = 0; master_index < e; ++master_index)
	{

	}
}

__global__
void kruskal_kernel_helpers(
	int *dfrom, int *dto, int *dwt,
	int partsize, int e,
	int *edge_type_helper
)
{
	int id = blockDim.x * blockIdx.x + threadIdx.x,
		start = (id + 1) * partsize,
		end = (start + partsize < e) ? (start + partsize) : e;

	while (master_index == -1)
		;

	while (master_index < start)
	{
		
	}
}