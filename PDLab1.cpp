#include <windows.h>
#include <iostream>

using namespace std;

// Function to query and display the ImagePath of all services
void QueryServiceImagePaths() {
    HKEY hKey;

    // Open the registry key (Convert string to wide string using L"")
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Services", 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        wcerr << L"Failed to open registry key." << endl;
        return;
    }

    wchar_t subKeyName[256]; // Wide char buffer for subkey names
    DWORD index = 0, subKeyNameSize;
    FILETIME ftLastWriteTime; // Stores last write time of subkeys

    // Loop through all subkeys (services)
    while ((subKeyNameSize = sizeof(subKeyName) / sizeof(wchar_t),
        RegEnumKeyEx(hKey, index++, subKeyName, &subKeyNameSize, NULL, NULL, NULL, &ftLastWriteTime)) == ERROR_SUCCESS) {

        HKEY hSubKey;
        // Open each service subkey
        if (RegOpenKeyEx(hKey, subKeyName, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {

            wchar_t imagePath[1024]; // Wide char buffer for ImagePath
            DWORD imagePathSize = sizeof(imagePath), type;

            // Query the ImagePath value
            if (RegQueryValueEx(hSubKey, L"ImagePath", NULL, &type, (LPBYTE)imagePath, &imagePathSize) == ERROR_SUCCESS && type == REG_EXPAND_SZ) {
                wcout << subKeyName << L" -> " << imagePath << endl; // Print service name and ImagePath
            }
            RegCloseKey(hSubKey); // Close the subkey handle
        }
    }

    RegCloseKey(hKey); // Close the main registry key handle
}

int main() {
    QueryServiceImagePaths(); // Execute the function to retrieve and print ImagePaths
    return 0;
}
