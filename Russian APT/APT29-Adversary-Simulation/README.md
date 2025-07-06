# Cozy Bear APT29 Adversary Simulation

This is a simulation of attack by the Cozy Bear group (APT-29) targeting diplomatic missions.
The campaign began with an innocuous and legitimate event. In mid-April 2023, a diplomat within the Polish Ministry of Foreign Affairs emailed his legitimate flyer to various embassies advertising the sale of a used BMW 5-series sedan located in Kyiv. The file was titled BMW 5 for sale in Kyiv - 2023.docx.
I relied on palo alto to figure out the details to make this simulation: https://unit42.paloaltonetworks.com/cloaked-ursa-phishing/

![photo_2024-04-12_01-35-08](https://github.com/S3N4T0R-0X0/APT29-Adversary-Simulation/assets/121706460/891d43ef-b749-4c08-ab60-df2df85e620d)



1.DOCX file: created DOCX file includes a Hyperlink that leads to downloading further HTML (HTML smuggling file).

2.HTML Smuggling: The attackcers  use the of HTML smuggling to obscure the ISO file.

3.LNK files: When the LNK files (shortcut) are executed they run a legitimate EXE and open a PNG file. However, behind the scenes, encrypted shellcode is read into memory and decrypted.

4.ISO file: The ISO file contains a number of LNK files that are masquerading as images. These LNK files are used to execute the malicious payload.

5.DLL hijacking: The EXE file loads a malicious DLL via DLL hijacking, which allows the attacker to execute arbitrary code in the context of the infected process.

6.Shellcode injection: The decrypted shellcode is then injected into a running Windows process, giving the attacker the ability to execute code with the privileges of the infected process.

7.Payload execution: The shellcode decrypts and loads the final payload inside the current process.

8.Dropbox C2: This payload beacons to Dropbox and Primary/Secondary C2s based on the Microsoft Graph API.

![Screenshot from 2024-03-11 15-43-36](https://github.com/S3N4T0R-0X0/APT29-Adversary-Simulation/assets/121706460/ff510ffd-3481-4978-bedc-d30629d65307)



## The first stage (delivery technique)

First the attackers created DOCX file includes a Hyperlink that leads to downloading further HTML (HTML smuggling file)
The advantage of the hyperlink is that it does not appear in texts, and this is exactly what the attackers wanted to exploit.


![Screenshot from 2024-03-01 19-18-51](https://github.com/S3N4T0R-0X0/APT29-Adversary-Simulation/assets/121706460/bb984b9c-5367-4fb2-9efc-3be7c098ec46)


HTML Smuggling used to obscure ISO file and the ISO contains a number of LNK files masquerading as images
command line to make payload base64 to then put it in the HTML smuggling file:
`base64 payload.iso -w 0` and i added a picture of the BMW car along with the text content of the phishing message in the HTML file. 

![Screenshot from 2024-03-01 19-39-42](https://github.com/S3N4T0R-0X0/APT29-Adversary-Simulation/assets/121706460/8e76572b-5d72-4d87-9cf9-4c7bf002c801)


## The second stage (implanting technique)

We now need to create a PNG image that contains images of the BMW car, but in the background when the image is opened, the malware is running in the background,
at this stage i used the WinRAR program to make the image open with Command Line execution via CMD when opening the image and I used an image in icon format.


![20240302_194641](https://github.com/S3N4T0R-0X0/APT29-Adversary-Simulation/assets/121706460/b3b7872e-1bf9-4637-a13f-ba720c113276)

After using WinRaR for this compressed file, we will make a short cut of this file and put it in another file with the actual images then we will convert it to an ISO file through the PowerISO program.

Note: This iso file is the one to which we will make base64 for this iso file and put in the html smuggling file before make hyperlink and place it in the docx file.

![Screenshot from 2024-03-02 15-39-55](https://github.com/S3N4T0R-0X0/APT29-Adversary-Simulation/assets/121706460/40fef200-416c-4de4-8999-f154a01b22dd)



## The third stage (execution technique)

Because i put the command line in the setup (run after extraction) menu in the Advanced SFX options for the WinRaR program now when the victim open the ISO file to see the high-quality images for the BMW car according to the phishing message he had previously received he will execute the payload with opening the actual image of the BMW car.



https://github.com/S3N4T0R-0X0/APT29-Adversary-Simulation/assets/121706460/343e37ce-28e0-4a52-8a68-673bfcc68ffe



## The fourth stage (Data Exfiltration) over Dropbox API C2 Channe

The attackers used the Dropbox C2 (Command and Control) API as a means to establish a communication channel between their payload and the attacker's server. By using Dropbox as a C2 server, attackers can hide their malicious activities among the legitimate traffic to Dropbox, making it harder for security teams to detect the threat.
First, we need to create a Dropbox account and activate its permissions, as shown in the following figure.

![Screenshot from 2024-03-12 16-10-13](https://github.com/S3N4T0R-0X0/APT29-Adversary-Simulation/assets/121706460/518a643a-f8bc-455c-acdd-a6ed6fe8735a)


After that, we will go to the settings menu to generate the access token for the Dropbox account, and this is what we will use in Dropbox C2.

![photo_2024-03-12_16-22-54](https://github.com/S3N4T0R-0X0/APT29-Adversary-Simulation/assets/121706460/00e41c7e-b2ac-4805-b1a9-77d00671ebf8)


This script integrates Dropbox API functionality to facilitate communication between the compromised system and the attacker-controlled server,
thereby  hiding the traffic within legitimate Dropbox communication, and take the access token as input prompts the user to enter an AES key 
(which must be 16, 24, or 32 bytes long) and encrypts the token using AES encryption in ECB mode. It then base64 encodes the encrypted token and returns it.

![171053992557140444](https://github.com/S3N4T0R-0X0/APT29-Adversary-Simulation/assets/121706460/15fdca80-68cb-41ac-9eb5-b56ded6e552e)


I used payload written by Python only to test C2 (testing payload.py), if there were any problems with the connection (just for test connection) before writing the actual payload.

## The fifth stage (payload with DLL hijacking) and injected Shellcode

this payload uses the Dropbox API to upload data, including command output to Dropbox. By leveraging the Dropbox API and providing an access token the payload hides its traffic within the legitimate traffic of the Dropbox servic and If the malicious DLL fails to load, it prints a warning message but continues executing without it.

![Screenshot from 2024-03-23 15-17-27](https://github.com/S3N4T0R-0X0/APT29-Adversary-Simulation/assets/121706460/7144331f-5635-49f7-b024-5152cb06cc03)

1.DLL Injection: The payload utilizes DLL hijacking to load a malicious DLL into the address space of a target process.

2.Shellcode Execution: Upon successful injection, the malicious DLL executes shellcode stored within its DllMain function.

3.Memory Allocation: The VirtualAlloc function is employed to allocate memory within the target process, where the shellcode will be injected.

4.Shellcode Injection: The shellcode is copied into the allocated memory region using memcpy, effectively injecting it into the process.

5.Privilege Escalation: If the compromised process runs with elevated privileges, the injected shellcode inherits those privileges, allowing the attacker to perform privileged operations.

![Screenshot from 2024-03-23 15-16-20](https://github.com/S3N4T0R-0X0/APT29-Adversary-Simulation/assets/121706460/29858785-5fc5-446c-a1d0-d0b1bb1e58d7)


## Final result: payload connect to Dropbox C2 server

the final step in this process involves the execution of the final payload. After being decrypted and loaded into the current process,
the final payload is designed to beacon out to both Dropbox API-based  C2 server. 

https://github.com/S3N4T0R-0X0/APT29-Adversary-Simulation/assets/121706460/c5b7b826-72a1-459e-9f19-6e34bd79aeab

