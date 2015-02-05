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
	{}
	
// Copy constructor. Copies the given dated_item.
dated_item::dated_item(const dated_item &other)
: _item(other._item), _date(other._date), quantity(other.quantity)
{
}

dated_item::dated_item()
{
	item();
	date();
	quantity = 0;
}

dated_item::~dated_item()
{
	
}

void dated_item::remove(int quantity)
{
	this->quantity -= quantity;
}

void dated_item::add(int quantity)
{
	this->quantity += quantity;
}

int dated_item::get_quantity()
{
	return this->quantity;
}

item dated_item::get_item()
{
	return this->_item;
}

date dated_item::get_date()
{
	return this->_date;
}

dated_item& dated_item::operator= (const dated_item &rhs)
{
	_item = rhs._item;
	_date = rhs._date;
	quantity = rhs.quantity;
	return *this;
}