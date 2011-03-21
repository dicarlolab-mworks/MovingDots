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

#include <boost/random.hpp>

#include <MWorksCore/StandardDynamicStimulus.h>

#include "SelfDescribingComponent.h"

using namespace mw;


struct ColorTrio {
    GLfloat red, green, blue;
};


class DynamicRandomDots : public StandardDynamicStimulus {

public:
    static void describeParameters(ParameterManifest &manifest);

    DynamicRandomDots(const ParameterValueMap &parameters);
    virtual ~DynamicRandomDots();
    
    virtual void willPlay();
    virtual void didStop();
    
    virtual void load(shared_ptr<StimulusDisplay> display);
    virtual void drawFrame(shared_ptr<StimulusDisplay> display, int frameNumber);
    virtual Datum getCurrentAnnounceDrawData();
    
private:
    DynamicRandomDots(const DynamicRandomDots &tocopy);
    
    void validateParameters();
    void computeDotSizeInPixels(shared_ptr<StimulusDisplay> display);
    void initializeDots();
    void updateDots();
    
    template<typename RealType>
    RealType rand(RealType min, RealType max) {
        boost::uniform_real<RealType> randDist(min, max);
        boost::variate_generator< boost::mt19937&, boost::uniform_real<RealType> > randVar(randGen, randDist);
        return randVar();
    }
    
    GLfloat fieldRadius;
    GLfloat fieldCenterX, fieldCenterY;
    GLint numDots;
    GLfloat dotSize;
    ColorTrio color;
    GLfloat alpha;
    shared_ptr<Variable> direction;
    shared_ptr<Variable> speed;
    
    static const GLint verticesPerDot = 2;
    std::vector<GLfloat> dots;
    std::vector<GLfloat> dotSizeInPixels;

    boost::mt19937 randGen;
    
    MWTime previousTime, currentTime;
    
};


#endif 























