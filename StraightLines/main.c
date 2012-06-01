//
//  main.c
//  StraightLines
//
//  Created by Jesse Rosalia on 5/31/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

void postKeyboardEvent(CGKeyCode keyCode, bool keyUp);
bool isActiveWindowByTitle(CFStringRef expectedTitle, AXUIElementRef *appRef);

int main(int argc, const char * argv[])
{

    CFStringRef parallelsTitle = CFStringCreateWithCString(CFAllocatorGetDefault(), "Parallels Desktop", kCFStringEncodingUTF8);
    // insert code here...
    CFShow(CFSTR("Hello, World!\n"));
    
    bool parallelsActive = false;
    AXUIElementRef parallelsApp;
    while (true) {
        //every second, check to see if Parallels is the active window
        CFShow(CFSTR("Checking active window"));
        bool isActive = isActiveWindowByTitle(parallelsTitle, &parallelsApp);
        if (isActive) {
            CFShow(CFSTR("Parallels window is active"));
        }
        //when parallels is being pushed into the background, thats when we send the control release keystrokes
        if (parallelsActive && !isActive) {
            CFShow(CFSTR("Parallels window is losing focus"));
            postKeyboardEvent(kVK_Control, false);
            postKeyboardEvent(kVK_Option,  false);
            postKeyboardEvent(kVK_Control, true);
            postKeyboardEvent(kVK_Option,  true);
            
        }
        parallelsActive = isActive;
        //wait for 1 second before trying again
        sleep(1);
    }
    return 0;
}

bool isActiveWindowByTitle(CFStringRef expectedTitle, AXUIElementRef *appRef) {
    
    //get the process id of the front process
    ProcessSerialNumber psn = { 0L, 0L };
    OSStatus err = GetFrontProcess(&psn); // get front process PSN
    /*error check*/
    pid_t pidt;
    GetProcessPID(&psn , &pidt); // get Pid from ProcessSerialNumber
    
    //get a list of all the windows in the system, and create description dictionaries
    // for all of the windows
    CFArrayRef array = CGWindowListCreate(kCGWindowListOptionAll, kCGNullWindowID);
    CFArrayRef descArray = CGWindowListCreateDescriptionFromArray(array);
    CFStringRef title = NULL;
    //iterate through the descArray, looking for the Dictionary for the front process
    //NOTE: this is required because contrary to the documentation, the dictionary order
    // is NOT in front-to-back order (or maybe that just applies to CGWindowListCreate)
    for (int ii = 0, count = CFArrayGetCount(descArray); ii < count; ii++) {
        CFDictionaryRef dict = (CFDictionaryRef)CFArrayGetValueAtIndex(descArray, ii);
        CFNumberRef val = CFDictionaryGetValue(dict, kCGWindowOwnerPID);
        
        //can assume that process id is an int
        pid_t winPid;
        CFNumberGetValue(val, kCFNumberIntType, &winPid);
        //if it matches our front process...
        if (winPid == pidt) {
            //get the title out of the dictionary, if it exists
            if (CFDictionaryContainsKey(dict, kCGWindowOwnerName)) {
                title = CFDictionaryGetValue(dict, kCGWindowOwnerName);
                CFShow(title);
            }
            break;
        }
    }

    //we found a valid title for the front process, and it compares to our expected title...or not
    bool retVal = title != NULL && CFStringCompare(title, expectedTitle, 0) == kCFCompareEqualTo;
    
    CFRelease(descArray);
    CFRelease(array);
    
    //return the retVal to the caller
    return retVal;
}

/**
 * Post a global keyboard event.
 *
 * TODO: would be nice if this was targeted, but since we're using a fairly innocuous keystroke (Ctrl+Alt)
 * in this app, we should not have any unintended consequences
 *
 */
void postKeyboardEvent( CGKeyCode keyCode, bool keyUp ) {
    
    CGEventSourceRef source = CGEventSourceCreate( kCGEventSourceStateHIDSystemState );
    CGEventRef keyEvent = CGEventCreateKeyboardEvent( source, keyCode, !keyUp );
    
    CGEventPost( kCGHIDEventTap, keyEvent );
    
    CFRelease( keyEvent );
    CFRelease( source );    
}


/**********************
 Old/saved code - use accessibility access to retrieve process info
     }    
 ProcessSerialNumber psn = { 0L, 0L };
 OSStatus err = GetFrontProcess(&psn); // get front process PSN
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
