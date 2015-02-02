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
	//The UPC code, The name, and the shelf life
	item(std::string, std::string, int);
	
	~item();
	
	std::string get_name() const;
	
	std::string get_code() const;
	
	int get_shelf_life() const;
	
	//implement an equals method and a hashcode method
	
	
	private:
	//The UPC code of the item
	std::string code;
	//The shelf life of the item
	int shelf_life;
	//The name of the item
	std::string name;
};