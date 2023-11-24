#import <AppKit/AppKit.h>

#import "gui.hh"

#import "chomp.hh"

#if __has_feature(objc_arc)
#error "ARC is on"
#endif

namespace erachnid::chomp {
  class GUIWrapperDarwin;
}

@interface FAESimpleClass : NSObject {
  erachnid::chomp::Plugin *_plugin;
}

@property (nonatomic, retain) IBOutlet NSView *view;

- (IBAction)buttonPressed:(id)sender;

@end

@implementation FAESimpleClass

- (id)initWithPlugin:(erachnid::chomp::Plugin *)plugin {
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
  //NSView *rootView;
  FAESimpleClass *controller;

  uint32_t width;
  uint32_t height;

};

GUIWrapperDarwin::GUIWrapperDarwin(Plugin *_plugin) :
  plugin(_plugin), /*rootView(nil),*/ controller(nil),
  width(500), height(300)
{
  int value;
  plugin->Log("GUIWrapperDarwin(%lx)  (this: %lx, stack: %lx)",
    reinterpret_cast<long>(_plugin), reinterpret_cast<long>(this),
    reinterpret_cast<long>(&value));

}

bool GUIWrapperDarwin::Create(const char *api, bool is_floating) {
  int value;
  plugin->Log("gui_create(%s, %d)  (this: %lx, stack: %lx)",
    api, static_cast<int>(is_floating),
    reinterpret_cast<long>(this), reinterpret_cast<long>(&value));
  NSBundle *bundle = [NSBundle bundleWithIdentifier:@"me.faec.erachnid"];
  if (bundle == nil) {
    plugin->Log("couldn't load plugin bundle");
    return false;
  } else {
    plugin->Log("got bundle %lx", reinterpret_cast<long>(bundle));
  }
  NSNib *nib = [NSNib alloc];
	NSLog(@"allocated nib: %lx", reinterpret_cast<long>(nib));
  plugin->Log("allocated nib: %lx", reinterpret_cast<long>(nib));
  bool responds = [nib respondsToSelector:@selector(initWithNibNamed:bundle:)];
  plugin->Log("responds to initWithNibNamed:bundle:  %d", static_cast<int>(responds));
  nib = [[[NSNib alloc] initWithNibNamed:@"Stuff" bundle:bundle] autorelease];
  //nib = [nib initWithNibNamed:@"lskjdfkldsjf" bundle:nil];
  if (nib == nil) {
    plugin->Log("couldn't load Stuff.nib from bundle");
    return false;//@"couldn't load Stuff.nib from bundle";
  } else {
    plugin->Log("loaded Stuff.nib: %lx", reinterpret_cast<long>(nib));
  }

  controller = [[FAESimpleClass alloc] initWithPlugin:plugin];

  NSArray* topLevelObjects;
  if (![nib instantiateWithOwner:controller topLevelObjects:&topLevelObjects]) {
    plugin->Log("Couldn't instantiate Stuff.nib");
    return false;//@"couldn't instantiate nib";
  }
  plugin->Log("Stuff.nib instantiated with %d objects", static_cast<int>(topLevelObjects.count));
  return true;
}

void GUIWrapperDarwin::Destroy() {
  plugin->Log("gui_destroy() (this=%lx, controller=%lx)", reinterpret_cast<long>(this), controller);
  if (controller != nil) {
    [controller.view removeFromSuperview];
    [controller release];
    controller = nil;
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
    plugin->Log("gui_destroy finished (this=%lx, rootView=%lx)", this, rootView);
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
  if (controller != nil) {
    NSRect frame = controller.view.frame;
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
  bool result = (controller != nil);
  plugin->Log("gui_can_resize() -> %d", static_cast<int>(result));
  return result;
}

bool GUIWrapperDarwin::AdjustSize(uint32_t *width, uint32_t *height) {
  plugin->Log("gui_adjust_size(width: %d, height: %d) -> 1", *width, *height);
  return true;
}

bool GUIWrapperDarwin::SetSize(uint32_t width, uint32_t height) {
  //plugin->Log("gui_set_size(width: %d, height: %d) -> 0", width, height);
  // this->width = width;
  // this->height = height;
  // return true;
  if (controller != nil) {
    plugin->Log("gui_set_size(width: %d, height: %d) -> 1", width, height);
    controller.view.frame = NSMakeRect(0, 0, width, height);
    return true;
  }
  plugin->Log("gui_set_size(width: %d, height: %d) -> 0", width, height);
  return false;
}

bool GUIWrapperDarwin::SetParent(const clap_window_t *window) {
  // plugin->Log("gui_set_parent(%lx) -> 1", window->cocoa);
  // return true;
  NSView *container = reinterpret_cast<NSView *>(window->cocoa);
  if (controller != nullptr) {
    plugin->Log("gui_set_parent(%lx) -> 1", container);
    [container addSubview:controller.view];
    return true;
  }
  plugin->Log("gui_set_parent(%lx) -> 0", container);
  return false;
}

bool GUIWrapperDarwin::SetTransient(const clap_window_t *window) {
  plugin->Log("gui_set_transient()");
  return false;
}

bool GUIWrapperDarwin::Show() {
  // plugin->Log("gui_show() -> 1");
  // return true;
  if (controller != nullptr) {
    plugin->Log("gui_show() -> 1");
    controller.view.hidden = false;
    return true;
  }
  plugin->Log("gui_show() -> 0");
  return false;
}

bool GUIWrapperDarwin::Hide() {
  //   plugin->Log("gui_hide() -> 1");
  // return true;
  if (controller != nil) {
    plugin->Log("gui_hide() -> 1");
    controller.view.hidden = true;
    return true;
  }
  plugin->Log("gui_hide() -> 0");
  return false;
}

GUIWrapper* NewGUIWrapperDarwin(Plugin *plugin) {
  return new GUIWrapperDarwin(plugin);
}

}

