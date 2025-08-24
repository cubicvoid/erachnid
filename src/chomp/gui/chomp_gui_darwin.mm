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
}
  
@end


namespace erachnid::chomp {



class ChompPluginDarwin : public ChompPlugin {
public:
  ChompPluginDarwin(const clap_host_t *_host);
  virtual ~ChompPluginDarwin() { }

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
}

bool ChompPluginDarwin::GUIIsAPISupported(const char *api, bool is_floating) {
  return strcmp(api, CLAP_WINDOW_API_COCOA) == 0 && !is_floating;
}

bool ChompPluginDarwin::GUIGetPreferredAPI(const char **api, bool *is_floating) {
  *api = CLAP_WINDOW_API_COCOA;
  *is_floating = false;
  return true;
}

bool ChompPluginDarwin::GUICreate(const char *api, bool is_floating) {
  NSBundle *bundle = [NSBundle bundleWithIdentifier:@"me.faec.erachnid"];
  if (bundle == nil) {
    return false;
  }
  NSNib *nib = [NSNib alloc];
	NSLog(@"allocated nib: %lx", reinterpret_cast<long>(nib));
  nib = [[[NSNib alloc] initWithNibNamed:@"Stuff" bundle:bundle] autorelease];
  if (nib == nil) {
    return false;
  }

  controller = [[FAESimpleClass alloc] initWithPlugin:this];

  NSArray* topLevelObjects;
  if (![nib instantiateWithOwner:controller topLevelObjects:&topLevelObjects]) {
    return false;
  }
  return true;
}

void ChompPluginDarwin::GUIDestroy() {
  if (controller != nil) {
    [controller.view removeFromSuperview];
    [controller release];
    controller = nil;
  }
}

bool ChompPluginDarwin::GUISetScale(double scale) {
  return true;
}

bool ChompPluginDarwin::GUIGetSize(uint32_t *width, uint32_t *height) {
  if (controller != nil) {
    NSRect frame = controller.view.frame;
    *width = static_cast<uint32_t>(NSWidth(frame));
    *height = static_cast<uint32_t>(NSHeight(frame));
    return true;
  }
  return false;
}

bool ChompPluginDarwin::GUICanResize() {
  bool result = (controller != nil);
  return result;
}

bool ChompPluginDarwin::GUIAdjustSize(uint32_t *width, uint32_t *height) {
  return true;
}

bool ChompPluginDarwin::GUISetSize(uint32_t width, uint32_t height) {
  this->width = width;
  this->height = height;
  if (controller != nil) {
    controller.view.frame = NSMakeRect(0, 0, width, height);
    return true;
  }
  return false;
}

bool ChompPluginDarwin::GUISetParent(const clap_window_t *window) {
  NSView *container = reinterpret_cast<NSView *>(window->cocoa);
  if (controller != nullptr) {
    [container addSubview:controller.view];
    return true;
  }
  return false;
}

bool ChompPluginDarwin::GUISetTransient(const clap_window_t *window) {
  return false;
}

bool ChompPluginDarwin::GUIShow() {
  if (controller != nullptr) {
    controller.view.hidden = false;
    return true;
  }
  return false;
}

bool ChompPluginDarwin::GUIHide() {
  if (controller != nil) {
    controller.view.hidden = true;
    return true;
  }
  return false;
}

ChompPlugin* NewChompPluginDarwin(const clap_host_t *host) {
  return new ChompPluginDarwin(host);
}

}

