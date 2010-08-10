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
                                     shared_ptr<Variable> numDots,
                                     shared_ptr<Variable> dotSize,
                                     shared_ptr<Variable> direction,
                                     shared_ptr<Variable> speed) :
    DynamicStimulusDriver(scheduler, display, framesPerSecond),
    Stimulus(tag),
    direction(direction),
    speed(speed),
    fieldRadius(10.0f)
{
    this->numDots = numDots->getValue().getInteger();
    this->dotSize = dotSize->getValue().getFloat();
    
    validateParameters();
    initializeDots();
}


DynamicRandomDots::~DynamicRandomDots() { }


void DynamicRandomDots::validateParameters() {
    if (numDots < 1) {
        throw SimpleException("number of dots must be greater than or equal to 1");
    }
    
    if (dotSize <= 0.0f) {
        throw SimpleException("dot size must be greater than 0");
    }
}


void DynamicRandomDots::computeDotSizeInPixels() {
    dotSizeInPixels.clear();
    
    GLdouble xMin, xMax, yMin, yMax;
    display->getDisplayBounds(xMin, xMax, yMin, yMax);
    
    for (int i = 0; i < display->getNContexts(); i++) {
        display->setCurrent(i);

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        GLfloat width = (GLfloat)(viewport[2]);
        
        GLfloat pointSize = dotSize / (xMax - xMin) * width;
        
        /*
        GLfloat bounds[2];
        glGetFloatv(GL_POINT_SIZE_RANGE, bounds);

        if (pointSize < bounds[0]) {
            
            mwarning(M_PLUGIN_MESSAGE_DOMAIN,
                     "dot size in pixels (%g) is too small; using %g instead",
                     pointSize,
                     bounds[0]);
            pointSize = bounds[0];
            
        } else if (pointSize > bounds[1]) {
            
            mwarning(M_PLUGIN_MESSAGE_DOMAIN,
                     "dot size in pixels (%g) is too large; using %g instead",
                     pointSize,
                     bounds[1]);
            pointSize = bounds[1];
            
        } else {
            
            GLfloat step;
            glGetFloatv(GL_POINT_SIZE_GRANULARITY, &step);
            if (fmod(pointSize - bounds[0], step) != 0.0f) {
                mwarning(M_PLUGIN_MESSAGE_DOMAIN,
                         "dot size in pixels (%g) is not aligned with point size step (%g)",
                         pointSize,
                         step);
            }
            
        }
        */
        
        dotSizeInPixels.push_back(pointSize);
    }
}


void DynamicRandomDots::initializeDots() {
    dots.resize(numDots * verticesPerDot);

    for (GLint i = 0; i < (numDots * verticesPerDot); i += verticesPerDot) {
        GLfloat &x = dots[i];
        GLfloat &y = dots[i+1];
        do {
            x = rand(-fieldRadius, fieldRadius);
            y = rand(-fieldRadius, fieldRadius);
        } while (x*x + y*y > fieldRadius*fieldRadius);
    }
}


void DynamicRandomDots::updateDots() {
    const GLfloat dt = (GLfloat)(currentTime - previousTime) / 1.0e6f;
    const GLfloat dr = dt * speed->getValue().getFloat();
    const GLfloat theta = direction->getValue().getFloat() / 180.0f * M_PI;  // Degrees to radians
    
    const GLfloat dx = dr * cos(theta);
    const GLfloat dy = dr * sin(theta);

    for (GLint i = 0; i < (numDots * verticesPerDot); i += verticesPerDot) {
        GLfloat &x = dots[i];
        GLfloat &y = dots[i+1];
        
        x += dx;
        y += dy;

        if (x*x + y*y > fieldRadius*fieldRadius) {
            GLfloat y1 = rand(-fieldRadius, fieldRadius);
            GLfloat x1 = -sqrt(fieldRadius*fieldRadius - y1*y1) + rand(0.0f, dr);
            
            x = x1*cos(theta) - y1*sin(theta);
            y = x1*sin(theta) + y1*cos(theta);
        }
    }
}


void DynamicRandomDots::willPlay() {
    computeDotSizeInPixels();
    previousTime = 0;
}


void DynamicRandomDots::didStop() {
    previousTime = -1;
}


void DynamicRandomDots::draw(shared_ptr<StimulusDisplay> display) {
    boost::mutex::scoped_lock locker(stim_lock);
    
    currentTime = getElapsedTime();
    if (-1 == currentTime) {
        // Not playing
        return;
    }
    
    // If we're drawing to the main display, update dot positions
    if (display->getCurrentContextIndex() == 0) {
        updateDots();
        previousTime = currentTime;
    }

    // Enable antialiasing so dots are round, not square
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);

    glEnableClientState(GL_VERTEX_ARRAY);

    glPointSize(dotSizeInPixels[display->getCurrentContextIndex()]);
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























