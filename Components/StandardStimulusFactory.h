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
class SelfDescribingStimulusFactory : public StandardComponentFactory<StimulusType> {
    
public:
    virtual boost::shared_ptr<mw::Component> createObject(StdStringMap parameters, mw::ComponentRegistry *reg) {
        boost::shared_ptr<StimulusType> newComponent(boost::dynamic_pointer_cast<StimulusType>(StandardComponentFactory<StimulusType>::createObject(parameters, reg)));
        
        newComponent->load(mw::StimulusDisplay::getCurrentStimulusDisplay());
        boost::shared_ptr<mw::StimulusNode> node(new mw::StimulusNode(newComponent));
        reg->registerStimulusNode(parameters["tag"], node);
        
        return newComponent;
    }
    
};


#endif
