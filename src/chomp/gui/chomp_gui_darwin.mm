#import <AppKit/AppKit.h>

//#import "gui.hh"

#import "chomp_plugin.hh"

#if __has_feature(objc_arc)
#error "ARC is on"
#endif

namespace erachnid::chomp {
  class ChompPluginDarwin;
}

@interface FAESimpleClass : NSObject {
  erachnid::chomp::ChompPlugin *_plugin;
}

@property (nonatomic, retain) IBOutlet NSView *view;

- (IBAction)buttonPressed:(id)sender;

@end

@implementation FAESimpleClass

- (id)initWithPlugin:(erachnid::chomp::ChompPlugin *)plugin {
  self = [super init];
  if (self != nil) {
    _plugin = plugin;
  }
  return self;
}

- (IBAction)buttonPressed:(id)sender {
  if (_plugin != nullptr) {
    _plugin->Log("buttonPressed");
  }
}
  
@end


namespace erachnid::chomp {



class ChompPluginDarwin : public ChompPlugin {
public:
  ChompPluginDarwin(const clap_host_t *_host);

  virtual bool GUIEnabled() { return true; }
  virtual bool GUIIsAPISupported(const char *api, bool is_floating);
  virtual bool GUIGetPreferredAPI(const char **api, bool *is_floating);
  virtual bool GUICreate(const char *api, bool is_floating);
  virtual void GUIDestroy();
  virtual bool GUISetScale(double scale);
  virtual bool GUIGetSize(uint32_t *width, uint32_t *height);
  virtual bool GUICanResize();
  virtual bool GUIAdjustSize(uint32_t *width, uint32_t *height);
  virtual bool GUISetSize(uint32_t width, uint32_t height);
  virtual bool GUISetParent(const clap_window_t *window);
  virtual bool GUISetTransient(const clap_window_t *window);
  virtual bool GUIShow();
  virtual bool GUIHide();

private:

  //ChompPlugin *plugin;
  //NSView *rootView;
  FAESimpleClass *controller;

