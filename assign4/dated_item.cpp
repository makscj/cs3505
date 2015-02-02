#include "item.h"
#include<string>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;
using namespace boost::gregorian;

dated_item:dated_item(item item, date date, int quantity){
	
}

dated_item::~dated_item(){
	
}

void dated_item::request(int quantity){
	
}

void dated_item::receive(int quantity){
	
}

int dated_item::get_quantity(){
	return this->quantity;
}

item dated_item::get_item(){
	
}

date dated_item::get_date(){
	
}