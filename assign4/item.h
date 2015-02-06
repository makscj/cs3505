/*
* author - Maks Cegielski-Johnson
* author - John Ballard
* CS 3505 - Assignment 4
* Item header object, encapsulates the name of an item, the UPC code, and shelf life
*/

#ifndef ITEM_H
#define ITEM_H

#include<string>
#include<fstream>
#include <boost/date_time/gregorian/gregorian.hpp>

class item
{
	public:
	//The UPC code, The name, and the shelf life
	item(std::string, std::string, int);
	//Copy constructor
	item(const item &other);
	//Constructor, takes in input stream.
	item(std::ifstream &stream);
	//Default constructor
	item();
	//Destructor
	~item();
	
	//Returns the name of the item
	std::string get_name() const;
	//Returns the UPC code of the item
	std::string get_code() const;
	// Returns the date this type of food item will expire when received on a given date.
	boost::gregorian::date get_expiration_date(boost::gregorian::date) const;
	
	//Equals operator for items
	bool operator== (const item & rhs) const;
	//Assign operator for items
	item& operator= (const item & rhs);


	
	private:
	//The UPC code of the item
	std::string code;
	//The shelf life of the item
	int shelf_life;
	//The name of the item
	std::string name;
	
	//Overloads the << operator, allowing for printing of items.
	friend std::ostream& operator<<(std::ostream&, const item&);
	
};

#endif