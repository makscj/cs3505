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
#include<boost/algorithm/string/trim.hpp>
#include<boost/date_time/gregorian/gregorian.hpp>
#include<boost/range/adaptor/map.hpp>

using namespace boost::gregorian;
using namespace std;


int main(int argc, const char* argv [])
{
	if(argc != 2)
	{
		cout << "Usage: Requires one argument that is a data text file." << endl;
		return 1;
	}
	date current_day;

	map<string, item> food_items;
	
	map<string, warehouse> warehouses;
	
	ifstream in(argv[1]);
	
	
	while(true)
	{
		string next;
		in >> next;
		
		if (in.fail())
			break;
			
		if(next == "FoodItem")
		{
			item added_item (in);
			food_items[added_item.get_code()] = added_item;
		}
		else if(next == "Warehouse")
		{
			string junk, name;
			in >> junk;
			getline(in, name);
			boost::algorithm::trim(name);
			warehouses[name] = warehouse(name);
		}
		else if(next == "Start")
		{
			//Convert MM/DD/YYYY -> YY/MM/DD
			string junk, date_in;
			in >> junk;
			in >> date_in;
			vector<string> date_array;
			string date_string = "";
			boost::split(date_array, date_in, boost::is_any_of("/"));
			date_string = date_array[2] + "/" + date_array[0] + "/" + date_array[1];
			current_day = from_simple_string(date_string);
			
		}
		else if(next == "Receive:")
		{
			string upc, warehouse_name;
			int quantity;
			in >> upc;
			in >> quantity;
			getline(in, warehouse_name);
			boost::algorithm::trim(warehouse_name);
			warehouse source = warehouses[warehouse_name];
			item requested_item = food_items[upc];
			source.receive(requested_item, current_day, quantity);
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
		}
		else if(next == "Next")
		{
			string junk;
			// Get rid of the rest of the line
			getline(in, junk);
			date_duration dd(1);
			current_day += dd;
			BOOST_FOREACH(warehouse cur_warehouse, warehouses | boost::adaptors::map_values)
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
						cout << cur_warehouse.get_name() << " contains " << cur_item.get_name() << endl;
						num_stocked++;
						if(num_stocked >= 2){
							break;
						}
					}
					else{
						cout << cur_warehouse.get_name() << " does not contain " << cur_item.get_name() << endl;
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
		

		
	}
	return 0;
}