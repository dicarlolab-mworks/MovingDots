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


class ParameterValueMap : public std::map<std::string, ParameterValue> {
    
public:
    const ParameterValue& operator [](const std::string &name) const {
        ParameterValueMap::const_iterator iter = find(name);
        if (iter == end()) {
            // FIXME: this is an internal, programmer error and should be flagged as such
            throw mw::SimpleException("unknown parameter", name);
        }
        return (*iter).second;
    }
    
};


#endif






















