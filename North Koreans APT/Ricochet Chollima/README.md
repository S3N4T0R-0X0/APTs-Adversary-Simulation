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

The decoy HWP document contains a letter addressed to North Korean soldiers deployed to Russia.

<img width="586" height="714" alt="imageedit_2_7840143710" src="https://github.com/user-attachments/assets/928315fd-596a-4906-9024-4a2b1017e974" />

