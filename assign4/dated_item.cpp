#include "item.h"
#include "dated_item.h"
#include<string>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;
using namespace boost::gregorian;

dated_item::dated_item(item _item, date _date, int quantity)
	: _item(_item)
	, _date(_date)
	, quantity(quantity)
	{}

dated_item::~dated_item(){
	
}

void dated_item::request(int quantity){
	this->quantity -= quantity;
}

void dated_item::receive(int quantity){
	this->quantity += quantity;
}

int dated_item::get_quantity(){
	return this->quantity;
}

item dated_item::get_item(){
	return this->_item;
}

date dated_item::get_date(){
	return this->_date;
}