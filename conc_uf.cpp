#include "conc_uf.h"

#if CONC
	__device__
#endif
conc_uf::conc_uf(int n)
{
	this->n = n;
	parent = new int[n];
	size = new int[n];

	for (int i = 0; i < n; ++i)
	{
		parent[i] = i;
		size[i] = 1;
	}

#if CONC
	locked = 0;
#endif
}

#if CONC
	__device__
#endif
conc_uf::~conc_uf()
{
	delete[] parent;
	delete[] size;
}

#if CONC
	__device__
#endif
int conc_uf::root(int i)
{
	while (i != parent[i])
	{
		parent[i] = parent[parent[i]];
		i = parent[i];
	}
	return i;
}

#if CONC
	__device__
#endif
bool conc_uf::find(int i, int j)
{
#if CONC
	while (atomicExch(locked, 1) == 1)
		;
#endif

	bool res = (root(i) == root(j));

#if CONC
	locked = 0;
#endif

	return res;
}

#if CONC
	__device__
#endif
void conc_uf::unite(int i, int j)
{
#if CONC
	while (atomicExch(locked, 1) == 1)
		;
#endif

	int ri = root(i), rj = root(j);
	if (ri != rj)
	{
		if (size[ri] < size[rj])
		{
			parent[ri] = rj;
			size[rj] += size[ri];
		}
		else
		{
			parent[rj] = ri;
			size[ri] += size[rj];
		}
	}

#if CONC
	locked = 0;
#endif
}