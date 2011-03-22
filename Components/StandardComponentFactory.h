/*
 *  StandardComponentFactory.h
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 12/13/10.
 *  Copyright 2010 MIT. All rights reserved.
 *
 */

#ifndef StandardComponentFactory_H_
#define StandardComponentFactory_H_

#include <MWorksCore/ComponentFactory.h>

#include "ComponentInfo.h"
#include "ParameterValueMap.h"


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
        FactoryTemplate<ComponentType> *factory = new FactoryTemplate<ComponentType>();
        registry->registerFactory(factory->getComponentInfo().getSignature(), (mw::ComponentFactory *)factory);
    }
    
    const ComponentInfo& getComponentInfo() const {
        return info;
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
    ComponentInfo info;
    
};


typedef std::map<std::string, std::string> StdStringMap;


template<typename ComponentType>
class StandardComponentFactory : public BaseComponentFactory {

    //
    // ComponentType must implement the following methods:
    //
    // static void describeComponent(ComponentInfo &info);
    // ComponentType(const ParameterValueMap &parameters);
    //
    
public:
    StandardComponentFactory() {
        ComponentType::describeComponent(info);
    }
    
    virtual boost::shared_ptr<mw::Component> createObject(StdStringMap parameters, mw::ComponentRegistry *reg) {
        requireAttributes(parameters, info.getRequiredParameters());

        const ParameterInfoMap &infoMap = info.getParameters();
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


#endif






















