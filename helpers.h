#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>

using namespace std;

#define PRINT_CUDA_ERR(tag) std::cout << tag << ": " << cudaGetErrorString(cudaGetLastError()) << std::endl
#define MAX_PART_SIZE 256
#define MAX_BLOCK_THREADS 1024

class edge
{
public:
	int from, to, wt;

	friend ostream& operator<< (ostream& os, const edge& e)
	{
		os << "[ (" << edge.from << "," << edge.to << ") wt = " << edge.wt << " ]";
		return os;
	}
};

bool edge_comp(edge& a, edge& b) { return a.wt < b.wt; }

template <typename T>
void prompt_input(T* pVar, const char* msg)
{
	cout << msg << "\t:\t";
	cin >> *pVar;
	cout << endl;
}

#if CONC
__host__ __device__
#endif
void partition(int n, int *blocks, int *threads, int *partsize)
{
	*threads = n;
	if (*threads > MAX_BLOCK_THREADS)
	{
		*threads = MAX_BLOCK_THREADS;
		n /= MAX_BLOCK_THREADS;

		*partsize = n + 1;
		if (*partsize > MAX_PART_SIZE)
		{
			*partsize = MAX_PART_SIZE;
			n /= MAX_PART_SIZE;

			*blocks = n + 1;
		}
		else
			*blocks = 1;
	}
	else
	{
		*partsize = *blocks = 1;
	}
}

#endif
