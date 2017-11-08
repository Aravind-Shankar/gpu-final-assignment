//#define CONC 0			// commented as not necessary, #if will still fail if CONC is undefined

#include "conc_uf.h"
#include "helpers.h"

int main(int argc, char const *argv[])
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

	return 0;
}