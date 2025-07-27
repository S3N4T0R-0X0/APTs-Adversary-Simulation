# Stardust Chollima APT Adversary Simulation

This is a simulation of attack by (Stardust Chollima) APT group targeting Chilean interbank network, The attack
campaign was active in December 2018, have used PowerRatankba, a PowerShell-based malware variant that
closely resembles the original Ratankba implant. The Redbanc corporate network was infected with a version of
the PowerRatankba that was not detected by anti-malware. The way attackers delivered the malware, according to
Flashpoint a trusted Redbanc IT professional clicked to apply to a job opening found on social media (linkedin).
I relied on Security Affairs to figure out the details to make this: https://securityaffairs.com/79929/breaking-news/chilean-research-redbank-lazarus.html

<img width="640" height="360" alt="imageedit_2_7042384654" src="https://github.com/user-attachments/assets/d984834c-babb-4eeb-8f46-49aa62fa7817" />


Stardust Chollima Operations performed: https://apt.etda.or.th/cgi-bin/showcard.cgi?g=Subgroup%3A%20Bluenoroff%2C%20APT%2038%2C%20Stardust%20Chollima&n=1


The dropper used to deliver the malware is related to the PowerRatankba, a Microsoft Visual C#/ Basic .NET
compiled executable associated with Stardust Chollima APT. The dropper was used to download a PowerRatankba
PowerShell reconnaissance tool, the dropper displays a fake job application form while downloads and executes
PowerRatankba in the background by useing (Base64).

Zdnet resources: https://www.zdnet.com/article/north-korean-hackers-infiltrate-chiles-atm-network-after-skype-job-interview/

The PowerRatankba sample used in the Chilean interbank attack, differently from other variants, communicates to
the C&C server on HTTPS, This latter code is registered as a service through the “sc create” command as,
the malware gain persistence by setting an autostart.


BushidoToken Threat Intel: https://blog.bushidotoken.net/2021/08/the-lazarus-heist-where-are-they-now.html


<img width="640" height="486" alt="NK_PIRsV2" src="https://github.com/user-attachments/assets/719f42c1-320f-44b8-ab40-376f4a886fae" />

1. Social engineering technique: The attackers delivered the malware, according toFlashpoint a trusted Redbanc IT
professional clicked to apply to a job opening found on social media.

2. Fake job application form: The dropper displays a fake job application form while downloads and executes
PowerRatankba in the background by useing (Base64).

3. PowerRatankba.ps1: The main backdoor creates a connection between the targeted device and gives the attacker full
control via C2 server and latter code is registered as a service through the “sc create” command as,“ the malware gain
persistence by setting an autostart .

4. C&C server on HTTPS: When a command is received, it is executed using the PowerShell command in Windows.
The output of the command is captured and sent back to the C2 server.



<img width="640" height="484" alt="SWIFTphish" src="https://github.com/user-attachments/assets/c06b2f3e-9112-46b7-bccf-9123ae58eb1b" />



## The first stage (social engineering technique)

The attackers delivered the malware, according to Flashpoint a trusted Redbanc IT professional clicked to apply to a job
opening found on social media.The person that published the job opening then contacted the employee via linkedin
Skype, etc for an interview and tricked him into installing the malicious code.

<img width="638" height="640" alt="ss" src="https://github.com/user-attachments/assets/70dd60a3-934a-4c28-8f3d-2c4c9b9b994c" />

The group addressed several employees of the company through LinkedIn's messaging. Passing himself as a Meta
recruiter, the attacker used a lure of job offer to attract the attention and confidence of the target

<img width="417" height="455" alt="pp" src="https://github.com/user-attachments/assets/03cde7e3-389b-4300-b3ad-f3918fc1df1d" />


## The second stage (Fake job application - Backdoor Downloader by base64)

This Stager is a graphical user interface (GUI) designed to look like a registration form for a fake company called "Global
Processing Center, LTD." However, in reality, it contains malicious code that executes a hidden PowerShell script when run.
The dropper downloads and executes PowerRatankba in the background by useing (Base64).

<img width="1366" height="768" alt="Screenshot From 2025-07-27 17-47-46" src="https://github.com/user-attachments/assets/17140d6d-76f0-43eb-8594-6b7390d68960" />

Breakdown of the Malicious Code Execution:

1. Automatic Execution: When the program starts, it automatically calls the function ExecuteBase64Script(), which is responsible for decoding
   and executing the malicious payload.

2. Base64-Encoded PowerShell Script: The program contains Base64-encoded data, which is often used to hide malicious commands from antivirus and
   security software.

3. Execution with Unrestricted Policy: The PowerShell script is executed with bypassed execution policy (-ExecutionPolicy Bypass), meaning it ignores any
   security restrictions on running scripts.

   This is a known technique used by attackers to execute unauthorized PowerShell commands without user consent.

4. Decoding and Writing to File: The Base64 string is decoded and saved as a PowerShell script file named "PowerRatankba.ps1"
   which is then used as the attack payload.

