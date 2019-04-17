#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv)
{
	long long  n = (atoi( argv[1] ));
	long long  k = (atoi( argv[2] ) );
	long long rv=1;

	//find minimum
	long long mmin = min(n-k,k);

	for(long long  i = 1; i <= mmin; i++)
	{
		rv *= (n - mmin+i );

		rv /= i;

	}

	cout<<rv<<"\n";
}
