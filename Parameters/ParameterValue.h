/*
 *  ParameterValue.h
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 3/21/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#ifndef ParameterValue_H_
#define ParameterValue_H_

#include <map>
#include <string>

#include <boost/lexical_cast.hpp>

#include <MWorksCore/ComponentRegistry.h>


typedef boost::shared_ptr<mw::Variable> VariablePtr;


class ParameterValue {
    
public:
    ParameterValue(const std::string &value, mw::ComponentRegistry *reg) :
        value(value),
        reg(reg)
    { }
    
    const std::string& getValue() const {
        return value;
    }
    
    template<typename Type>
    operator Type() const {
        return convert<Type>(value, reg);
    }
    
    template<typename Type>
    static Type convert(const std::string &s, mw::ComponentRegistry *reg);
    
private:
    std::string value;
    mw::ComponentRegistry *reg;
    
};


template<typename Type>
Type ParameterValue::convert(const std::string &s, mw::ComponentRegistry *reg) {
    Type val;
    
    try {
        val = boost::lexical_cast<Type>(s);
    } catch (boost::bad_lexical_cast &e) {
        val = Type(ParameterValue::convert<VariablePtr>(s, reg)->getValue());
    }
    
    return val;
}


template<>
const std::string& ParameterValue::convert(const std::string &s, mw::ComponentRegistry *reg);


template<>
VariablePtr ParameterValue::convert(const std::string &s, mw::ComponentRegistry *reg);


class ParameterValueMap {
    
    typedef std::map<std::string, ParameterValue> _ParameterValueMap;
    
public:
    void addValue(const std::string &name, const ParameterValue &value) {
        values.insert(std::make_pair(name, value));
    }
    
    const ParameterValue& operator [](const std::string &name) const {
        _ParameterValueMap::const_iterator iter = values.find(name);
        if (iter == values.end()) {
            // FIXME: this is an internal, programmer error and should be flagged as such
            throw mw::SimpleException("unknown parameter", name);
        }
        return (*iter).second;
    }
    
private:
    _ParameterValueMap values;
    
};


#endif























