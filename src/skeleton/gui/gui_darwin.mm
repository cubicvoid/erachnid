#import <AppKit/AppKit.h>

#import "plugin.hh"

#if __has_feature(objc_arc)
#error "ARC is on"
#endif

namespace erachnid::skeleton {
  class PluginDarwin;
}

@interface FAESimpleClass : NSObject {
  erachnid::skeleton::Plugin *_plugin;
}

@property (nonatomic, retain) IBOutlet NSView *view;

- (IBAction)buttonPressed:(id)sender;

@end

@implementation FAESimpleClass

- (id)initWithPlugin:(erachnid::skeleton::Plugin *)plugin {
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


namespace erachnid::skeleton {



class PluginDarwin : public Plugin {
public:
  PluginDarwin(const clap_host_t *_host);

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

  FAESimpleClass *controller;

  uint32_t width;
  uint32_t height;
};

PluginDarwin::PluginDarwin(const clap_host_t *_host) : Plugin(_host),
  /*rootView(nil),*/ controller(nil),
  width(500), height(300)
{
}

bool PluginDarwin::GUIIsAPISupported(const char *api, bool is_floating) {
  return strcmp(api, CLAP_WINDOW_API_COCOA) == 0 && !is_floating;
}

bool PluginDarwin::GUIGetPreferredAPI(const char **api, bool *is_floating) {
  *api = CLAP_WINDOW_API_COCOA;
  *is_floating = false;
  return true;
}

bool PluginDarwin::GUICreate(const char *api, bool is_floating) {
  int value;
  NSBundle *bundle = [NSBundle bundleWithIdentifier:@"me.faec.erachnid"];
  if (bundle == nil) {
    //Log("couldn't load plugin bundle");
    return false;
  }
  NSNib *nib = [[[NSNib alloc] initWithNibNamed:@"Stuff" bundle:bundle] autorelease];
  if (nib == nil) {
    //Log("couldn't load Stuff.nib from bundle");
    return false;
  }
  controller = [[FAESimpleClass alloc] initWithPlugin:this];

  NSArray* topLevelObjects;
  if (![nib instantiateWithOwner:controller topLevelObjects:&topLevelObjects]) {
    //Log("Couldn't instantiate Stuff.nib");
    return false;
  }
  NSLog(@"Stuff.nib instantiated with %d objects", static_cast<int>(topLevelObjects.count));
  return true;
}

void PluginDarwin::GUIDestroy() {
  NSLog(@"gui_destroy() (this=%lx, controller=%lx)", reinterpret_cast<long>(this), controller);
  if (controller != nil) {
    [controller.view removeFromSuperview];
    [controller release];
    controller = nil;
  }
}

bool PluginDarwin::GUISetScale(double scale) {
  NSLog(@"gui_set_scale(%lf) -> 1", scale);
  return true;
}

bool PluginDarwin::GUIGetSize(uint32_t *width, uint32_t *height) {
  // *width = this->width;
  // *height = this->height;
  // Log("gui_get_size() -> width: %d, height: %d", *width, *height);
  // return true;
  if (controller != nil) {
    NSRect frame = controller.view.frame;
    *width = static_cast<uint32_t>(NSWidth(frame));
    *height = static_cast<uint32_t>(NSHeight(frame));
    NSLog(@"gui_get_size() -> width: %d, height: %d", *width, *height);
    return true;
  } else {
    NSLog(@"gui_get_size() -> false");
    return false;
  }
}

bool PluginDarwin::GUICanResize() {
  bool result = (controller != nil);
  Log("gui_can_resize() -> %d", static_cast<int>(result));
  return result;
}

bool PluginDarwin::GUIAdjustSize(uint32_t *width, uint32_t *height) {
  Log("gui_adjust_size(width: %d, height: %d) -> 1", *width, *height);
  return true;
}

bool PluginDarwin::GUISetSize(uint32_t width, uint32_t height) {
  //Log("gui_set_size(width: %d, height: %d) -> 0", width, height);
  this->width = width;
  this->height = height;
  // return true;
  if (controller != nil) {
    NSLog(@"gui_set_size(width: %d, height: %d) -> 1", width, height);
    controller.view.frame = NSMakeRect(0, 0, width, height);
    return true;
  }
  NSLog(@"gui_set_size(width: %d, height: %d) -> 0", width, height);
  return false;
}

bool PluginDarwin::GUISetParent(const clap_window_t *window) {
  // Log("gui_set_parent(%lx) -> 1", window->cocoa);
  // return true;
  NSView *container = reinterpret_cast<NSView *>(window->cocoa);
  if (controller != nullptr) {
    NSLog(@"gui_set_parent(%lx) -> 1", container);
    [container addSubview:controller.view];
    return true;
  }
  NSLog(@"gui_set_parent(%lx) -> 0", container);
  return false;
}

bool PluginDarwin::GUISetTransient(const clap_window_t *window) {
  NSLog(@"gui_set_transient()");
  return false;
}

bool PluginDarwin::GUIShow() {
  if (controller != nullptr) {
    NSLog(@"gui_show() -> 1");
    controller.view.hidden = false;
    return true;
  }
  NSLog(@"gui_show() -> 0");
  return false;
}

bool PluginDarwin::GUIHide() {
  if (controller != nil) {
    NSLog(@"gui_hide() -> 1");
    controller.view.hidden = true;
    return true;
  }
  NSLog(@"gui_hide() -> 0");
  return false;
}

Plugin* NewPluginDarwin(const clap_host_t *host) {
  return new PluginDarwin(host);
}

}

