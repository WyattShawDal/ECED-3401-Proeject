//
// Created by wyatt on 2023-11-22.
//
#include "dependencies.h"

#ifndef EXAMPLE_EVENTS_H
#define EXAMPLE_EVENTS_H
int EventHandler(int time,EventNode **current);
OrderData OrderHandler(OrderNode** Root, Customer Order, Customer Delivery);
Cord AdjustOrder(Cord location);


#endif //EXAMPLE_EVENTS_H
