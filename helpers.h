#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>

using namespace std;

#define PRINT_CUDA_ERR(tag) std::cout << tag << ": " << cudaGetErrorString(cudaGetLastError()) << std::endl

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

#endif
