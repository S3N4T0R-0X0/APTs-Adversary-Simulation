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


## The first stage (DodgeBox) DLL loader


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


