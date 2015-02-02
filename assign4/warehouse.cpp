#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "warehouse.h"
#include "item.h"
#include "dated_item.h"

using namespace std;

warehouse::warehouse(string name){
	this->city_name = name;
}
warehouse::~warehouse(){
	
}

void warehouse::receive(item item, boost::gregorian::date date, int quantity){
	
}

void warehouse::request(item item, int quantity){
	
}

bool warehouse::contains(item item){
	return false;
}

void warehouse::clear_expired_items(){
	
}