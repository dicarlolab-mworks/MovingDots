/*
 *  ParameterValue.cpp
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 3/21/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#include "ParameterValue.h"


template<>
const std::string& ParameterValue::convert(const std::string &s, mw::ComponentRegistry *reg) {
    return s;
}


template<>
VariablePtr ParameterValue::convert(const std::string &s, mw::ComponentRegistry *reg) {
    return reg->getVariable(s);
}
