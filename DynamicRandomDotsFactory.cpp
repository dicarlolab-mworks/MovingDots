/*
 *  DynamicRandomDotsFactory.cpp
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 8/6/10.
 *  Copyright 2010 MIT. All rights reserved.
 *
 */

#include "DynamicRandomDotsFactory.h"

#include <boost/regex.hpp>
#include "MWorksCore/ComponentRegistry.h"

using namespace mw;

shared_ptr<mw::Component> DynamicRandomDotsFactory::createObject(std::map<std::string, std::string> parameters,
                                                               mw::ComponentRegistry *reg) {
	REQUIRE_ATTRIBUTES(parameters, 
					   "tag",
                       "another_attribute");
	
	std::string tagname(parameters["tag"]);

    
	shared_ptr<Variable> another_attribute = reg->getVariable(parameters["another_attribute"]);	
	
	shared_ptr <DynamicRandomDots> new_component = shared_ptr<DynamicRandomDots>(new DynamicRandomDots(tagname, another_attribute));
	
	return new_component;
}
