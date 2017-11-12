#include "conc_uf.h"

__global__
void kruskal_kernel_master(
	int v, int e,
	int *dfrom, int *dto, int *dwt,
	int *mincost, edge *mst,
	int *edge_type_helper
);

__global__
void kruskal_kernel_helpers(
	int *dfrom, int *dto, int *dwt,
	int partsize, int e,
	int *edge_type_helper
);
