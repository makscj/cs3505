#include "dated_item.h"
#include "warehouse.h"
#include "item.h"
#include<fstream>
#include<iterator>
#include<iostream>
#include<map>
#include<vector>
#include<set>

using namespace boost::gregorian;
using namespace std;

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
		
		cout << word << endl;
		
		if(heads.find(word) == 0)
		{
			cout << word << " -> " << "EY" << endl;
		}
		{
			cout << word << " ! " << "NO LUCK"<< endl;
		}
		if (in.fail())
			break;
			
	}
	cout << "############################################" << endl;
	for(vector<string>::iterator it = arr.begin(); it != arr.end(); it++)
	{
		string s = *it;
		cout << s << endl;
	}
		
	cout << "End of main" << endl;
	
}