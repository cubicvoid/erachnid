#import <AppKit/AppKit.h>

#import "gui.h"

#import "chomp_impl.hh"

namespace chomp {

class GUIWrapperDarwin : public GUIWrapper {
public:
  GUIWrapperDarwin(Plugin *_plugin);

  virtual bool Create(const char *api, bool is_floating);
  virtual void Destroy();
  virtual bool SetScale(double scale);
  virtual bool GetSize(uint32_t *width, uint32_t *height);
  virtual bool CanResize();
  virtual bool AdjustSize(uint32_t *width, uint32_t *height);
  virtual bool SetSize(uint32_t width, uint32_t height);
  virtual bool SetParent(const clap_window_t *window);
  virtual bool SetTransient(const clap_window_t *window);
  virtual bool Show();
  virtual bool Hide();

private:

  Plugin *plugin;
  NSView *rootView;

  uint32_t width;
  uint32_t height;

};

GUIWrapperDarwin::GUIWrapperDarwin(Plugin *_plugin) :
  plugin(_plugin), rootView(nullptr), width(500), height(300) {
}

bool GUIWrapperDarwin::Create(const char *api, bool is_floating) {
  plugin->Log("gui_create(%s, %d)", api, static_cast<int>(is_floating));
    NSBundle *bundle = [NSBundle bundleWithIdentifier:@"org.surge-synth-team.erachnid"];
    if (bundle == nil) {
      plugin->Log("couldn't load plugin bundle");
      return false;
    }
    NSNib *nib = [[[NSNib alloc] initWithNibNamed:@"Stuff" bundle:bundle] autorelease];
    if (nib == nil) {
      plugin->Log("couldn't load Stuff.nib from bundle");
      return false;//@"couldn't load Stuff.nib from bundle";
    }
    NSArray* topLevelObjects;
    if (![nib instantiateWithOwner:nil topLevelObjects:&topLevelObjects]) {
      plugin->Log("Couldn't instantiate Stuff.nib");
      return false;//@"couldn't instantiate nib";
    }
    plugin->Log("Stuff.nib instantiated with %d objects", static_cast<int>(topLevelObjects.count));
    NSString *c1 = NSStringFromClass([topLevelObjects[0] class]);
    NSString *c2 = NSStringFromClass([topLevelObjects[1] class]);

    for (id obj in topLevelObjects) {
      if ([obj isKindOfClass:[NSView class]]) {
        rootView = [obj retain];
        NSRect frame = rootView.frame;
        plugin->Log("rootView: %x  (width: %d, height: %d)",
          rootView, static_cast<int>(NSWidth(frame), static_cast<int>(NSHeight(frame))));
      }
    }

    //[topLevelObjects retain];
    plugin->Log("obj 0 (%x): %s, obj 1 (%x): %s",
      topLevelObjects[0], [c1 UTF8String], topLevelObjects[1], [c2 UTF8String]
      //static_cast<int>([topLevelObjects[0] isKindOfClass:[NSView class]]),
      //static_cast<int>([topLevelObjects[1] isKindOfClass:[NSView class]])
    );
    //rootView = [topLevelObjects[1] retain];
    //[rootView retain];
    // NSRect frame = rootView.frame;
    // plugin->Log("rootView: %x  (width: %d, height: %d)",
    //   rootView, static_cast<int>(NSWidth(frame), static_cast<int>(NSHeight(frame))));
  //@autoreleasepool {

    /*NSObject *owner = [[NSObject alloc] init];


    NSBundle *bundle = [NSBundle bundleWithIdentifier:@"org.surge-synth-team.erachnid"];
    if (bundle == nil) {
      plugin->Log("couldn't load plugin bundle");
      return false;
    }
    NSNib *nib = [[NSNib alloc] initWithNibNamed:@"Stuff" bundle:bundle];
    if (nib == nil) {
      plugin->Log("couldn't load Stuff.nib from bundle");
      return false;//@"couldn't load Stuff.nib from bundle";
    }
    NSArray* topLevelObjects;
    if (![nib instantiateWithOwner:owner topLevelObjects:&topLevelObjects]) {
      plugin->Log("Couldn't instantiate Stuff.nib");
      return false;//@"couldn't instantiate nib";
    }
    [topLevelObjects retain];
    rootView = topLevelObjects[0];
    [rootView retain];
    plugin->Log("rootView: %x", rootView);
  //}
  plugin->Log("gui_create successful");
  */
  return true;
}

void GUIWrapperDarwin::Destroy() {
  plugin->Log("gui_destroy() (this=%x, rootView=%x)", this, rootView);
  if (rootView != nil) {
    [rootView removeFromSuperview];
    [rootView release];
    rootView = nil;
  }
  //@autoreleasepool {
    /*plugin->Log("what even is happening");
    if (rootView != nil) {
      plugin->Log("about to check if superview is nil");
      if (rootView.superview != nil) {
        plugin->Log("trying to remove from superview");
        [rootView removeFromSuperview];
      } else {
        plugin->Log("superview was nil");
      }
      plugin->Log("skipping release");//"trying to release");
      //[rootView release];
      plugin->Log("resetting rootView to nil");
      rootView = nil;
    }
    plugin->Log("gui_destroy finished (this=%x, rootView=%x)", this, rootView);
  //}
  */
}

bool GUIWrapperDarwin::SetScale(double scale) {
  plugin->Log("gui_set_scale(%lf) -> 1", scale);
  return true;
}

bool GUIWrapperDarwin::GetSize(uint32_t *width, uint32_t *height) {
  // *width = this->width;
  // *height = this->height;
  // plugin->Log("gui_get_size() -> width: %d, height: %d", *width, *height);
  // return true;
  if (rootView != nullptr) {
    NSRect frame = rootView.frame;
    *width = static_cast<uint32_t>(NSWidth(frame));
    *height = static_cast<uint32_t>(NSHeight(frame));
    plugin->Log("gui_get_size() -> width: %d, height: %d", *width, *height);
    return true;
  } else {
    plugin->Log("gui_get_size() -> false");
    return false;
  }
}

bool GUIWrapperDarwin::CanResize() {
  plugin->Log("gui_can_resize() -> 0");
  return false;
  // bool result = (rootView != nullptr);
  // plugin->Log("gui_can_resize() -> %d (this=%x, rootView=%x)", static_cast<int>(result), this, rootView);
  // return result;
}

bool GUIWrapperDarwin::AdjustSize(uint32_t *width, uint32_t *height) {
  plugin->Log("gui_adjust_size(width: %d, height: %d) -> 1", *width, *height);
  return true;
}

bool GUIWrapperDarwin::SetSize(uint32_t width, uint32_t height) {
  plugin->Log("gui_set_size(width: %d, height: %d) -> 0", width, height);
  return false;
  // this->width = width;
  // this->height = height;
  // return true;
  // if (rootView != nullptr) {
  //   plugin->Log("gui_set_size(width: %d, height: %d) -> 1", width, height);
  //   //rootView.frame = NSMakeRect(0, 0, width, height);
  //   return true;
  // }
  // plugin->Log("gui_set_size(width: %d, height: %d) -> 0", width, height);
  // return false;
}

bool GUIWrapperDarwin::SetParent(const clap_window_t *window) {
  // plugin->Log("gui_set_parent(%x) -> 1", window->cocoa);
  // return true;
  NSView *container = reinterpret_cast<NSView *>(window->cocoa);
  if (rootView != nullptr) {
    plugin->Log("gui_set_parent(%x) -> 1", container);
    [container addSubview:rootView];
    return true;
  }
  plugin->Log("gui_set_parent(%x) -> 0", container);
  return false;
}

bool GUIWrapperDarwin::SetTransient(const clap_window_t *window) {
  plugin->Log("gui_set_transient()");
  return false;
}

bool GUIWrapperDarwin::Show() {
  // plugin->Log("gui_show() -> 1");
  // return true;
  if (rootView != nullptr) {
    plugin->Log("gui_show() -> 1");
    rootView.hidden = false;
    return true;
  }
  plugin->Log("gui_show() -> 0");
  return false;
}

bool GUIWrapperDarwin::Hide() {
  //   plugin->Log("gui_hide() -> 1");
  // return true;
  if (rootView != nullptr) {
    plugin->Log("gui_hide() -> 1");
    rootView.hidden = true;
    return true;
  }
  plugin->Log("gui_hide() -> 0");
  return false;
}

GUIWrapper* NewGUIWrapperDarwin(Plugin *plugin) {
  return new GUIWrapperDarwin(plugin);
}

}

