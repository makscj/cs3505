/*
* author - Maks Cegielski-Johnson
* author - John Ballard
* CS 3505 - Assignment 4
*
*/

#ifndef ITEM_H
#define ITEM_H

#include<string>
#include <boost/date_time/gregorian/gregorian.hpp>


class item
{

	public:
	//The UPC code, The name, and the shelf life
	item(std::string, std::string, int);
	item(const item &other);
	item();
	
	~item();
	
	std::string get_name() const;
	
	std::string get_code() const;
	
	//int get_shelf_life() const;
	
	bool operator== (const item & rhs) const;
	
	item& operator= (const item & rhs);
	
	//implement an equals method and a hashcode method
	
	// Returns the date this type of food item will expire when received on a given date.
	boost::gregorian::date get_expiration_date(boost::gregorian::date) const;
	
	private:
	//The UPC code of the item
	std::string code;
	//The shelf life of the item
	int shelf_life;
	//The name of the item
	std::string name;
};

#endif