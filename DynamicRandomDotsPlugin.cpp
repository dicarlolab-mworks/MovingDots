/*
 *  DynamicRandomDotsPlugin.cpp
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 8/6/10.
 *  Copyright 2010 MIT. All rights reserved.
 *
 */

#include "DynamicRandomDots.h"


class DynamicRandomDotsPlugin : public Plugin {
	void registerComponents(shared_ptr<ComponentRegistry> registry) MW_OVERRIDE {
        registry->registerFactory<StandardStimulusFactory, DynamicRandomDots>();
    }	
};


MW_SYMBOL_PUBLIC
extern "C" Plugin* getPlugin() {
    return new DynamicRandomDotsPlugin();
}
