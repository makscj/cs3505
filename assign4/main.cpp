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


int main2()
{
	/*
	vector<string> temp;
	temp.push_back("05/01/2010");
	
	for(vector
	
	vector<string> date_parts;
	
	string start_date = "05/01/2010";
	
	boost::split(date_parts, start_date, boost::is_any_of("/"));
	
	string temp_date (date_parts[2] + "/" + date_parts[0] + "/" + date_parts[1]);
	
	date current_day;
	current_day = from_string(temp_date.c_str());
	
	
	std::cout 
	<< boost::lexical_cast<std::u32string>(1.0).size() 
	<< " " 
	<< boost::lexical_cast<std::u16string>(1.0).size();
	*/
	return 0;
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
			//Food Item
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
		else if(!word.compare(0,headers[1].size(), headers[1]))
		{	//Warehouse
			string warehouse_name = "";
			for(vector<string>::iterator it = word_array.begin(); it != word_array.end(); it++)
			{
				if(!(*it).compare("Warehouse") || !(*it).compare("-") || !(*it).compare(""))
					continue;
				else
				{
					while(it != word_array.end())
					{
						warehouse_name += *it;
						it++;
					}
					warehouse new_warehouse (warehouse_name);
					warehouses[warehouse_name] = new_warehouse;
					break;
				}
				
			}
		}
		else if(!word.compare(0,headers[2].size(), headers[2]))
		{	
			//Start Date
			string start_date = "";
			for(vector<string>::iterator it = word_array.begin(); it != word_array.end(); it++)
			{
				if(!(*it).compare("date:"))
				{
					it++;
					start_date = *it;
					break;
				}	
			}
			
		
			vector<string> date_parts;
			
			boost::split(date_parts, start_date, boost::is_any_of("/"));
			
			string temp_date (date_parts[2] + "-" + date_parts[0] + "-" + date_parts[1]);
			string temp_date2 ("2010/01/05");
			
			/*
			if(typeid(temp_date) == typeid(temp_date2))
				cout << "They're the same!" << endl;
			else
				cout << "They're different!" << endl;
			
			cout << typeid(date_parts[0]).name() << endl;
			*/
			current_day = from_simple_string(temp_date.c_str());
			
		}
		else if(!word.compare(0,headers[3].size(), headers[3]))
		{
			//Receive
			string code = word_array[0];
			int quantity = atoi(word_array[1].c_str());
			string warehouse = word_array[2];
			
			warehouses[warehouse].receive(food_items[code], current_day, quantity);

		}
		else if(!word.compare(0,headers[4].size(), headers[4]))
		{
			//Request
			string code = word_array[0];
			int quantity = atoi(word_array[1].c_str());
			string warehouse = word_array[2];
			warehouses[warehouse].request(food_items[code], quantity);
		}
		else if(!word.compare(0,headers[5].size(), headers[5]))
		{
			//Next Day
			date_duration dd(1);
			current_day += dd;
		}
		else if(!word.compare(0,headers[6].size(), headers[6]))
		{
			break;
			
		}	
		else
		{
			cout << "WRONG COMMAND" << endl;
		}

		if (in.fail())
			break;
			
	}

	

		
	cout << "End of main" << endl;
	
}