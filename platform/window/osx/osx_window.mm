
#import <Cocoa/Cocoa.h>

#include "platform/window/osx/osx_window.h"
#include "base/logging.h"

// On OSX 10.12 a number of AppKit interfaces have been renamed for consistency, and the previous
// symbols tagged as deprecated. However we can't simply use the new symbols as it would break
// compilation on our automated testing that doesn't use OSX 10.12 yet. So we just ignore the
// warnings.
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

@interface Application : NSApplication
@end

@implementation Application

- (void)_crashOnException:(NSException *)exception
{
    NSLog(@"*** OSXWindow aborting on exception:  <%@> %@", [exception name], [exception reason]);
    NSLog(@"%@", [exception callStackSymbols]);
    abort();
}
@end

// The Delegate receiving application-wide events.
@interface ApplicationDelegate : NSObject
@end

@implementation ApplicationDelegate
- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
    return NSTerminateCancel;
}
@end
static ApplicationDelegate *gApplicationDelegate = nil;


static bool InitializeAppKit() {
    if (NSApp != nil) {
        return true;
    }

    // Initialize the global variable "NSApp"
    [Application sharedApplication];

    // Make us appear in the dock
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

    // Register our global event handler
    gApplicationDelegate = [[ApplicationDelegate alloc] init];
    if (gApplicationDelegate == nil)
    {
        return false;
    }
    [NSApp setDelegate:static_cast<id>(gApplicationDelegate)];

    // Set our status to "started" so we are not bouncing in the doc and can activate
    [NSApp finishLaunching];
    return true;
}


OSXWindow::OSXWindow() : window_(nil), view_(nil) {}

OSXWindow::~OSXWindow() {
    Destroy();
}

bool OSXWindow::InitializeImpl(const std::string &name, int width, int height) {
    if (!InitializeAppKit()) {
        return false;
    }

    unsigned int styleMask = NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask |
                             NSMiniaturizableWindowMask;
    window_ = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, width, height)
                                          styleMask:styleMask
                                            backing:NSBackingStoreBuffered
                                              defer:NO];

    if (window_ == nil) {
        return false;
    }

    view_ = [NSView alloc];
    [view_ init];
    if (view_ == nil) {
        return false;
    }
    [view_ setWantsLayer:YES];
    view_.layer.contentsScale = 1;

    [window_ setContentView:view_];
    [window_ setTitle:[NSString stringWithUTF8String:name.c_str()]];
    [window_ setAcceptsMouseMovedEvents:YES];
    [window_ center];

    [NSApp activateIgnoringOtherApps:YES];
    // [NSAppbeginModalSessionForWindow:window_]
    x_      = 0;
    y_      = 0;
    width_  = width;
    height_ = height;

    return true;
}

void OSXWindow::Destroy() {
    [view_ release];
    view_ = nil;
    [window_ setContentView:nil];
    [window_ release];
    window_ = nil;
}

void OSXWindow::MessageLoop() {
    @autoreleasepool
    {
        while (true)
        {
            // TODO(http://anglebug.com/6570): @try/@catch is a workaround for
            // exceptions being thrown from Cocoa-internal function
            // NS_setFlushesWithDisplayLink starting in macOS 11.
            @try
            {
                NSEvent *event = [NSApp nextEventMatchingMask:NSAnyEventMask
                                                    untilDate:[NSDate distantPast]
                                                       inMode:NSDefaultRunLoopMode
                                                      dequeue:YES];
                if (event == nil)
                {
                    break;
                }

                if ([event type] == NSAppKitDefined)
                {
                    continue;
                }
                [NSApp sendEvent:event];
            }
            @catch (NSException *localException)
            {
                NSLog(@"*** OSXWindow discarding exception: <%@> %@", [localException name],
                      [localException reason]);
            }
        }
    }
}

void OSXWindow::SetVisible(bool visible) {
    if (visible) {
        [window_ makeKeyAndOrderFront:nil];
    }
    else {
        [window_ orderOut:nil];
    }
}

void OSXWindow::ResetNativeWindow() {}

EGLNativeWindowType OSXWindow::GetNativeWindow() const {
    return [view_ layer];
}

EGLNativeDisplayType OSXWindow::GetNativeDisplay() const {
    return static_cast<EGLNativeDisplayType>(0);
}

NSWindow *OSXWindow::GetNSWindow() const {
    return window_;
}

// static
platform::OSWindow* platform::OSWindow::New() {
    return new OSXWindow;
}
