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

#include "ParameterManifest.h"
#include "ParameterValue.h"


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


class BaseComponentFactory : public mw::ComponentFactory {
    
public:
    template< template<typename> class FactoryTemplate, typename ComponentType >
    static void registerFactory(boost::shared_ptr<mw::ComponentRegistry> registry) {
        registry->registerFactory(ComponentType::getSignature(),
                                  (mw::ComponentFactory *)(new FactoryTemplate<ComponentType>()));
    }
    
    const ParameterManifest& getParameterManifest() const {
        return manifest;
    }
    
    virtual bool isInternalParameter(const std::string &name) const {
        // Identify parameters added by the parser
        return ((name == "reference_id") ||
                (name == "type") ||
                (name == "variable_assignment") ||
                (name == "working_path") ||
                (name == "xml_document_path"));
    }

protected:
    ParameterManifest manifest;
    
};


typedef std::map<std::string, std::string> StdStringMap;


template<typename ComponentType>
class SelfDescribingComponentFactory : public BaseComponentFactory {

    //
    // ComponentType must implement the following methods:
    //
    // static std::string getSignature();
    // static void describeParameters(ParameterManifest &manifest);
    // ComponentType(const ParameterValueMap &parameters);
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

            if ((iter == infoMap.end()) && !isInternalParameter(name)) {
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






















