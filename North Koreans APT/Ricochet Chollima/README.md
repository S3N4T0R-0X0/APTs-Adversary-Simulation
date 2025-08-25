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


5. shellcode injection: As a result the shellcode is loaded into memory and the memory area becomes executable.


6. Dropbox C2: Get Command and Control through payload uses the Dropbox API to upload data including command output to Dropbox.

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

https://github.com/user-attachments/assets/ccf40af9-5412-43cb-9691-b6e87054b4e4








