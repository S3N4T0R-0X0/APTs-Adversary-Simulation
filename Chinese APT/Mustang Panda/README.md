# Mustang Panda APT Adversary Simulation

This is a simulation of attack by (Mustang Panda) APT group targeting government entities in Southeast Asia, the attack campaign was active from late September 2023, The attack chain starts with abuse of Visual Studio Code's reverse shell to execute arbitrary code and deliver additional payloads. To abuse Visual Studio Code for malicious purposes, an attacker can use the portable version of code.exe (the executable file for Visual Studio Code), or an already installed version of the software. By running the command code.exe tunnel, an attacker receives a link that requires them to log into GitHub with their own account, I relied on paloalto to figure out the details to make this simulation: https://unit42.paloaltonetworks.com/stately-taurus-abuses-vscode-southeast-asian-espionage/

![imageedit_3_4935006221](https://github.com/user-attachments/assets/8769d80f-3ddd-43d3-a6ff-9171af4e6acc)

This attack included several stages including redirect to a Visual Studio Code web environment that is connected to the compromised machine. They are then permitted to execute commands and scripts, and to create new files on the infected machine, Stately Taurus used this technique to deliver malware to infected environments, perform reconnaissance and exfiltrate sensitive data. To establish constant access to the reverse shell, the attacker created persistence for a script named (startcode.bat) using a scheduled task that is responsible for starting the shell.

1. Use Visual Studio Code's reverse shell to execute arbitrary code and deliver additional payloads.

2. used ToneShell to archive files for exfiltration, protecting the RAR archives with a unique password.

![imageedit_2_2412587919](https://github.com/user-attachments/assets/ccc11e78-40c9-4573-bb3f-1854e0058a0d)

## The first stage (delivery technique)

To abuse Visual Studio Code for malicious purposes, an attacker can use the portable version of code.exe (the executable file for Visual Studio Code), or an already installed version of the software. By running the command code.exe tunnel, an attacker receives a link that requires them to log into GitHub with their own account, One of the novel techniques Stately Taurus used to bypass security protections leverages Visual Studio Code’s embedded reverse shell feature to execute arbitrary code and deliver additional payloads. Truvis Thornton described this technique in a Medium post: https://medium.com/@truvis.thornton/visual-studio-code-embedded-reverse-shell-and-how-to-block-create-sentinel-detection-and-add-e864ebafaf6d

In the beginning i downloaded VScode on Windows Target Machine, then I logged in to my GitHub account in the browser, and through it I logged in to my VScode account.


![Screenshot from 2024-09-10 16-27-21](https://github.com/user-attachments/assets/4bb7cb38-9773-4440-85c6-e7d1a4aa8773)

_______________________________________________________________________________________________________________________

![Screenshot from 2024-09-10 16-27-50](https://github.com/user-attachments/assets/f691cecf-c1c5-4404-af2a-c89589c585db)

_______________________________________________________________________________________________________________________

To complete the process of linking the account you got with the VS code, I open CMD in Windows and write the command `code tunnel` so that the terminal gives us the link that we will use to complete the authentication process.

![Screenshot from 2024-09-10 16-45-42](https://github.com/user-attachments/assets/baaa7152-4263-4b4f-9e7e-694f2a42f52d)

_______________________________________________________________________________________________________________________


![Screenshot from 2024-09-10 16-48-09](https://github.com/user-attachments/assets/c7e2aeeb-762d-41dc-9363-21c9795d10ce)

_______________________________________________________________________________________________________________________



![Screenshot from 2024-09-10 16-49-02](https://github.com/user-attachments/assets/81e59367-3331-4ef0-b68b-abaa2e03695c)

_______________________________________________________________________________________________________________________


![Screenshot from 2024-09-10 16-49-23](https://github.com/user-attachments/assets/4e2fb722-876b-4585-974c-067615a8fc98)


_______________________________________________________________________________________________________________________

After that, the link will appear in the CMD, which i will use and open from the attacker browser to gain control over the victim VSCode through the attacker browser.

![photo_2024-09-10_18-12-27](https://github.com/user-attachments/assets/2a7236ba-bf11-4d56-b71a-346c509f165d)


_______________________________________________________________________________________________________________________

Now I have control over the target machine through my browser in Kali Linux.

![Screenshot from 2024-09-10 17-35-16](https://github.com/user-attachments/assets/c87e61d8-fbe6-4ef7-8b29-606aab7432fe)


_______________________________________________________________________________________________________________________

## The second stage (ToneShell Backdoor)

Upon logging in, the attacker is directed to a Visual Studio Code web environment linked to the compromised machine, where they are granted the ability to run commands, execute scripts, and create new files on the infected system.

Stately Taurus employed this method to deploy malware in compromised environments, carry out reconnaissance, and extract sensitive data. To ensure ongoing access to the reverse shell, the attacker set up persistence for a script called startcode.bat using a scheduled task that launches the shell.

If you need know more about ToneShell Backdoor: https://hunt.io/blog/toneshell-backdoor-used-to-target-attendees-of-the-iiss-defence-summit

![Screenshot from 2024-09-11 15-15-02](https://github.com/user-attachments/assets/c9b60539-4828-4eeb-9ea4-d319d746886b)


To create a payload similar to the ToneShell backdoor and incorporate the functionality described, I need to ensure several aspects are covered:

1. Proper Functionality: Implement SetupAndEnumWindowProps to allocate memory, set up function pointers, and enumerate window properties.

2. Password Handling: Implement logic to handle a password-protected RAR archive.

3. 64-bit Compatibility: Ensure that the code is suitable for 64-bit Windows systems.


![Screenshot from 2024-09-12 07-57-52](https://github.com/user-attachments/assets/61290a5f-ce8a-4878-b6a7-8d8e6131f558)

ToneShell Backdoor initializes data and dynamically allocates memory for a function pointer (PROPENUMPROCEXW), using it to enumerate window properties. It includes functions for dummy data handling, debug message output, and simulating file archiving with a password (without actual implementation). Key operations include:

  1.Data Handling: Obtains and processes dummy data.
  
  2.Memory Management: Allocates memory for executing function pointers.
  
  3.Window Enumeration: Uses EnumPropsExW() to list window properties.
  
  4.Debugging: Outputs static messages for diagnostics.

    manual compile: x86_64-w64-mingw32-gcc -o ToneShellBackdoor.exe ToneShellBackdoor.c -lwinhttp
    

## The third stage (execution technique)

Validate and process data

![Screenshot from 2024-09-12 08-23-09](https://github.com/user-attachments/assets/94004d84-3beb-4b9b-84f8-52562862de81)

Now I will use the terminal and start the execut the payload after I uploaded it through another feature on the web page that controls the VScode.

![Screenshot from 2024-09-12 08-32-33](https://github.com/user-attachments/assets/66eebaf2-2ff1-4aaa-afe3-af5654e9f5d6)

## The fourth stage (Data Exfiltration) over Dropbox API C2 Channe

![Screenshot from 2024-09-11 15-11-47](https://github.com/user-attachments/assets/04ddd7d4-444d-45aa-b3d8-5e08a21a50f1)


The attackers used the Dropbox C2 (Command and Control) API as a means to establish a communication channel between their payload and the attacker's server. By using Dropbox as a C2 server, attackers can hide their malicious activities among the legitimate traffic to Dropbox, making it harder for security teams to detect the threat.
First, we need to create a Dropbox account and activate its permissions, as shown in the following figure.

![Screenshot from 2024-03-12 16-10-13](https://github.com/S3N4T0R-0X0/APT29-Adversary-Simulation/assets/121706460/518a643a-f8bc-455c-acdd-a6ed6fe8735a)


After that, we will go to the settings menu to generate the access token for the Dropbox account, and this is what we will use in Dropbox C2.

![photo_2024-03-12_16-22-54](https://github.com/S3N4T0R-0X0/APT29-Adversary-Simulation/assets/121706460/00e41c7e-b2ac-4805-b1a9-77d00671ebf8)


In simulating this attack, I used the third profile found in BEAR-C2. https://github.com/S3N4T0R-0X0/BEAR


![Screenshot from 2024-09-12 09-35-43](https://github.com/user-attachments/assets/4651e43d-fec4-481d-bce4-4033119a5355)


Example: Upload a file to Dropbox

![Screenshot from 2024-09-12 09-05-57](https://github.com/user-attachments/assets/0cdd6f8f-4d65-41f3-a878-03d77becdd67)

## The fifth stage (Dump-Lsass by Mimikatz)

After this I uploaded the Mimikatz.exe to  dump for lsass credentials

![Screenshot from 2024-09-12 10-34-37](https://github.com/user-attachments/assets/77cee2bf-3577-4b95-9b6e-9863bcf78800)

## MITRE ATT&CK Techniques

| Technique ID | Technique | Implementation |
|---------------|-----------|----------------|
| **T1204.002** | User Execution: Malicious File | Requires the victim to execute the portable Visual Studio Code binary to initiate the attack chain. |
| **T1059** | Command and Scripting Interpreter | Executes attacker-issued commands through the established reverse shell. |
| **T1219** | Remote Access Software | Abuses Visual Studio Code Remote Tunnel as legitimate remote access software for persistence and remote administration. |
| **T1071.001** | Application Layer Protocol: Web Protocols | Uses HTTPS to communicate with the Visual Studio Code tunnel infrastructure. |
| **T1105** | Ingress Tool Transfer | Downloads additional payloads and tools from attacker-controlled infrastructure after initial access. |
| **T1102.001** | Web Service: Dead Drop Resolver | Uses GitHub authentication and Microsoft-hosted VS Code services to blend C2 traffic with legitimate cloud services. |
| **T1027** | Obfuscated Files or Information | Obfuscates payload components to reduce detection by static analysis. |
| **T1082** | System Information Discovery | Collects basic host information before establishing the remote session. |
| **T1057** | Process Discovery | Enumerates running processes to gather situational awareness on the compromised host. |
| **T1041** | Exfiltration Over C2 Channel | Returns command output and collected data through the established encrypted tunnel. |
