#import <AppKit/AppKit.h>
#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>

#include <nlohmann/json.hpp>

#import "plugin.hh"

#if __has_feature(objc_arc)
#error "ARC is on"
#endif

using json = nlohmann::json;

namespace erachnid::scan {
  class PluginDarwin;
}

@interface ScanController : NSObject <NSOpenSavePanelDelegate> {
  erachnid::scan::Plugin *_plugin;
}

@property (nonatomic, retain) IBOutlet NSView *view;
@property (nonatomic, retain) IBOutlet NSTextField *eventCountField;
@property (nonatomic, retain) IBOutlet NSSwitch *includeEmptyProcessSwitch;
@property (nonatomic, retain) IBOutlet NSSwitch *includeOnMainThreadSwitch;
@property (nonatomic, retain) IBOutlet NSSwitch *includeProcessWithoutTransportSwitch;
@property (nonatomic, retain) NSURL *saveTarget;

- (IBAction)buttonPressed:(id)sender;

@end

@implementation ScanController

- (id)initWithPlugin:(erachnid::scan::Plugin *)plugin {
  self = [super init];
  if (self != nil) {
    _plugin = plugin;
  }
  return self;
}

- (void)setEventCount:(int)count {
  if (self.eventCountField != nil) {
	  self.eventCountField.stringValue = [NSString stringWithFormat:@"%d", count];
  }
  //_count = count;
}

- (IBAction)setIncludeEmptyProcess:(id)sender {
	if (_plugin != nullptr && self.includeEmptyProcessSwitch != nil) {
		_plugin->setIncludeEmptyProcess(
			self.includeEmptyProcessSwitch.state == NSControlStateValueOn);
	}
}

- (IBAction)setIncludeOnMainThread:(id)sender {
  if (_plugin != nullptr && self.includeOnMainThreadSwitch != nil) {
    _plugin->setIncludeOnMainThread(
      self.includeOnMainThreadSwitch.state == NSControlStateValueOn);
  }
}

- (IBAction)setIncludeProcessWithoutTransport:(id)sender {
  if (_plugin != nullptr && self.includeProcessWithoutTransportSwitch != nil) {
    _plugin->setIncludeProcessWithoutTransport(
                                               self.includeProcessWithoutTransportSwitch.state == NSControlStateValueOn);

  }
}

- (void)refreshGUI {
  if (_plugin != nullptr) {
    NSLog(@"refreshGUI");
    self.includeEmptyProcessSwitch.state = _plugin->getIncludeEmptyProcess();
    self.includeOnMainThreadSwitch.state = _plugin->getIncludeOnMainThread();
    self.includeProcessWithoutTransportSwitch.state = _plugin->getIncludeProcessWithoutTransport();
  }
}

- (IBAction)buttonPressed:(id)sender {
}

- (IBAction)resetLog:(id)sender {
  NSLog(@"resetLog");
  if (_plugin != nullptr) {
		_plugin->ResetLog();
  }
}

- (NSString *)panel:(id)sender 
userEnteredFilename:(NSString *)filename 
          confirmed:(BOOL)okFlag {
  //self.saveTarget = filename;
  return filename;
}

- (IBAction)saveLog:(id)sender {
  NSLog(@"saveLog");
  NSSavePanel *panel = [NSSavePanel savePanel];
	panel.allowedContentTypes = @[UTTypeJSON];
	panel.delegate = self;
	if (self.view.window != nil) {
		[panel beginSheetModalForWindow:self.view.window completionHandler:^(NSModalResponse result) {
			if (result == NSModalResponseOK) {
				NSLog(@"saving %@...", panel.URL);
				
				std::string serialized = _plugin->GetData().dump();
				NSData *data = [NSData dataWithBytes:serialized.data() length:serialized.length()];
				[data writeToURL:panel.URL atomically:YES];
			} else {
				NSLog(@"cancelled");
			}
		}];
	} else {
		NSLog(@"No window");
	}
  //+-panel begin
  //[panel begin]
}
  
@end


namespace erachnid::scan {



class PluginDarwin : public Plugin {
protected:
  virtual void setEventCount(int count);

public:
  PluginDarwin(const clap_host_t *_host);
  virtual ~PluginDarwin() { }

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

  ScanController *controller;

  uint32_t width;
  uint32_t height;
};

PluginDarwin::PluginDarwin(const clap_host_t *_host) : Plugin(_host),
  /*rootView(nil),*/ controller(nil),
  width(500), height(300)
{
}

void PluginDarwin::setEventCount(int count) {
  [controller setEventCount:count];
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
  NSBundle *bundle = [NSBundle bundleWithIdentifier:@"me.faec.erachnid"];
  if (bundle == nil) {
    //Log("couldn't load plugin bundle");
    return false;
  }
  NSNib *nib = [[[NSNib alloc] initWithNibNamed:@"scan" bundle:bundle] autorelease];
  if (nib == nil) {
    //Log("couldn't load Stuff.nib from bundle");
    return false;
  }
  controller = [[ScanController alloc] initWithPlugin:this];

  NSArray* topLevelObjects;
  if (![nib instantiateWithOwner:controller topLevelObjects:&topLevelObjects]) {
    //Log("Couldn't instantiate Stuff.nib");
    return false;
  }
  NSLog(@"Stuff.nib instantiated with %d objects", static_cast<int>(topLevelObjects.count));
  return true;
}

void PluginDarwin::GUIDestroy() {
  NSLog(@"gui_destroy(%lx, %@)", reinterpret_cast<unsigned long>(this), controller);
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
  return result;
}

bool PluginDarwin::GUIAdjustSize(uint32_t *width, uint32_t *height) {
  return true;
}

bool PluginDarwin::GUISetSize(uint32_t width, uint32_t height) {
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
    NSLog(@"gui_set_parent(%@) -> 1", container);
    [container addSubview:controller.view];
    return true;
  }
  NSLog(@"gui_set_parent(%@) -> 0", container);
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
    [controller refreshGUI];
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

CLAPPlugin* NewPluginDarwin(const clap_host_t *host) {
  return new PluginDarwin(host);
}

}

