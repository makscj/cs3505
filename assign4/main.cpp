#include "dated_item.h"
#include "warehouse.h"
#include "item.h"
#include<fstream>
#include<iterator>
#include<iostream>
#include<map>
#include<vector>

using namespace boost::gregorian;
using namespace std;

int main(){
	
	
	
	ifstream in("data1.txt");
	vector<string> arr;

	
	while(true)
	{
		string word;
		in >> word;
		getline(in, word);
		if (in.fail())
			break;
		arr.push_back(word);
		
	}
	
	
	
	
}