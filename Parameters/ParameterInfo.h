/*
 *  ParameterInfo.h
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 3/21/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#ifndef ParameterInfo_H_
#define ParameterInfo_H_

#include <string>


class ParameterInfo {
    
public:
    explicit ParameterInfo(bool required = true) :
        required(required)
    { }
    
    explicit ParameterInfo(const std::string &defaultValue) :
        required(true),
        defaultValue(defaultValue)
    { }
    
    bool isRequired() const {
        return required;
    }

    const std::string& getDefaultValue() const {
        return defaultValue;
    }
    
private:
    bool required;
    std::string defaultValue;
    
};


#endif























