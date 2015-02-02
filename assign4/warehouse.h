#include "dated_item.h"
#include <string>
#include <map>
#include <vector>

class warehouse
{
	
	class dated_item;
	class item;
	
	public:
	
	//Constructor for a warehouse. Takes in the city name.
	warehouse(std::string);
	
	void receive(std::string, int);
	
	void request(std::string, int);
	
	private:
	
	std::string city_name;
	//Map of the items in the inventory and how many there are
	std::vector<dated_item> inventory;
};