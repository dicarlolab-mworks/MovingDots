/*
 *  ParameterManifest.h
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 3/21/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#ifndef ParameterManifest_H_
#define ParameterManifest_H_

#include <map>
#include <string>
#include <vector>

#include "ParameterInfo.h"


typedef std::vector<std::string> StdStringVector;
typedef std::map<std::string, ParameterInfo> ParameterInfoMap;


class ParameterManifest {
    
public:
    void addParameter(const std::string &name, const std::string &defaultValue) {
        addParameter(name, ParameterInfo(defaultValue));
    }
    
    void addParameter(const std::string &name, bool required = true) {
        addParameter(name, ParameterInfo(required));
    }
    
    void addParameter(const std::string &name, const ParameterInfo &info) {
        parameters[name] = info;
        if (info.isRequired()) {
            requiredParameters.push_back(name);
        }
    }
    
    const ParameterInfoMap& getParameters() const {
        return parameters;
    }
    
    const StdStringVector& getRequiredParameters() const {
        return requiredParameters;
    }
    
private:
    ParameterInfoMap parameters;
    StdStringVector requiredParameters;
    
};


#endif























