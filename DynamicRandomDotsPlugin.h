/*
 *  DynamicRandomDotsPlugin.h
 *  DynamicRandomDots
 *
 *  Created by Christopher Stawarz on 8/6/10.
 *  Copyright 2010 MIT. All rights reserved.
 *
 */
#ifndef DynamicRandomDots_PLUGIN_H_
#define DynamicRandomDots_PLUGIN_H_

#include <MWorksCore/Plugin.h>
using namespace mw;

extern "C"{
    Plugin *getPlugin();
}

class DynamicRandomDotsPlugin : public Plugin {
    
	virtual void registerComponents(shared_ptr<mw::ComponentRegistry> registry);	
};


#endif