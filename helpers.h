#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>

using namespace std;

#define DEBUG 1

template <typename T>
void prompt_input(T* pVar, const char* msg)
{
	cout << endl << msg << "\t:\t";
	cin >> *pVar;
	cout << endl;
}

#endif
