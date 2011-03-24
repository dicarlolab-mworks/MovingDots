/*
 *  DynamicRandomDotsPlugin.cpp
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 8/6/10.
 *  Copyright 2010 MIT. All rights reserved.
 *
 */

#include <MWorksCore/Plugin.h>
#include <MWorksCore/StandardStimulusFactory.h>

#include "DynamicRandomDots.h"

using namespace mw;


class DynamicRandomDotsPlugin : public Plugin {
	virtual void registerComponents(shared_ptr<ComponentRegistry> registry) {
        registry->registerFactory<StandardStimulusFactory, DynamicRandomDots>();
    }	
};


extern "C" Plugin* getPlugin() {
    return new DynamicRandomDotsPlugin();
}
