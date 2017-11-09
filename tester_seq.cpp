//#define CONC 0			// commented as not necessary, #if will still fail if CONC is undefined

#include "conc_uf.h"
#include "helpers.h"

#include <algorithm>		// for sort()

void plain_test()
{
	int n, i, j;
	char choice;

	prompt_input(&n, "Enter total size");
	conc_uf uf(n);

	do
	{
		prompt_input(&choice, "Find/Union? (f/u)");
		prompt_input(&i, "Enter i");
		prompt_input(&j, "Enter j");

		switch (choice)
		{
			case 'f':
			case 'F':
				cout << "(" << i << ", " << j << ") are in same component: " <<
						uf.find(i, j) << endl;
				break;
			case 'u':
			case 'U':
				uf.unite(i, j);
				cout << "Union complete\n";
				break;
			default:
				break;
		}

		prompt_input(&choice, "Continue? (y/n)");
	} while (choice == 'y' || choice == 'Y');
}

struct edge
{
	int from, to, wt;
} *edges;

bool edge_comp(edge& a, edge& b) { return a.wt < b.wt; }

void kruskal()
{
	int v, e;
	cin >> v >> e;

	edge *edges = new edge[e];

	for (int i = 0; i < e; ++i)
	{
		cin >> edges[i].from >> edges[i].to >> edges[i].wt;
	}

	sort(edges, edges + e, edge_comp);
	conc_uf verts(v);
	int mincost = 0;

	for (int i = 0; i < e; ++i)
	{
		if (!verts.find(edges[i].from, edges[i].to))
		{
			cout << "Kruskal adds edge (" << edges[i].from << ", " << edges[i].to << ") wt = " << edges[i].wt << endl;

			verts.unite(edges[i].from, edges[i].to);
			mincost += edges[i].wt;
		}
	}

	cout << "Kruskal complete with mincost = " << mincost << endl;
	delete [] edges;
}

int main(int argc, char const *argv[])
{
	// plain_test();
	kruskal();
	return 0;
}