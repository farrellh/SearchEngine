#include "webpage.h"
#include "searchengine.h"
#include <set>
#include <unordered_map>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
 	// Error message 
    if (argc < 4) 
    {
	   cerr << "Not enough input" << endl;
	   return -1;
    }
    // Reading in arguments
    ifstream input(argv[1]);
    ifstream query(argv[2]);
    ofstream output(argv[3]);

    SearchEngine searchengine(input, query, output); 
    
    searchengine.Query(query, output);

    return 0;
}