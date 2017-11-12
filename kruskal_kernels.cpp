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
__device__ volatile int master_stage = -1;		// -1 = not yet started

__global__
void kruskal_kernel_master(
	conc_uf *duf, int v, int e,
	int *dfrom, int *dto, int *dwt,
	int *mincost, edge *mst
)
{
	
	#ifdef DP

	#endif
}

__global__
void kruskal_kernel_helpers(
	conc_uf *duf, int v, int e,
	int *dfrom, int *dto, int *dwt,
	int *mincost, edge *mst
)
{
	int id = blockDim.x * blockIdx.x + threadIdx.x,
		stage = ;
}