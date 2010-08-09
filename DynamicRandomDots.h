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
    
    static const GLint verticesPerDot = 2;
    std::vector<GLfloat> dots;
    GLint numDots;
    GLfloat dotSize;
    
    GLfloat fieldRadius;
    
	DynamicRandomDots(const DynamicRandomDots &tocopy);

    void validateParameters();
    void initializeDots();

public:
	DynamicRandomDots(const std::string &tag,
                      shared_ptr<Scheduler> scheduler,
                      shared_ptr<StimulusDisplay> display,
                      shared_ptr<Variable> framesPerSecond,
                      shared_ptr<Variable> numDots,
                      shared_ptr<Variable> dotSize,
                      shared_ptr<Variable> direction,
                      shared_ptr<Variable> speed);
    
	virtual ~DynamicRandomDots();
    
    virtual void draw(shared_ptr<StimulusDisplay> display);
    virtual Datum getCurrentAnnounceDrawData();
    
};


#endif 























