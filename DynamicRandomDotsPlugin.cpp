/*
 *  DynamicRandomDotsPlugin.cpp
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 8/6/10.
 *  Copyright 2010 MIT. All rights reserved.
 *
 */

#include "DynamicRandomDotsPlugin.h"
#include "DynamicRandomDotsFactory.h"
#include "MWorksCore/ComponentFactory.h"
using namespace mw;

Plugin *getPlugin(){
    return new DynamicRandomDotsPlugin();
}


void DynamicRandomDotsPlugin::registerComponents(shared_ptr<mw::ComponentRegistry> registry) {
	
    // TODO: you need to customize the "signature" of the object your plugin will create
    //       The signature is of the form component/type Ð(e.g. stimulus/circle, or iodevice/NIDAQ)
    registry->registerFactory(std::string("stimulus/DynamicRandomDots"),
							  (ComponentFactory *)(new DynamicRandomDotsFactory()));
}

