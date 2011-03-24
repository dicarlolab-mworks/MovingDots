/*
 *  StandardStimulusFactory.h
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 3/22/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#ifndef StandardStimulusFactory_H_
#define StandardStimulusFactory_H_

#include "StandardComponentFactory.h"


template<typename StimulusType>
class StandardStimulusFactory : public StandardComponentFactory<StimulusType> {
    
public:
    virtual boost::shared_ptr<mw::Component> createObject(StdStringMap parameters, mw::ComponentRegistry *reg) {
        ParameterValueMap values;
        boost::shared_ptr<StimulusType> stim(StandardComponentFactory<StimulusType>::createObject(parameters,
                                                                                                  reg,
                                                                                                  values));
        
        stim->load(mw::StimulusDisplay::getCurrentStimulusDisplay());
        boost::shared_ptr<mw::StimulusNode> node(new mw::StimulusNode(stim));
        reg->registerStimulusNode(values.at("tag"), node);
        
        return stim;
    }
    
};


#endif
