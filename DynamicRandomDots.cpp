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
                                     shared_ptr<Variable> direction,
                                     shared_ptr<Variable> speed) :
    DynamicStimulusDriver(scheduler, display, framesPerSecond),
    Stimulus(tag),
    direction(direction),
    speed(speed),
    numPoints(1000),
    pointSize(2.0f)
{
    points = shared_array<GLfloat>(new GLfloat[numPoints * verticesPerPoint]);
    initializeDots();
    
    /*GLfloat sizes[2];
    GLfloat step;
    glGetFloatv(GL_POINT_SIZE_RANGE, sizes);
    glGetFloatv(GL_POINT_SIZE_GRANULARITY, &step);
    fprintf(stderr, "%g : %g : %g\n", sizes[0], sizes[1], step);*/
}


DynamicRandomDots::~DynamicRandomDots() { }


void DynamicRandomDots::initializeDots() {
    boost::mt19937 randGen;
    boost::uniform_real<GLfloat> randDist(-10.0, 10.0);
    boost::variate_generator< boost::mt19937&, boost::uniform_real<GLfloat> > rand(randGen, randDist);
    
    for (GLint i = 0; i < (numPoints * verticesPerPoint); i++) {
        points[i] = rand();
    }
}


void DynamicRandomDots::draw(shared_ptr<StimulusDisplay> display) {
    glEnableClientState(GL_VERTEX_ARRAY);

    glPointSize(pointSize);
    glVertexPointer(verticesPerPoint, GL_FLOAT, 0, points.get());
    glDrawArrays(GL_POINTS, 0, numPoints);

    glDisableClientState(GL_VERTEX_ARRAY);
}


Datum DynamicRandomDots::getCurrentAnnounceDrawData() {
	boost::mutex::scoped_lock locker(stim_lock);
	Datum announceData = DynamicStimulusDriver::getCurrentAnnounceDrawData();
	announceData.addElement(STIM_NAME, "DynamicRandomDots");
	announceData.addElement(STIM_TYPE, "dynamic_random_dots");  
	return announceData;
}























