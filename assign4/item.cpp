#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "warehouse.h"
#include "item.h"
#include "dated_item.h"


item::item(string upc, string _name, int _shelf_life)
{
	this->code = upc;
	this->name = _name;
	this->shelf_life = _shelf_life;
}

item::~item()
{
}

string item::get_name()
{
	return name;
}


string item::get_code()
{
	return code;
}

boost::gregorian::date item::get_expiration_date(boost::gregorian::date _date)
{
	boost::gregorian::date_duration duration(shelf_life);
	
	return _date + duration;
}

bool item::operator== (const item & rhs) const
{
	return upc == rhs.upc;
}