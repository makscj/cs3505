/*
* author - Maks Cegielski-Johnson
* author - John Ballard
* CS 3505 - Assignment 4
* Dated Item object, encapsulates an item with the date it entered the system.
*/
#include "item.h"
#include "dated_item.h"
#include<string>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;
using namespace boost::gregorian;

// Initialization constructor. Initializes member variables to the given values.
dated_item::dated_item(item _item, date _date, int quantity)
	: _item(_item)
	, _date(_date)
	, quantity(quantity)
{

}
	
// Copy constructor. Copies the given dated_item.
dated_item::dated_item(const dated_item &other)
: _item(other._item), _date(other._date), quantity(other.quantity)
{

}

//Default constructor
dated_item::dated_item()
{
	item();
	date();
	quantity = 0;
}

//Destructor
dated_item::~dated_item()
{

}

//Decreases the quantity of the dated item
void dated_item::remove(int quantity)
{
	this->quantity -= quantity;
}

//Increases the quanitty of the dated item
void dated_item::add(int quantity)
{
	this->quantity += quantity;
}

//Returns the quantity of this dated item
int dated_item::get_quantity()
{
	return this->quantity;
}

//Get the item value of this dated item
item dated_item::get_item()
{
	return this->_item;
}

//Get the date this item was added to the system
date dated_item::get_date()
{
	return this->_date;
}

//Assignment operator for a dated item.
dated_item& dated_item::operator= (const dated_item &rhs)
{
	_item = rhs._item;
	_date = rhs._date;
	quantity = rhs.quantity;
	return *this;
}