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
    static const std::string FIELD_RADIUS;
    static const std::string FIELD_CENTER_X;
    static const std::string FIELD_CENTER_Y;
    static const std::string DOT_DENSITY;
    static const std::string DOT_SIZE;
    static const std::string COLOR;
    static const std::string ALPHA_MULTIPLIER;
    static const std::string DIRECTION;
    static const std::string SPEED;
    static const std::string COHERENCE;
    static const std::string ANNOUNCE_DOTS;
    
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
    
    GLfloat& getX(GLint i) { return dotPositions[i*verticesPerDot]; }
    GLfloat& getY(GLint i) { return dotPositions[i*verticesPerDot + 1]; }
    GLfloat& getDirection(GLint i) { return dotDirections[i]; }
    
    GLfloat newDirection() {
        if ((coherence == 0.0f) || ((coherence != 1.0f) && (rand(0.0f, 1.0f) > coherence))) {
            return rand(0.0f, 360.0f);
        }
        return direction;
    }
    
    const GLfloat fieldRadius;
    const GLfloat fieldCenterX, fieldCenterY;
    const GLfloat dotDensity;
    const GLfloat dotSize;
    const mw::RGBColor color;
    const GLfloat alpha;
    const GLfloat directionInDegrees;
    const GLfloat direction;
    shared_ptr<Variable> speed;
    const GLfloat coherence;
    shared_ptr<Variable> announceDots;
    
    std::vector<GLfloat> dotSizeInPixels;
    GLint numDots;
    static const GLint verticesPerDot = 2;
    std::vector<GLfloat> dotPositions;
    std::vector<GLfloat> dotDirections;

    boost::mt19937 randGen;
    
    MWTime previousTime, currentTime;
    
};


#endif 























