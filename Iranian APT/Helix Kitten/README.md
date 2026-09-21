# Helix Kitten APT Adversary Simulation

This is a simulation of attack by (Helix Kitten / APT34 / OilRig) APT group targeting multiple sectors across the Middle East including finance, government, energy, chemical industry, and telecommunications entities. The campaign uses a fake marketing services company called "GGMS" (Ganjavi Global Marketing Services) and malicious Word documents to deliver "SystemFailureReporter" - a GCC-compiled variant of the SideTwist Trojan. The attack campaign was active as early as August 2023. The attackers have relied on base64-encoded payloads and scheduled tasks for initial access and persistence. The introduction of GCC-compiled variants represents a notable tooling evolution toward more portable and low-noise backdoor capabilities. I relied on NSFOCUS to figure out the details to make this simulation: https://nsfocusglobal.com/apt34-unleashes-new-wave-of-phishing-attack-with-variant-of-sidetwist-trojan/
 
<img width="696" height="433" alt="imageedit_3_7254732259" src="https://github.com/user-attachments/assets/ae4d2ed1-2da6-407f-94ee-c602d4df8c82" />


Throughout, the macro itself is heavily obfuscated to dodge static detection, and the overall chain emphasizes low-noise execution with anti-sandbox tricks built into the Trojan. This progression — phishing email → malicious doc → base64-encoded dropper → SideTwist variant — shows Helix Kitten's shift toward more resilient modular tooling in recent campaigns targeting Middle East sectors.


<img width="1024" height="718" alt="Attack-Process-1024x718" src="https://github.com/user-attachments/assets/f88193d7-8afc-430f-8a40-a35408cc5460" />


1. Delivery Technique: Create a document file named GGMS_Overview.doc which is used in the next stage to embed and execute a VBA macro loader that extracts and drops the subsequent payload.

2. Malicious VBA macro: The malicious VBA macro embedded in the document is heavily obfuscated, extracting a base64-encoded payload hidden within the document structure, decoding it, and writing it to disk.

3. SystemFailureReporter Trojan: The final payload dropped by the VBA macro loader is a GCC-compiled executable (SystemFailureReporter.exe), a variant of the SideTwist Trojan, featuring anti-sandbox checks and HTTP-based C2 communication.

4. C2 infrastructure: relies on HTTP protocol for all communications, using a custom URI path containing the victim ID. Data payloads are structured as HTML responses with hidden instructions between script tags, encoded in Base64 followed by a multi-byte XOR encryption layer (key: "notmersenne") to obfuscate traffic and complicate analysis.

---

## The first stage Kill Chain (delivery technique)

The attack kicks off with a spear-phishing email disguised as a legitimate marketing services company. The decoy file is called "GGMS Overview.doc", and the document's body shows an introduction to a so-called "Ganjavi Global Marketing Services" company. The introduction claimed that the company was able to provide worldwide marketing services. Apparently, it targets enterprises. There are twice upload records, located in the United States, demonstrating that APT34 was actually targeted at United States businesses.

<img width="1024" height="637" alt="Decoy-doc-used-by-APT34-1024x637" src="https://github.com/user-attachments/assets/7e8f5e4c-5cab-4b0a-ae39-bc5c8c6201fa" />


The email carries an attachment named GGMS_Overview.doc which acts as the initial payload. When the recipient opens the document and enables macros (as prompted), an obfuscated VBA macro runs. This macro extracts a base64-encoded blob hidden in the document's structure, cleans it up, converts it to binary, and drops it as SystemFailureReporter.exe in the %LOCALAPPDATA%\SystemFailureReporter\ directory. It also creates a text file named update.xml under the same directory, acting as the start switch of the Trojan program.

---

## The second stage (Malicious VBA macro and Base64-encoded)

