//
//  main.c
//  StraightLines
//
//  Created by Jesse Rosalia on 5/31/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <CoreFoundation/CoreFoundation.h>
//#include <Foundation/Foundation.h>
#include <ApplicationServices/ApplicationServices.h>
//#include <HIToolbox/Events.h>
#include <Carbon/Carbon.h>

void PostKeyWithModifiers(CGKeyCode key, CGEventFlags modifiers);
void postKeyboardEvent( int keyCode, bool keyUp);
bool isActiveWindowByTitle(CFStringRef expectedTitle, AXUIElementRef *appRef);

int main(int argc, const char * argv[])
{

    CFStringRef parallelsTitle = CFStringCreateWithCString(CFAllocatorGetDefault(), "Parallels Desktop", kCFStringEncodingUTF8);
    // insert code here...
    CFShow(CFSTR("Hello, World!\n"));
    
    bool parallelsActive = false;
    AXUIElementRef parallelsApp;
    while (true) {
//        PostKeyWithModifiers(kVK_ANSI_R, kCGEventFlagMaskCommand | kCGEventFlagMaskControl | kCGEventFlagMaskShift | kCGEventFlagMaskAlternate);
        CFShow(CFSTR("Checking active window"));
        bool isActive = isActiveWindowByTitle(parallelsTitle, &parallelsApp);
        if (isActive) {
            CFShow(CFSTR("Parallels window is active"));
        }
        if (parallelsActive && !isActive) {
            CFShow(CFSTR("Parallels window is losing focus"));
            postKeyboardEvent(kVK_Control, false);
            postKeyboardEvent(kVK_Option,  false);
            postKeyboardEvent(kVK_Control, true);
            postKeyboardEvent(kVK_Option,  true);
            
        }
        parallelsActive = isActive;
        sleep(1);
    }
    return 0;
}

bool isActiveWindowByTitle(CFStringRef expectedTitle, AXUIElementRef *appRef) {
    
    CFArrayRef array = CGWindowListCreate(kCGWindowListOptionAll, kCGNullWindowID);

    ProcessSerialNumber psn = { 0L, 0L };
    OSStatus err = GetFrontProcess(&psn); // get front process PSN
    /*error check*/
    pid_t pidt;
    GetProcessPID(&psn , &pidt); // get Pid from ProcessSerialNumber
    CFArrayRef descArray = CGWindowListCreateDescriptionFromArray(array);
//    CFDictionaryRef dict = (CFDictionaryRef)CFArrayGetValueAtIndex(descArray, 0);
    CFStringRef title;
    for (int ii = 0, count = CFArrayGetCount(descArray); ii < count; ii++) {
        CFDictionaryRef dict = (CFDictionaryRef)CFArrayGetValueAtIndex(descArray, ii);
        CFNumberRef val = CFDictionaryGetValue(dict, kCGWindowOwnerPID);
        
        pid_t winPid;
        CFNumberGetValue(val, kCFNumberIntType, &winPid);
        if (winPid == pidt) {
            if (CFDictionaryContainsKey(dict, kCGWindowOwnerName)) {
                title = CFDictionaryGetValue(dict, kCGWindowOwnerName);
                CFShow(title);
            }
            break;
        }
    }
//    }    
    /*
    ProcessSerialNumber psn = { 0L, 0L };
    OSStatus err = GetFrontProcess(&psn); // get front process PSN
    /*error check*
    pid_t pidt;
    GetProcessPID(&psn , &pidt); // get Pid from ProcessSerialNumber
    AXUIElementRef theApp = AXUIElementCreateApplication(pidt); //get Application from Pid
    AXUIElementRef focusWindow;
    CFStringRef title;
    //get focused window from Carbon Application
    AXUIElementCopyAttributeValue(theApp, kAXMainWindowAttribute, (CFTypeRef *)&focusWindow);
        
    //Now we retrieve Position
    AXUIElementCopyAttributeValue(theApp, kAXTitleAttribute, (CFTypeRef *)&title);
    if (title == NULL) {
        AXUIElementCopyAttributeValue(theApp, kAXTitleUIElementAttribute, (CFTypeRef *)&title);
        
    }*/
//    AXUIElementRef focusWindow;
//    AXValueRef value;
//    CGPoint xy;
//    CGSize wh;
//    
//    //get focused window from Carbon Application
//    AXUIElementCopyAttributeValue(theApp, kAXFocusedWindowAttribute, (CFTypeRef *)&focusWindow);
//    
//    //Now we retrieve Position
//    AXUIElementCopyAttributeValue(focusWindow, kAXPositionAttribute, (CFTypeRef *)&value);
//    AXValueGetValue(value, kAXValueCGPointType, &xy);
//    rect.setX(xy.x);
//    rect.setY(xy.y);
//    
//    //Now we retrieve Size
//    AXUIElementCopyAttributeValue(focusWindow, kAXSizeAttribute, (CFTypeRef *)&value);
//    AXValueGetValue(value, kAXValueCGSizeType, &wh);
//    
//    rect.setWidth(wh.width);
//    rect.setHeight(wh.height);
//    
//    
//    delete pidt;
//    
//    return rect;
//    *appRef = theApp;
    bool retVal = CFStringCompare(title, expectedTitle, 0) == kCFCompareEqualTo;
    CFRelease(descArray);
    CFRelease(array);
    
    return retVal;
}

int shiftKeyCode = 56;
bool shiftIsDown = false;

void postKeyboardEvent( int keyCode, bool keyUp )
{
    
    // Don't send keys while blocked.
//    if( blockState == 1 ){
//        
//        return;
//        
//    }
    
    if( keyCode == shiftKeyCode ){
        
        if( keyUp ){
            
            shiftIsDown = false;
            
        }else{
            
            shiftIsDown = false;
            
        }
        
    }
    
    CGEventSourceRef source = CGEventSourceCreate( kCGEventSourceStateHIDSystemState );
    CGEventRef keyEvent = CGEventCreateKeyboardEvent( source, (CGKeyCode) keyCode, !keyUp );
    
    if( shiftIsDown ){
        
        // Use Shift flag
        CGEventSetFlags( keyEvent, CGEventGetFlags( keyEvent ) | kCGEventFlagMaskShift );
        
    }else{
        
        // Use all existing flag except Shift
        CGEventSetFlags( keyEvent, CGEventGetFlags( keyEvent ) & ~kCGEventFlagMaskShift );
        
    }
    
    CGEventPost( kCGHIDEventTap, keyEvent );
    
    CFRelease( keyEvent );
    CFRelease( source );
    
}

// you can find key codes in <HIToolbox/Events.h>, for example kVK_ANSI_A is 'A' key
// modifiers are flags such as kCGEventFlagMaskCommand

void PostKeyWithModifiers(CGKeyCode key, CGEventFlags modifiers)
{
    CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
    
    CGEventRef keyDown = CGEventCreateKeyboardEvent(source, key, TRUE);
    CGEventSetFlags(keyDown, modifiers);
    CGEventRef keyUp = CGEventCreateKeyboardEvent(source, key, FALSE);
    
    CGEventPost(kCGAnnotatedSessionEventTap, keyDown);
    CGEventPost(kCGAnnotatedSessionEventTap, keyUp);
    
    CFRelease(keyUp);
    CFRelease(keyDown);
    CFRelease(source);  
}
