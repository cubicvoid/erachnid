#import <AppKit/AppKit.h>


#import "gui.h"

#import "gui_darwin.h"


namespace chomp {

GUIWrapper* GUIWrapper::New(Plugin *plugin) {
    // If we supported more than one platform, this is where we would
    // return the appropriate subclass of GUIWrapper.
    return NewGUIWrapperDarwin(plugin);
}

GUIWrapper::~GUIWrapper() { }

/*
bool doSomethingWithView(void *_view) {
    NSView *view = static_cast<NSView *>(_view);
	NSBundle *bundle = [NSBundle bundleWithIdentifier:@"org.surge-synth-team.erachnid"];
	//NSBundle *main = [NSBundle mainBundle];
	NSString *path = [main pathForResource:@"CocoaStuffLibBundle" ofType:@"bundle"];
	if (path == nil) {
		return false;//@"No path found for bundle";
	}
	NSBundle *bundle = [NSBundle bundleWithPath:path];
	if (bundle == nil) {
		return false;//[NSString stringWithFormat:@"couldn't open bundle with path %@", path];
	}
	NSNib *contents = [[NSNib alloc] initWithNibNamed:@"Stuff" bundle:bundle];
	if (contents == nil) {
		return false;//@"couldn't load Stuff.nib from bundle";
	}
	NSArray* topLevelObjects;
	if (![contents instantiateWithOwner:view topLevelObjects:&topLevelObjects]) {
		return false;//@"couldn't instantiate nib";
	}
	NSView *rootView = topLevelObjects[0];
	rootView.frame = view.bounds;
	rootView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
	rootView.autoresizesSubviews = true;
	[view addSubview:rootView];
	return true;//@"loaded Stuff.nib";

	//NSNib *contents = [NSNib initWithNibNamed:@"contents"];
}*/

}

