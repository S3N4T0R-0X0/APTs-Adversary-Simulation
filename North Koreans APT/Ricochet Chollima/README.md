# Ricochet Chollima APT Adversary Simulation

This is a simulation of an attack by the (Ricochet Chollima) APT group, targeting several activists focused on North Korea. The attack campaign began in March 2025. The attack chain started with spear-phishing. The email contained a Dropbox link leading to a compressed archive that included a malicious shortcut (LNK) file. When extracted and executed, the LNK file activated additional malware containing the keyword "toy." The content was disguised as an academic forum invitation from a South Korean national security think tank to attract attention.

This simulation is based on research from Genians: https://www.genians.co.kr/en/blog/threat_intelligence/toybox-story

<img width="640" height="360" alt="imageedit_2_8051213650" src="https://github.com/user-attachments/assets/894f133b-1b99-41a4-bc80-19a8ce8a2260" />

Based on the characteristics of the threat, Genians Security Center (GSC) named the campaign “Operation: ToyBox Story”

The attacker impersonated a North Korea-focused expert based in South Korea, and the email used the subject line “러시아 전장에 투입된 인민군 장병들에게.hwp” (To North Korean People’s Army Soldiers Deployed to the Russian Battlefield.hwp) with the attachment carrying the same file name, the attachment mimicked a Hangul (HWP) document by displaying the HWP icon image used by Naver Mail, and the attacker leveraged this icon to make the attachment appear as a legitimate file link, however the actual download link redirected to Dropbox, which led to a ZIP archive named “러시아 전장에 투입된 인민군 장병들에게.zip” (To North Korean People’s Army Soldiers Deployed to the Russian Battlefield.zip).



1. Delivery Technique: Create document file  masquerading as information on North Korean troops deployed to Russia.



2. Malicious shortcut: make single shortcut (LNK) file. This LNK file executes malicious code and shares the same name as the ZIP     archive, with only the file extension being different.


3. PowerShell Commands: The shortcut (LNK) file is configured to run via PowerShell commands embedded arguments.


4. Toy.Bat: When the PowerShell command in “toy03.bat” file is executed, it loads “toy02.dat” file created in temporary folder,       functioning as a loader.


5. Dropbox C2: Get Command and Control through payload uses the Dropbox API to upload data including command output to Dropbox.

<img width="702" height="354" alt="imageedit_3_2570117683" src="https://github.com/user-attachments/assets/cf10354c-b377-4baf-b217-76f01b353f15" />

## The first stage (delivery technique)

The attacker impersonated a North Korea-focused expert based in South Korea. The spear-phishing email employed the subject line “러시아 전장에 투입된 인민군 장병들에게.hwp” (To North Korean Soldiers Deployed to the Russian Battlefield.hwp), with an attachment carrying the identical file name. The attachment was crafted to mimic a Hangul (HWP) document by leveraging the HWP icon image commonly associated with Naver Mail, thereby increasing its credibility. The threat actor intentionally used this icon to make the file appear as a legitimate document; however, the embedded link redirected the victim to a Dropbox-hosted payload instead of delivering a benign file. The Dropbox URL ultimately provided a compressed archive containing additional malicious components.

<img width="620" height="449" alt="imageedit_1_7438116774" src="https://github.com/user-attachments/assets/39b0c384-16af-4405-9281-6c73642ba74b" />


## The second stage (Malicious shortcut)

The archive named “관련 포스터.zip” (Related Poster.zip) contained a benign JPG image alongside a malicious LNK shortcut. Upon execution of the LNK file, a hidden PowerShell command embedded within the shortcut was triggered, initiating the malicious activity. 

<img width="1022" height="342" alt="Screenshot From 2025-08-25 17-44-29" src="https://github.com/user-attachments/assets/c6c2d433-a7ac-4a88-b791-7ed684717224" />


Now I need to create a JPG image that looks like a normal picture, but in the background when the image is opened the PowerShell command starts running silently. At this stage I used WinRAR to bind the JPG with a command line execution via CMD so that when the image is opened it triggers the hidden activity, and I also used an icon format to make the file appear legitimate.

![photo_2025-08-25_18-06-34](https://github.com/user-attachments/assets/3c8ffecd-29d5-4bea-8d2f-b90655c818e9)



After using WinRAR to compress the file, I will create a shortcut to this file and place it in another folder together with the actual images, then convert the folder into a ZIP file using standard ZIP compression.

<img width="971" height="264" alt="Screenshot From 2025-08-25 18-23-37" src="https://github.com/user-attachments/assets/3f75e16f-5010-450c-84f1-99b945263922" />



## The third stage (execution technique)

Because I put the command line in the setup (Run after extraction) menu in the Advanced SFX options of the WinRAR program, now when the victim opens the ZIP file to view the images, they also see an HWP document containing a letter addressed to North Korean soldiers deployed to Russia.


https://github.com/user-attachments/assets/45420912-5110-49eb-b64a-531410459f94

## The fourth stage (Toy.bat - shellcode)

When the PowerShell command in “toy03.bat” executes, it loads the “toy02.dat” file created in the temporary folder to function as a loader; the embedded PowerShell within “toy02.dat” then runs and loads “toy01.dat” from the same folder, during which XOR transformed data is decoded and mapped into memory and a new thread is spawned; as a result, the shellcode is placed in memory and the region is made executable.

<img width="1021" height="484" alt="Screenshot From 2025-10-09 12-00-21" src="https://github.com/user-attachments/assets/dbf95c8c-b85d-4d40-bc8d-43d2cb796e14" />

After which another thread is created to run the memory-resident code constituting a fileless technique for dynamic code execution and runtime malware injection.

As a result, the shellcode is loaded into memory and the memory area becomes executable.

<img width="1321" height="563" alt="Screenshot From 2025-10-09 12-06-39" src="https://github.com/user-attachments/assets/1be791ba-35dd-41b8-931d-755a8d36f8e8" />

## The fifth stage (Data Exfiltration) over Dropbox API C2 Channe

The attackers used the Dropbox C2 (Command and Control) API as a means to establish a communication channel between their payload and the attacker's server. By using Dropbox as a C2 server, attackers can hide their malicious activities among the legitimate traffic to Dropbox, making it harder for security teams to detect the threat.

<img width="814" height="146" alt="Screenshot From 2025-10-10 16-46-21" src="https://github.com/user-attachments/assets/8f5e2984-5319-469f-bfc2-556273bd5c9c" />

First, I need to create a Dropbox account and activate its permissions, as shown in the following figure.

<img width="1185" height="543" alt="316279637-518a643a-f8bc-455c-acdd-a6ed6fe8735a" src="https://github.com/user-attachments/assets/55b283b5-a36e-4e29-81c4-875c19f0b3d7" />

After that, I will go to the settings menu to generate the access token for the Dropbox account, and this is what we will use in Dropbox C2.

![316279662-00e41c7e-b2ac-4805-b1a9-77d00671ebf8](https://github.com/user-attachments/assets/5f26bbbe-9fe3-491b-b540-7d736d41feb0)

