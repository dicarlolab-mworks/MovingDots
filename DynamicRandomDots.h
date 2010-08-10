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

#include <MWorksCore/DynamicStimulusDriver.h>
#include <MWorksCore/Stimulus.h>

using namespace mw;


class DynamicRandomDots : public DynamicStimulusDriver, public Stimulus {

public:
    DynamicRandomDots(const std::string &tag,
                      shared_ptr<Scheduler> scheduler,
                      shared_ptr<StimulusDisplay> display,
                      shared_ptr<Variable> framesPerSecond,
                      shared_ptr<Variable> fieldRadius,
                      shared_ptr<Variable> fieldCenterX,
                      shared_ptr<Variable> fieldCenterY,
                      shared_ptr<Variable> numDots,
                      shared_ptr<Variable> dotSize,
                      shared_ptr<Variable> direction,
                      shared_ptr<Variable> speed);
    
    virtual ~DynamicRandomDots();
    
    virtual void willPlay();
    virtual void didStop();
    
    virtual void draw(shared_ptr<StimulusDisplay> display);
    virtual Datum getCurrentAnnounceDrawData();
    
private:
    DynamicRandomDots(const DynamicRandomDots &tocopy);
    
    void validateParameters();
    void computeDotSizeInPixels();
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
    shared_ptr<Variable> direction;
    shared_ptr<Variable> speed;
    
    static const GLint verticesPerDot = 2;
    std::vector<GLfloat> dots;
    std::vector<GLfloat> dotSizeInPixels;

    boost::mt19937 randGen;
    
    MWTime previousTime, currentTime;
    
};


#endif 























