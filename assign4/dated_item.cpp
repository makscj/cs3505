#include "item.h"
#include<string>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;
using namespace boost::gregorian;

dated_item:dated_item(item item, date date, int quantity){
	this->item = item;
	this->date = date;
	this->quantity = quantity;
}

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
	return this->item;
}

date dated_item::get_date(){
	return this->date
}