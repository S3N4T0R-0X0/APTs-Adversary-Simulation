# Energetic Bear APT Adversary Simulation
This is a simulation of attack by (Energetic Bear) APT group targeting “eWon” is a Belgian producer of SCADA and industrial network equipmen,
the attack campaign was active from January 2014,The attack chain starts with malicious XDP file containing the PDF/SWF exploit (CVE-2011-0611)
and was used in spear-phishing attack. This exploit drops the loader DLL which is stored in an encrypted form in the XDP file,
The exploit is delivered as an XDP (XML Data Package) file which is actually a PDF file packaged within an XML container.
I relied on Kaspersky tofigure out the details to make this simulation:
https://media.kasperskycontenthub.com/wp-content/uploads/sites/43/2018/03/08080817/EB-YetiJuly2014-Public.pdf 

![imageedit_6_9165265996](https://github.com/S3N4T0R-0X0/Energetic-Bear-APT-Adversary-Simulation/assets/121706460/25bb36f0-0a63-4dbe-941f-dd64ceb05e2f)

This attack included several stages including exploitation of the (CVE-2011-0611) vulnerability which allows attackers to overwrite a pointer in memory by embedding a specially crafted .swf, The XDP file contains a SWF exploit CVE-2011-0611 and two files encrypted with XOR stored in the XDP file One of the files is malicious DLL the other is a JAR file which is used to copy and run the DLL by executing the Cmd command line

1. CVE-2011-0611: this module exploits a memory corruption vulnerability in Adobe Flash Player versions 10.2.153.1 and earlier, i maked Modified version of the exploit based on Windows 10.




2. CVE-2012-1723: this exploit allows for sandbox escape and remote code execution on any target with a vulnerable JRE (Java IE 8). 



3. XDP file: this XDP file contains a malicious XML Data Package (XDP) with a SWF exploit (CVE-2011-0611), It also includes functionality to download additional files via HTML-Smuggling by apache host.


4. HTML Smuggling: the html-smuggling file is used after uploading it to the apache server to download other files, One of the files is DLL payload the other is a small JAR file.

5. JAR file: this jar file used to copy and run the DLL by executing the cmd command.



6. DLL payload: the attackers used havex trojan, havex scanned the infected system to locate any supervisory control and data acquisition SCADA.



7. Encrypted with XOR: the XDP file contains a SWF exploit and two files encrypted with XOR.


8. PHP backend C2-Server: the attckers used hacked websites as simple PHP C2 Server backend.

   
9. Final result: make remote communication by utilizes XOR encryption for secure data transmission between the attacker server and the target.

![Screenshot from 2024-05-04 17-37-00](https://github.com/S3N4T0R-0X0/EnergeticBear-APT/assets/121706460/5cd199b5-9af1-4258-b5de-ecdf4e97cca1)



## The first stage (exploit Adobe SWF Memory Corruption Vulnerability CVE-2011-0611)

This module exploits a memory corruption vulnerability (CVE-2011-0611) in Adobe Flash Player
versions 10.2.153.1 and earlier. The vulnerability allows for arbitrary code execution by
exploiting a flaw in how Adobe Flash Player handles certain crafted .swf files. By leveraging
this vulnerability, an attacker can execute arbitrary code on the victim's system.


![Screenshot from 2024-05-02 10-49-29](https://github.com/S3N4T0R-0X0/EnergeticBear-APT/assets/121706460/43751d02-3289-42ed-9971-90c34e0bbdcd)


`sudo cp EnergeticBear_exploit.rb /usr/share/metasploit-framework/modules/exploits`

`sudo updatedb`

`msf6 > search EnergeticBear_exploit`


![Screenshot from 2024-05-06 05-54-55](https://github.com/S3N4T0R-0X0/EnergeticBear-APT/assets/121706460/34bf9736-214b-49e1-89d1-b96570f6b863)


This Modified version of the exploit CVE-2011-0611 based on Windows 10 ,the original exploit from : https://github.com/rapid7/metasploit-framework/blob/master/modules/exploits/windows/browser/adobe_flashplayer_flash10o.rb

## The Second stage (CVE-2012-1723 Oracle Java Applet Field Bytecode Verifier Cache RCE)

This vulnerability in the Java Runtime Environment (JRE) component in Oracle Java SE 7 update 4 and earlier, 6 update 32 and earlier, 5 update 35 and earlier, and 1.4.2_37 and earlier allows remote attackers to affect confidentiality, integrity, and availability via unknown vectors related to Hotspot. if you need know more about CVE-2012-1723: https://www.microsoft.com/en-us/wdsi/threats/malware-encyclopedia-description?Name=Exploit:Java/CVE-2012-1723!generic&threatId=-2147302241

![Screenshot from 2024-05-04 18-10-19](https://github.com/S3N4T0R-0X0/EnergeticBear-APT/assets/121706460/63ff31a9-6a4e-4a8b-ae52-4b216a539b59)

`use exploit/multi/browser/java_verifier_field_access`

The attackers actively compromises legitimate websites for watering hole attacks. These hacked
websites in turn redirect victims to malicious JAR or HTML files hosted on other sites maintained
by the group (exploiting CVE-2013-2465, CVE-2013-1347, and CVE-2012-1723 in Java 6, Java 7,
IE 7 and IE 8), These hacked websites will be using a simple PHP C2 Server backend. 



![Screenshot from 2024-05-05 13-24-47](https://github.com/S3N4T0R-0X0/EnergeticBear-APT/assets/121706460/8502eba2-ab15-4b18-8c49-a0517276d6a4)





##  The third stage (XML Data Package XDP with a SWF exploit)

The exploit is delivered as an XDP (XML Data Package) file which is actually a PDF file packaged within an XML container. This is a known the PDF obfuscation method and serves as an additional anti-detection layer.
if you need know more about XDP file: https://filext.com/file-extension/XDP


![Screenshot from 2024-05-07 10-26-24](https://github.com/S3N4T0R-0X0/EnergeticBear-APT/assets/121706460/a05844ed-bad1-4d9a-aaf6-808249f09a86)


The XDP file contains a SWF exploit (CVE-2011-0611) and two files (encrypted with XOR) stored in the PDF file, It also includes functionality to download additional files via HTML-Smuggling by apache host.

## The fourth stage (HTML-Smuggling with DLL payload & JAR file)

The HTML smuggling file is used after uploading it to the apache server to download other files, One of the files is DLL payload the other is a small JAR file which is used to copy and run the DLL, the command line to make payload base64 to then put it in the HTML smuggling file: `base64 payload.dll -w 0` and the same command but with jar file.

![Screenshot from 2024-05-07 16-04-06](https://github.com/S3N4T0R-0X0/EnergeticBear-APT/assets/121706460/bf4b3892-3521-41f9-aa5e-740c5229e204)

     
## The fifth stage (Copy DLL by JAR file)

This jar file used to copy and run the DLL by executing the following command:
`cmd /c copy payload.dll %TEMP%\\payload.dll /y & rundll32.exe %TEMP%\\payload.dll,RunDllEntry`

It constructs a command to copy a file named payload.dll to the %TEMP% directory (typically the temporary directory) as payload.dll and then execute it using rundll32.exe and it waits for the process to finish using process.waitFor().


![Screenshot from 2024-05-04 09-44-48](https://github.com/S3N4T0R-0X0/EnergeticBear-APT/assets/121706460/4ade01c3-7539-45da-941e-1d8425b68320)



## The sixth stage DLL payload (Havex trojan)

The attackers gained access to eWon’s FTP site and replaced the legitimate file with one that is bound with the Havex dropper several times.

The main functionality of this component is to download and load additional DLL modules into the
memory. These are stored on compromised websites that act as C&C servers. In order to do that, the
malware injects itself into the EXPLORER.EXE process, sends a GET/POST request to the PHP script
on the compromised website, then reads the HTML document returned by the script, looking for a
base64 encrypted data between the two “havex” strings in the comment tag `<!--havexhavex-->`
and writes this data to a %TEMP%\<tmp>.xmd file (the filename is generated by GetTempFilename
function).


Full Disclosure of Havex Trojans: https://www.netresec.com/?page=Blog&month=2014-10&post=Full-Disclosure-of-Havex-Trojans


![Screenshot from 2024-05-05 07-59-00](https://github.com/S3N4T0R-0X0/EnergeticBear-APT/assets/121706460/9212b904-78cd-40af-a29d-61bd41970c3e)



If you need know more about Havex trojan: https://malpedia.caad.fkie.fraunhofer.de/details/win.havex_rat

Notes on havex trojan: http://pastebin.com/qCdMwtZ6


In this simulation i used a simple payload with XOR encryption to secure the connection between the C2 Server and the Target Machine, 
this payload uses Winsock for establishing a tcp connection between the target machine and the attacker machine, in an infinite loop the payload receives commands from the attacker c2 decrypts them using (XOR) encryption executes them using system and then sleeps for 10 seconds before repeating the loop.



![Screenshot from 2024-05-08 16-33-22](https://github.com/S3N4T0R-0X0/EnergeticBear-APT/assets/121706460/5ac1dcf5-e68a-43a6-985b-51dce1ea74aa)

This network forensics form (SCADA hacker) about havex trojan: https://scadahacker.com/library/Documents/Cyber_Events/NETRESEC%20-%20SCADA%20Network%20Forensics.pdf

## The seventh stage (encrypted XDP with XOR)

After making compile for the payload and jar file and make base64 for the jar file and DLL payload, i put them in the html smuggling file, then i make host for the html file, then i put this host in the XDP file next to CVE-2011-0611, then i make XOR encryption for XDP file, after this convert xdp to pdf.

![Screenshot from 2024-05-08 17-41-26](https://github.com/S3N4T0R-0X0/EnergeticBear-APT/assets/121706460/d16dad38-99a8-4ff5-820a-235437bb74a6)

i used browserling to make xor encrypt: https://www.browserling.com/tools/xor-encrypt

## The eighth stage (PHP backend C2-Server)

This PHP C2 server script enable to make remote communication by utilizes XOR encryption for secure data transmission between the attacker server and the target.


![Screenshot from 2024-05-03 13-21-06](https://github.com/S3N4T0R-0X0/EnergeticBear-APT/assets/121706460/17237410-7464-4503-a97e-cea00a20e97b)


`xor_encrypt($data, $key)` This function takes two parameters: the data to be encrypted ($data) and the encryption key ($key) it iterates over each character in the data and performs an XOR operation between the character and the corresponding character in the key (using modulo to repeat the key if it's shorter than the data), the result is concatenated to form the encrypted output which is returned.


`send_to_payload($socket, $data, $encryption_key)` This function sends encrypted data to the target system (payload) over a socket connection it first encrypts the data using the xor_encrypt function with the provided encryption key then it writes the encrypted data to the socket using socket_write.


`receive_from_payload($socket, $buffer_size, $encryption_key)` This function receives encrypted data from the target system over a socket connection it reads data from the socket with a maximum buffer size specified by $buffer_size, the received encrypted data is then decrypted using the xor_encrypt function with the provided encryption key before being returned.

if you  chose (command or URL) is encrypted using XOR encryption with a user-defined key before being sent to the target.

![Screenshot from 2024-05-08 18-28-21](https://github.com/S3N4T0R-0X0/EnergeticBear-APT/assets/121706460/d3c50bdb-5b59-4a46-b4ac-8acdeebff440)

This other simulation for the same attack by cobaltstrike: https://www.youtube.com/watch?v=XkBvo6z0Tqo

## Final result: payload connect to PHP C2-server

1.Set up a web server or any HTTP server that can serve text content.

2.Upload a text file containing the commands you want the compromised system to execute.

3.Make sure the text file is accessible via HTTP and note down the URL.

4.When prompted by the script, enter the URL you obtained in step.

NOTE: If you choose to fetch commands from a URL it will prompt you to enter the URL, If you choose to enter commands directly it will prompt you to Enter a command to execute




https://github.com/S3N4T0R-0X0/EnergeticBear-APT/assets/121706460/27186732-723b-4b6c-b233-0da479ea5b7a

## MITRE ATT&CK Techniques

| Technique ID | Technique | Implementation |
|---------------|-----------|----------------|
| **T1566.001** | Phishing: Spearphishing Attachment | Delivers a malicious XDP (XML Data Package) document through a spearphishing email. |
| **T1203** | Exploitation for Client Execution | Exploits **CVE-2011-0611 (Adobe Flash Player)** embedded within the XDP/PDF document to execute code. |
| **T1027** | Obfuscated Files or Information | Stores the malicious DLL and JAR in encrypted form within the XDP container using XOR obfuscation. |
| **T1574.001** | Hijack Execution Flow: DLL Search Order Hijacking | Executes the malicious DLL dropped by the exploit chain. |
| **T1059.003** | Command and Scripting Interpreter: Windows Command Shell | Uses `cmd.exe` to launch the extracted components and continue execution. |
| **T1059.005** | Command and Scripting Interpreter: Visual Basic | Uses the embedded exploit document to initiate the execution chain. |
| **T1105** | Ingress Tool Transfer | Retrieves additional payloads from attacker-controlled infrastructure. |
| **T1132** | Data Encoding | Encrypts C2 commands and responses using XOR before transmission. |
| **T1071.001** | Application Layer Protocol: Web Protocols | Communicates with the PHP C2 server over HTTP. |
| **T1497** | Virtualization/Sandbox Evasion | Uses anti-analysis techniques before executing the payload. |
| **T1082** | System Information Discovery | Collects host information prior to establishing command-and-control. |
