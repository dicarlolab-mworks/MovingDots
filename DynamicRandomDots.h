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

#include <MWorksCore/DynamicStimulusDriver.h>
#include <MWorksCore/Stimulus.h>

using namespace mw;


class DynamicRandomDots : public DynamicStimulusDriver, public Stimulus {

private:
    shared_ptr<Variable> direction;
    shared_ptr<Variable> speed;
    
	DynamicRandomDots(const DynamicRandomDots &tocopy);

public:
	DynamicRandomDots(const std::string &tag,
                      shared_ptr<Scheduler> scheduler,
                      shared_ptr<StimulusDisplay> display,
                      shared_ptr<Variable> framesPerSecond,
                      shared_ptr<Variable> direction,
                      shared_ptr<Variable> speed);
    
	virtual ~DynamicRandomDots();
    
    virtual Datum getCurrentAnnounceDrawData();
    virtual void draw(shared_ptr<StimulusDisplay> display);
    
};


#endif 























