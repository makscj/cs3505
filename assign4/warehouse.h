#include <string>
#include <map>

class warehouse
{
	
	class item;
	
	public:
	
	//Constructor for a warehouse. Takes in the city name.
	warehouse(std::string);
	
	void receive(std::string, int);
	
	void request(std::string, int);
	
	private:
	
	std::string city_name;
	//Map of the items in the inventory and how many there are
	map<item, int> inventory;
};