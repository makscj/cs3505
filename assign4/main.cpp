#include "dated_item.h"
#include "warehouse.h"
#include "item.h"
#include<fstream>
#include<iterator>
#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<set>
#include<boost/foreach.hpp>
#include<sstream>
#include<boost/algorithm/string.hpp>
#include<boost/date_time/gregorian/gregorian.hpp>

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
	
	map<string, warehouse> warehouses;
	
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
		getline(in, word, '\n');
		vector<string> word_array;
		boost::split(word_array, word, boost::is_any_of(" "));
		
		if(!word.compare(0,headers[0].size(), headers[0]))
		{
			for(vector<string>::iterator it = word_array.begin(); it != word_array.end(); it++)
			{
				string name, code, shelf_life;
				if(!(*it).compare("Code:"))
				{
					it++;
					code = *it;
				}
				if(!(*it).compare("life:"))
				{
					it++;
					shelf_life = *it;
				}
				if(!(*it).compare("Name:"))
				{
					it++;
					string name = "";
					while(it != word_array.end())
					{
						name += *it + " ";
						it++;
					}
					item new_item (code, name, atoi(shelf_life.c_str()));
					food_items[code] = new_item;
					break;
				}
			}
		}
		/*
		else if(!word.compare(0,headers[1].size(), headers[1]))
		{	
			for(vector<string>::iterator it = word_array.begin(); it != word_array.end(); it++)
			{
				string _warehouse = "";
				if(!(*it).compare("Warehouse") || !(*it).compare("-") || !(*it).compare(" "))
					continue;
				else
				{
					while(it != word_array.end())
					{
						warehouse += *it;
						it++;
					}
					warehouse new_warehouse (_warehouse);
					warehouses[_warehouse] = new_warehouse;
					break;
				}
				
			}
		}
		*/
		/*
		else if(!word.compare(0,headers[2].size(), headers[2]))
		{
			//Start date
			
		}
		else if(!word.compare(0,headers[3].size(), headers[3]))
		{
			//Receive
			cout << headers[3] << word <<  endl;
		}
		else if(!word.compare(0,headers[4].size(), headers[4]))
		{
			//Request
			cout << headers[4] << word <<  endl;
		}
		else if(!word.compare(0,headers[5].size(), headers[5]))
		{
			//Next day
			cout << headers[5] << word <<  endl;
		}
		else if(!word.compare(0,headers[6].size(), headers[6]))
		{
			//End
			cout << headers[6] << word << endl;
		}	
		else
		{
			cout << "WRONG COMMAND" << endl;
		}
		
		
		if(contains(heads, word))
		{
			cout << word << " -> " << "AYY" << endl;
		}
		else
		{
			cout << word << " ! " << "lmao"<< endl;
		}
		*/
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