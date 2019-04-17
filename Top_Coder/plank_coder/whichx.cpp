#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm>
#include <cstdlib>
using namespace std;

long long  comb(long n, long  k)
{
	long long rv=1;

	//find minimum
	long long mmin = min(n-k,k);

	for(long long  i = 1; i <= mmin; i++)
	{
		rv *= (n - mmin+i );

		rv /= i;

	}

	return rv;
}



int main(int argc, char ** argv)
{
	long n = stoll(argv[1]);
	long  k = stoll(argv[2]);
	long long x = stoll(argv[3]);
	int lpn = n;
	long long num;
	for(int i = 0; i < n && k > 0; i++)
	{
		//get combinations of this 
		num = comb(lpn-1,k-1);
		//if this is so then the answer begins with i
		//so find the xth iteration of this
		if( x < num )
		{
			cout<<" "<<i;

			k--;
		}
		else
		{
			//subtract the number of combos from x to adjust the index
			x -= num;
		}

		lpn--;


	}
	
	cout<<"\n";
	


}
