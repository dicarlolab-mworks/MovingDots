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
    shared_ptr<Variable> direction;
    shared_ptr<Variable> speed;

public:
	DynamicRandomDots(const std::string &tag,
                      shared_ptr<Variable> direction,
                      shared_ptr<Variable> speed);
	DynamicRandomDots(const DynamicRandomDots &tocopy);
	~DynamicRandomDots();

};


#endif 
