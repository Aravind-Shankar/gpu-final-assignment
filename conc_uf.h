#ifndef CONC_UF_H
#define CONC_UF_H

#include <iostream>

#if CONC
#include <cuda.h>
#endif

class conc_uf
{
private:
	int n;
	int *parent, *size;

#if CONC
	short locked;
#endif

#if CONC
	__device__
#endif
	int root(int i);
public:

#if CONC
	__device__
#endif
	conc_uf(int n);

#if CONC
	__device__
#endif
	~conc_uf();

#if CONC
	__device__
#endif
	bool find(int i, int j);

#if CONC
	__device__
#endif
	void unite(int i, int j);
};

#endif
