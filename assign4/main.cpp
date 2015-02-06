/*
* author - Maks Cegielski-Johnson
* author - John Ballard
* CS 3505 - Assignment 4
* Main method. Parses a text file of data and will print out the results.
*/
#include "dated_item.h"
#include "warehouse.h"
#include "item.h"
#include<fstream>
#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<boost/foreach.hpp>
#include<boost/algorithm/string.hpp>
#include<boost/algorithm/string/trim.hpp>
#include<boost/date_time/gregorian/gregorian.hpp>
#include<boost/range/adaptor/map.hpp>

using namespace boost::gregorian;
using namespace std;

//Debugging method
void print_warehouse_status(map<string, warehouse> &warehouses);


int main(int argc, const char* argv [])
{
	//If we don't get a data file then stop.
	if(argc != 2)
	{
		cout << "Usage: Requires one argument that is a data text file." << endl;
		return 1;
	}
	//The current day of the system
	date current_day;
	//All the items that exist
	map<string, item> food_items;
	//All the warehouses that exist
	map<string, warehouse> warehouses;
	//The file reader
	ifstream in(argv[1]);	
	
	while(true)
	{
		string next;
		in >> next;
		
		//Break if the reader fails
		if (in.fail())
			break;
			
		if(next == "FoodItem")
		{
			//Deal with the file stream in item constructor
			item added_item (in);
			//Add the item to the map
			food_items[added_item.get_code()] = added_item;
		}
		else if(next == "Warehouse")
		{
			string junk, name;
			in >> junk;//Deal with the junk
			//Read the rest of the line for the warehouse
			getline(in, name);
			boost::algorithm::trim(name);
			//Add the warehouse to the map
			warehouses[name] = warehouse(name);
		}
		else if(next == "Start")
		{
			//We want to convert MM/DD/YYYY -> YYYY/MM/DD
			string junk, date_in;
			in >> junk;
			in >> date_in;
			vector<string> date_array;
			string date_string = "";
			boost::split(date_array, date_in, boost::is_any_of("/"));
			date_string = date_array[2] + "/" + date_array[0] + "/" + date_array[1];
			current_day = from_string(date_string);
			
		}
		else if(next == "Receive:")
		{
			string upc, warehouse_name;
			int quantity;
			in >> upc;
			in >> quantity;
			getline(in, warehouse_name);
			boost::algorithm::trim(warehouse_name);
			warehouse dest = warehouses[warehouse_name];
			item requested_item = food_items[upc];
			dest.receive(requested_item, current_day, quantity);
			warehouses[warehouse_name] = dest;
		}
		else if(next == "Request:")
		{
			string upc, warehouse_name;
			int quantity;
			in >> upc;
			in >> quantity;
			getline(in, warehouse_name);
			boost::algorithm::trim(warehouse_name);
			warehouse source = warehouses[warehouse_name];
			item requested_item = food_items[upc];
			source.request(requested_item, quantity);
			warehouses[warehouse_name] = source;
		}
		else if(next == "Next")
		{
			string junk;
			// Get rid of the rest of the line
			getline(in, junk);
			date_duration dd(1);
			current_day += dd;
			BOOST_FOREACH(warehouse &cur_warehouse, warehouses | boost::adaptors::map_values)
			{
				cur_warehouse.clear_expired_items(current_day);
			}
		}
		else if(next == "End")
		{
			vector<item> well_stocked;
			vector<item> unstocked;
			BOOST_FOREACH(item cur_item, food_items | boost::adaptors::map_values)
			{
				int num_stocked = 0;
				BOOST_FOREACH(warehouse cur_warehouse, warehouses | boost::adaptors::map_values)
				{
					if(cur_warehouse.contains(cur_item))
					{
						num_stocked++;
						if(num_stocked >= 2){
							break;
						}
					}
				}
				if(num_stocked >= 2)
				{
					well_stocked.push_back(cur_item);
				}
				else if(num_stocked == 0)
				{
					unstocked.push_back(cur_item);
				}
			}
			
			cout << "Report by Maks Cegielski-Johnson and John Ballard" << endl;
			cout << endl;
			cout << "Unstocked Products:" << endl;
			
			BOOST_FOREACH(item cur_item, unstocked)
			{
				cout << cur_item << endl;
			}
			cout << endl;
			cout << "Well-Stocked Products:" << endl;
			BOOST_FOREACH(item cur_item, well_stocked)
			{
				cout << cur_item << endl;
			}
			cout << endl;
			break;
		}
		else
		{
			cout << "WRONG COMMAND: " << next << endl;
		}
		

	//print_warehouse_status(warehouses);
	}
	return 0;
}

void print_warehouse_status(map<string, warehouse> &warehouses)
{
	BOOST_FOREACH(warehouse cur_warehouse, warehouses | boost::adaptors::map_values)
	{
		cur_warehouse.print_summary();
	}
}