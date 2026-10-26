#!/bin/bash

# ========================================================================
# Environment Setup & Build Script for SignedConnection (Charming Kitten)
# ========================================================================

echo "[*] Starting environment setup for SignedConnection.exe..."

# 1. Update package lists
echo "[*] Updating package lists..."
sudo apt-get update

# 2. Install Mono (C# compiler) and Wine (Windows compatibility layer)
echo "[*] Installing Mono and Wine..."
sudo apt-get install -y mono-mcs wine winetricks

# 3. Check if the C# source file exists
if [ ! -f "SignedConnection.cs" ]; then
	echo "[!] Error: SignedConnection.cs not found in the current directory."
	echo "[!] Please ensure the file is in the same folder as this script."
	exit 1
fi

# 4. Compile the C# code
echo "[*] Compiling SignedConnection.cs..."
# Using TERM=konsole to bypass the known Mono terminfo 4K crash on Kali Linux
TERM=konsole mcs -platform:x64 -r:System.Windows.Forms -r:System.Drawing -out:SignedConnection.exe SignedConnection.cs

if [ $? -eq 0 ]; then
	echo "[+] Compilation successful: SignedConnection.exe created."
else
	echo "[!] Compilation failed. Please check the errors above."
	exit 1
fi

# 5. Setup Wine environment for .NET Framework (Required for WinForms)
echo "[*] Checking Wine .NET environment..."
# Check if dotnet48 is already installed in the Wine prefix
if ! winetricks list-installed | grep -q "dotnet48"; then
	echo "[*] .NET Framework 4.8 not found in Wine. Installing now..."
	echo "[*] NOTE: This will open a few installation windows. Please click 'Next' and 'Install' through them."
	winetricks dotnet48
	
	if [ $? -ne 0 ]; then
	    echo "[!] Failed to install .NET Framework 4.8. The GUI might not render correctly."
	else
	    echo "[+] .NET Framework 4.8 installed successfully."
	fi
else
	echo "[+] .NET Framework 4.8 is already installed in Wine."
fi

# 6. Instructions to run
echo ""
echo "====================================================================="
echo "[+] Setup Complete!"
echo "[*] To run your GUI, execute:"
echo "    wine SignedConnection.exe"
echo ""
echo "[!] IMPORTANT: Make sure your malicious 'secur32.dll' is in the"
echo "    exact same folder as SignedConnection.exe before clicking Connect."
echo "====================================================================="
