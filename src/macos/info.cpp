#include "os/info.hpp"

#include "os/macros.h"
#if !IS_OS_MACOS
    #error "This code is for macOS only!"
#endif

#include <CoreFoundation/CoreFoundation.h>

namespace os
{

// Name of OS without version
std::string name() { return "macOS"; }

// Name of OS + version
std::string pretty_name() { return info().pretty_name; }

// Codename of OS (if present)
std::string codename() { return info().codename; }

// Major, minor and patch of OS
::version version() { return info().version; }

// Version [+ some additional data]
std::string version_string() { return info().version_string; }

// Get whole OS info
const info_t & info()
{
    static info_t i;

    // Reading from file is expensive.
    // Init info only once.
    if (static bool init = true; init)
    {
        i.type = type();
        i.name = name();

        CFURLRef fileURL = CFURLCreateWithFileSystemPath(
            kCFAllocatorDefault, 
            CFSTR("/System/Library/CoreServices/SystemVersion.plist"),
            kCFURLPOSIXPathStyle,
            false // not a directory
        );
        CFReadStreamRef stream = CFReadStreamCreateWithFile(kCFAllocatorDefault, fileURL);
        if (CFReadStreamOpen(stream))
        {
            constexpr CFIndex bufferLength = 1024;
            UInt8 buffer[bufferLength] = {0};

            CFIndex bytesNumber = CFReadStreamRead(stream, buffer, bufferLength);
            CFReadStreamClose(stream);

            if (bytesNumber > 0)
            {
                CFDataRef data = CFDataCreate(kCFAllocatorDefault, buffer, bytesNumber);
                CFPropertyListRef plist = CFPropertyListCreateWithData(
                    kCFAllocatorDefault, 
                    data, 
                    kCFPropertyListImmutable, 
                    nullptr, 
                    nullptr
                );

                CFDictionaryRef dict = static_cast<CFDictionaryRef>(plist);
                CFStringRef productVersion = static_cast<CFStringRef>(CFDictionaryGetValue(dict, CFSTR("ProductVersion")));
                CFStringRef productBuildVersion = static_cast<CFStringRef>(CFDictionaryGetValue(dict, CFSTR("ProductBuildVersion")));

                std::string version(CFStringGetCStringPtr(productVersion, kCFStringEncodingUTF8));
                std::string build(CFStringGetCStringPtr(productBuildVersion, kCFStringEncodingUTF8));
                
                i.version = ::version(version);
                i.version_string = version + " (" + build + ")";

                CFRelease(data);
                CFRelease(plist);
            }
        }
        CFRelease(fileURL);
        CFRelease(stream);

        if (i.version.major == 12)
        {
            i.codename = "Monterey";
        }
        else if (i.version.major == 11)
        {
            i.codename = "Big Sur";
        }
        else if (i.version.major == 10)
        {
            switch (i.version.minor)
            {
            case 12: i.codename = "Sierra";      break;
            case 13: i.codename = "High Sierra"; break;
            case 14: i.codename = "Mojave";      break;
            case 15: i.codename = "Catalina";    break;
            }
        }

        i.pretty_name = i.name + " " + i.codename;

        init = false;
    }

    return i;
}

} // namespace os