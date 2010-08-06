/*
 *  DynamicRandomDots.h
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 8/6/10.
 *  Copyright 2010 MIT. All rights reserved.
 *
 */

#ifndef DynamicRandomDots_H_
#define DynamicRandomDots_H_

#include <MWorksCore/Plugin.h>

using namespace mw;

class DynamicRandomDots : public mw::Component {

protected:


public:
	DynamicRandomDots(std::string _tag, shared_ptr<Variable> another_attribute);
	DynamicRandomDots(const DynamicRandomDots &tocopy);
	~DynamicRandomDots();

};

#endif 
