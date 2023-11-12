#import <AppKit/AppKit.h>

#import "gui.h"


namespace chomp {

class GUIWrapperDarwin : public GUIWrapper {
public:
  GUIWrapperDarwin(Plugin *_plugin);

  virtual bool Create(const char *api, bool is_floating);
  virtual void Destroy();
  virtual bool GetSize(uint32_t *width, uint32_t *height);
  virtual bool AdjustSize(uint32_t *width, uint32_t *height);
  virtual bool SetSize(uint32_t width, uint32_t height);
  virtual bool SetParent(const clap_window_t *window);
  virtual bool SetTransient(const clap_window_t *window);
  virtual bool Show();
  virtual bool Hide();

private:

  Plugin *plugin;
  NSView *rootView;
};

GUIWrapperDarwin::GUIWrapperDarwin(Plugin *_plugin) : plugin(_plugin) {
}

bool GUIWrapperDarwin::Create(const char *api, bool is_floating) {
  @autoreleasepool {

    NSBundle *bundle = [NSBundle bundleWithIdentifier:@"org.surge-synth-team.erachnid"];
    if (bundle == nil) {
      return false;
    }
    NSNib *nib = [[[NSNib alloc] autorelease] initWithNibNamed:@"Stuff" bundle:bundle];
    if (nib == nil) {
      return false;//@"couldn't load Stuff.nib from bundle";
    }
    NSArray* topLevelObjects;
    if (![nib instantiateWithOwner:nil topLevelObjects:&topLevelObjects]) {
      return false;//@"couldn't instantiate nib";
    }
    rootView = [topLevelObjects[0] retain];
  }
  return true;
}

void GUIWrapperDarwin::Destroy() {
  if (rootView != nil) {
    [rootView removeFromSuperview];
    [rootView release];
    rootView = nil;
  }
}

  bool GUIWrapperDarwin::GetSize(uint32_t *width, uint32_t *height) {
    *width = static_cast<uint32_t>(NSWidth(rootView.frame));
    *height = static_cast<uint32_t>(NSHeight(rootView.frame));
    return true;
  }

  bool GUIWrapperDarwin::AdjustSize(uint32_t *width, uint32_t *height) {
    return true;
  }

  bool GUIWrapperDarwin::SetSize(uint32_t width, uint32_t height) {
    rootView.frame = NSMakeRect(0, 0, width, height);
    return true;
  }

  bool GUIWrapperDarwin::SetParent(const clap_window_t *window) {
    NSView *parent = reinterpret_cast<NSView *>(window->cocoa);
    [parent addSubview:rootView];
    return true;
  }

  bool GUIWrapperDarwin::SetTransient(const clap_window_t *window) {
    return false;
  }

  bool GUIWrapperDarwin::Show() {
    rootView.hidden = false;
  }
  bool GUIWrapperDarwin::Hide() {
    rootView.hidden = true;
  }

/*
bool doSomethingWithView(void *_view) {
  NSView *view = reinterpret_cast<NSView *>(_view);
	NSBundle *bundle = [NSBundle bundleWithIdentifier:@"org.surge-synth-team.erachnid"];

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
	
}
*/

GUIWrapper* NewGUIWrapperDarwin(Plugin *plugin) {
  return new GUIWrapperDarwin(plugin);
}

}

