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
                                     shared_ptr<Scheduler> scheduler,
                                     shared_ptr<StimulusDisplay> display,
                                     shared_ptr<Variable> framesPerSecond,
                                     shared_ptr<Variable> direction,
                                     shared_ptr<Variable> speed) :
    DynamicStimulusDriver(scheduler, display, framesPerSecond),
    Stimulus(tag),
    direction(direction),
    speed(speed)
{
}


DynamicRandomDots::~DynamicRandomDots() { }


Datum DynamicRandomDots::getCurrentAnnounceDrawData() {
	boost::mutex::scoped_lock locker(stim_lock);
	Datum announceData = DynamicStimulusDriver::getCurrentAnnounceDrawData();
	announceData.addElement(STIM_NAME, "DynamicRandomDots");
	announceData.addElement(STIM_TYPE, "dynamic_random_dots");  
	return announceData;
}


void DynamicRandomDots::draw(shared_ptr<StimulusDisplay> display) {
}























