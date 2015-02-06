/*
* author - Maks Cegielski-Johnson
* author - John Ballard
* CS 3505 - Assignment 4
* Item object, encapsulates the name of an item, the UPC code, and shelf life
*/
#include <string>
#include <fstream>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/algorithm/string/trim.hpp>
#include "warehouse.h"
#include "item.h"
#include "dated_item.h"

using namespace std;

//Constructor
item::item(string upc, string _name, int _shelf_life)
{
	this->code = upc;
	this->name = _name;
	this->shelf_life = _shelf_life;
}

// Constructor that takes an input stream from which the
// "FoodItem" has been read and creates an item using the remainder of the line.
item::item(ifstream &stream)
{
	// String to hold junk words from the file that we do not need.
	string junk;
	// Get rid of the first 3 words
	for(int i = 0; i < 3; i++)
	{
		stream >> junk;
	}
	// Save the fourth string as the code
	stream >> this->code;
	boost::algorithm::trim(this->code);
	for(int i = 0; i < 2; i++)
	{
		stream >> junk;
	}
	// Save the 7th item as the shelf life.
	stream >> this->shelf_life;
	stream >> junk;
	// Save the remainder of the line as the name
	getline(stream, this->name);
	boost::algorithm::trim(this->name);
}

//Copy constructor
item::item(const item &other)
{
	*this = other;
}

//Default constructor
item::item()
{
	code = "";
	name = "";
	shelf_life = 0;
}

//Destructor
item::~item()
{
}

//Return name of item
string item::get_name() const
{
	return name;
}

//Return UPC code of item
string item::get_code() const
{
	return code;
}

//Return if the expiration date of the item
boost::gregorian::date item::get_expiration_date(boost::gregorian::date _date) const
{
	boost::gregorian::date_duration duration(shelf_life);
	
	return _date + duration;
}

//Equals operator, compares UPC codes.
bool item::operator== (const item & rhs) const
{
	return code == rhs.code;
}

//Assignment operator
item& item::operator= (const item &rhs){
	code = rhs.code;
	name = rhs.name;
	shelf_life = rhs.shelf_life;
	return *this;
}
//Stream output << operator
std::ostream& operator<<(std::ostream &stream, const item &_item)
{
	string output = _item.code + " "+ _item.name;
	return stream << output;
}