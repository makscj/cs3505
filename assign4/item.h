/*
* author - Maks Cegielski-Johnson
* author - John Ballard
* CS 3505 - Assignment 4
*
*/

#include<string>


class item
{

	public:
	
	item(std::string, std::string, int);
	~item();
	
	std::string get_name();
	
	int get_shelf_life();
	
	//implement an equals method and a hashcode method
	
	
	private:
	//The UPC code of the item
	std::string code;
	//The shelf life of the item
	int shelf_life;
	//The name of the item
	std::string name;
};