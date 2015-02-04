#include "dated_item.h"
#include "warehouse.h"
#include "item.h"
#include<fstream>
#include<iterator>
#include<iostream>
#include<map>
#include<vector>
#include<set>
#include<boost/foreach.hpp>

using namespace boost::gregorian;
using namespace std;


bool contains(set<string> data, string to_find)
{
	BOOST_FOREACH(string s, data)
	{
		if( s == to_find)
			return true;
	}
	return false;
}


int main(){
	date current_day;
	
	map<string, item> food_items;
	
	set<string> heads;	
	
	string headers [7] = {"FoodItem", "Warehouse", "Start date", "Receive", "Request", "Next day", "End"};
	
	ifstream in("data1.txt");
	vector<string> arr;

	for(int i = 0; i < 7; i++)
	{
		heads.insert(headers[i]);
	}
	
	while(true)
	{
		string word;
		in >> word;
		
		
		if(contains(heads, word))
		{
			cout << word << " -> " << "AYY" << endl;
		}
		else
		{
			cout << word << " ! " << "lmao"<< endl;
		}
		if (in.fail())
			break;
			
	}
	//cout << "############################################" << endl;
	for(vector<string>::iterator it = arr.begin(); it != arr.end(); it++)
	{
		string s = *it;
		//cout << s << endl;
	}
		
	cout << "End of main" << endl;
	
}