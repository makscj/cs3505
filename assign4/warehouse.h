
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
	
	//The item, date, and quantity
	void receive(item, boost::gregorian::date, int);
	
	//The item and quantity
	void request(item, int);
	
	//Check if the warehouse contains the specific item
	bool contains(item);
	
	
	
	private:
	
	std::string city_name;
	//Map of the items in the inventory and how many there are
	std::vector<dated_item> inventory;
};

#endif