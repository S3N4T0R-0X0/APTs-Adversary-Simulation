# Fancy Bear APT28 Adversary Simulation
This is a simulation of attack by Fancy Bear group (APT28) targeting high-ranking government officials Western Asia and Eastern Europe
the attack campaign was active from October to November 2021, The attack chain starts with the execution of an Excel downloader sent 
to the victim via email which exploits an MSHTML remote code execution vulnerability (CVE-2021-40444) to execute a malicious executable
in memory, I relied on trellix tofigure out the details to make this simulation: https://www.trellix.com/blogs/research/prime-ministers-office-compromised/


![photo_2024-04-06_23-42-01](https://github.com/S3N4T0R-0X0/APT28-Adversary-Simulation/assets/121706460/bd9e3d64-a453-4aaf-9653-255a0cf4fe68)

This attack included several stages including exploitation of the CVE-2021-40444 vulnerability through which remote access execution can be accessed through word file this is done by injecting the DLL into Word file through this exploit, Also use OneDrive c2 Server to get command and control and this is to data exfiltration with hide malicious activities among the legitimate traffic to OneDrive.


1.Create dll downloads files through base64, This is to download two files the first is (dfsvc.dll) the second is (Stager.dll).

2.Exploiting the zero-day vulnerability to inject the DLL file into Word File and create an execution for DLL by opening Word File.

3.Word File is running and the actual payload is downloaded through DLLDownloader.dll and we have two files Stager.dll and dfsvc.dll.


4.The Stager decrypts the actual payload and runs it which in turn is responsible for command and control.

5.Data exfiltration over OneDrive API C2 Channe, This integrates OneDrive API functionality to facilitate communication between the compromised system and the attacker-controlled server thereby potentially hiding the traffic within legitimate OneDrive communication.

6.Get Command and Control through payload uses the OneDrive API to upload data including command output to OneDrive, the payload calculates the CRC32 checksum of the MachineGuid and includes it in the communication with the server for identification purposes.


![Screenshot from 2024-04-08 01-28-29](https://github.com/S3N4T0R-0X0/APT28-Adversary-Simulation/assets/121706460/d6d418db-2d9a-4e4c-94fb-74596207d95a)

## The first stage (delivery technique)

First the attackers created DLL executable (DLLDownloader.dll) this DLL it can download two payloads by command line to make payload base64 
`base64 dfsvc.dll -w 0` and `base64 Stager.dll  -w 0` the first is (dfsvc.dll) the second is (Stager.dll), This DLL will be used in the next stage by injecting it into a Word file via the Zero-day vulnerability.


![Screenshot from 2024-04-16 21-38-27](https://github.com/S3N4T0R-0X0/APT28-Adversary-Simulation/assets/121706460/d4fdc9a2-5268-42cf-98b0-4e8aff660ac6)

## The Second stage (implanting technique)

second the attackers exploited the Zero-day vulnerability (CVE-2021-40444) https://github.com/lockedbyte/CVE-2021-40444/
this vulnerability works by injecting a DLL file into Microsoft Word When the file is opened it executes the DLL payload, which is responsible for downloading two other payload (dfsvc.dll) and (Stager.dll).

![Screenshot from 2024-04-13 01-09-30](https://github.com/S3N4T0R-0X0/APT28-Adversary-Simulation/assets/121706460/e03bfa09-ed13-4ddc-bf41-64d97187099b)

When a victim opens the malicious Office document using Microsoft Office, the application parses the document's content, including the embedded objects. The flaw in the MSHTML component is triggered during this parsing process, allowing the attacker's malicious code to be executed within the context of the Office application.

## The third stage (execution technique)

Now i have a Word file when i open it performs an execution for the DLL Downloader and thus downloads the two files (dfsvc.dll) and (Stager.dll) this is through the  vulnerability CVE-2021-40444.

![Screenshot from 2024-04-16 17-21-17](https://github.com/S3N4T0R-0X0/APT28-Adversary-Simulation/assets/121706460/b496b5a9-28e9-49f1-a5c7-8324913cbf2f)

After that the stager decrypts the payload using the Decrypt-Payload function (you need to implement the decryption algorithm) and then executes the payload using the Execute-Payload function, In this simulation i made the build perform an execution directly without the need for the stager script, and it can be modified to suit the stager making an execution for the actual payload.


![Screenshot from 2024-04-16 17-59-42](https://github.com/S3N4T0R-0X0/APT28-Adversary-Simulation/assets/121706460/30729f3f-d294-4ccc-82c3-7f1821f792df)

## The fourth stage (Data Exfiltration) over OneDrive API C2 Channe

The attackers used the OneDrive C2 (Command and Control) API as a means to establish a communication channel between their payload and the attacker's server, 
By using OneDrive as a C2 server, attackers can hide their malicious activities among the legitimate traffic to OneDrive, making it harder for security teams to detect the threat. First, we need to create a Microsoft Azure account and activate its permissions, as shown in the following figure.

We will use the Application (client) ID for the inputs needed by the C2 server

![photo_2024-04-14_16-24-06](https://github.com/S3N4T0R-0X0/APT28-Adversary-Simulation/assets/121706460/6e73395a-2221-411b-ab4a-e6c23f2b2897)

After that, we will go to the Certificates & secrets menu to generate the Secret ID for the Microsoft Azure account, and this is what we will use in OneDrive C2.

![photo_2024-04-14_16-24-14](https://github.com/S3N4T0R-0X0/APT28-Adversary-Simulation/assets/121706460/fec5b59d-57ed-47f4-b640-d06782d8c16b)

To make simulation of this attack at the present time i did not use the PowerShell Empire to avoid detection and i make customization of the OneDrive C2 server,
This script integrates OneDrive API functionality to facilitate communication between the compromised system and the attacker-controlled server, thereby potentially hiding the traffic within legitimate OneDrive communication and i used AES Encryption to secure the connection just like the PowerShell Empire  server that the attackers used in the actual attack, The customization OneDrive C2 Server inspired by PowerShell Empire.

![photo_2024-04-14_02-55-02](https://github.com/S3N4T0R-0X0/APT28-Adversary-Simulation/assets/121706460/0e4a4178-053b-493d-90b2-f0988d80f5da)

## The fifth stage (payload with OneDrive API requests)

This payload establishes covert communication via socket to a remote server, disguising traffic within OneDrive API requests. It identifies machines using CRC32 checksums of their MachineGuids. Commands are executed locally, with outputs sent back to the server or uploaded to OneDrive. Its dynamic configuration enables flexible and stealthy remote control and data exfiltration.

![Screenshot from 2024-04-14 16-59-47](https://github.com/S3N4T0R-0X0/APT28-Adversary-Simulation/assets/121706460/b784cfdd-e83e-41b3-857b-23e56396312d)


1.Covert communication: The payload initiates a socket connection to a specified IP address and port.
  
2.Identification mechanism: It retrieves the MachineGuid from the Windows registry and calculates its CRC32 checksum.

![171351508026027259](https://github.com/S3N4T0R-0X0/APT28-Adversary-Simulation/assets/121706460/ba5979bc-eb9b-4e98-b74a-002e6846ff36)


3.Command execution: The payload enters a loop to receive commands from the remote server or OneDrive.

4.Data exfiltration: After execution it captures output and sends it back to the server or uploads it to OneDrive.

5.Stealthy communication: Utilizing OneDrive API it blends network traffic with legitimate OneDrive traffic.

6.Dynamic configuration: Behavior is configured by specifying IP address, port and optionally an access token for OneDrive.


![Screenshot from 2024-04-14 22-43-29](https://github.com/S3N4T0R-0X0/APT28-Adversary-Simulation/assets/121706460/b2eda097-d7f7-48ab-823d-f720badf69f1)

## Final result: payload connect to OneDrive C2 server

the final step in this process involves the execution of the final payload. After being decrypted and loaded into the current process, the final payload is designed to beacon out to both OneDrive API-based C2 server.




https://github.com/S3N4T0R-0X0/APT28-Adversary-Simulation/assets/121706460/becef683-c49b-40d5-9047-d8e8c6303eaa

## MITRE ATT&CK Techniques

| Technique ID | Technique | Implementation |
|---------------|-----------|----------------|
| **T1566.001** | Phishing: Spearphishing Attachment | Delivers a weaponized Microsoft Excel document via spearphishing email. |
| **T1203** | Exploitation for Client Execution | Exploits **CVE-2021-40444 (MSHTML)** to execute malicious code. |
| **T1218.011** | System Binary Proxy Execution: Rundll32 | Executes the malicious DLL through `rundll32.exe`. |
| **T1059.001** | Command and Scripting Interpreter: PowerShell | Uses PowerShell to execute the loader and retrieve the next-stage payload. |
| **T1574.001** | Hijack Execution Flow: DLL Search Order Hijacking | Loads a malicious DLL during the exploitation chain. |
| **T1027** | Obfuscated Files or Information | Obfuscates the payload to evade static analysis and detection. |
| **T1105** | Ingress Tool Transfer | Downloads the final payload from attacker-controlled infrastructure. |
| **T1102.001** | Web Service: Dead Drop Resolver | Uses the OneDrive API as a trusted web service for payload retrieval and C2. |
| **T1071.001** | Application Layer Protocol: Web Protocols | Communicates with the C2 server over HTTPS using the OneDrive API. |
| **T1567.002** | Exfiltration to Cloud Storage | Exfiltrates collected data through OneDrive cloud storage. |
| **T1497** | Virtualization/Sandbox Evasion | Detects analysis environments before executing the payload. |
| **T1082** | System Information Discovery | Collects host information before establishing C2 communications. |


