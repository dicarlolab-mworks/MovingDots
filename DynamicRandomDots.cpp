/*
 *  DynamicRandomDots.cpp
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 8/6/10.
 *  Copyright 2010 MIT. All rights reserved.
 *
 */

#include "DynamicRandomDots.h"

#include <boost/math/special_functions/round.hpp>

#include <MWorksCore/ParsedColorTrio.h>


const std::string DynamicRandomDots::FIELD_RADIUS("field_radius");
const std::string DynamicRandomDots::FIELD_CENTER_X("field_center_x");
const std::string DynamicRandomDots::FIELD_CENTER_Y("field_center_y");
const std::string DynamicRandomDots::DOT_DENSITY("dot_density");
const std::string DynamicRandomDots::DOT_SIZE("dot_size");
const std::string DynamicRandomDots::COLOR("color");
const std::string DynamicRandomDots::ALPHA_MULTIPLIER("alpha_multiplier");
const std::string DynamicRandomDots::DIRECTION("direction");
const std::string DynamicRandomDots::SPEED("speed");
const std::string DynamicRandomDots::COHERENCE("coherence");
const std::string DynamicRandomDots::ANNOUNCE_DOTS("announce_dots");


void DynamicRandomDots::describeComponent(ComponentInfo &info) {
    StandardDynamicStimulus::describeComponent(info);
    
    info.setSignature("stimulus/dynamic_random_dots");
    info.setDisplayName("Dynamic Random Dots");
    info.setDescription("A dynamic random dots stimulus.");

    info.addParameter(FIELD_RADIUS);
    info.addParameter(FIELD_CENTER_X);
    info.addParameter(FIELD_CENTER_Y);
    info.addParameter(DOT_DENSITY);
    info.addParameter(DOT_SIZE);
    info.addParameter(COLOR, "1.0,1.0,1.0");
    info.addParameter(ALPHA_MULTIPLIER, "1.0");
    info.addParameter(DIRECTION);
    info.addParameter(SPEED);
    info.addParameter(COHERENCE, "1.0");
    info.addParameter(ANNOUNCE_DOTS, "0");
}


DynamicRandomDots::DynamicRandomDots(const ParameterValueMap &parameters) :
    StandardDynamicStimulus(parameters),
    fieldRadius(parameters[FIELD_RADIUS]),
    fieldCenterX(parameters[FIELD_CENTER_X]),
    fieldCenterY(parameters[FIELD_CENTER_Y]),
    dotDensity(parameters[DOT_DENSITY]),
    dotSize(parameters[DOT_SIZE]),
    color(parameters[COLOR]),
    alpha(parameters[ALPHA_MULTIPLIER]),
    directionInDegrees(parameters[DIRECTION]),
    direction(directionInDegrees / 180.0f * M_PI),  // Degrees to radians
    speed(parameters[SPEED]),
    coherence(parameters[COHERENCE]),
    announceDots(parameters[ANNOUNCE_DOTS]),
    previousTime(-1),
    currentTime(-1)
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

    if (dotDensity <= 0.0f) {
        throw SimpleException("dot density must be greater than 0");
    }
    
    numDots = GLint(boost::math::round(dotDensity * (M_PI * fieldRadius * fieldRadius)));
    if (numDots < 1) {
        throw SimpleException("field radius and dot density yield 0 dots");
    }
    
    if (dotSize <= 0.0f) {
        throw SimpleException("dot size must be greater than 0");
    }
    
    if ((coherence < 0.0f) || (coherence > 1.0f)) {
        throw SimpleException("coherence must be between 0 and 1");
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
    dotPositions.resize(numDots * verticesPerDot);
    dotDirections.resize(numDots);

    for (GLint i = 0; i < numDots; i++) {
        GLfloat &x = getX(i);
        GLfloat &y = getY(i);
        GLfloat &theta = getDirection(i);

        do {
            x = rand(-fieldRadius, fieldRadius);
            y = rand(-fieldRadius, fieldRadius);
        } while (x*x + y*y > fieldRadius*fieldRadius);

        x += fieldCenterX;
        y += fieldCenterY;
        
        theta = newDirection();
    }
}


void DynamicRandomDots::updateDots() {
    const GLfloat dt = (GLfloat)(currentTime - previousTime) / 1.0e6f;
    const GLfloat dr = dt * speed->getValue().getFloat();

    for (GLint i = 0; i < numDots; i++) {
        GLfloat &x = getX(i);
        GLfloat &y = getY(i);
        GLfloat &theta = getDirection(i);
        
        x += dr * cos(theta);
        y += dr * sin(theta);
        
        GLfloat x1 = x - fieldCenterX;
        GLfloat y1 = y - fieldCenterY;

        if (x1*x1 + y1*y1 > fieldRadius*fieldRadius) {
            theta = newDirection();
            
            y1 = rand(-fieldRadius, fieldRadius);
            x1 = -sqrt(fieldRadius*fieldRadius - y1*y1) + rand(0.0f, dr);
            
            x = x1*cos(theta) - y1*sin(theta);
            y = x1*sin(theta) + y1*cos(theta);
            
            x += fieldCenterX;
            y += fieldCenterY;
        }
    }
}


void DynamicRandomDots::drawFrame(shared_ptr<StimulusDisplay> display) {
    // If we're drawing to the main display, update dot positions
    if (display->getCurrentContextIndex() == 0) {
        currentTime = getElapsedTime();
        if ((previousTime != -1) && (previousTime != currentTime)) {
            updateDots();
        }
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
    glVertexPointer(verticesPerDot, GL_FLOAT, 0, &(dotPositions[0]));
    glDrawArrays(GL_POINTS, 0, numDots);

    glDisableClientState(GL_VERTEX_ARRAY);
    
    glDisable(GL_BLEND);
    glDisable(GL_POINT_SMOOTH);
}


Datum DynamicRandomDots::getCurrentAnnounceDrawData() {
    boost::mutex::scoped_lock locker(stim_lock);

    Datum announceData = StandardDynamicStimulus::getCurrentAnnounceDrawData();

    announceData.addElement(STIM_TYPE, "dynamic_random_dots");
    announceData.addElement(FIELD_RADIUS, fieldRadius);
    announceData.addElement(FIELD_CENTER_X, fieldCenterX);
    announceData.addElement(FIELD_CENTER_Y, fieldCenterY);
    announceData.addElement(DOT_DENSITY, dotDensity);
    announceData.addElement(DOT_SIZE, dotSize);
    announceData.addElement(STIM_COLOR_R, color.red);
    announceData.addElement(STIM_COLOR_G, color.green);
    announceData.addElement(STIM_COLOR_B, color.blue);
    announceData.addElement(ALPHA_MULTIPLIER, alpha);
    announceData.addElement(DIRECTION, directionInDegrees);
    announceData.addElement(SPEED, speed->getValue().getFloat());
    announceData.addElement("num_dots", (long)numDots);
    
    if (announceDots->getValue().getBool()) {
        Datum dotsData;
        dotsData.setString((char*)(&(dotPositions[0])), dotPositions.size() * sizeof(GLfloat));
        announceData.addElement("dots", dotsData);
    }
    
    return announceData;
}


void DynamicRandomDots::stopPlaying() {
    StandardDynamicStimulus::stopPlaying();
    previousTime = -1;
}























