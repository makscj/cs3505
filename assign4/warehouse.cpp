#include <string>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>
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

void warehouse::receive(item new_item, date current_date, int quantity)
{
	// Get the date the item expires.
	date expiration_date = new_item.get_expiration_date(current_date);
	// Add a dated item containing the item to the inventory.
	inventory.push_back(dated_item(new_item, expiration_date, quantity));
}

void warehouse::request(item requested_item, int quantity)
{
	
}

bool warehouse::contains(item item)
{
	return false;
}

void warehouse::clear_expired_items()
{
	
}

warehouse& warehouse::operator=(const warehouse &rhs)
{
	city_name = rhs.city_name;
	inventory = rhs.inventory;
	return *this;
}