A special thanks to **[Mahmoud Mohamed](https://www.linkedin.com/in/m4v3x/)** for his valuable contribution to this adversary simulation project. He was responsible for developing and refining the VBA macro components used throughout this simulation. His expertise and contributions played an important role in making this project possible.


## Sub AutoOpen()

<img width="1366" height="701" alt="1" src="https://github.com/user-attachments/assets/44f5afad-e21d-4984-862e-d2c718e032bb" />


It executes the code automatically when the document is opened. Then the following actions run automatically:

1. Asks Windows for the `%LOCALAPPDATA%` path → gets `C:\Users\<User>\AppData\Local`, then adds `\SystemFailureReporter\` to it. This spot is hidden and needs no admin rights. It then checks if that folder exists — if not, it creates it.

2. Takes the Base64 string (the payload) and decodes it back into raw EXE bytes using `Base64Decode()`, then writes those bytes to disk as `SystemFailureReporter.exe` inside the folder.

3. Creates an empty `update.xml` file as a decoy.

4. Runs a hidden `schtasks` command that registers a task named `SystemFailureReporter` to launch the EXE every 5 minutes. No window pops up.


https://github.com/user-attachments/assets/01303a92-f480-488f-92f0-df2c464e11b8

---

## The third stage (SystemFailureReporter implanting technique)

SystemFailureReporter represents the main payload and the backbone of the entire adversarial operation in Helix Kitten group attacks.
SystemFailureReporter is a GCC-compiled executable (SystemFailureReporter.exe) known as a variant of the SideTwist Trojan, featuring strong anti-sandbox evasion and HTTP-based C2 communication.

### 1. ANTI-ANALYSIS

SystemFailureReporter.exe implements an anti-analysis system that actively probes the execution environment for signs of monitoring or sandboxing. Each layer acts as a filter ensuring the payload only detonates on a genuine target.


**Layer 1: update.xml Check**

After the Trojan runs, it will first check whether there is a file named update.xml in the same directory. If not, output a line of prompt text through the debugging port and exit. This is a typical anti-sandbox operation.

**Layer 2: Victim Fingerprinting**

The Trojan will then collect the user name, computer name and local domain name of the victim's host, assemble and calculate a 4-byte hash as the unique ID of the victim.

<img width="1119" height="286" alt="ANTI-ANALYSIS" src="https://github.com/user-attachments/assets/fa1c67a1-8be2-4275-8d49-1078ba0593db" />


### 2. REGISTRY PERSISTENCE

Once the anti-analysis checks pass, SystemFailureReporter.exe establishes robust persistence via a scheduled task to ensure it survives system reboots:

The implant cleverly disguises itself as a legitimate Windows component (SystemFailureReporter), making it less suspicious to casual observers. The persistence mechanism includes:

- Scheduled task named "SystemFailureReporter" that runs every 5 minutes
- Path validation to ensure the executable exists at the specified location
- Startup verification to confirm the scheduled task was successfully created

This ensures that every time the scheduled task is triggered, SystemFailureReporter.exe automatically executes.

<img width="1128" height="354" alt="REGISTRY PERSISTENCE" src="https://github.com/user-attachments/assets/f281a606-52b3-4e0a-a48b-8b515ea6e4f3" />


### 3. C2 COMMUNICATION

The final and most sophisticated capability is C2 communication over HTTP, allowing the implant to receive commands and exfiltrate data.

SystemFailureReporter.exe implements a classic but effective C2 communication technique:

- **Process Discovery:** The Trojan attempts to establish communication with the C2 and obtain return information using the generated victim ID.
- **Instruction Extraction:** If the C2 path is online, the Trojan will extract and parse specific contents in the HTML code returned by C2 into C2 instructions. These specific contents are hidden between `<script>/*` and `*/<script>` tags of the HTML code.
- **Decryption:** The C2 instruction is stored in base64 encoding and decrypted as a multi-byte XOR key with the string "notmersenne".
- **Execution:** The decrypted C2 instruction is divided into three segments, namely C2 number, C2 instruction code and operating parameters, which are separated by the symbol "|".

This technique allows the attackers to hide their malicious instructions inside seemingly benign HTML responses, making detection significantly more difficult for security solutions.

**C2 Instruction Codes:**

| C2 Instruction Code | Function |
|----------------------|----------|
| 101 | Run the shell command issued by C2, and the command line is specified by operation parameter 1. |
| 102 | Download the specified file on the C2 server. The file save path and remote file name are respectively specified by operating parameters 1 and 2. |
| 103 | Upload a local file to the C2 server. The file path is specified by operation parameter 1. |
| 104 | Execute the shell command issued by C2, and the command line is specified by operation parameter 1 (the same as instruction code 101). |

**Important:** The 102 instruction code of this Trojan will trigger a subsequent C2 communication behavior. The Trojan program will initiate an HTTP GET request according to the remote file name in the C2 instruction parameters, obtain and decrypt the files in the remote location "/getFile/[file name]". The decryption method is also base64 transcoding and multi-byte XOR.

After all the above C2 instructions are completed, the Trojan will reply an HTTP POST request to the C2 to report the instruction execution result. The POST request body contains information in the following format:

```
{"[C2 number]":"[C2 instruction execution result]"}
```

Unlike common Trojan programs, this Trojan does not have a cyclic or sleep mechanism and will automatically exit after a C2 communication, waiting for the scheduled task to invoke the Trojan again 5 minutes later.

Open [BEAR-C2](https://github.com/S3N4T0R-0X0/BEAR-C2), go to Reapr Node. In the Prepend field put `<script>/*`, and in the Append field put `*/<script>`

### Example

**Prepend:**

```<html>
  <body>
    <h1>Welcome</h1>
    <script>/*
    
```

**Append:**

```    */<script>
    <p>Some normal content</p>
  </body>
</html>
```

<img width="1356" height="684" alt="Screenshot From 2026-09-21 13-58-01" src="https://github.com/user-attachments/assets/41c53a52-ab4a-425f-b9b8-32ec440736a6" />


---

## Final result: (Actual payload connect to BEAR-C2 server)

This payload establishes an HTTP connection to a remote server for command execution. It communicates using custom-formatted messages, with all data encrypted using XOR key "notmersenne".

The client authenticates with a predefined victim ID and executes commands received from the server supporting both CMD and PowerShell commands. Output from executed commands is XOR encrypted and sent back to the server in HTML format. Data is also Base64 encoded for safe transmission.

---

## Pivoting technique

**VBA Macro Code Reuse:** The macro code extracted from GGMS_Overview.doc exhibits striking similarities to previously documented APT34 campaigns. Specifically, the base64-encoded payload embedding and the scheduled task creation patterns, including the distinctive use of %LOCALAPPDATA%\SystemFailureReporter\ directory.

The campaign analysed in this report shares significant overlap with another report. Similar TTPs can be observed in that chain too where the initial email was impersonating a legitimate marketing services company.

---

## MITRE ATT&CK Techniques

| Technique ID | Technique | Implementation |
|---------------|-----------|----------------|
| **T1566.001** | Phishing: Spearphishing Attachment | `GGMS_Overview.doc` delivered via a fake marketing services company. |
| **T1059.005** | Command and Scripting Interpreter: VBA | Obfuscated `AutoOpen` VBA macro used to execute the initial payload. |
| **T1027** | Obfuscated Files or Information | Uses base64 encoding and hidden payloads to evade analysis. |
| **T1547.001** | Scheduled Task/Job: Scheduled Task | Establishes persistence through `SystemFailureReporter` scheduled task running every 5 minutes. |
| **T1132** | Data Encoding | Encodes C2 traffic using Base64 combined with XOR encryption. |
| **T1071.001** | Application Layer Protocol: Web Protocols | Communicates with the C2 server over HTTP. |
| **T1036** | Masquerading | Disguises the payload as `SystemFailureReporter.exe` and the document as a legitimate marketing overview. |
| **T1497** | Virtualization/Sandbox Evasion | Implements anti-sandbox checks via `update.xml` file existence and victim fingerprinting. |
| **T1105** | Ingress Tool Transfer | Downloads files from the C2 server using HTTP GET requests. |

---

