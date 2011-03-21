/*
 *  SelfDescribingComponent.h
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 12/13/10.
 *  Copyright 2010 MIT. All rights reserved.
 *
 */

#ifndef SelfDescribingComponent_H_
#define SelfDescribingComponent_H_

#include <MWorksCore/ComponentFactory.h>
#include <MWorksCore/ComponentRegistry.h>

#include <boost/lexical_cast.hpp>


class UnknownAttributeException : public mw::ComponentFactoryException {

public:
    UnknownAttributeException(const std::string &referenceID, const std::string &attributeName) :
        mw::ComponentFactoryException(referenceID)
    {
        _what = "Unknown attribute: \"" + attributeName + "\"";
    }

    virtual ~UnknownAttributeException() throw() { }

    virtual const char* what() const throw() {
        return _what.c_str();
    }

};


class ParameterInfo {
    
public:
    
    explicit ParameterInfo(bool required = true) :
        required(required)
    { }

    explicit ParameterInfo(const std::string &defaultValue) :
        required(true),
        defaultValue(defaultValue)
    { }
    
    bool isRequired() const { return required; }
    const std::string& getDefaultValue() const { return defaultValue; }
    
private:
    bool required;
    std::string defaultValue;
    
};


typedef boost::shared_ptr<mw::Variable> VariablePtr;
typedef std::vector<std::string> StdStringVector;
typedef std::map<std::string, std::string> StdStringMap;
typedef std::map<std::string, ParameterInfo> ParameterInfoMap;


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


class BaseComponentFactory : public mw::ComponentFactory {
    
public:
    BaseComponentFactory() {
        // Register parameters added/used solely by the parser
        manifest.addParameter("reference_id", false);
        manifest.addParameter("type", false);
        manifest.addParameter("variable_assignment", false);
        manifest.addParameter("working_path", false);
        manifest.addParameter("xml_document_path", false);
    }

    const ParameterManifest& getParameterManifest() const {
        return manifest;
    }
    
protected:
    ParameterManifest manifest;
    
};


template<typename ComponentType>
class SelfDescribingComponentFactory : public BaseComponentFactory {

    //
    // ComponentType must implement the following methods:
    //
    // static void describeParameters(ParameterManifest &manifest);
    // ComponentType(ParameterValueMap &parameters);
    //
    
public:
    SelfDescribingComponentFactory() {
        ComponentType::describeParameters(manifest);
    }
    
    virtual boost::shared_ptr<mw::Component> createObject(StdStringMap parameters, mw::ComponentRegistry *reg) {
        requireAttributes(parameters, manifest.getRequiredParameters());

        const ParameterInfoMap &infoMap = manifest.getParameters();
        ParameterValueMap values;

        for (StdStringMap::iterator param = parameters.begin(); param != parameters.end(); param++) {
            const std::string &name = (*param).first;
            ParameterInfoMap::const_iterator iter = infoMap.find(name);

            if (iter == infoMap.end()) {
                std::string referenceID("<unknown object>");
                if (parameters.find("reference_id") != parameters.end()) {
                    referenceID = parameters["reference_id"];
                }
                throw UnknownAttributeException(referenceID, name);
            }

            const std::string &value = (*param).second;
            const ParameterInfo &info = (*iter).second;

            values.addValue(name, ParameterValue(value, reg));
        }

        return boost::shared_ptr<ComponentType>(new ComponentType(values));
    }

};


template<typename StimulusType>
class SelfDescribingStimulusFactory : public SelfDescribingComponentFactory<StimulusType> {
    
public:
    virtual boost::shared_ptr<mw::Component> createObject(StdStringMap parameters, mw::ComponentRegistry *reg) {
        boost::shared_ptr<StimulusType> newComponent(boost::dynamic_pointer_cast<StimulusType>(SelfDescribingComponentFactory<StimulusType>::createObject(parameters, reg)));
        
        newComponent->load(mw::StimulusDisplay::getCurrentStimulusDisplay());
        boost::shared_ptr<mw::StimulusNode> node(new mw::StimulusNode(newComponent));
        reg->registerStimulusNode(parameters["tag"], node);
        
        return newComponent;
    }
    
};


#endif






















