# Ember Bear APT Adversary Simulation

This is a simulation of attack by (Ember Bear) APT group targeting energy Organizations in Ukraine the attack campaign was active on April 2021, The attack chain starts wit spear phishing email sent to an employee of the organization, which used a social engineering theme that suggested the individual had committed a crime. The email had a Word document attached that contained a malicious JavaScript file that would download and install a payload known as SaintBot (a downloader) and OutSteel (a document stealer).
The OutSteel tool is a simple document stealer. It searches for potentially sensitive documents based on their file type and uploads the files to a remote server. The use of OutSteel may suggest that this threat group’s primary goals involve data collection on government organizations and companies involved with critical infrastructure. The SaintBot tool is a downloader that allows the threat actors to download and run additional tools on the infected system. SaintBot provides the actors persistent access to the system while granting the ability to further their capabilities. I relied on palo alto to figure out the details to make this simulation: https://unit42.paloaltonetworks.com/ukraine-targeted-outsteel-saintbot/

![imageedit_2_8449936728](https://github.com/S3N4T0R-0X0/Ember-Bear-APT/assets/121706460/755eabf4-c79a-4910-bddf-9a0c945c5141)

This attack included several stages including links to Zip archives that contain malicious shortcuts (LNK) within the spear phishing emails, as well as attachments in the form of PDF documents, Word documents, JavaScript files and Control Panel File (CPL) executables. Even the Word documents attached to emails have used a variety of techniques, including malicious macros, embedded JavaScript and the exploitation of CVE-2017-11882 to install payloads onto the system. With the exception of the CPL executables, most of the delivery mechanisms rely on PowerShell scripts to download and execute code from remote servers.

1. Create the Word Document: Write a Word document (.docx) containing the exploitation of CVE-2017-11882 to install payloads onto the system.

2. CVE-2017-11882: this exploit allow an attacker to run arbitrary code in the context of the current user by failing to properly handle objects in memory.

3. Data exfiltration: over Discord API C2 Channe, This integrates Discord API functionality to facilitate communication between the compromised system and the attacker-controlled server thereby potentially hiding the traffic within legitimate Discord communication.

4. SaintBot: is a payload loader, It contains capabilities to download further payloads as requested by attackers.

5. The attackers used .BAT file to disable Windows Defender functionality, It accomplishes this by executing multiple commands via CMD that modify registry keys and disabling Windows Defender scheduled tasks.

6. OutSteel: is a file uploader and document stealer developed with the scripting language.


Some examples of the PDF and docx files that was used in this attack.

![imageedit_3_9227726456](https://github.com/S3N4T0R-0X0/Ember-Bear-APT/assets/121706460/4a8bf68d-d249-457b-b046-ebc8cfcf3b9b)


## The first stage (delivery technique)

In the beginning, I will create a Word file that I will use to  injections for a vulnerability that attackers used in the actual attack to install payloads on the system.

April 2021: Bitcoin-themed spear phishing emails targeting Ukrainian government organizations.

![Screenshot from 2024-06-26 07-39-00](https://github.com/S3N4T0R-0X0/Ember-Bear-APT/assets/121706460/b360e79a-194d-4ff1-89c9-5f4169a0b2e4)


## The second stage (exploit Microsoft Office Memory Corruption Vulnerability CVE-2017-11882)

Second the attackers exploited the Zero-day vulnerability (CVE-2017-11882) is a vulnerability in Microsoft Office, specifically affecting Microsoft Office 2007 Service Pack 3, Microsoft Office 2010 Service Pack 2, Microsoft Office 2013 Service Pack 1, and Microsoft Office 2016. This vulnerability is classified as a memory corruption issue that occurs due to improper handling of objects in memory.

Exploitation repository: https://github.com/0x09AL/CVE-2017-11882-metasploit?tab=readme-ov-file 

This vulnerability allow an attacker to run arbitrary code in the context of the current user by failing to properly handle objects in memory, I then placed a Word file in the 
phishing email, including links to Zip files containing malicious shortcuts (LNK).



![Screenshot from 2024-06-26 07-28-07](https://github.com/S3N4T0R-0X0/Ember-Bear-APT/assets/121706460/d1b43f2a-6816-44fb-ba5e-f407da4b0152)



`sudo cp cve_2017_11882.rb /usr/share/metasploit-framework/modules/exploits/windows/fileformat`

`sudo updatedb`

`msf6 > use exploit/windows/fileformat/office_ms17_11882`


## The third stage (Data Exfiltration) over Discord API C2 Channe

The attackers used the Discord C2 (Command and Control) API as a means to establish a communication channel between their payload and the attacker's server. By using Discord as a C2 server, attackers can hide their malicious activities among the legitimate traffic to Discord, making it harder for security teams to detect the threat.


![Screenshot from 2024-06-25 14-43-39](https://github.com/S3N4T0R-0X0/Ember-Bear-APT/assets/121706460/bace255e-fb00-447a-81a9-91dea91f01df)


First, i need to create a Discord account and activate its permissions, as shown in the following figure.


1. Create Discord Application.

<img width="1679" alt="image-20231221113019757" src="https://github.com/S3N4T0R-0X0/Ember-Bear-APT/assets/121706460/2ac6a61e-719d-49eb-a610-f75808dd5e1a">

2. Configure Discord Application.


<img width="1679" alt="image-20231221113340790" src="https://github.com/S3N4T0R-0X0/Ember-Bear-APT/assets/121706460/f88c0cc8-1abd-462e-bdff-0adadc85914a">


3. Go to "Bot", find "Privileged Gateway Intents", turn on all three "Intents", and save.


<img width="1679" alt="image-20231221113617087" src="https://github.com/S3N4T0R-0X0/Ember-Bear-APT/assets/121706460/cbb3858f-f51c-454f-8458-6478a56b92c6">




This script integrates Discord API functionality to facilitate communication between the compromised system and the attacker-controlled server, thereby potentially hiding the traffic within legitimate Discord communication and checks if the Discord bot token and channel ID are provided. If they are, it starts the Discord bot functionalities; otherwise, it proceeds with just the IP and port. This way, the script can continue the connection without the Discord details if they are not entered.

![photo_2024-07-02_11-38-29](https://github.com/S3N4T0R-0X0/Ember-Bear-APT/assets/121706460/6fb172b1-a864-470c-9535-d899dba65d5b)

## The fourth stage (SaintBot payload Loader)

SaintBot is a recently discovered malware loader, documented in April 2021 by MalwareBytes. It contains capabilities to download further payloads as requested by threat actors, executing the payloads through several different means, such as injecting into a spawned process or loading into local memory. It can also update itself on disk – and remove any traces of its existence – as and when needed. SHA-256: e8207e8c31a8613112223d126d4f12e7a5f8caf4acaaf40834302ce49f37cc9c

1.Locale Check: The IsSupportedLocale function checks if the system's locale matches specific locales.

2.Downloading Payload: The DownloadPayload function downloads a file from a specified URL and saves it to a specified filepath.


![Screenshot from 2024-07-04 15-36-33](https://github.com/S3N4T0R-0X0/Ember-Bear-APT/assets/121706460/75ddbafe-5642-42b0-8e46-17bdc923ebd9)


3.Injecting into a Process: The InjectIntoProcess function injects a DLL into a running process by its name.

4.Self-Deleting: The SelfDelete function deletes the executable after its execution.

![Screenshot from 2024-07-04 15-34-57](https://github.com/S3N4T0R-0X0/Ember-Bear-APT/assets/121706460/4ecb437f-1fb8-4264-bb3d-3f4e931c1e34)


## The fifth stage (disable windows defender)

This batch file is used to disable Windows Defender functionality. It accomplishes this by executing multiple commands via CMD that modify registry keys and disabling Windows Defender scheduled tasks. 


![Screenshot from 2024-06-18 08-31-01](https://github.com/S3N4T0R-0X0/Ember-Bear-APT/assets/121706460/2d3ddff9-2068-432a-ad13-907844e5d376)


## The sixth stage (OutSteel stealer)

OutSteel is a file uploader and document stealer developed with the scripting language AutoIT. It is executed along with the other binaries. It begins by scanning through the local disk in search of files containing specific extensions, before uploading those files to a hardcoded command and control (C2) server. I simulated this Infostealer but through PowerShell Script.


![Screenshot from 2024-07-04 15-37-31](https://github.com/S3N4T0R-0X0/Ember-Bear-APT/assets/121706460/85849f48-7608-4db2-b6c1-c8b924e36d39)

## MITRE ATT&CK Techniques

| Technique ID | Technique | Implementation |
|---------------|-----------|----------------|
| **T1566.001** | Phishing: Spearphishing Attachment | Delivers a weaponized Microsoft Word document through a spearphishing email. |
| **T1203** | Exploitation for Client Execution | Exploits **CVE-2017-11882 (Microsoft Office Equation Editor)** to execute code without requiring macros. :contentReference[oaicite:0]{index=0} |
| **T1059.007** | Command and Scripting Interpreter: JavaScript | Executes the embedded JavaScript downloader (SaintBot). |
| **T1105** | Ingress Tool Transfer | SaintBot downloads and executes the next-stage payload from attacker-controlled infrastructure. |
| **T1547.001** | Registry Run Keys / Startup Folder | Establishes persistence through Windows Registry Run keys. |
| **T1005** | Data from Local System | OutSteel collects documents from the local system based on targeted file extensions. |
| **T1039** | Data from Network Shared Drive | Searches accessible network shares for documents of interest. |
| **T1102.001** | Web Service: Dead Drop Resolver | Uses Discord as a legitimate web service for C2 and data transfer through `Discord-C2.py`. |
| **T1071.001** | Application Layer Protocol: Web Protocols | Communicates with Discord over HTTPS. |
| **T1567.001** | Exfiltration Over Web Service | Exfiltrates collected documents through Discord using the Discord API. |
| **T1027** | Obfuscated Files or Information | Uses obfuscation techniques to hinder static analysis of scripts and payloads. |
| **T1497** | Virtualization/Sandbox Evasion | Detects virtualized or sandboxed environments before continuing execution. |
| **T1082** | System Information Discovery | Collects basic host information before executing follow-on actions. |
