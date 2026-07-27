# Velvet Chollima APT Adversary Simulation

This is a simulation of an attack by the (Velvet Chollima) APT group targeting South Korean government officials. The attack campaign began in January 2025 and also targeted NGOs, government agencies, and media companies across North America, South America, Europe, and East Asia. The attack chain starts with a spear-phishing email containing a PDF attachment. However, when targets attempt to read the document, they are redirected to a Fake-Captcha link instructing them to run PowerShell as an administrator and execute attacker-provided code. This simulation is based on research from Microsoft's Threat Intelligence and Bleeping Computer: https://www.bleepingcomputer.com/news/security/dprk-hackers-dupe-targets-into-typing-powershell-commands-as-admin/


![imageedit_3_8386779397](https://github.com/user-attachments/assets/91dc82bd-27cf-4edc-a35a-a3b6cc87d909)

The attackers used a new tactic known as ClickFix, a social engineering technique that has gained traction, particularly for distributing malware.

ClickFix involves deceptive error messages or prompts that trick victims into executing malicious code themselves, often via PowerShell commands, ultimately leading to malware infections.

Microsoft's Threat Intelligence: https://x.com/MsftSecIntel/status/1889407814604296490

According to Microsoft's Threat Intelligence team, the attackers masquerade as South Korean government officials, gradually building trust with their targets. Once a certain level of rapport is established, the attacker sends a spear-phishing email with a PDF attachment. However, when targets attempt to read the document, they are redirected to a fake device registration link that instructs them to run PowerShell as an administrator and execute attacker-provided code.

https://www.bleepingcomputer.com/news/security/fake-google-meet-conference-errors-push-infostealing-malware/

![dialogs](https://github.com/user-attachments/assets/9d5a1b31-5479-4a67-826c-68195fb2c3a5)



1. Social Engineering: Create PDF file which will be sent spear-phishing.

2. ClickFix Technique: (Fake-Captcha) to make the target run PowerShell as an administrator and paste attacker-provided code.

3. Reverse Shell: Make simple reverse shell (payload.ps1) to creates a TCP connection to a command and control (C2) server and listens for commands to execute on the target machine.


## The first stage (delivery technique)

First the attackers created PDF file includes a Hyperlink that leads to a (Fake-Captcha) page, The advantage of the hyperlink is that it does not appear in texts, and this is exactly what the attackers wanted to exploit.

![Screenshot From 2025-02-14 17-40-19](https://github.com/user-attachments/assets/ffcebb81-af32-4700-83ed-1a916b6db8a5)

## The second stage (Fake-Captcha)

This file is a fake CAPTCHA verification page, designed as part of a phishing attack or malicious script execution.

![Screenshot From 2025-02-14 17-52-49](https://github.com/user-attachments/assets/3758dfcb-6725-4154-8466-03970b492afd)

If you need know more about fake CAPTCHA: https://www.bleepingcomputer.com/news/security/malicious-ads-push-lumma-infostealer-via-fake-captcha-pages/

Here’s what it does:

1. Fake Verification Interface

The page displays a message prompting the user to verify that they are not a robot.
The design mimics a legitimate CAPTCHA verification page but is actually completely fake.

2. Social Engineering Trick

When the user clicks the checkbox ("I'm not a robot"), the verify() function in JavaScript is triggered.
This function displays a popup instructing the user to execute specific commands in PowerShell.

3. Execution of Malicious PowerShell Code

The script automatically copies a PowerShell command to the clipboard.
If the user follows the instructions and executes the code, it:
Establishes a reverse shell connection to IP:PORT.

<img width="1299" height="652" alt="Screenshot From 2026-06-28 06-04-54" src="https://github.com/user-attachments/assets/7e1e9fb0-9451-4ae5-91de-76b758255f23" />


Allows the attacker to remotely execute commands on the victim’s machine.
Persists by adding itself to the Windows registry (Run Key), ensuring execution every time the system starts.

<img width="1335" height="445" alt="Screenshot From 2026-06-29 09-01-20" src="https://github.com/user-attachments/assets/deabd1f2-03af-4945-acf9-6cdd3829b356" />


## The third stage (Reverse shell by PowerShell)

The final result of this fake CAPTCHA attack is that the attacker gains remote access to the victim's machine through a reverse shell connection. Once the victim unknowingly runs the copied PowerShell command, their system establishes a connection to the attacker's server, allowing remote command execution.

This access enables the attacker to control the system, extract sensitive data, install additional malware, and potentially spread within a network if the victim is part of a corporate environment. To ensure persistence, the script modifies the Windows registry so that the malicious command runs every time the system starts. Even after a reboot, the attack remains active.

![Screenshot From 2025-02-14 18-22-11](https://github.com/user-attachments/assets/e5a74fee-7384-4f6c-9108-5b85f8227575)

## MITRE ATT&CK Techniques

| Technique ID | Technique | Implementation |
|---------------|-----------|----------------|
| **T1566.001** | Phishing: Spearphishing Attachment | Delivers a weaponized Microsoft Office document targeting South Korean government officials. |
| **T1204.002** | User Execution: Malicious File | Requires the victim to open the malicious document and enable macro execution. |
| **T1059.005** | Command and Scripting Interpreter: Visual Basic | Executes an obfuscated VBA macro to initiate the infection chain. |
| **T1059.001** | Command and Scripting Interpreter: PowerShell | Uses PowerShell to download and execute the next-stage payload. |
| **T1027** | Obfuscated Files or Information | Uses VBA obfuscation, encoded strings, and hidden macro logic to evade static analysis. |
| **T1105** | Ingress Tool Transfer | Downloads the payload from the attacker-controlled infrastructure. |
| **T1547.001** | Registry Run Keys / Startup Folder | Establishes persistence using Windows Registry Run keys. |
| **T1071.001** | Application Layer Protocol: Web Protocols | Communicates with the C2 server over HTTPS. |
| **T1036** | Masquerading | Uses legitimate-looking filenames and metadata to disguise the payload. |
| **T1497** | Virtualization/Sandbox Evasion | Detects virtualized or sandboxed environments before executing the payload. |
| **T1082** | System Information Discovery | Collects basic host information before processing attacker commands. |
