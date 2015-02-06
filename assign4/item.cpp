#include <string>
#include <fstream>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "warehouse.h"
#include "item.h"
#include "dated_item.h"

using namespace std;

item::item(string upc, string _name, int _shelf_life)
{
	this->code = upc;
	this->name = _name;
	this->shelf_life = _shelf_life;
}

item::item(ifstream &stream)
{
	string junk;
	for(int i = 0; i < 3; i++)
	{
		stream >> junk;
	}
	stream >> this->code;
	for(int i = 0; i < 2; i++)
	{
		stream >> junk;
	}
	stream >> this->shelf_life;
	stream >> junk;
	getline(stream, this->name);
	
}

item::item(const item &other)
{
	*this = other;
}

item::item()
{
	code = "";
	name = "";
	shelf_life = 0;
}

item::~item()
{
}

string item::get_name() const
{
	return name;
}


string item::get_code() const
{
	return code;
}

boost::gregorian::date item::get_expiration_date(boost::gregorian::date _date) const
{
	boost::gregorian::date_duration duration(shelf_life);
	
	return _date + duration;
}

bool item::operator== (const item & rhs) const
{
	return code == rhs.code;
}

item& item::operator= (const item &rhs){
	code = rhs.code;
	name = rhs.name;
	shelf_life = rhs.shelf_life;
	return *this;
}

std::ostream& operator<<(std::ostream &stream, const item &_item)
{
	string output = _item.code + " "+ _item.name;
	stream << output;
}