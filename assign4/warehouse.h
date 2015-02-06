/*
* author - Maks Cegielski-Johnson
* author - John Ballard
* CS 3505 - Assignment 4
* Warehouse object, encapsulates a warehouse that contains a collection of
* dated items that can expire.
*/

#ifndef WAREHOUSE_H
#define WAREHOUSE_H
#include "dated_item.h"
#include "item.h"
#include <string>
#include <map>
#include <vector>

class warehouse
{	
	public:
	
	//Constructor for a warehouse. Takes in the city name.
	warehouse(std::string);
	// Copy constructor
	warehouse(const warehouse &other);
	// Default constructor
	warehouse();
	// Destructor
	~warehouse();
	
	std::string get_name();
	
	//The item, date, and quantity
	void receive(item, boost::gregorian::date, int);
	
	//The item and quantity
	void request(item, int);
	
	//Check if the warehouse contains the specific item
	bool contains(item);
	
	// Clears expired items from this warehouse's inventory
	void clear_expired_items(boost::gregorian::date current_date);
	
	// Assignment operator
	warehouse& operator=(const warehouse &rhs);
	
	private:
	
	//The name of the warehouse
	std::string city_name;
	//Map of the items in the inventory and how many there are
	std::vector<dated_item> inventory;
	// Removes the given quantity of the given item with the given date from the inventory.
	void remove_item(item requested_item, boost::gregorian::date expiration_date, int quantity);
	// Gets the quantity of the given item with the given date.
	int get_quantity(item requested_item, boost::gregorian::date expiration_date);
	// Gets the soonest (farthest back in time) date that any item of the given type will expire.
	boost::gregorian::date get_soonest_date(item requested_item);
};

#endif