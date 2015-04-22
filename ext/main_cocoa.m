#import <Cocoa/Cocoa.h>
#include "../front.h"

/* I really don't know enough about
   Objective C to fix these or prove
   they're not fixable by me */
#pragma clang diagnostic ignored "-Wselector"
#pragma clang diagnostic ignored "-Wstrict-selector-match"

static CGImageRef cgim;

static id makeNSImage(void)
{
  unsigned width = front_width();
  unsigned height = front_height();
  void* pixels = front_pixels();
  id nsim;
  CGDataProviderRef dp = CGDataProviderCreateWithData(
      NULL, pixels, width * height * 3, NULL);
  CGColorSpaceRef cs = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
  CGBitmapInfo bmi = kCGImageAlphaNone | kCGBitmapByteOrderDefault;
  cgim = CGImageCreate(width, height, 8, 24, width * 3,
      cs, bmi, dp, NULL, NO, kCGRenderingIntentDefault);
  CGDataProviderRelease(dp);
  CGColorSpaceRelease(cs);
  nsim = [NSImage alloc];
  [nsim initWithCGImage:cgim size:NSZeroSize]; 
  return nsim;
}

static void updateNSImage(id nsimv)
{
  id nsim_prev = [nsimv image];
  CGImageRef cgim_prev = cgim;
  id nsim = makeNSImage();
  [nsimv setImage: nsim];
  [nsim_prev release];
  CGImageRelease(cgim_prev);
}

@interface MyView : NSImageView

- (BOOL)acceptsFirstResponder;
- (void)mouseDown:(NSEvent*)event;
- (void)mouseUp:(NSEvent*)event;
- (void)keyDown:(NSEvent*)event;
- (void)keyUp:(NSEvent*)event;

@end

static NSPoint getMouse(id nsimv, NSEvent* event)
{
  return [nsimv convertPoint:[event locationInWindow] fromView:nil];
}

@implementation MyView

- (BOOL)acceptsFirstResponder
{
  return YES;
}

- (void)mouseDown:(NSEvent*)event
{
  NSPoint p = getMouse(self, event);
  front_mouse_down(p.x, front_height() - p.y - 1);
}

- (void)mouseUp:(NSEvent*)event
{
  NSPoint p = getMouse(self, event);
  front_mouse_up(p.x, front_height() - p.y - 1);
  updateNSImage(self);
}

- (void)keyDown:(NSEvent*)event
{
  front_key_down([[event characters] UTF8String][0]);
}

- (void)keyUp:(NSEvent*)event
{
  (void)event;
  front_key_up();
  updateNSImage(self);
}

@end

@interface MyDelegate : NSObject <NSApplicationDelegate>

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification;
- (void)applicationWillTerminate:(NSNotification*)aNotification;

@end

@implementation MyDelegate

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification
{
  (void)aNotification;
}

- (void)applicationWillTerminate:(NSNotification*)aNotification
{
  (void)aNotification;
  front_stop();
}

@end

int main(int argc, char** argv)
{
  id menubar;
  id appMenuItem;
  id appMenu;
  id appName;
  id quitTitle;
  id quitMenuItem;
  id window;
  id im;
  id imv;
  id deleg;
  NSUInteger mask;
  NSRect rect;
  front_start(argc, argv);
  [NSApplication sharedApplication];
  [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
  menubar = [NSMenu new];
  appMenuItem = [NSMenuItem new];
  [menubar addItem:appMenuItem];
  [NSApp setMainMenu:menubar];
  appMenu = [NSMenu new];
  appName = [[NSProcessInfo processInfo] processName];
  quitTitle = [@"Quit " stringByAppendingString:appName];
  quitMenuItem = [[NSMenuItem alloc] initWithTitle:quitTitle
    action:@selector(terminate:) keyEquivalent:@"q"];
  [appMenu addItem:quitMenuItem];
  [appMenuItem setSubmenu:appMenu];
  mask = NSTitledWindowMask | NSClosableWindowMask |
      NSMiniaturizableWindowMask | NSResizableWindowMask;
  rect = NSMakeRect(0, 0, 640, 480);
  window = [[NSWindow alloc] initWithContentRect:rect
    styleMask:mask backing:NSBackingStoreBuffered defer:NO];
  im = makeNSImage();
  imv = [[MyView alloc] initWithFrame:rect];
  [imv setImage:im];
  [[window contentView] addSubview: imv];
  [window cascadeTopLeftFromPoint:NSMakePoint(20, 20)];
  [window setTitle:appName];
  [window makeFirstResponder:imv];
  [window makeKeyAndOrderFront:nil];
  deleg = [MyDelegate new];
  [NSApp setDelegate: deleg];
  [NSApp activateIgnoringOtherApps:YES];
  [NSApp run];
  return 0;
}
