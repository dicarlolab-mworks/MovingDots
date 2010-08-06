/*
 *  DynamicRandomDotsFactory.cpp
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 8/6/10.
 *  Copyright 2010 MIT. All rights reserved.
 *
 */

#include <MWorksCore/ComponentRegistry.h>

#include "DynamicRandomDotsFactory.h"
#include "DynamicRandomDots.h"

using namespace mw;


shared_ptr<mw::Component> DynamicRandomDotsFactory::createObject(std::map<std::string, std::string> parameters,
                                                                 ComponentRegistry *reg)
{
    const char* TAG = "tag";
    const char* DIRECTION = "direction";
    const char* SPEED = "speed";

	REQUIRE_ATTRIBUTES(parameters, 
					   TAG,
                       DIRECTION,
                       SPEED);
	
	std::string tag(parameters[TAG]);

	shared_ptr<Variable> direction(reg->getVariable(parameters[DIRECTION]));
	CHECK_ATTRIBUTE(direction, parameters, DIRECTION);
    
	shared_ptr<Variable> speed(reg->getVariable(parameters[SPEED]));
	CHECK_ATTRIBUTE(speed, parameters, SPEED);
	
	shared_ptr<DynamicRandomDots> newComponent(new DynamicRandomDots(tag,
                                                                     direction,
                                                                     speed));
	
	return newComponent;
}






















