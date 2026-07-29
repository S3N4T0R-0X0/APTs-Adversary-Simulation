# Gossamer Bear APT Adversary Simulation

This is a simulation of attack by (Gossamer Bear) APT group targeting Institutions logistics support and defense to Ukraine the attack campaign was active from April 2023,
The attack chain starts with send message with either an attached PDF file or a link to a PDF file hosted on a cloud storage platform. The PDF file will be unreadable, with a prominent button purporting to enable reading the content, Pressing the button in a PDF lure causes the default browser to open a link embedded in the PDF file code this is the beginning of the redirection chain. Targets will likely see a web page titled “Docs” in the initial page opened and may be presented with a CAPTCHA to solve before continuing the redirection. The browsing session will end showing a sign-in screen to the account where the spear-phishing email was received, with the targeted email already appearing in the username field. I relied on microsoft tofigure out the details to make this simulation: https://www.microsoft.com/en-us/security/blog/2023/12/07/star-blizzard-increases-sophistication-and-evasion-in-ongoing-attacks/


![imageedit_2_4168611963](https://github.com/S3N4T0R-0X0/Gossamer-Bear-APT/assets/121706460/0580f5fb-b020-4ca9-be84-af4c313a24f6)

This attack included several stages including creating a PDF file and placing a hyperlink inside it. The PDF file will be unreadable, with a prominent button intended to enable reading the content, Pressing the button in the PDF file causes the default browser to open a link to a fake page that steals the target's Credential, From the same PDF I also made it possible for me to get Command and Control.

1. PDF file: created PDF file includes a Hyperlink that leads to a fake page that steals Credential.

2. HTML Smuggling: it was used to open the URL of the credentials phishing page and also to install the payload.

3. Now when you click the prominent button in the PDF file it launches the html smuggling file on the apache server which contains payload in base64 encod and the phishing link.

4. Data exfiltration: over GoogleDrive API C2 Channe, This integrates GoogleDrive API functionality to facilitate communication between the compromised system and the attacker-controlled server thereby potentially hiding the traffic within legitimate GoogleDrive communication.

5. Make simple reverse shell payload to creates a TCP connection to a command and control (C2) server and listens for commands to execute on the target machine.

6. The final step in this process involves the execution of the final payload, After it was downloaded through an obfuscated HTML file with base64 encoding and a phishing link was opened.

![Figure-7 -Examples-of-Star-Blizzard-PDF-lures-when-opened-1536x509](https://github.com/S3N4T0R-0X0/Gossamer-Bear-APT/assets/121706460/e6161bf6-16e5-4865-a4b5-bba916150e6f)


## The first stage (delivery technique)

First the attackers created PDF file includes a Hyperlink that leads to a fake page that steals Credential, The advantage of the hyperlink is that it does not appear in texts, and this is exactly what the attackers wanted to exploit.

![Screenshot from 2024-05-29 16-19-48](https://github.com/S3N4T0R-0X0/Gossamer-Bear-APT/assets/121706460/3b29fad1-16ef-4d46-862e-7bd6b825db3e)


HTML Smuggling it was used to open the URL of the credentials phishing page and also to create an install for payload to get Command and Control,
After that i will place the HTML file in the apache server, take the localhost  and place it as a hyperlink in the prominent button in the PDF file.


![Screenshot from 2024-05-29 18-54-38](https://github.com/S3N4T0R-0X0/Gossamer-Bear-APT/assets/121706460/6cbb6df5-7444-4546-9eb8-282570d9ec3c)


## The second stage (implanting technique)

Now i will place the phishing link inside the HTML file in addition to the payload through base64 inside the HTML file, In this simulation i used the PyPhisher tool.

PyPhisher: https://github.com/KasRoudra2/PyPhisher.git

`base64 payload.exe`


![Screenshot from 2024-05-29 19-28-02](https://github.com/S3N4T0R-0X0/Gossamer-Bear-APT/assets/121706460/5a1aa546-0990-4521-866c-1d9ac8862309)


After that i will obfuscate the html file after putting the phishing link and the payload inside it before putting it in the apache server

I used wmtips to make obfuscation for the html file : https://www.wmtips.com/tools/html-obfuscator/#google_vignette

![Screenshot from 2024-05-29 19-41-51](https://github.com/S3N4T0R-0X0/Gossamer-Bear-APT/assets/121706460/c835211b-d03c-4c14-a261-01af7612f12c)


## The third stage (execution technique)
Now when i click the prominent button in the PDF file it launches the html smuggling file on the apache server which contains payload in base64 encod and the phishing link.

![Screenshot from 2024-06-04 17-00-45](https://github.com/S3N4T0R-0X0/Gossamer-Bear-APT/assets/121706460/ebeb9321-e025-4921-920f-590d24ddce98)

## The fourth stage (Data Exfiltration) over GoogleDrive API C2 Channe

In the actual attack, the attackers did not use an actual c2 server or payload and limited themselves to spear phishing, but here I wanted to exploit the presence of a larger HTML file to download the payload and open malicious url.

First i need to create a google Drive account, as shown in the following figure

1. Log into the Google Cloud Platform
2. Create a project in Google Cloud Platform dashboard
3. Enable Google Drive API
4. Create a Google Drive API key

![google-data-api-copy-key-600x386](https://github.com/S3N4T0R-0X0/Gossamer-Bear-APT/assets/121706460/b90e328c-5184-4072-adcb-6a6d7fb2debd)

I used the GoogleDrive C2 (Command and Control) API as a means to establish a communication channel between the payload and the attacker's server, By using GoogleDrive as a C2 server, i can hide the malicious activities among the legitimate traffic to GoogleDrive, making it harder for security teams to detect the threat.


![photo_2024-06-05_08-57-19](https://github.com/S3N4T0R-0X0/Gossamer-Bear-APT/assets/121706460/17205a4c-4150-4b1f-85de-5463d333952b)

## The fifth stage (payload with reverse shell)

This payload is a simple reverse shell written in Rust it creates a TCP connection to a command and control (C2) server and listens for commands to execute on the infected machine, the payload first sets up the IP address and port number of the C2 server. 

When a command is received, it is executed using the cmd command in Windows. The output of the command is captured and sent back to the C2 server, the loop continues until the connection is closed by the C2 server or an error occurs while receiving data from the server.

![Screenshot from 2024-06-05 17-44-02](https://github.com/S3N4T0R-0X0/Gossamer-Bear-APT/assets/121706460/76d00609-de5b-41b9-b3b8-421b4f48d6c2)


## Final result: payload connect to GoogleDrive C2 server

The final step in this process involves the execution of the final payload, After it was downloaded through an obfuscated HTML file with base64 encoding and a phishing link was opened.





https://github.com/S3N4T0R-0X0/Gossamer-Bear-APT/assets/121706460/a4c96a49-7ab6-4665-ad4d-2b80faf646ec




## MITRE ATT&CK Techniques

| Technique ID | Technique | Implementation |
|---------------|-----------|----------------|
| **T1566.001** | Phishing: Spearphishing Attachment | Delivers a weaponized Microsoft Office document through a spearphishing email. |
| **T1204.002** | User Execution: Malicious File | Requires the victim to open the malicious document to initiate execution. |
| **T1059.001** | Command and Scripting Interpreter: PowerShell | Uses PowerShell to execute the payload and attacker-issued commands. |
| **T1105** | Ingress Tool Transfer | Downloads additional payloads from attacker-controlled infrastructure. |
| **T1547.001** | Registry Run Keys / Startup Folder | Establishes persistence through Windows Registry Run keys. |
| **T1102.001** | Web Service: Dead Drop Resolver | Uses the Google Drive API as a trusted web service for Command and Control. |
| **T1071.001** | Application Layer Protocol: Web Protocols | Communicates with the C2 server over HTTPS using the Google Drive API. |
| **T1567.002** | Exfiltration to Cloud Storage | Exfiltrates collected files through Google Drive cloud storage. |
| **T1027** | Obfuscated Files or Information | Obfuscates payload components to evade static analysis and detection. |
| **T1497** | Virtualization/Sandbox Evasion | Detects virtualized and sandboxed environments before executing the payload. |
| **T1082** | System Information Discovery | Collects host information before establishing C2 communications. |


