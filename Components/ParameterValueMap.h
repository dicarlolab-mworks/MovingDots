/*
 *  ParameterValueMap.h
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 3/22/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#ifndef ParameterValueMap_H_
#define ParameterValueMap_H_

#include <map>
#include <string>

#include "ParameterValue.h"


class ParameterValueMap {
    
public:
    void addValue(const std::string &name, const ParameterValue &value) {
        values.insert(std::make_pair(name, value));
    }
    
    const ParameterValue& operator [](const std::string &name) const {
        std::map<std::string, ParameterValue>::const_iterator iter = values.find(name);
        if (iter == values.end()) {
            // FIXME: this is an internal, programmer error and should be flagged as such
            throw mw::SimpleException("unknown parameter", name);
        }
        return (*iter).second;
    }
    
private:
    std::map<std::string, ParameterValue> values;
    
};


#endif






















