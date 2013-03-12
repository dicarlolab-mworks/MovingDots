/*
 *  MovingDotsPlugin.cpp
 *  MovingDots
 *
 *  Created by Christopher Stawarz on 8/6/10.
 *  Copyright 2010 MIT. All rights reserved.
 *
 */

#include "MovingDots.h"


class MovingDotsPlugin : public Plugin {
	void registerComponents(shared_ptr<ComponentRegistry> registry) MW_OVERRIDE {
        registry->registerFactory<StandardStimulusFactory, MovingDots>();
    }	
};


MW_SYMBOL_PUBLIC
extern "C" Plugin* getPlugin() {
    return new MovingDotsPlugin();
}
