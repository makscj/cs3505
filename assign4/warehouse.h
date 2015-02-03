
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
	
	//The item, date, and quantity
	void receive(item, boost::gregorian::date, int);
	
	//The item and quantity
	void request(item, int);
	
	//Check if the warehouse contains the specific item
	bool contains(item);
	
	// Clears expired items from this warehouse's inventory
	void clear_expired_items();
	
	// Assignment operator
	warehouse& operator=(const warehouse &rhs);
	
	private:
	
	std::string city_name;
	//Map of the items in the inventory and how many there are
	std::vector<dated_item> inventory;
};

#endif