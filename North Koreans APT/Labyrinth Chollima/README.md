# Labyrinth Chollima APT Adversary Simulation

This is a simulation of attack by (Labyrinth Chollima) APT group targeting victims working on energy company and the aerospace industry, the attack campaign was active before June 2024, The attack chain starts with relies on legitimate job description content to target victims employed in U.S. critical infrastructure verticals. The job description is delivered to the victim in a password-protected ZIP archive containing an encrypted PDF file and a modified version of an open-source PDF viewer application, I relied on Mandiant to figure out the details to make this simulation: https://cloud.google.com/blog/topics/threat-intelligence/unc2970-backdoor-trojanized-pdf-reader/?linkId=10998021

![imageedit_3_4780888868](https://github.com/user-attachments/assets/50214b93-9f5c-40ed-a31e-50aaacf448cc)

Based on the surrounding context, the user was instructed to open the PDF file with the enclosed trojanized PDF viewer program based on the open-source project SumatraPDF.

SumatraPDF is an open-source document viewing application that is capable of viewing multiple document file formats such as PDF, XPS, and CHM, along with many more. Its source code is publically available. 
If you need to know more about SumatraPDF: https://github.com/sumatrapdfreader/sumatrapdf

When accessed this way, the DLL files are loaded by the SumatraPDF.exe executable, including the trojanized libmupdf.dll file representing the first stage of the infection chain. This file is responsible for decrypting the contents of BAE_Vice President of Business Development.pdf, thus allowing the job description document to be displayed as well as loading into memory the payload named MISTPEN. Mandiant found that later versions (after 3.4.3) of SumatraPDF implement countermeasures to prevent modified versions of this DLL from being loaded.

![imageedit_5_5991500850](https://github.com/user-attachments/assets/2c6f83ad-eab6-4445-98ec-5265d1b6dd34)

1. Create job description PDF file which will be sent spear phishing.

2. Use Shellter to inject DLL in the Sumatra pdf.exe which I will use in the attack through the pdf file that is used in the phishing campaign.

3. Executes the PDF file while simultaneously running (SumatraPDF.exe) in the background. This executable contains the DLL payload, through which I will establish the reverse connection.


4. Waits for the incoming connection from the backdoor to data exfiltration when it is executed on the target machine.


5. Executing an encrypted payload stored in a file and writing it to disk by BURNBOOK launcher



## The first stage (delivery technique)

Since the attackers here wanted to target victims working on energy company and the aerospace industry, The attack starts with relies on legitimate job description content to target victims employed in U.S, Now I will create a pdf file with the same phishing message that the actual attackers used in their phishing campaign.


![Screenshot from 2024-09-19 15-34-00](https://github.com/user-attachments/assets/99564509-8016-49e9-aba7-925c89232f3d)


## The Second stage (implanting technique)

Now I need to download the SumatraPDF program before version 3.4.3, so I chose version 3.4.1 because after 3.4.3 of SumatraPDF implement countermeasures to prevent modified versions of this DLL from being loaded.

uptodown to download the old version: https://sumatra-pdf-portable.en.uptodown.com/windows/download/62634650

![photo_2024-09-19_17-52-12](https://github.com/user-attachments/assets/619252d2-0a0c-4d32-93ce-8af1c4a6e720)

After that I will use Shellter to inject DLL in the Sumatra pdf.exe which I will use in the attack through the pdf file that is used in the phishing campaign.

![Screenshot from 2024-09-20 17-34-18](https://github.com/user-attachments/assets/8bcfa575-2d4f-4ba7-939e-b96d6156a75b)

After injecting the malicious DLL into (SumatraPDF.exe), I will bundle it with a non-malicious (job description.pdf) file inside a ZIP archive. When the user open the ZIP file, it will trigger the background execution of the injected (SumatraPDF.exe), which will establish a reverse connection.

![Screenshot from 2024-09-20 19-01-53](https://github.com/user-attachments/assets/5150035c-d570-4a6e-9455-d8124cc7c6c7)


In a previous simulation of a Russian APT, I used a similar approach but with an image file instead of a PDF. I will replicate the same method now but without selecting the icon.

![20240302_194641](https://github.com/S3N4T0R-0X0/APT29-Adversary-Simulation/assets/121706460/b3b7872e-1bf9-4637-a13f-ba720c113276)


## The third stage (execution technique)

Now, when I open the ZIP file, it executes the PDF file while simultaneously running (SumatraPDF.exe) in the background. This executable contains the DLL payload, through which I will establish the reverse connection.

![Screenshot from 2024-09-20 18-59-13](https://github.com/user-attachments/assets/89306f3f-8d52-4e9b-9cc7-99ed3d487aa3)

## The fourth stage (Backdoor Listener)

In simulating this attack, I used the sixth C2 profile found in BEAR-C2.

https://github.com/S3N4T0R-0X0/BEAR

This C2-profile waits for the incoming connection from the backdoor when it is executed on the target machine.

![Screenshot from 2024-09-20 19-14-34](https://github.com/user-attachments/assets/f77cf010-c902-4d4a-bf34-437a31c9850e)


## The fifth stage (DLL backdoor)

BURNBOOK is a launcher written in C that is capable of executing an encrypted payload stored in a file and writing it to disk. 
This file is a modified version of a legitimate DLL file used by the SumatraPDF.exe binary. The DLL contains malicious code that is triggered when the user opens the PDF lure (BAE_Vice President of Business Development.pdf) using the provided SumatraPDF.exe file.

![Screenshot from 2024-09-21 12-50-16](https://github.com/user-attachments/assets/41832c2e-db12-431b-b5a0-05249d64b2f6)

The BURNBOOK  includes a network connectivity check that prevents the trojanized reader from displaying the decrypted PDF lure if it cannot reach google[.]com. 

![Screenshot from 2024-09-21 12-53-34](https://github.com/user-attachments/assets/517c7639-dcb9-4ac9-9491-e96163742c14)


## MITRE ATT&CK Techniques

| Technique ID | Technique | Implementation |
|---------------|-----------|----------------|
| **T1566.001** | Phishing: Spearphishing Attachment | Delivers a weaponized Microsoft Office document to initiate the attack chain. |
| **T1204.002** | User Execution: Malicious File | Relies on the victim opening the malicious document and enabling macro execution. |
| **T1059.005** | Command and Scripting Interpreter: Visual Basic | Uses an obfuscated VBA macro to execute the initial payload. |
| **T1027** | Obfuscated Files or Information | Hides malicious logic using VBA obfuscation, encoded strings, and macro evasion techniques. |
| **T1105** | Ingress Tool Transfer | Downloads the next-stage payload from the attacker-controlled infrastructure. |
| **T1055.001** | Process Injection: Dynamic-link Library Injection | Injects the payload into a legitimate process to evade detection. |
| **T1547.001** | Registry Run Keys / Startup Folder | Establishes persistence through Windows Registry Run keys. |
| **T1071.001** | Application Layer Protocol: Web Protocols | Uses HTTP/HTTPS for Command and Control communications. |
| **T1132** | Data Encoding | Protects C2 traffic using Base64 and TLS. |
| **T1036** | Masquerading | Disguises payloads as legitimate Windows components using trusted filenames and metadata. |
| **T1497** | Virtualization/Sandbox Evasion | Detects virtualized and sandboxed environments before executing the payload. |
| **T1082** | System Information Discovery | Collects host information to profile the victim before continuing execution. |
