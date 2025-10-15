#define NOMINMAX  
#define WIN32_LEAN_AND_MEAN  
#include <windows.h>

BOOL APIENTRY DllMain(
    HANDLE hModule,// Handle to DLL module
    DWORD ul_reason_for_call,// Reason for calling function
    LPVOID lpReserved) // Reserved
{
    return TRUE;
}