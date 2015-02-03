#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "warehouse.h"
#include "item.h"
#include "dated_item.h"

using namespace std;
using namespace boost::gregorian;

warehouse::warehouse(string name){
	this->city_name = name;
}
warehouse::~warehouse(){
	
}

void warehouse::receive(item new_item, date current_date, int quantity){
	date expiration_date = new_item.get_expiration_date(current_date);
	inventory.push_back(dated_item(new_item, expiration_date, quantity));
}

void warehouse::request(item item, int quantity){
	
}

bool warehouse::contains(item item){
	return false;
}

void warehouse::clear_expired_items(){
	
}