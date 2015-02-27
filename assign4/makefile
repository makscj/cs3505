#authors: Maks Cegielski-Johnson and John Ballard
#cs3505 - Assignment 4 Makefile

a.out:	warehouse.o item.o dated_item.o main.o 
	g++ warehouse.o item.o dated_item.o main.o -lboost_date_time

warehouse.o:	warehouse.cpp warehouse.h item.h dated_item.h
	g++ -c warehouse.cpp

item.o:	item.cpp item.h
	g++ -c item.cpp

dated_item.o: dated_item.cpp dated_item.h item.h
	g++ -c dated_item.cpp 

main.o:	main.cpp item.h warehouse.h
	g++ -c main.cpp
