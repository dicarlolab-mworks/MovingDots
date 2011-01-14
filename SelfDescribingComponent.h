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

#define PARAMETER_NAME(name, value)  static const std::string name(value)


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
    explicit ParameterInfo(bool required = true, bool getVariable = true) :
        required(required),
        getVariable(getVariable)
    { }

    explicit ParameterInfo(const std::string &defaultValue, bool getVariable = true) :
        required(true),
        defaultValue(defaultValue),
        getVariable(getVariable)
    { }
    
    bool isRequired() const { return required; }
    const std::string& getDefaultValue() const { return defaultValue; }
    bool shouldGetVariable() const { return getVariable; }
    
private:
    bool required;
    std::string defaultValue;
    bool getVariable;
    
};


typedef std::vector<std::string> StdStringVector;
typedef std::map<std::string, std::string> StdStringMap;
typedef std::map<std::string, ParameterInfo> ParameterInfoMap;
typedef std::map< std::string, boost::shared_ptr<mw::Variable> > MWVariableMap;


class ParameterManifest {
    
public:
    void addParameter(const std::string &name, const std::string &defaultValue, bool getVariable = true) {
        addParameter(name, ParameterInfo(defaultValue, getVariable));
    }
    
    void addParameter(const std::string &name, bool required = true, bool getVariable = true) {
        addParameter(name, ParameterInfo(required, getVariable));
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


template<typename ComponentType>
class SelfDescribingComponentFactory : public mw::ComponentFactory {

    //
    // ComponentType must implement the following methods:
    //
    // static void describeParameters(ParameterManifest &manifest);
    // ComponentType(StdStringMap &parameters, MWVariableMap &variables, mw::ComponentRegistry *reg);
    //
    
public:
    virtual const ParameterManifest& getParameterManifest() {
        static ParameterManifest manifest;
        static bool initialized = false;
        
        if (!initialized) {
            manifest.addParameter("reference_id", false, false);
            manifest.addParameter("type", false, false);
            manifest.addParameter("variable_assignment", false, false);
            manifest.addParameter("working_path", false, false);
            manifest.addParameter("xml_document_path", false, false);
            ComponentType::describeParameters(manifest);
            initialized = true;
        }
        
        return manifest;
    }
    
    virtual boost::shared_ptr<mw::Component> createObject(StdStringMap parameters, mw::ComponentRegistry *reg) {
        const ParameterManifest &manifest = getParameterManifest();
        requireAttributes(parameters, manifest.getRequiredParameters());

        const ParameterInfoMap &infoMap = manifest.getParameters();
        MWVariableMap variables;

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
            
            if (info.shouldGetVariable()) {
                boost::shared_ptr<mw::Variable> var(reg->getVariable(value));
                checkAttribute(var, parameters["reference_id"], name, value);
                variables[name] = var;
            }
        }

        return boost::shared_ptr<ComponentType>(new ComponentType(parameters, variables, reg));
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






















