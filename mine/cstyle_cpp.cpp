#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>

using namespace std;

int main(int argc, char ** argv)
{
	char buf[10];			//will be used as a buffer to hold data

	char * str;				//will point to a c-style string which is a array of chars
	
	const char * cs;		//will be used to get the constant char pointer found by c_str();

	string cppstr;			//will be used to convert a c-style string to a c++ style

	int i;					//good ole i, used for iterations and such

	str = buf;				//since buff is a char array(c-style string) it is a address to somewhere in memory

	//load the buffer with A,B,C,A,B,C
	for(i = 0 ; i < 6; i++)
	{
        buf[i] = 'A' + (i % 3);		
	}
	
	//buff at 6 = NULL to terminate the c-style string
	buf[i] = '\0';

	printf("When I print buf with percent s, I get: %s\n", buf);
	printf("When I print str with percent s, I get: %s\n", str);

	cppstr = buf;			//this will convert buf into a c++ style string

	
	for(i = 1; i < 5; i+=2)
	{
		str[i] = 'I';	//this will go inside the 0th memory index pointed to by str and set it to I
	}					//this will also change buf since str is pointing to it
	
	for(i = 2; i < 6; i+=3)
	{
		cppstr[i] = 'Q';		//this will go inside the 1st index of the string cppstr and set it to Q
	}
	cout<<"now using cout(cstdio) here are the different string\n";

	cout<<"Here is cppstr: "<<cppstr<<"\n";
	cout<<"Here is str:    "<<str <<"\n";
	cout<<"Here is buf:    "<<buf<<"\n";


	//now for some c_str() stuff
	cout<<"now for some c_str() stuff.....\n";

	//empty out the c++ string
	cppstr = "";

	//use the c_str() to get a const char pointer to it like a c style string
	cs = cppstr.c_str();


	//now keep loading the string and watch for when its pointer changes indicating that it over grew
	//is current memory location and had to get a bigger one
	for(i = 0 ; i < 1000; i++)
	{
		cppstr.push_back('A');

		if(cs != cppstr.c_str())		//watch for the address change
		{
			printf("the address changed at %4d:\n",i);

			cs = cppstr.c_str();
		}
	}
	



	return 0;

}
