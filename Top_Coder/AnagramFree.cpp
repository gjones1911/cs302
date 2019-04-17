// BEGIN CUT HERE
// PROBLEM STATEMENT
// A string X is an anagram of string Y if X can be obtained by arranging all characters of Y in some order, without removing any characters and without adding new characters. For example, each of the strings "baba", "abab", "aabb" and "abba" is an anagram of "aabb", and strings "aaab", "aab" and "aabc" are not anagrams of "aabb".

A set of strings is anagram-free if it contains no pair of strings which are anagrams of each other. Given a set of strings S, return the size of its largest anagram-free subset. Note that the entire set is considered a subset of itself.

DEFINITION
Class:AnagramFree
Method:getMaximumSubset
Parameters:vector <string>
Returns:int
Method signature:int getMaximumSubset(vector <string> S)


CONSTRAINTS
-S will contain between 1 and 50 elements, inclusive.
-Each element of S will contain between 1 and 50 characters, inclusive.
-Each element of S will consist of lowercase letters ('a'-'z') only.
-All elements of S will be distinct.


EXAMPLES

0)
{"abcd","abdc","dabc","bacd"}

Returns: 1

All of the strings in S are anagrams of each other, so no two of them can simultaneously belong to an anagram-free set.

1)
{"abcd","abac","aabc","bacd"}

Returns: 2

One of the maximum anagram-free subsets of S is {"abcd","abac"}.

2)
{"aa","aaaaa","aaa","a","bbaaaa","aaababaa"}

Returns: 6

Note that strings of different length cannot be anagrams of each other.

3)
{"creation","sentence","reaction","sneak","star","rats","snake"}

Returns: 4



// END CUT HERE
#line 54 "AnagramFree.cpp"
#include <string>
#include <vector>
#include <algorithm> 
#include <iostream> 
#include <set>

using namespace std;

class AnagramFree 
{
	
	public:
	int getMaximumSubset(vector <string> S)
	 {
		//get string and sort it
		//compare to
		//
		//
		for(int i = 0; i < S.size();i++)
		{
			sort(S.at(i).begin(), S.at(i).end());
				 
		}


		set<string> subset(S.begin(), S.end());
		return subset.size();
	}
};
