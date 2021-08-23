#include "os/info.hpp"

#include "os/macros.h"
#if !IS_OS_WINDOWS
	#error "This code is for Windows only!"
#endif

/* WMI to get OS info */
#define _WIN32_DCOM
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

namespace os
{

// Name of OS without version
std::string name() { return "Windows"; }

// Name of OS + version
std::string pretty_name() { return info().pretty_name; }

// Codename of OS (if present)
std::string codename() { return ""; }

// Major, minor and patch of OS
::version version() { return info().version; }

// Version [+ some additional data]
std::string version_string() { return info().version_string; }

// Get whole OS info
const info_t& info()
{
    static info_t i;

    // Accessing WMI is expensive.
    // Init info only once.
    if (static bool init = true; init)
    {
        i.type = type();
        i.name = name();
        i.pretty_name = "Windows"; // Will be updated
        i.codename = codename();

        // Accessing WMI like here:
        // https://docs.microsoft.com/en-us/windows/win32/wmisdk/example--getting-wmi-data-from-the-local-computer

        HRESULT hres;

        // Step 1: --------------------------------------------------
        // Initialize COM. ------------------------------------------

        hres = CoInitializeEx(0, COINIT_MULTITHREADED);
        if (FAILED(hres)) { return i; }

        // Step 2: --------------------------------------------------
        // Set general COM security levels --------------------------

        hres = CoInitializeSecurity(
            NULL,
            -1,                          // COM authentication
            NULL,                        // Authentication services
            NULL,                        // Reserved
            RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
            RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
            NULL,                        // Authentication info
            EOAC_NONE,                   // Additional capabilities
            NULL                         // Reserved
        );


        if (FAILED(hres))
        {
            CoUninitialize();
            return i;
        }

        // Step 3: ---------------------------------------------------
        // Obtain the initial locator to WMI -------------------------

        IWbemLocator* pLoc = NULL;

        hres = CoCreateInstance(
            CLSID_WbemLocator,
            0,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator, (LPVOID*)&pLoc);

        if (FAILED(hres))
        {
            CoUninitialize();
            return i;
        }

        // Step 4: -----------------------------------------------------
        // Connect to WMI through the IWbemLocator::ConnectServer method

        IWbemServices* pSvc = NULL;

        // Connect to the root\cimv2 namespace with
        // the current user and obtain pointer pSvc
        // to make IWbemServices calls.
        hres = pLoc->ConnectServer(
            _bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
            NULL,                    // User name. NULL = current user
            NULL,                    // User password. NULL = current
            NULL,                    // Locale. NULL indicates current
            NULL,                    // Security flags.
            0,                       // Authority (for example, Kerberos)
            0,                       // Context object
            &pSvc                    // pointer to IWbemServices proxy
        );

        if (FAILED(hres))
        {
            pLoc->Release();
            CoUninitialize();
            return i;
        }


        // Step 5: --------------------------------------------------
        // Set security levels on the proxy -------------------------

        hres = CoSetProxyBlanket(
            pSvc,                        // Indicates the proxy to set
            RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
            RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
            NULL,                        // Server principal name
            RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
            RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
            NULL,                        // client identity
            EOAC_NONE                    // proxy capabilities
        );

        if (FAILED(hres))
        {
            pSvc->Release();
            pLoc->Release();
            CoUninitialize();
            return i;
        }

        // Step 6: --------------------------------------------------
        // Use the IWbemServices pointer to make requests of WMI ----

        // For example, get the name of the operating system
        IEnumWbemClassObject* pEnumerator = NULL;
        hres = pSvc->ExecQuery(
            bstr_t("WQL"),
            bstr_t("SELECT * FROM Win32_OperatingSystem"),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL,
            &pEnumerator);

        if (FAILED(hres))
        {
            pSvc->Release();
            pLoc->Release();
            CoUninitialize();
            return i;
        }

        // Step 7: -------------------------------------------------
        // Get the data from the query in step 6 -------------------

        IWbemClassObject* pclsObj = NULL;
        ULONG uReturn = 0;

        while (pEnumerator)
        {
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

            if (uReturn == 0) { break; }

            VARIANT vtProp{};

            // Get the value of the Version property
            hr = pclsObj->Get(L"Version", 0, &vtProp, 0, 0);

            std::wstring v_wstr = vtProp.bstrVal;

            // Digits and points are ANSI symbols, so narrowing conversion from UTF-16 to UTF-8 is fine.
            std::string v_str(v_wstr.begin(), v_wstr.end());

            i.version = ::version{ v_str };
            i.version_string = v_str;

            i.pretty_name += " ";
            if (i.version >= ::version{ 10, 0 })
            {
                i.pretty_name += std::to_string(i.version.major);
            }
            else if (i.version >= ::version{ 6, 3 })
            {
                i.pretty_name += "8.1";
            }
            else if (i.version >= ::version{ 6, 2 })
            {
                i.pretty_name += "8";
            }
            else if (i.version >= ::version{ 6, 1 })
            {
                i.pretty_name += "7";
            }
            else if (i.version >= ::version{ 6, 0 })
            {
                i.pretty_name += "Vista";
            }
            else if (i.version >= ::version{ 5, 2 })
            {
                i.pretty_name += "XP 64-Bit Edition";
            }
            else if (i.version >= ::version{ 5, 1 })
            {
                i.pretty_name += "XP";
            }
            else if (i.version >= ::version{ 5, 0 })
            {
                i.pretty_name += "2000";
            }
            else
            {
                i.pretty_name.pop_back(); // remove space
            }

            VariantClear(&vtProp);
            pclsObj->Release();
        }

        // Cleanup
        // ========

        pSvc->Release();
        pLoc->Release();
        pEnumerator->Release();
        CoUninitialize();

        init = false;
    }

    return i;
}

} // namespace os