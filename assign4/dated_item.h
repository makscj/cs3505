/*
* author - Maks Cegielski-Johnson
* author - John Ballard
* CS 3505 - Assignment 4
* Dated Item object, encapsulates an item with the date it entered the system.
*/
#ifndef DATED_ITEM_H
#define DATED_ITEM_H
#include "item.h"
#include<string>
#include <boost/date_time/gregorian/gregorian.hpp>

class dated_item
{	
	public:
	//Constructor, takes the: Item, Date, Quantity
	dated_item(item _item, boost::gregorian::date _date, int quantity);
	//Copy constructor
	dated_item(const dated_item &other);
	//Default constructor
	dated_item();
	//Destructor
	~dated_item();
	
	//Decreases the quantity by the number given
	void remove(int);
	//Increases the quantity by the number given
	void add(int);
	//Returns the quantity of the dated item
	int get_quantity();
	//Returns the item variable of the dated item
	item get_item();
	//Get the date that the item was added
	boost::gregorian::date get_date();
	
	//Assignment operator for dated items
	dated_item& operator= (const dated_item &rhs);
	
	private:
	
	//Date the item was added to the system
	boost::gregorian::date _date;
	//Quantity of this type of dated item
	int quantity;
	//The item that is represented by the dated item
	item _item;
};
#endif