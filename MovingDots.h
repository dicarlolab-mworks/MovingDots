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
    void validateParameters() const;
    void computeDotSizeInPixels(shared_ptr<StimulusDisplay> display);
    void initializeDots();
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
        if ((coherence == 0.0f) || ((coherence != 1.0f) && (rand(0.0f, 1.0f) > coherence))) {
            return rand(0.0f, 360.0f);
        }
        return direction;
    }
    
    GLfloat newAge() {
        if (lifetime != 0.0f) {
            return rand(0.0f, lifetime);
        }
        return 0.0f;
    }
    
    const GLfloat fieldRadius;
    shared_ptr<Variable> fieldCenterX;
    shared_ptr<Variable> fieldCenterY;
    const GLfloat dotDensity;
    const GLfloat dotSize;
    shared_ptr<Variable> red;
    shared_ptr<Variable> green;
    shared_ptr<Variable> blue;
    shared_ptr<Variable> alpha;
    const GLfloat directionInDegrees;
    const GLfloat direction;
    shared_ptr<Variable> speed;
    const GLfloat coherence;
    const GLfloat lifetime;
    shared_ptr<Variable> announceDots;
    
    std::vector<GLfloat> dotSizeInPixels;
    GLint numDots;
    static const GLint verticesPerDot = 2;
    std::vector<GLfloat> dotPositions;
    std::vector<GLfloat> dotDirections;
    std::vector<GLfloat> dotAges;

    boost::mt19937 randGen;
    
    MWTime previousTime, currentTime;
    
};


#endif 























