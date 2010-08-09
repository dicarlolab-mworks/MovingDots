/*
 *  DynamicRandomDots.cpp
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 8/6/10.
 *  Copyright 2010 MIT. All rights reserved.
 *
 */

#include "DynamicRandomDots.h"

#include <boost/random.hpp>


DynamicRandomDots::DynamicRandomDots(const std::string &tag,
                                     shared_ptr<Scheduler> scheduler,
                                     shared_ptr<StimulusDisplay> display,
                                     shared_ptr<Variable> framesPerSecond,
                                     shared_ptr<Variable> numDots,
                                     shared_ptr<Variable> dotSize,
                                     shared_ptr<Variable> direction,
                                     shared_ptr<Variable> speed) :
    DynamicStimulusDriver(scheduler, display, framesPerSecond),
    Stimulus(tag),
    direction(direction),
    speed(speed)
{
    this->numDots = numDots->getValue().getInteger();
    this->dotSize = dotSize->getValue().getFloat();
    
    validateParameters();
    initializeDots();
}


DynamicRandomDots::~DynamicRandomDots() { }


void DynamicRandomDots::validateParameters() {
    //
    // Validate num_dots
    //
    
    if (numDots < 1) {
        throw SimpleException("number of dots must be greater than or equal to 1");
    }
    
    //
    // Validate dot_size
    //
    
    GLfloat bounds[2];
    glGetFloatv(GL_POINT_SIZE_RANGE, bounds);
    
    if (dotSize < bounds[0]) {
        
        mwarning(M_PLUGIN_MESSAGE_DOMAIN, "dot size (%g) is too small; using %g instead", dotSize, bounds[0]);
        dotSize = bounds[0];
        
    } else if (dotSize > bounds[1]) {
        
        mwarning(M_PLUGIN_MESSAGE_DOMAIN, "dot size (%g) is too large; using %g instead", dotSize, bounds[1]);
        dotSize = bounds[1];
        
    } else {
        
        GLfloat step;
        glGetFloatv(GL_POINT_SIZE_GRANULARITY, &step);
        if (fmod(dotSize - bounds[0], step) != 0.0f) {
            mwarning(M_PLUGIN_MESSAGE_DOMAIN, "dot size (%g) is not aligned with point size step (%g)", dotSize, step);
        }
        
    }
}


void DynamicRandomDots::initializeDots() {
    dots.resize(numDots * verticesPerDot);

    boost::mt19937 randGen;
    boost::uniform_real<GLfloat> randDist(-10.0, 10.0);
    boost::variate_generator< boost::mt19937&, boost::uniform_real<GLfloat> > rand(randGen, randDist);
    
    for (GLint i = 0; i < (numDots * verticesPerDot); i++) {
        dots[i] = rand();
    }
}


void DynamicRandomDots::draw(shared_ptr<StimulusDisplay> display) {
    // Enable antialiasing so dots are round, not square
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);

    glEnableClientState(GL_VERTEX_ARRAY);

    glPointSize(dotSize);
    glVertexPointer(verticesPerDot, GL_FLOAT, 0, &(dots[0]));
    glDrawArrays(GL_POINTS, 0, numDots);

    glDisableClientState(GL_VERTEX_ARRAY);
    
    glDisable(GL_BLEND);
    glDisable(GL_POINT_SMOOTH);
}


Datum DynamicRandomDots::getCurrentAnnounceDrawData() {
	boost::mutex::scoped_lock locker(stim_lock);
	Datum announceData = DynamicStimulusDriver::getCurrentAnnounceDrawData();
	announceData.addElement(STIM_NAME, "Dynamic Random Dots");
	announceData.addElement(STIM_TYPE, "dynamic_random_dots");  
	return announceData;
}























