# Primitive Bear APT Adversary Simulation

This is a simulation of attack by (Primitive Bear) APT group targeting the State Migration Service of Ukraine the attack campaign was active from first of December to June 2021, The attack chain starts with Word document sent to the victim via email then VBS payload is used to obtain the command and control, before placing the payload or injecting it into the Word file an obfuscation of the payload is done to create an evasion of the detection then it is injected through the macro into the Word document, Then i create an SFX archive and put the payload Word file inside it to get command and control and use this SFX archive to perform a spear phishing attack then i get command and control by opening the Word file. I relied on palo alto networks to figure out the details to make this simulation: https://unit42.paloaltonetworks.com/gamaredon-primitive-bear-ukraine-update-2021/


![imageedit_2_9352621513](https://github.com/S3N4T0R-0X0/Primitive-Bear-APT/assets/121706460/a715b1e5-5d3f-48af-a749-7651cb857341)

This attack included several stages including Create an SFX file with Word File inside it. This Word File contains VBS script which is responsible for command and control and make obfuscation VBS script payload before putting it inside the word file this sent through spear phishing attack and make remote communication by utilizes DES encryption for secure data transmission between the attacker server and the target.

1. Create the Word Document: Write a Word document (.doc or .docx) containing the macro with the obfuscated VBS payload. The macro should be designed to execute the payload when the document is opened. 

2. Create a VBScript payload designed to establish a reverse connection to the Command and Control (C2) server.

3. Obfuscate the VBS Payload: Obfuscate the VBS payload to make it more difficult to detect by antivirus software or security solutions.

4. Create a Self-Extracting Archive with WinRAR: Use WinRAR to create a self-extracting (SFX) archive.
Add the Word document containing the macro and the obfuscated VBS payload to the archive.

5. Place the obfuscated VBS payload and word file inside the SFX archive to send to the target.

6. Final result make remote communication by utilizes DES encryption for secure data transmission between the attacker server and the target. 


![word-image-4](https://github.com/S3N4T0R-0X0/Primitive-Bear-APT/assets/121706460/9e4ac08a-9ae5-4b39-ad41-ed9d82cc65b6)

## The first stage (delivery technique)

I began by drafting the phishing email in a Word document for the upcoming attack. Subsequently, prior to crafting the payload, which will consist of a VBS Script injected into macros, I will encapsulate them within an SFX file. The assault targeted the Ukrainian Immigration Department, with the phishing correspondence purporting to offer financial assistance totaling 2 billion dollars.

![Screenshot from 2024-05-25 16-59-59](https://github.com/S3N4T0R-0X0/Primitive-Bear-APT/assets/121706460/44745418-6d38-4bcc-bc42-368227fe63c0)

This word file will be used to place the VBS script payload into it after obfuscation here will help make detection more difficult when placing this VBS script inside the macro in word file.

## The Second stage (VBScript payload)

First i will create a VBS payload which is a simple VBS script designed to establish a reverse connection to the C2 server then open a Word file enable macros and insert the payload into the macro finally i will save the document.

![Screenshot from 2024-05-25 18-54-11](https://github.com/S3N4T0R-0X0/Primitive-Bear-APT/assets/121706460/c6389cb6-2d22-44ec-9bac-bb3db6d153d6)


## The third stage (Obfuscation VBS payload)
But before I put the VBS payload in the macro i will make an obfuscate to the scripts to make it difficult to detect and i used online VBScript obfuscator to make obfuscate: https://isvbscriptdead.com/vbs-obfuscator/


![Screenshot from 2024-05-25 18-43-46](https://github.com/S3N4T0R-0X0/Primitive-Bear-APT/assets/121706460/43336935-c058-4f96-9847-478951c6eccc)


## The fourth stage (implanting technique)

Now i will place the obfuscated VBS payload in the microsoft Word File by opening the View menu clicking on Micros, and creating a new macro file.

![Screenshot from 2024-05-25 18-16-38](https://github.com/S3N4T0R-0X0/Primitive-Bear-APT/assets/121706460/34e0bade-fc71-4646-ba23-cad761920f96)

Save the Word file with the obfuscated VBScript payload embedded in the macro, thus i will be able to execute for the payload file when opening word file.

![Screenshot from 2024-05-25 20-04-19](https://github.com/S3N4T0R-0X0/Primitive-Bear-APT/assets/121706460/5dca0cd3-2ef8-483b-bd38-c1b902c0b5e6)

## The fifth stage (make SFX archive)

Now i will create SFX Archive using WinRAR and take the SFX file that contains the Word Document inside it with obfuscated VBS payload via the macro and send it in a  spear phishing.

1.Open WinRAR and select the files to be included in the archive.

2.Go to the "Add" menu and choose "Add to archive..."

3.In the "Archive name and parameters" window, select "SFX" as the archive format.

4.Configure the SFX options as desired, including the extraction path and execution parameters.



![Screenshot from 2024-05-26 09-23-43](https://github.com/S3N4T0R-0X0/Primitive-Bear-APT/assets/121706460/dafe9156-4b6f-4712-97de-cd2d4734439b)

## Final result (payload connect to C2-server)

This Perl C2 server script enable to make remote communication by utilizes DES encryption for secure data transmission between the attacker server and the target.

get_attacker_info and get_port: Prompts for the IP address and port number.

get_des_key: Prompts for a DES key of 8 bytes.

encrypt_data: Encrypts command results using DES with padding.

main: Sets up a TCP server, accepts connections, executes commands, encrypts results, and sends them to the client.

https://github.com/S3N4T0R-0X0/Primitive-Bear-APT/assets/121706460/e226ac70-42de-4f84-9e15-7f8ac2b47836



## MITRE ATT&CK Techniques

| Technique ID | Technique | Implementation |
|---------------|-----------|----------------|
| **T1566.001** | Phishing: Spearphishing Attachment | Delivers a malicious Microsoft Office document through a spearphishing email to initiate the attack chain. |
| **T1204.002** | User Execution: Malicious File | Requires the victim to open the malicious Office document to trigger the embedded payload. |
| **T1059.001** | Command and Scripting Interpreter: PowerShell | Uses PowerShell to execute the next-stage payload and perform remote command execution. |
| **T1105** | Ingress Tool Transfer | Downloads the second-stage payload from attacker-controlled infrastructure. |
| **T1547.001** | Registry Run Keys / Startup Folder | Establishes persistence by creating a Windows Registry Run key. |
| **T1027** | Obfuscated Files or Information | Obfuscates the payload and execution logic to evade static analysis and detection. |
| **T1082** | System Information Discovery | Collects basic host information before contacting the command-and-control server. |
| **T1071.001** | Application Layer Protocol: Web Protocols | Uses HTTP/HTTPS for command-and-control communications. |
| **T1102.001** | Web Service: Dead Drop Resolver | Retrieves commands from a trusted web service to blend with legitimate network traffic. |
| **T1041** | Exfiltration Over C2 Channel | Sends collected data back to the operator through the established C2 channel. |
| **T1497** | Virtualization/Sandbox Evasion | Checks for virtualized or sandbox environments before executing malicious actions. |
