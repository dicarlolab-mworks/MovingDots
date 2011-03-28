/*
 *  DynamicRandomDots.cpp
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 8/6/10.
 *  Copyright 2010 MIT. All rights reserved.
 *
 */

#include "DynamicRandomDots.h"

#include <MWorksCore/ParsedColorTrio.h>


static const std::string SIGNATURE("stimulus/dynamic_random_dots");

static const std::string FIELD_RADIUS("field_radius");
static const std::string FIELD_CENTER_X("field_center_x");
static const std::string FIELD_CENTER_Y("field_center_y");
static const std::string NUM_DOTS("num_dots");
static const std::string DOT_SIZE("dot_size");
static const std::string COLOR("color");
static const std::string ALPHA_MULTIPLIER("alpha_multiplier");
static const std::string DIRECTION("direction");
static const std::string SPEED("speed");


void DynamicRandomDots::describeComponent(ComponentInfo &info) {
    StandardDynamicStimulus::describeComponent(info);
    
    info.setSignature(SIGNATURE);

    info.addParameter(FIELD_RADIUS);
    info.addParameter(FIELD_CENTER_X);
    info.addParameter(FIELD_CENTER_Y);
    info.addParameter(NUM_DOTS);
    info.addParameter(DOT_SIZE);
    info.addParameter(COLOR, "1.0,1.0,1.0");
    info.addParameter(ALPHA_MULTIPLIER, "1.0");
    info.addParameter(DIRECTION);
    info.addParameter(SPEED);
}


DynamicRandomDots::DynamicRandomDots(const ParameterValueMap &parameters) :
    StandardDynamicStimulus(parameters),
    fieldRadius(parameters[FIELD_RADIUS]),
    fieldCenterX(parameters[FIELD_CENTER_X]),
    fieldCenterY(parameters[FIELD_CENTER_Y]),
    numDots(parameters[NUM_DOTS]),
    dotSize(parameters[DOT_SIZE]),
    color(parameters[COLOR]),
    alpha(parameters[ALPHA_MULTIPLIER]),
    direction(parameters[DIRECTION]),
    speed(parameters[SPEED])
{ }


void DynamicRandomDots::load(shared_ptr<StimulusDisplay> display) {
    if (loaded)
        return;

    validateParameters();
    computeDotSizeInPixels(display);
    initializeDots();
    
    loaded = true;
}


void DynamicRandomDots::validateParameters() {
    if (fieldRadius <= 0.0f) {
        throw SimpleException("field radius must be greater than 0");
    }

    if (numDots < 1) {
        throw SimpleException("number of dots must be greater than or equal to 1");
    }
    
    if (dotSize <= 0.0f) {
        throw SimpleException("dot size must be greater than 0");
    }
}


void DynamicRandomDots::computeDotSizeInPixels(shared_ptr<StimulusDisplay> display) {
    dotSizeInPixels.clear();
    
    GLdouble xMin, xMax, yMin, yMax;
    GLint width, height;

    display->getDisplayBounds(xMin, xMax, yMin, yMax);
    
    for (int i = 0; i < display->getNContexts(); i++) {
        display->setCurrent(i);
        display->getCurrentViewportSize(width, height);
        dotSizeInPixels.push_back(dotSize / (xMax - xMin) * (GLfloat)width);
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

        x += fieldCenterX;
        y += fieldCenterY;
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
        
        GLfloat x1 = x - fieldCenterX;
        GLfloat y1 = y - fieldCenterY;

        if (x1*x1 + y1*y1 > fieldRadius*fieldRadius) {
            y1 = rand(-fieldRadius, fieldRadius);
            x1 = -sqrt(fieldRadius*fieldRadius - y1*y1) + rand(0.0f, dr);
            
            x = x1*cos(theta) - y1*sin(theta);
            y = x1*sin(theta) + y1*cos(theta);
            
            x += fieldCenterX;
            y += fieldCenterY;
        }
    }
}


void DynamicRandomDots::willPlay() {
    StandardDynamicStimulus::willPlay();
    previousTime = 0;
}


void DynamicRandomDots::didStop() {
    StandardDynamicStimulus::didStop();
    previousTime = -1;
}


void DynamicRandomDots::drawFrame(shared_ptr<StimulusDisplay> display, int frameNumber) {
    // If we're drawing to the main display, update dot positions
    if (display->getCurrentContextIndex() == 0) {
        currentTime = getElapsedTime();
        updateDots();
        previousTime = currentTime;
    }

    // Enable antialiasing so dots are round, not square
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
    
    glColor4f(color.red, color.green, color.blue, alpha);

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
    Datum announceData = StandardDynamicStimulus::getCurrentAnnounceDrawData();
    announceData.addElement(STIM_TYPE, SIGNATURE);
    announceData.addElement(FIELD_RADIUS, fieldRadius);
    announceData.addElement(FIELD_CENTER_X, fieldCenterX);
    announceData.addElement(FIELD_CENTER_Y, fieldCenterY);
    announceData.addElement(NUM_DOTS, (long)numDots);
    announceData.addElement(DOT_SIZE, dotSize);
    announceData.addElement(STIM_COLOR_R, color.red);
    announceData.addElement(STIM_COLOR_G, color.green);
    announceData.addElement(STIM_COLOR_B, color.blue);
    announceData.addElement(ALPHA_MULTIPLIER, alpha);
    announceData.addElement(DIRECTION, direction->getValue().getFloat());
    announceData.addElement(SPEED, speed->getValue().getFloat());
    
    return announceData;
}























