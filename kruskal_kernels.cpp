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
	partition(e, &blocks, &threads, &partsize);
	kruskal_kernel_helpers<<<blocks, threads>>>(
		dfrom, dto, dwt,
		partsize, e,
		edge_type_helper
	);
	#endif

	int mst_index = 0, i;
	edge *mst_edge = mst;
	for (master_index = i = 0; i < e; ++i, ++master_index)
	{
		__threadfence();
		if (
			edge_type_helper[i] == 0 &&
			!(duf->find(dfrom[i], dto[i]))
		)
		{
			mst_edge->from = dfrom[i];
			mst_edge->to = dto[i];
			mst_edge->wt = dwt[i];
			*mincost = *mincost + dwt[i];

			duf->unite(dfrom[i], dto[i]);
			__threadfence();

			++mst_index;
			if (mst_index == v - 1)
			{
				master_index = e;	// to make helpers also break out
				break;
			}
			else
				++mst_edge;
		}
	}
	__threadfence();

	delete duf;
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

	if (start < e)	// valid helper thread
	{
		while (master_index == -1)
			;						// ensures that the UF is set up

		int i = start, cycle_edges_offset = start;	// cycle_edges_offset == end => no work left
		while (master_index < i && cycle_edges_offset < end)
		{
			if (edge_type_helper[i] == 0)
				if (duf->safe_find(dfrom[i], dto[i]))
				{
					edge_type_helper[i] = 1;
					++cycle_edges_offset;
				}

			++i;
			if (i == end)
				i = start;
		}
	}
}
