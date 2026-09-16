# Helix Kitten APT Adversary Simulation

This is a simulation of attack by (Helix Kitten / APT34 / OilRig) APT group targeting multiple sectors across the Middle East including finance, government, energy, chemical industry, and telecommunications entities. The campaign uses a fake marketing services company called "GGMS" (Ganjavi Global Marketing Services) and malicious Word documents to deliver "SystemFailureReporter" - a GCC-compiled variant of the SideTwist Trojan. The attack campaign was active as early as August 2023. The attackers have relied on base64-encoded payloads and scheduled tasks for initial access and persistence. The introduction of GCC-compiled variants represents a notable tooling evolution toward more portable and low-noise backdoor capabilities. I relied on NSFOCUS to figure out the details to make this simulation: https://nsfocusglobal.com/apt34-unleashes-new-wave-of-phishing-attack-with-variant-of-sidetwist-trojan/

Throughout, the macro itself is heavily obfuscated to dodge static detection, and the overall chain emphasizes low-noise execution with anti-sandbox tricks built into the Trojan. This progression — phishing email → malicious doc → base64-encoded dropper → SideTwist variant — shows Helix Kitten's shift toward more resilient modular tooling in recent campaigns targeting Middle East sectors.

1. Delivery Technique: Create a document file named GGMS_Overview.doc which is used in the next stage to embed and execute a VBA macro loader that extracts and drops the subsequent payload.

2. Malicious VBA macro: The malicious VBA macro embedded in the document is heavily obfuscated, extracting a base64-encoded payload hidden within the document structure, decoding it, and writing it to disk.

3. SystemFailureReporter Trojan: The final payload dropped by the VBA macro loader is a GCC-compiled executable (SystemFailureReporter.exe), a variant of the SideTwist Trojan, featuring anti-sandbox checks and HTTP-based C2 communication.

4. C2 infrastructure: relies on HTTP protocol for all communications, using a custom URI path containing the victim ID. Data payloads are structured as HTML responses with hidden instructions between script tags, encoded in Base64 followed by a multi-byte XOR encryption layer (key: "notmersenne") to obfuscate traffic and complicate analysis.

---

## The first stage Kill Chain (delivery technique)

The attack kicks off with a spear-phishing email disguised as a legitimate marketing services company. The decoy file is called "GGMS Overview.doc", and the document's body shows an introduction to a so-called "Ganjavi Global Marketing Services" company. The introduction claimed that the company was able to provide worldwide marketing services. Apparently, it targets enterprises. There are twice upload records, located in the United States, demonstrating that APT34 was actually targeted at United States businesses.

The email carries an attachment named GGMS_Overview.doc which acts as the initial payload. When the recipient opens the document and enables macros (as prompted), an obfuscated VBA macro runs. This macro extracts a base64-encoded blob hidden in the document's structure, cleans it up, converts it to binary, and drops it as SystemFailureReporter.exe in the %LOCALAPPDATA%\SystemFailureReporter\ directory. It also creates a text file named update.xml under the same directory, acting as the start switch of the Trojan program.

---

## The second stage (Malicious VBA macro and Base64-encoded)

**Sub AutoOpen():** It executes the code automatically when the document is opened.

**Sub DeployPayload():** This subroutine serves as the main controller of the macro managing the entire payload workflow from start to finish. It begins by extracting a base64-encoded string hidden within the document. Once decoded, the payload file is written to the target directory. Based on this check it makes a strategic decision: if the file is present it executes it immediately if not it calls the download function to fetch the payload from a remote server before running it. This two pronged approach ensures the payload can operate whether the file is already on the system or needs to be delivered.

**Sub WriteHexToFile:** This function writes the decoded payload to disk as SystemFailureReporter.exe in %LOCALAPPDATA%\SystemFailureReporter\.

**Sub CreatePersistence():** Creates a scheduled task called "SystemFailureReporter" that calls up the Trojan every 5 minutes, through which it runs repeatedly.

**Sub ExecuteFile:** Is the execution engine that ensures the payload file runs on the system. It employs multiple methods to launch the file first using the Shell command with hidden window settings then falling back to Windows Script Host for redundancy. Both approaches run silently in the background leaving no visual indicators for the victim to notice.

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

### 2. REGISTRY PERSISTENCE

