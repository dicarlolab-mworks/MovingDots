/*
 *  MovingDots.h
 *  MovingDots
 *
 *  Created by Christopher Stawarz on 8/6/10.
 *  Copyright 2010 MIT. All rights reserved.
 *
 */

#ifndef MovingDots_H_
#define MovingDots_H_

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

using namespace mw;


class MovingDots : public StandardDynamicStimulus {

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
    static const std::string LIFETIME;
    static const std::string ANNOUNCE_DOTS;
    
    static void describeComponent(ComponentInfo &info);

    explicit MovingDots(const ParameterValueMap &parameters);
    ~MovingDots() { }
    
    void load(shared_ptr<StimulusDisplay> display) MW_OVERRIDE;
    void drawFrame(shared_ptr<StimulusDisplay> display) MW_OVERRIDE;
    Datum getCurrentAnnounceDrawData() MW_OVERRIDE;
   
protected:
    void stopPlaying() MW_OVERRIDE;
    
private:
    bool computeNumDots();
    void computeDotSizeToPixels(shared_ptr<StimulusDisplay> display);
    void initializeDots(GLint startIndex = 0);
    void updateDots();
    void replaceDot(GLint i, GLfloat age);
    void advanceDot(GLint i, GLfloat dt, GLfloat dr);
    
    GLfloat rand(GLfloat min, GLfloat max) {
        const boost::uniform_real<GLfloat> randDist(min, max);
        boost::variate_generator< boost::mt19937&, boost::uniform_real<GLfloat> > randVar(randGen, randDist);
        return randVar();
    }
    
    GLfloat& getX(GLint i) { return dotPositions[i*verticesPerDot]; }
    GLfloat& getY(GLint i) { return dotPositions[i*verticesPerDot + 1]; }
    GLfloat& getDirection(GLint i) { return dotDirections[i]; }
    GLfloat& getAge(GLint i) { return dotAges[i]; }
    
    GLfloat newDirection() {
        if ((currentCoherence == 0.0f) || ((currentCoherence != 1.0f) && (rand(0.0f, 1.0f) > currentCoherence))) {
            return rand(0.0f, 360.0f);
        }
        return 0.0f;
    }
    
    GLfloat newAge() {
        if (currentLifetime != 0.0f) {
            return rand(0.0f, currentLifetime);
        }
        return 0.0f;
    }
    
    shared_ptr<Variable> fieldRadius;
    shared_ptr<Variable> fieldCenterX;
    shared_ptr<Variable> fieldCenterY;
    shared_ptr<Variable> dotDensity;
    shared_ptr<Variable> dotSize;
    shared_ptr<Variable> red;
    shared_ptr<Variable> green;
    shared_ptr<Variable> blue;
    shared_ptr<Variable> alpha;
    shared_ptr<Variable> direction;
    shared_ptr<Variable> speed;
    shared_ptr<Variable> coherence;
    shared_ptr<Variable> lifetime;
    shared_ptr<Variable> announceDots;
    
    std::vector<GLfloat> dotSizeToPixels;
    GLfloat currentFieldRadius;
    GLint numDots, previousNumDots;
    GLfloat currentCoherence;
    GLfloat currentLifetime;
    static const GLint verticesPerDot = 2;
    std::vector<GLfloat> dotPositions;
    std::vector<GLfloat> dotDirections;
    std::vector<GLfloat> dotAges;
    
    boost::mt19937 randGen;
    
    MWTime previousTime, currentTime;
    
};


#endif 























