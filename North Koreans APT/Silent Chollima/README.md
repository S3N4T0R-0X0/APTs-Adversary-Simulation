# Silent Chollima APT Adversary Simulation

This is a simulation of attack by (Silent Chollima) APT group targeting several customers and their users in North America, Asia, and Europe. The attack campaign was active in June 2025, have sent a link leading to a ZIP or RAR archive file. Inside this file would be a legitimate executable that was given a filename relevant to the targeted organization or tied to the theme of the spear phish email.When executed, this legitimate executable would load a malicious payload in an included Dynamic Link Library (DLL), via search order hijacking which provided operators with the ability to remotely execute commands on infected devices. I relied on volexity to figure out the details to make this: https://www.volexity.com/blog/2025/10/08/apt-meets-gpt-targeted-operations-with-untamed-llms/

![1623606708028](https://github.com/user-attachments/assets/7d4cc2c8-fce1-4e7c-b276-2a7bed69248f)

1. Social engineering technique: The attackers sent phishing emails containing HTML that included an image to make it appear a document was attached to the email. If the image were clicked, it led to the download of a remotely hosted archive file.

2. GOVERSHELL: All implants were DLL files that were loaded via search order hijacking from the legitimate version of either the 32- or 64-bit version of an open-source project.

3. Persistence & C2 traffic: Each variant of GOVERSHELL sets up persistence via a scheduled task on its first execution and includes a command-line flag in that persistence execution, which is required to execute the logic that includes C2 communication.

<img width="938" height="289" alt="imageedit_1_9709470955" src="https://github.com/user-attachments/assets/69920aa9-ecc3-48d6-823a-06e26ef433e7" />



## The first stage (social engineering technique)

Silent Chollima primary and sole method for targeting organizations is by conducting spear phishing campaigns. Between June and August 2025, Silent Chollima sent phishing emails containing HTML that included an image to make it appear a document was attached to the email. If the image were clicked, it led to the download of a remotely hosted archive file. Users would then need to open and execute the executable file within the archive in order to become infected. An example body from one such email is included below.

<img width="606" height="403" alt="imageedit_3_3905276592" src="https://github.com/user-attachments/assets/2de80e43-4933-421d-bb61-c34281e6bc5e" />

I recreated an HTML file identical to that used by the attackers. When the PDF icon is clicked, it redirects the user to a browser link that downloads the malicious payload hosted by the BEAR-C2 project.

<img width="1365" height="735" alt="Screenshot From 2025-12-23 04-36-43" src="https://github.com/user-attachments/assets/39124cb1-2a81-4858-979b-de5716f56345" />

## The second stage (GOVERSHELL payload)

Throughout various campaigns Volexity observed active changes in the malware, with significant differences in how the malware communicated and functioned. All variants observed by Volexity make use of a scheduled task for persistence and provide the operator the ability to execute arbitrary commands on the target’s device. With the exception of the first variant, all GOVERSHELL implants were DLL files that were loaded via search order hijacking from the legitimate version of either the 32- or 64-bit version of an open-source project called Tablacus Explorer.

<img width="931" height="491" alt="Screenshot From 2025-12-23 05-42-05" src="https://github.com/user-attachments/assets/2c79644f-2ae3-4005-885f-7b0f144a773d" />

GOVERSHELL is a stealthy Windows implant that communicates over HTTPS with a remote command and control server.It uses XOR encryption combined with Base64 to protect all network traffic and payloads from casual inspection. Upon first execution without its specific argument, it silently copies itself to a random folder in C:\ProgramData and establishes persistence via a hidden scheduled task. Once persistent, it repeatedly checks in with the server, receives encrypted commands, executes them locally (via cmd or PowerShell), and sends back results. The implant applies configurable jitter delays between communications to evade pattern-based detection.

<img width="930" height="398" alt="Screenshot From 2025-12-23 05-43-30" src="https://github.com/user-attachments/assets/f135f9a3-d84d-4c5e-b9c4-cc74c0b5f854" />

## The third stage (Persistence & C2 traffic)

This function creates a hidden scheduled task named "SystemHealthMonitor" using schtasks.exe executed silently via CreateProcessA.
It builds a command string that runs the copied malware with -SilentChollima every 5 minutes at highest privileges. The process is launched with CREATE_NO_WINDOW and SW_HIDE flags to prevent any visible console or window. It safely copies the command into a fixed buffer, null-terminates it, and waits up to 5 seconds for completion. Returns true on success, ensuring stealthy, self-healing persistence without dropping additional files.

<img width="1257" height="474" alt="Screenshot From 2025-12-23 05-45-16" src="https://github.com/user-attachments/assets/5bb24a9c-c6de-4204-8404-386b8f43e660" />

C&C server on HTTPS (port 465): The implant communicates exclusively over encrypted HTTPS channels with the remote command-and-control server. All network traffic, including check ins, task retrieval, and results, is protected using XOR encryption (key: 11) combined with Base64 encoding before transmission. When a command is received, it is first XOR decrypted, then checked for the "EP" prefix to determine if it should be executed via PowerShell or cmd.exe. PowerShell commands are properly escaped and run silently using powershell.exe -NoProfile -NonInteractive -Command, with full stdout/stderr capture. The command output is captured, XOR-encrypted with the same key, Base64-encoded, and securely sent back to the C2 server over HTTPS.

![photo_2025-12-23_06-15-38](https://github.com/user-attachments/assets/f5d3f945-5ae7-432e-bc31-4dbe3b58b275)





## MITRE ATT&CK Techniques

| Technique ID | Technique | Implementation |
|---------------|-----------|----------------|
| **T1566.001** | Phishing: Spearphishing Attachment | Delivers a weaponized Microsoft Office document to the target. |
| **T1204.002** | User Execution: Malicious File | Requires the victim to open the document and enable macro execution. |
| **T1059.005** | Command and Scripting Interpreter: Visual Basic | Executes an obfuscated VBA macro to initiate the attack chain. |
| **T1059.001** | Command and Scripting Interpreter: PowerShell | Uses PowerShell to download and execute the next-stage payload. |
| **T1027** | Obfuscated Files or Information | Uses VBA obfuscation, encoded strings, and hidden macro logic to evade static analysis. |
| **T1105** | Ingress Tool Transfer | Downloads the second-stage payload from the attacker-controlled C2 server. |
| **T1547.001** | Registry Run Keys / Startup Folder | Establishes persistence through Windows Registry Run keys. |
| **T1071.001** | Application Layer Protocol: Web Protocols | Communicates with the C2 server over HTTPS. |
| **T1036** | Masquerading | Disguises payloads using trusted filenames and legitimate-looking metadata. |
| **T1497** | Virtualization/Sandbox Evasion | Detects virtualized and sandboxed environments before executing the payload. |
| **T1082** | System Information Discovery | Collects basic host information before continuing execution. |