Once the anti-analysis checks pass, SystemFailureReporter.exe establishes robust persistence via a scheduled task to ensure it survives system reboots:

The implant cleverly disguises itself as a legitimate Windows component (SystemFailureReporter), making it less suspicious to casual observers. The persistence mechanism includes:

- Scheduled task named "SystemFailureReporter" that runs every 5 minutes
- Path validation to ensure the executable exists at the specified location
- Startup verification to confirm the scheduled task was successfully created

This ensures that every time the scheduled task is triggered, SystemFailureReporter.exe automatically executes.

### 3. C2 COMMUNICATION

The final and most sophisticated capability is C2 communication over HTTP, allowing the implant to receive commands and exfiltrate data.

SystemFailureReporter.exe implements a classic but effective C2 communication technique:

- **Process Discovery:** The Trojan attempts to establish communication with the CnC and obtain return information using the generated victim ID.
- **Instruction Extraction:** If the CnC path is online, the Trojan will extract and parse specific contents in the HTML code returned by CnC into CnC instructions. These specific contents are hidden between `<script>/*` and `*/<script>` tags of the HTML code.
- **Decryption:** The CnC instruction is stored in base64 encoding and decrypted as a multi-byte XOR key with the string "notmersenne".
- **Execution:** The decrypted CnC instruction is divided into three segments, namely CnC number, CnC instruction code and operating parameters, which are separated by the symbol "|".

This technique allows the attackers to hide their malicious instructions inside seemingly benign HTML responses, making detection significantly more difficult for security solutions.

**CnC Instruction Codes:**

| CnC Instruction Code | Function |
|----------------------|----------|
| 101 | Run the shell command issued by CnC, and the command line is specified by operation parameter 1. |
| 102 | Download the specified file on the CnC server. The file save path and remote file name are respectively specified by operating parameters 1 and 2. |
| 103 | Upload a local file to the CnC server. The file path is specified by operation parameter 1. |
| 104 | Execute the shell command issued by CnC, and the command line is specified by operation parameter 1 (the same as instruction code 101). |

**Important:** The 102 instruction code of this Trojan will trigger a subsequent CnC communication behavior. The Trojan program will initiate an HTTP GET request according to the remote file name in the CnC instruction parameters, obtain and decrypt the files in the remote location "/getFile/[file name]". The decryption method is also base64 transcoding and multi-byte XOR.

After all the above CnC instructions are completed, the Trojan will reply an HTTP POST request to the CnC to report the instruction execution result. The POST request body contains information in the following format:

```
{"[CnC number]":"[CnC instruction execution result]"}
```

Unlike common Trojan programs, this Trojan does not have a cyclic or sleep mechanism and will automatically exit after a CnC communication, waiting for the scheduled task to invoke the Trojan again 5 minutes later.

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
| **T1055** | Process Injection | Injects shellcode into `explorer.exe` using `CreateRemoteThread`. |
| **T1132** | Data Encoding | Encodes C2 traffic using Base64 combined with XOR encryption. |
| **T1071.001** | Application Layer Protocol: Web Protocols | Communicates with the C2 server over HTTP. |
| **T1036** | Masquerading | Disguises the payload as `SystemFailureReporter.exe` and the document as a legitimate marketing overview. |
| **T1497** | Virtualization/Sandbox Evasion | Implements anti-sandbox checks via `update.xml` file existence and victim fingerprinting. |
| **T1105** | Ingress Tool Transfer | Downloads files from the C2 server using HTTP GET requests. |

---

## IoC

| Type | Value |
|------|-------|
| SHA256 | `056378877c488af7894c8f6559550708` |
| SHA256 | `5e0b8bf38ad0d8c91310c7d6d8d7ad64` |
| URL | `http[:]//11.0.188[.]38:443/` |

### File System Artifacts

| Path | Description |
|------|-------------|
| `%LOCALAPPDATA%\SystemFailureReporter\` | SystemFailureReporter directory |
| `%LOCALAPPDATA%\SystemFailureReporter\SystemFailureReporter.exe` | SideTwist Trojan variant |
| `%LOCALAPPDATA%\SystemFailureReporter\update.xml` | Anti-sandbox marker / start switch |

### Scheduled Tasks

| Task Name | Variant |
|-----------|---------|
| `SystemFailureReporter` | Runs every 5 minutes |
