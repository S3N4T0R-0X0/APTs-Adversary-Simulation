# Static Kitten APT Adversary Simulation

This is a simulation of attack by (Static Kitten) APT group targeting multiple sectors across the Middle East including diplomatic, maritime, financial, and telecom entities. The campaign uses icon spoofing and malicious Word documents to deliver "RustyWater" a Rust-based implant representing a significant upgrade to their traditional toolkit, the attack campaign was active early as January 2026. The attackers has relied on PowerShell and VBS loaders for initial access and post-compromise operations. The introduction of Rust based implants represents a notable tooling evolution toward more structured modular and low noise RAT capabilities. I relied on cloudsek to figure out the details to make this simulation: https://www.cloudsek.com/blog/reborn-in-rust-muddywater-evolves-tooling-with-rustywater-implant

![imageedit_3_7338603847](https://github.com/user-attachments/assets/3248f058-273c-423d-b323-f94099898668)


Throughout, the macro itself is heavily obfuscated to dodge static detection, and the overall chain emphasizes low-noise execution with strong anti-analysis tricks built into the Rust binary. This progression — phishing email → malicious doc → obfuscated macro dropper → RustyWater implant — shows Static Kitten's shift toward more resilient modular tooling in recent campaigns targeting Middle East sectors.

![695fae75028332920172b552_54c74b00](https://github.com/user-attachments/assets/eb4ed208-5e65-425d-9723-51639870c813)

1. Delivery Technique: Create a document file named Cybersecurity.doc which is used in the next stage to embed and execute a VBA macro loader that extracts and drops the subsequent payload.

2. Malicious VBA macro: The malicious VBA macro embedded in the document is heavily obfuscated using string concatenation, hex-encoded payloads hidden in UserForm controls, multiple Replace() calls to strip whitespace and line breaks.

3. RustyWater implant: The final payload dropped by the obfuscated VBA macro loader is a Rust-compiled executable (disguised as reddit.exe with a fake Cloudflare icon), known as RustyWater (or linked to Archer RAT/RUSTRIC), featuring strong AV/EDR evasion through process injection, registry based persistence.

4. C2 infrastructure: relies on HTTP protocol for all communications, leveraging the Rust reqwest library with configurable timeouts, connection pooling, and retry mechanisms for reliability. Data payloads are structured as JSON, then encoded in Base64 followed by a final XOR encryption layer to obfuscate traffic and complicate analysis.

## The first stage Kill Chain (delivery technique)

The attack kicks off with a spear-phishing email sent from the compromised info@tmcell address a legitimate domain tied to TMCell (Altyn Asyr CJSC), Turkmenistan's main mobile telecom provider. The subject line reads "Cybersecurity Guidelines" making it look like an official advisory from a trusted source.

<img width="1600" height="354" alt="695fae75028332920172b555_a74569f3" src="https://github.com/user-attachments/assets/7be7c2f0-76d2-4271-8fb8-b10201859a29" />

The email carries an attachment named Cybersecurity.doc which acts as the initial payload. When the recipient opens the document and enables macros (as prompted), an obfuscated VBA macro runs. This macro extracts a hex-encoded blob hidden in the document's structure (often within UserForm elements) cleans it up by stripping spaces and line breaks, converts it to binary and drops it as CertificationKit.ini in the ProgramData folder.

<img width="584" height="585" alt="imageedit_8_8478180398" src="https://github.com/user-attachments/assets/64db8bb4-e6cb-4ae8-be91-fbd81f7bd6f2" />

## The second stage (Malicious VBA macro and Hex-encoded)

Sub love_me_____(): This subroutine serves as the main controller of the macro managing the entire payload workflow from start to finish. It begins by decoding a hidden file path from a hexadecimal string which was deliberately obfuscated to evade security software and string based detection methods. Once the real path is revealed the code checks if the payload file already exists on the victim's system. Based on this check it makes a strategic decision: if the file is present it executes it immediately if not it calls the download function to fetch the payload from a remote server before running it. This two pronged approach ensures the payload can operate whether the file is already on the system or needs to be delivered.

![photo_2026-03-09_16-52-35](https://github.com/user-attachments/assets/9fdd2efc-9726-42d0-aa0f-195c093a804c)

must specify the file path that is being checked and executed. For example you can use the path C:\ProgramData\CertificationKit.ini where CertificationKit.ini is the payload file. You can use https://www.hexhero.com/converters/text-to-hex to convert the path to HEX format for hiding it in the code.

Sub DownloadAndRun: This is the download manager of the payload responsible for reaching out to the internet and pulling down the file. It begins by decoding another hidden hexadecimal string that contains the actual URL where the payload is hosted. Using Windows HTTP services it establishes a connection to the remote server and downloads the file in binary format. Once the download completes successfully the code saves the file to the location specified earlier and immediately triggers its execution ensuring the payload becomes active on the system without any delay.

![photo_2026-03-09_16-54-29](https://github.com/user-attachments/assets/fb5f0856-399a-4ab3-9fe1-f939c54de155)

must put the download link for the payload file where CertificationKit.ini is the payload file. You must convert this link to HEX format just like we did with the file path.

Function DecodeHex: Serves as the decoder ring for the entire operation. This clever obfuscation technique helps hide true intentions by keeping file paths and download URLs encrypted until the very moment they're needed.

Sub ExecuteFile: Is the execution engine that ensures the payload file runs on the system. It employs multiple methods to launch the file first using the Shell command with hidden window settings then falling back to Windows Script Host for redundancy. Both approaches run silently in the background leaving no visual indicators for the victim to notice.

Sub AutoOpen: It executes the code automatically when the document is opened.

![photo_2026-03-09_16-56-38](https://github.com/user-attachments/assets/7b8a5018-9bd9-4166-aab3-689b29914d12)

## The third stage (RustyWater implanting technique)

RustyWater represents the main payload and the backbone of the entire adversarial operation in Static Kitten group attacks. 
RustyWater is a Rust compiled executable (disguised as reddit.exe with a fake Cloudflare icon) known as RustyWater (or linked to Archer RAT/RUSTRIC) featuring strong AV/EDR evasion through process injection, registry based persistence.

1. ANTI-ANALYSIS
Reddit.exe implements a comprehensive 8 layer anti-analysis system that actively probes the execution environment for signs of monitoring, virtualization, or debugging. Each layer acts as a filter ensuring the payload only detonates on a genuine target.

Layer 1: CPU Core Count Verification

Initially the RustyWater soldier looks around to gauge the power of the machine it finds itself on. It asks itself: How many cores does this processor have? Analysis environments, like sandboxes, are typically resource limited and often have two cores or fewer. If the soldier finds the machine is this weak it immediately decides the environment is unsafe and vanishes without a trace wasting all the analysts' efforts.

In the provided image a section of the Reddit.exe program's code illustrates this mechanism. The arrow points to the line checking the "cpu count" where the program examines the number of processor cores. If the count is two or less it means the surrounding environment is suspicious and execution is halted immediately.

![photo_2026-03-10_01-05-14](https://github.com/user-attachments/assets/8726b61b-992e-4399-9b74-69a0869ec3d6)

Layer 2: Virtual Machine Artifact Detection

Not convinced by the CPU check alone the soldier digs deeper. It knows that virtual machines leave behind specific digital footprints like a trail of breadcrumbs. It scans the list of running processes looking for familiar names associated with virtualization software: vmtoolsd.exe (VMware) vboxtray.exe (VirtualBox) and xenservice.exe (Xen). It also checks for the existence of specific driver files on disk such as vmmouse.sys or VBoxGuest.sys.

The logic is simple: if a machine is running VMware tools  it is likely a VM. If it is a VM it is likely an analysis environment. If it is an analysis environment the soldier aborts the mission.

![photo_2026-03-10_01-15-15](https://github.com/user-attachments/assets/db8ed5a6-9c15-4d6e-8d59-e20197ebab88)


Layer 3: Analysis Tool Registry Scanning

The soldier then ventures into the Windows Registry a vast database of system settings. It knows that security analysts often leave their tools behind and these tools leave artifacts. It searches for registry keys associated with debugging and monitoring software like Wireshark, Process Hacker, OllyDbg, and IDA Pro. The presence of any of these keys confirms the environment is hostile triggering an immediate shutdown.

![photo_2026-03-10_01-16-59](https://github.com/user-attachments/assets/fc2c14b2-b1ec-4cc2-9f06-6bbc1b064330)


Layer 4: RAM Size Analysis

the system reports less than 4GB, the soldier suspects a resource starved sandbox and halts execution. This check is a reliable way to filter out many automated analysis systems.

![photo_2026-03-10_01-12-53](https://github.com/user-attachments/assets/255cc4ec-df18-486b-83e2-6bfb4ed6b7ce)

Layer 5: Debugger Detection

The soldier now checks its immediate surroundings. It uses a simple but effective Windows API call—IsDebuggerPresent—to determine if it is being run under the control of a debugger. A debugger is like a microscope; if one is present, it means someone is watching the soldier's every instruction. The soldier will not perform under such surveillance.

![photo_2026-03-10_01-21-47](https://github.com/user-attachments/assets/55b527e6-2355-4c23-a6f3-56a13a882705)

Layer 6: System Uptime Check

Time itself becomes a factor. The soldier checks how long the system has been running since the last boot. Sandboxes and analysis environments are often freshly booted, right before a sample is executed. If the system uptime is less than 15 minutes, the soldier flags it as a suspicious, short lived environment and retreats.

![photo_2026-03-10_01-25-21](https://github.com/user-attachments/assets/9c14c6e5-10d6-4636-9cc5-701978957635)

Layer 7: Username Analysis

The soldier then checks the identity of the user. It compares the current username against a blacklist of common analysis accounts: "sandbox", "virus", "malware", "analysis", "vmware", and "test". These usernames are frequently used in isolated analysis environments. If the username matches any entry on the list, the mission is immediately aborted.

![photo_2026-03-10_01-27-33](https://github.com/user-attachments/assets/e8c8fb6f-42e9-44ac-9cb1-828a8521beb9)

Layer 8: MAC Address and Hardware Profile Verification

Finally the soldier looks at the machine's network card. It checks the MAC address against known vendor prefixes used by virtualization software. A MAC address starting with 00:0C:29 belongs to VMware, while 08:00:27 belongs to VirtualBox. It also scans hardware profiles for strings like "VMware" or "VirtualBox" in the system's description. If any of these are found, the soldier knows it is inside a virtual machine and pulls the plug.

![photo_2026-03-10_01-34-39](https://github.com/user-attachments/assets/02531d47-501e-41a0-9133-1ea4154acf9d)

2.REGISTRY PERSISTENCE
Once the anti analysis checks pass, Reddit.exe establishes robust registry persistence to ensure it survives system reboots:

<img width="998" height="43" alt="Screenshot From 2026-03-10 07-49-31" src="https://github.com/user-attachments/assets/4b13c18b-78c3-4083-b0bc-9fac6ee0dcb6" />

The implant cleverly disguises itself as a legitimate Windows Update component, making it less suspicious to casual observers. The persistence mechanism includes:

- Error handling for permission issues (fallback to HKCU if HKLM access fails)

- Path validation to ensure the executable exists at the specified location

- Startup verification to confirm the registry entry was successfully created

This ensures that every time the user logs in Reddit.exe automatically executes with their privileges.

![photo_2026-03-10_13-02-39](https://github.com/user-attachments/assets/16ba36b1-8c20-4fba-8a55-70532ea416bd)

3. PROCESS INJECTION

The final and most sophisticated capability is process injection into explorer.exe, allowing the implant to execute shellcode within a trusted system process.

Reddit.exe implements a classic but effective remote thread injection technique:

- Process Discovery: Scans running processes to locate explorer.exe and obtain its Process ID (PID).

- Handle Acquisition: Opens the target process with PROCESS_ALL_ACCESS privileges.

- Memory Allocation: Uses VirtualAllocEx to allocate RWX memory (Read, Write, Execute) within explorer.exe's address space.

- Shellcode Transfer: Writes the malicious payload using WriteProcessMemory.

- Execution: Creates a remote thread via CreateRemoteThread that points to the injected shellcode.

This technique allows the attackers to hide their malicious code inside a trusted system process, making detection significantly more difficult for security solutions.

![photo_2026-03-10_13-03-23](https://github.com/user-attachments/assets/957b2184-4396-42ef-a539-26f36bf97f49)

Why Attackers Use explorer.exe for Process Injection?

the attackers deliberately chose to target explorer.exe for injecting their malicious shellcode. This choice was not random—it was based on strategic advantages:

- Legitimate system process - Its presence is normal and doesn't raise alarms.

- Always running - Guaranteed availability on all Windows systems.

- User context - Inherits the current user's privileges.

- Persistence - Survives even if the main malicious process is terminated.
  
![photo_2026-03-10_13-04-19](https://github.com/user-attachments/assets/c45b572d-e5a6-4af0-bf20-084aee8231b4)

Put an HTTP payload: Encrypt it using XOR encryption to avoid detection over the network.


Modifying File Metadata to Evade Detection

After building the reddit.exe payload, we must modify its metadata to make it appear as a legitimate, non-suspicious program. Security solutions and EDR tools examine these metadata fields, so changing them is essential for evasion.


What Metadata Do Target?

File Version: The version number of the file
Product Version: The version number of the product
File Description: What the file describes itself as (appears in Task Manager)
Product Name: The name of the product
Company Name: The name of the developing company
LegalCopyright: Copyright information
OriginalFilename: The original name of the file

The tool used: rcedit

To perform these modifications, we use a lightweight, free tool called rcedit, officially available on GitHub.
link: https://github.com/electron/rcedit/releases/download/v2.0.0/rcedit-x64.exe

![photo_2026-03-10_13-08-33](https://github.com/user-attachments/assets/a7507be8-10fb-4db7-a8bf-ef0ba858e359)


- Dropper : CertificationKit.ini

I observed that the attackers do not execute reddit.exe directly on the target system. Instead they rely on an intermediary program known as CertificationKit.ini (Dropper) which carries the encrypted payload decrypts it at runtime and executes it on the victim is machine.

and that the dropper was written in Rust. Although it disguises itself as a seemingly harmless configuration file (CertificationKit.ini) it is in reality a compiled binary responsible for deploying and executing the main payload on the target system.

This technique provides several advantages:

1. Evasion: The actual payload remains encrypted making it more difficult for antivirus and security tools to detect it.

2. Analysis Bypass: Even if the dropper is discovered or analyzed the main payload remains encrypted and protected.

3. Multi Stage Execution: The attack operates in multiple stages which complicates the analysis process and slows down incident response for security teams.

![photo_2026-03-10_13-09-19](https://github.com/user-attachments/assets/490fb362-fefc-4bac-95e2-0d1e2e03f5fa)

Payload Encryption: The main payload (reddit.exe) is stored in the ENCRYPTED_PAYLOAD section and encrypted using the XOR key defined in the dropper (XOR_KEY). This ensures that the payload remains protected until runtime.

Target Path: The dropper writes the decrypted payload to the location specified in the dropper configuration (TARGET_PATH). This path must be correctly set to the intended directory on the victim is system.

Runtime Decryption: Upon execution the dropper decrypts the payload using the XOR key and deploys it as CertificationKit.ini at the target location.

This setup allows the attackers to maintain stealth, evade detection, and ensure the payload is only accessible when executed on the victim is system.

![photo_2026-03-10_13-10-59](https://github.com/user-attachments/assets/f318f3ae-e8b4-41b4-b62e-9daee6fe09e2)

## Final result: (Actual payload connect to BEAR-C2 server)

This payload establishes an HTTP connection to a remote server for command execution. It communicates using JSON formatted messages, with all data encrypted using XOR key.

![photo_2026-03-10_13-26-39](https://github.com/user-attachments/assets/6a2205fd-d6db-485e-af40-caded41cea67)


The client authenticates with a predefined ID and executes commands received from the server supporting both CMD and PowerShell commands. Output from executed commands is XOR encrypted and sent back to the server in JSON format. Data is also Base64 encoded for safe transmission.

![photo_2026-03-10_13-39-56](https://github.com/user-attachments/assets/7de9a4c7-f553-47e6-87d0-fe6f3b508576)

- Pivoting technique

VBA Macro Code Reuse: The macro code extracted from Cybersecurity.doc exhibits striking similarities to previously documented Muddy Water campaigns. Specifically, the WriteHexToFile and love_me_ function patterns, including the distinctive use of hex encoded payload embedding within UserForm controls.

The campaign analysed in this report shares significant overlap with another report.Similar TTPs can be observed in that chain too where the initial email was impersonating the L.M. Group, a legitimate Israeli HR company.

<img width="1484" height="710" alt="695fae75028332920172b570_9e14499d" src="https://github.com/user-attachments/assets/5a66af77-1496-476c-9e87-893a355694da" />




## MITRE ATT&CK Techniques

| Technique ID | Technique | Implementation |
|---------------|-----------|----------------|
| **T1566.001** | Phishing: Spearphishing Attachment | `Cybersecurity.doc` delivered via a compromised telecom domain. |
| **T1059.001** | Command and Scripting Interpreter: PowerShell | Executes C2 commands using PowerShell. |
| **T1059.005** | Command and Scripting Interpreter: VBA | Obfuscated `AutoOpen` VBA macro used to execute the initial payload. |
| **T1027** | Obfuscated Files or Information | Uses hexadecimal encoding, string concatenation, and hidden `UserForm` payloads to evade analysis. |
| **T1574.002** | Hijack Execution Flow: DLL Side-Loading | Loads a malicious DLL while disguised as a legitimate `.ini` file. |
| **T1547.001** | Registry Run Keys / Startup Folder | Establishes persistence through `HKLM`/`HKCU` Run keys disguised as Windows Update entries. |
| **T1055.001** | Process Injection: Remote Thread | Injects shellcode into `explorer.exe` using `CreateRemoteThread`. |
| **T1132** | Data Encoding | Encodes C2 traffic using Base64 combined with XOR encryption. |
| **T1071.001** | Application Layer Protocol: Web Protocols | Communicates with the C2 server over HTTP using the Rust `reqwest` library. |
| **T1036** | Masquerading | Disguises the payload as `reddit.exe` with a fake Cloudflare icon and spoofed file metadata. |
| **T1497** | Virtualization/Sandbox Evasion | Implements an 8-layer anti-analysis and sandbox evasion system. |
