#include <string>
#include <vector>
#include <iostream>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/foreach.hpp>
#include "warehouse.h"
#include "item.h"
#include "dated_item.h"

using namespace std;
using namespace boost::gregorian;

// Constructor for a warehouse. Saves the given name as the city name.
warehouse::warehouse(string name)
{
	city_name = name;
}

warehouse::warehouse(const warehouse &other)
: city_name(other.city_name), inventory(other.inventory)
{
}

warehouse::warehouse()
{
	city_name = "";
}

warehouse::~warehouse()
{
	
}

// Receives a given quantity of a given item on a given date.
void warehouse::receive(item new_item, date current_date, int quantity)
{
	BOOST_FOREACH(dated_item cur, inventory)
	{
		// If we find a dated item with the same item and date, just increase the quantity of that item.
		if(cur.get_item() == new_item && cur.get_date() == current_date)
		{
			cur.add(quantity);
			return;
		}
	}
	// Get the date the item expires.
	date expiration_date = new_item.get_expiration_date(current_date);
	// Add a dated item containing the item to the inventory.
	inventory.push_back(dated_item(new_item, expiration_date, quantity));
}

void warehouse::request(item requested_item, int quantity)
{
	// DELETE THIS AND FIX THE METHOD
	cout << "THIS DOES NOT WORK" << endl;
	vector<vector<dated_item>::iterator> remove_iterators;
	for(vector<dated_item>::iterator it = inventory.begin(); it != inventory.end(); it++)
	{
		if((*it).get_item() == requested_item)
		{
			if((*it).get_quantity() > quantity)
			{
				(*it).remove(quantity);
				break;
			}
			else if((*it).get_quantity() == quantity)
			{
				remove_iterators.push_back(it);
				break;
			}
			else
			{
				quantity -= (*it).get_quantity();
				remove_iterators.push_back(it);
			}
			
		}
	}
}

bool warehouse::contains(item requested_item)
{
	return false;
}

void warehouse::clear_expired_items(date current_date)
{
	// Tracks the iterator locations for the items we want to remove.
	vector<vector<dated_item>::iterator> remove_iterators;
	// Loop through the inventory and store the iterator locations for all
	// dated_items with date at or after the current date.
	for(vector<dated_item>::iterator it = inventory.begin(); it != inventory.end(); it++){
		if((*it).get_date() >= current_date){
			remove_iterators.push_back(it);
		}
	}
	// Loop through the iterator vector in reverse and remove all the items at the
	// iterator locations.
	BOOST_REVERSE_FOREACH(vector<dated_item>::iterator it, remove_iterators)
	{
		inventory.erase(it);
	}
}

warehouse& warehouse::operator=(const warehouse &rhs)
{
	city_name = rhs.city_name;
	inventory = rhs.inventory;
	return *this;
}