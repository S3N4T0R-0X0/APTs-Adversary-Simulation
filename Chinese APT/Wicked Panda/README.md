# Wicked Panda APT Adversary Simulation

This is a simulation of attack by the Wicked Panda group (APT-41) targeting U.S. state government networks the attack campaign was active between May 2021 and February 2022, in addition to attacks targeting Taiwanese media, the attack chain starts with the in-memory execution of MoonWalk backdoor. Once the MoonWalk backdoor is successfully loaded by DodgeBox, the malware decrypts and reflectively loads two embedded plugins (C2 and Utility). The C2 plugin uses a custom encrypted C2 protocol to communicate with the attacker-controlled Google Drive account.
I relied on zscaler to figure out the details to make this simulation: 
part 1: https://www.zscaler.com/blogs/security-research/dodgebox-deep-dive-updated-arsenal-apt41-part-1

part 2: https://www.zscaler.com/blogs/security-research/moonwalk-deep-dive-updated-arsenal-apt41-part-2

![imageedit_3_7808595478](https://github.com/user-attachments/assets/9e7691fa-0407-409a-bf71-e0f6ea00d19e)

This attack included several stages including DodgeBox, a reflective DLL loader written in C, showcases similarities to StealthVector in terms of concept but incorporates significant improvements in its implementation. It offers various capabilities, including decrypting and loading embedded DLLs, conducting environment checks and bindings, and executing cleanup procedures. What sets DodgeBox apart from other malware is its unique algorithms and techniques.

![imageedit_2_3915351931](https://github.com/user-attachments/assets/1ddd642e-4cd1-4bb5-bfc1-6a8e342d6364)

1. Employs DLL sideloading as a means of executing DodgeBox. employs DLL sideloading as a means of executing DodgeBox. They utilize a legitimate executable (taskhost.exe).

2.  The malicious DLL, DodgeBox, serves as a loader and is responsible for decrypting a second stage payload from an encrypted DAT file (sbiedll.dat), The decrypted payload, MoonWalk functions as a backdoor.

3. Data exfiltration: over GoogleDrive API C2 Channe, This integrates GoogleDrive API functionality to facilitate communication between the compromised system and the attacker-controlled server thereby potentially hiding the traffic within legitimate GoogleDrive communication.


## The first stage (DodgeBox DLL loader)


This payload detects sandbox environments by checking for the presence of the SbieDll module and halts execution if found. It dynamically resolves API functions using obfuscated hashes to evade detection. The code allocates memory in the process using NtAllocateVirtualMemory, potentially for injecting or executing malicious code. It employs FNV-1a hashing to obscure strings like DLL and function names. Additionally, it uses DLL sideloading to execute DodgeBox, leveraging a legitimate executable like taskhost.exe to bypass security mechanisms.

1. Sandbox Detection

The SbieDll_Hook function attempts to detect a sandbox environment (e.g., Sandboxie) by checking for the SbieDll module using GetModuleHandle(L"SbieDll").

If the module is detected, it triggers an infinite sleep (Sleep(INFINITE)) to prevent further execution, a common evasion tactic used by malware to avoid analysis in sandboxed environments.

2. Triggering Core Logic

If the sandbox module is not detected, it proceeds to execute the core malicious functionality in the MalwareMain function.


![Screenshot From 2024-12-09 00-26-50](https://github.com/user-attachments/assets/c8380ffa-729b-452b-93ff-3b898f350b1f)


3. Dynamic API Resolution

The ResolveImport function dynamically resolves Windows API functions by loading the required DLLs and identifying functions by their hashed names. This is an obfuscation and anti-analysis technique to hide API calls.
It is specifically designed to resolve APIs like NtAllocateVirtualMemory.

4. Memory Allocation

MalwareMain uses the resolved API NtAllocateVirtualMemory to allocate memory in the process space. This allocated memory could be used for malicious purposes such as:
Injecting malicious code.
Executing a payload from memory.
The allocation is made with read-write permissions (PAGE_READWRITE).


![Screenshot From 2024-12-09 00-35-19](https://github.com/user-attachments/assets/9da6c6c8-65f6-4181-b658-71dbdc557e78)


5. Custom Hashing

The fnv1a_salted function calculates a hash value using the FNV-1a algorithm, which is commonly used in malware for:
Obfuscating strings or API names.
Making it harder for analysts to interpret the payload.
The provided test values demonstrate hashing of DLL and function names (ntdll and LdrLoadDll) with a specific salt.

![Screenshot From 2024-12-09 00-37-20](https://github.com/user-attachments/assets/b95faa30-b9aa-4d0c-a631-e06f37af81d3)


## The Second stage (generate obfuscated hashes - fnv1a_salted)

The fnv1a_salted function can be used to generate obfuscated hashes for string identifiers (like DLL or function names) by combining them with a salt. These hashes can then be used in the code for API function resolution or obfuscation to avoid detection.

![Screenshot From 2024-12-09 14-21-21](https://github.com/user-attachments/assets/07497535-0aaa-40ca-ac88-fb357c4d0ac0)


Use fnv1a_salted

Input Preparation:
Prepare the data (e.g., ntdll or LdrLoadDll) as a byte string.
Prepare the salt, which is also a byte string.
Use the default or custom seed_value for the hash calculation.

Function Usage:
Call fnv1a_salted(data, salt) to generate the obfuscated hash.
The hash output is a 32-bit integer, which can be converted to hexadecimal for use in your program.

Practical Application:
Use the calculated hash values as constants in your code for obfuscated API resolution, avoiding the use of plain strings (e.g., "ntdll").

Why the attackers Use Hashing?

1.Evasion: Prevents easy detection by antivirus or static analysis tools.
  
2.Obfuscation: Masks the real purpose of the payload by hiding sensitive strings.
  
3.Memory-Only Execution: Useful in memory-resident malware that avoids writing cleartext strings to disk.

## The third stage (MoonWalk backdoor)

This payload is a malicious program that establishes a reverse shell to an attacker's machine, enabling them to remotely execute commands on the victim's system via cmd.exe. It communicates with the attacker's system using an unencrypted TCP connection. The shell receives commands, executes them, and sends the output back to the attacker. To ensure persistence, it modifies the Windows Registry under HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run, adding a key named MoonWalkBackdoor. This key points to a file path intended to make the payload launch automatically on user login. However, the path specified (`C:\\Windows\\System32\\payload.dll`).

![Screenshot From 2024-12-29 07-24-47](https://github.com/user-attachments/assets/e4a1d6c4-923d-4a40-a179-e07e2a4fae9e)

1.Reverse Shell Creation:

  Purpose: Establishes a reverse shell connection to a remote attacker's machine (IP,port).
    Mechanism:
        Connects to the attacker's system.
        Opens a cmd.exe process on the victim's machine.
        Sends commands received from the attacker to the shell and returns the output to the attacker.

![Screenshot From 2024-12-29 07-31-37](https://github.com/user-attachments/assets/c8a504c7-e77a-4005-a3ad-03e33fa86943)


2. Persistence Mechanism:

Registry Manipulation:
Adds an entry in the Windows Registry under `HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run`.   
This ensures the backdoor runs every time the user logs in.
The value points to `C:\\Windows\\System32\\payload.dll` (a likely misconfiguration, as the current code produces an executable, not a DLL).

4. Networking:

Uses Windows' Winsock API to set up the socket connection to the attacker.

## The fourth stage (Data Exfiltration) over Google Drive API C2 Channe

I have previously performed Data Exfiltration during an Gossamer-Bear-APT attack via Google Drive. You can refer to this link: https://github.com/S3N4T0R-0X0/APT-Attack-Simulation/tree/main/Russian%20APT/Gossamer-Bear-APT for detailed steps on how this can be accomplished. However, in this particular attack, a more advanced and non-open-source version of BEAR-C2 was utilized for Data Exfiltration.

The attackers used the Google Drive C2 (Command and Control) API as a means to establish a communication channel between their payload and the attacker's server, By using Google Drive as a C2 server, attackers can hide their malicious activities among the legitimate traffic to GoogleDrive, making it harder for security teams to detect the threat. First i need to create a google drive account, as shown in the following figure

1.Log into the Google Cloud Platform

2.Create a project in Google Cloud Platform dashboard

3.Enable Google Drive API

4.Create a Google Drive API key


![337354597-b90e328c-5184-4072-adcb-6a6d7fb2debd](https://github.com/user-attachments/assets/8c63b7b4-6458-45ba-8715-374d471906dc)


I used the GoogleDrive C2 (Command and Control) API as a means to establish a communication channel between the payload and the attacker's server, By using GoogleDrive as a C2 server, i can hide the malicious activities among the legitimate traffic to GoogleDrive, making it harder for security teams to detect the threat.

![Screenshot From 2025-01-01 02-48-44](https://github.com/user-attachments/assets/0d47a318-c0c2-4846-b272-9ee30395b2c8)


## Final result: payload connect to Google Drive By using BEAR-C2


The final step in this process involves the execution of the final payload. After being decrypted and loaded into the current process, the final payload is designed to beacon out to both GoogleDrive API-based BEAR-C2 server profile.



https://github.com/user-attachments/assets/1c910c85-1ef5-4752-b8f9-0e06a64d3a0c




