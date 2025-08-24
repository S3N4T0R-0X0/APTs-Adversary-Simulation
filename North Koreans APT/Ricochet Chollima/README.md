# Ricochet Chollima APT Adversary Simulation

This is a simulation of an attack by the (Ricochet Chollima) APT group, targeting several activists focused on North Korea. The attack campaign began in March 2025. The attack chain started with spear-phishing. The email contained a Dropbox link leading to a compressed archive that included a malicious shortcut (LNK) file. When extracted and executed, the LNK file activated additional malware containing the keyword "toy." The content was disguised as an academic forum invitation from a South Korean national security think tank to attract attention.

This simulation is based on research from Genians: https://www.genians.co.kr/en/blog/threat_intelligence/toybox-story

<img width="640" height="360" alt="imageedit_2_8051213650" src="https://github.com/user-attachments/assets/894f133b-1b99-41a4-bc80-19a8ce8a2260" />

Based on the characteristics of the threat, Genians Security Center (GSC) named the campaign “Operation: ToyBox Story”

The attacker impersonated a North Korea-focused expert based in South Korea, and the email used the subject line “러시아 전장에 투입된 인민군 장병들에게.hwp” (To North Korean People’s Army Soldiers Deployed to the Russian Battlefield.hwp) with the attachment carrying the same file name, the attachment mimicked a Hangul (HWP) document by displaying the HWP icon image used by Naver Mail, and the attacker leveraged this icon to make the attachment appear as a legitimate file link, however the actual download link redirected to Dropbox, which led to a ZIP archive named “러시아 전장에 투입된 인민군 장병들에게.zip” (To North Korean People’s Army Soldiers Deployed to the Russian Battlefield.zip).


