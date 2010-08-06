/*
 *  DynamicRandomDots.cpp
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 8/6/10.
 *  Copyright 2010 MIT. All rights reserved.
 *
 */

#include "DynamicRandomDots.h"


DynamicRandomDots::DynamicRandomDots(const std::string &tag,
                                     shared_ptr<Variable> direction,
                                     shared_ptr<Variable> speed) :
    direction(direction),
    speed(speed)
{
}


DynamicRandomDots::DynamicRandomDots(const DynamicRandomDots &tocopy){ }


DynamicRandomDots::~DynamicRandomDots(){ }
