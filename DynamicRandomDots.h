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

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

#include <MWorksCore/StandardDynamicStimulus.h>

using namespace mw;


class DynamicRandomDots : public StandardDynamicStimulus {

public:
    static void describeComponent(ComponentInfo &info);

    explicit DynamicRandomDots(const ParameterValueMap &parameters);
    virtual ~DynamicRandomDots() { }
    
    virtual void load(shared_ptr<StimulusDisplay> display);
    virtual void drawFrame(shared_ptr<StimulusDisplay> display);
    virtual Datum getCurrentAnnounceDrawData();
   
protected:
    virtual void stopPlaying();
    
private:
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
    
    const GLfloat fieldRadius;
    const GLfloat fieldCenterX, fieldCenterY;
    const GLint numDots;
    const GLfloat dotSize;
    const mw::RGBColor color;
    const GLfloat alpha;
    shared_ptr<Variable> direction;
    shared_ptr<Variable> speed;
    shared_ptr<Variable> announceDots;
    
    static const GLint verticesPerDot = 2;
    std::vector<GLfloat> dots;
    std::vector<GLfloat> dotSizeInPixels;

    boost::mt19937 randGen;
    
    MWTime previousTime, currentTime;
    
};


#endif 























