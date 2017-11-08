#ifndef CONC_UF_H
#define CONC_UF_H

#include <iostream>
//#include <cuda.h>

class conc_uf
{
private:
	int n;
	int *parent, *size;

	int root(int i);
public:
	conc_uf(int n);
	~conc_uf();

	bool find(int i, int j);
	void unite(int i, int j);
};

#endif
