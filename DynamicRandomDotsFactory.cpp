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
    const char* FRAMES_PER_SECOND = "frames_per_second";
    const char* DIRECTION = "direction";
    const char* SPEED = "speed";

	REQUIRE_ATTRIBUTES(parameters, 
					   TAG,
                       DIRECTION,
                       SPEED);
	
	std::string tag(parameters[TAG]);
    
	shared_ptr<Variable> framesPerSecond(reg->getVariable(parameters[FRAMES_PER_SECOND]));
	CHECK_ATTRIBUTE(framesPerSecond, parameters, FRAMES_PER_SECOND);
    
	shared_ptr<Variable> direction(reg->getVariable(parameters[DIRECTION]));
	CHECK_ATTRIBUTE(direction, parameters, DIRECTION);
    
	shared_ptr<Variable> speed(reg->getVariable(parameters[SPEED]));
	CHECK_ATTRIBUTE(speed, parameters, SPEED);
	
	if (!GlobalCurrentExperiment) {
		throw SimpleException("No experiment currently defined");
	}
	
	shared_ptr<StimulusDisplay> display(GlobalCurrentExperiment->getStimulusDisplay());
	if (!display) {
		throw SimpleException("No stimulus display in current experiment");
	}
	
	shared_ptr<Scheduler> scheduler(Scheduler::instance());
	if (!scheduler) {
		throw SimpleException("No scheduler registered");
	}
	
	shared_ptr<DynamicRandomDots> newComponent(new DynamicRandomDots(tag,
                                                                     scheduler,
                                                                     display,
                                                                     framesPerSecond,
                                                                     direction,
                                                                     speed));
    
    newComponent->load(display);
	shared_ptr<StimulusNode> node(new StimulusNode(newComponent));
	reg->registerStimulusNode(tag, node);
	
	return newComponent;
}






















