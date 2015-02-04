#ifndef DATED_ITEM_H
#define DATED_ITEM_H


#include "item.h"
#include<string>
#include <boost/date_time/gregorian/gregorian.hpp>

class dated_item
{	
	
	public:
	
	//Item, Date, Quantity
	dated_item(item _item, boost::gregorian::date _date, int quantity);
	dated_item(const dated_item &other);
	dated_item();
	
	~dated_item();
	
	//Decreases the quantity by the number given
	void remove(int);
	// Increases the quantity by the number given
	void add(int);
	// Returns the quantity
	int get_quantity();
	
	item get_item();
	
	boost::gregorian::date get_date();
	
	dated_item& operator= (const dated_item &rhs);
	
	private:
	
	//date
	boost::gregorian::date _date;
	
	int quantity;
	
	item _item;

};

#endif