  uint32_t width;
  uint32_t height;

};

ChompPluginDarwin::ChompPluginDarwin(const clap_host_t *_host) : ChompPlugin(_host),
  /*rootView(nil),*/ controller(nil),
  width(500), height(300)
{
  int value;
  Log("ChompPluginDarwin(%lx)  (this: %lx, stack: %lx)",
    reinterpret_cast<long>(_host), reinterpret_cast<long>(this),
    reinterpret_cast<long>(&value));

}

bool ChompPluginDarwin::GUIIsAPISupported(const char *api, bool is_floating) {
  Log("GUIIsAPISupported(%s, %d)", api, static_cast<int>(is_floating));
  return strcmp(api, CLAP_WINDOW_API_COCOA) == 0 && !is_floating;
}

bool ChompPluginDarwin::GUIGetPreferredAPI(const char **api, bool *is_floating) {
  *api = CLAP_WINDOW_API_COCOA;
  *is_floating = false;
  return true;
}

bool ChompPluginDarwin::GUICreate(const char *api, bool is_floating) {
  int value;
  Log("gui_create(%s, %d)  (this: %lx, stack: %lx)",
    api, static_cast<int>(is_floating),
    reinterpret_cast<long>(this), reinterpret_cast<long>(&value));
  NSBundle *bundle = [NSBundle bundleWithIdentifier:@"me.faec.erachnid"];
  if (bundle == nil) {
    Log("couldn't load plugin bundle");
    return false;
  } else {
    Log("got bundle %lx", reinterpret_cast<long>(bundle));
  }
  NSNib *nib = [NSNib alloc];
	NSLog(@"allocated nib: %lx", reinterpret_cast<long>(nib));
  Log("allocated nib: %lx", reinterpret_cast<long>(nib));
  bool responds = [nib respondsToSelector:@selector(initWithNibNamed:bundle:)];
  Log("responds to initWithNibNamed:bundle:  %d", static_cast<int>(responds));
  nib = [[[NSNib alloc] initWithNibNamed:@"Stuff" bundle:bundle] autorelease];
  //nib = [nib initWithNibNamed:@"lskjdfkldsjf" bundle:nil];
  if (nib == nil) {
    Log("couldn't load Stuff.nib from bundle");
    return false;//@"couldn't load Stuff.nib from bundle";
  } else {
    Log("loaded Stuff.nib: %lx", reinterpret_cast<long>(nib));
  }

  controller = [[FAESimpleClass alloc] initWithPlugin:this];

  NSArray* topLevelObjects;
  if (![nib instantiateWithOwner:controller topLevelObjects:&topLevelObjects]) {
    Log("Couldn't instantiate Stuff.nib");
    return false;//@"couldn't instantiate nib";
  }
  Log("Stuff.nib instantiated with %d objects", static_cast<int>(topLevelObjects.count));
  return true;
}

void ChompPluginDarwin::GUIDestroy() {
  Log("gui_destroy() (this=%lx, controller=%lx)", reinterpret_cast<long>(this), controller);
  if (controller != nil) {
    [controller.view removeFromSuperview];
    [controller release];
    controller = nil;
  }
  //@autoreleasepool {
    /*Log("what even is happening");
    if (rootView != nil) {
      Log("about to check if superview is nil");
      if (rootView.superview != nil) {
        Log("trying to remove from superview");
        [rootView removeFromSuperview];
      } else {
        Log("superview was nil");
      }
      Log("skipping release");//"trying to release");
      //[rootView release];
      Log("resetting rootView to nil");
      rootView = nil;
    }
    Log("gui_destroy finished (this=%lx, rootView=%lx)", this, rootView);
  //}
  */
}

bool ChompPluginDarwin::GUISetScale(double scale) {
  Log("gui_set_scale(%lf) -> 1", scale);
  return true;
}

bool ChompPluginDarwin::GUIGetSize(uint32_t *width, uint32_t *height) {
  // *width = this->width;
  // *height = this->height;
  // Log("gui_get_size() -> width: %d, height: %d", *width, *height);
  // return true;
  if (controller != nil) {
    NSRect frame = controller.view.frame;
    *width = static_cast<uint32_t>(NSWidth(frame));
    *height = static_cast<uint32_t>(NSHeight(frame));
    Log("gui_get_size() -> width: %d, height: %d", *width, *height);
    return true;
  } else {
    Log("gui_get_size() -> false");
    return false;
  }
}

bool ChompPluginDarwin::GUICanResize() {
  bool result = (controller != nil);
  Log("gui_can_resize() -> %d", static_cast<int>(result));
  return result;
}

bool ChompPluginDarwin::GUIAdjustSize(uint32_t *width, uint32_t *height) {
  Log("gui_adjust_size(width: %d, height: %d) -> 1", *width, *height);
  return true;
}

bool ChompPluginDarwin::GUISetSize(uint32_t width, uint32_t height) {
  //Log("gui_set_size(width: %d, height: %d) -> 0", width, height);
  this->width = width;
  this->height = height;
  // return true;
  if (controller != nil) {
    Log("gui_set_size(width: %d, height: %d) -> 1", width, height);
    controller.view.frame = NSMakeRect(0, 0, width, height);
    return true;
  }
  Log("gui_set_size(width: %d, height: %d) -> 0", width, height);
  return false;
}

bool ChompPluginDarwin::GUISetParent(const clap_window_t *window) {
  // Log("gui_set_parent(%lx) -> 1", window->cocoa);
  // return true;
  NSView *container = reinterpret_cast<NSView *>(window->cocoa);
  if (controller != nullptr) {
    Log("gui_set_parent(%lx) -> 1", container);
    [container addSubview:controller.view];
    return true;
  }
  Log("gui_set_parent(%lx) -> 0", container);
  return false;
}

bool ChompPluginDarwin::GUISetTransient(const clap_window_t *window) {
  Log("gui_set_transient()");
  return false;
}

bool ChompPluginDarwin::GUIShow() {
  // Log("gui_show() -> 1");
  // return true;
  if (controller != nullptr) {
    Log("gui_show() -> 1");
    controller.view.hidden = false;
    return true;
  }
  Log("gui_show() -> 0");
  return false;
}

bool ChompPluginDarwin::GUIHide() {
  //   Log("gui_hide() -> 1");
  // return true;
  if (controller != nil) {
    Log("gui_hide() -> 1");
    controller.view.hidden = true;
    return true;
  }
  Log("gui_hide() -> 0");
  return false;
}

ChompPlugin* NewChompPluginDarwin(const clap_host_t *host) {
  return new ChompPluginDarwin(host);
}

}

