#include "conc_uf.h"

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
}

conc_uf::~conc_uf()
{
	delete[] parent;
	delete[] size;
}

int conc_uf::root(int i)
{
	while (i != parent[i])
	{
		parent[i] = parent[parent[i]];
		i = parent[i];
	}
	return i;
}

bool conc_uf::find(int i, int j)
{
	return root(i) == root(j);
}

void conc_uf::unite(int i, int j)
{
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
}