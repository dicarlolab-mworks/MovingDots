MWorks Dynamic Random Dots Stimulus
===================================


How to Build
------------

1. Download and install the [MWorks nightly
   build](http://dicarlo-mwdev.mit.edu/mw/MWorks-NIGHTLY.dmg).  Note
   that you need to perform a custom installation and select the
   "Development files" component, which is not installed by default.

2. Open `DynamicRandomDots.xcodeproj` in
   [Xcode](http://developer.apple.com/technologies/tools/xcode.html)
   and build the "Everything" target.

3. To verify that the plugin works, try running one of the included
   example experiments.


Customization
-------------

In the current implementation, the dots are distributed uniformly
throughout the field, with 100% coherence and infinite lifetime.  If
you want to make the dots' behavior more complicated, the primary
methods of interest are `DynamicRandomDots::initializeDots`, which
sets the initial dot positions, and `DynamicRandomDots::updateDots`,
which updates the dot positions at each frame.
