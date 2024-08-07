 /********************************
 * Title :          DLL injector
 * Description :    Injects a DLL into a target progress by allocating memory and executing LoadLibraryA
 * Author :         Joey
 * Date :           8/7/2024
 ********************************/

#include <iostream>
#include <Windows.h>
#include <filesystem>

int main()
{
    /* Take user input for Process ID */
    DWORD procID;
    std::cout << "Enter process id: ";
    std::cin >> procID;


    /* Take user input for DLL name */
    std::string dllName;
    std::cout << "Enter DLL name or path: ";
    std::cin >> dllName;
    

    /* Check if DLL exists */
    std::filesystem::path dllpath(dllName);
    if (!std::filesystem::exists(dllpath)) {
        std::cout << "DLL not found";
        Sleep(2000);
        return -1;
    }


    /* Open a HANDLE to the process using OpenProcess on the process ID */
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 
        false, 
        procID);

    if (hProc == NULL) {
        std::cout << "Could not open handle to process";
        Sleep(2000);
        return -1;
    }
    else {
        std::cout << "Opened handle to process\n";
    }

    
    /* Allocate enough space inside the process to store the PATH to the DLL */
    LPVOID memAlloc = VirtualAllocEx(hProc,
        NULL,
        strlen(dllName.c_str()) + 1,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE);

    if (memAlloc == NULL) {
        std::cout << "Error allocating memory";
        Sleep(2000);
        return -1;
    }

    std::cout << "Base address: " << memAlloc << std::endl;


    /* Write the DLL path into the allocated memory region */
    BOOL write = WriteProcessMemory(hProc,
        memAlloc,
        dllName.c_str(),
        strlen(dllName.c_str()) + 1,
        NULL);

    if (write == 0) {
        std::cout << "Error writing DLL path to target process memory";
        Sleep(2000);
        return -1;
    }


    /* Create remote thread inside the target process to execute LoadLibraryA */
    HANDLE hThread = CreateRemoteThread(hProc, 
        0, 
        0, 
        (LPTHREAD_START_ROUTINE)LoadLibraryA, 
        memAlloc, 
        0, 
        0);

    if (hThread == NULL) {
        std::cout << "Failed to create remote thread";
        Sleep(2000);
        return -1;
    }


    /* Clean up and close handles */
    CloseHandle(hThread);
    CloseHandle(hProc);

    system("pause");
    return 0;
}