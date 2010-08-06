/*
 *  DynamicRandomDotsFactory.h
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 8/6/10.
 *  Copyright 2010 MIT. All rights reserved.
 *
 */

#ifndef DynamicRandomDots_FACTORY_H_
#define DynamicRandomDots_FACTORY_H_

#include <MWorksCore/ComponentFactory.h>

using namespace mw;


class DynamicRandomDotsFactory : public ComponentFactory {
	virtual shared_ptr<mw::Component> createObject(std::map<std::string, std::string> parameters,
                                                   ComponentRegistry *reg);
};


#endif
