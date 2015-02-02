#ifndef DATED_ITEM_H
#define DATED_ITEM_H


#include "item.h"
#include<string>
#include <boost/date_time/gregorian/gregorian.hpp>

class dated_item
{	
	
	public:
	
	//Item, Date, Quantity
	dated_item(item, boost::gregorian::date, int);
	
	~dated_item();
	
	//Returns number remaining, 
	void request();
	
	void receive();
	
	int get_quantity();
	
	item get_item();
	
	boost::gregorian::date get_date();
	
	private:
	
	//date
	boost::gregorian::date date;
	
	int quantity;
	
	item this_item;

};

#endif