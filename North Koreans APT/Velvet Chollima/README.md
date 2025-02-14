# Velvet Chollima APT Adversary Simulation

This is a simulation of an attack by the (Velvet Chollima) APT group targeting South Korean government officials. The attack campaign began in January 2025 and also targeted NGOs, government agencies, and media companies across North America, South America, Europe, and East Asia. The attack chain starts with a spear-phishing email containing a PDF attachment. However, when targets attempt to read the document, they are redirected to a fake device registration link instructing them to run PowerShell as an administrator and execute attacker-provided code. This simulation is based on research from Microsoft's Threat Intelligence and Bleeping Computer: https://www.bleepingcomputer.com/news/security/dprk-hackers-dupe-targets-into-typing-powershell-commands-as-admin/

![imageedit_3_8386779397](https://github.com/user-attachments/assets/91dc82bd-27cf-4edc-a35a-a3b6cc87d909)

The attackers used a new tactic known as ClickFix, a social engineering technique that has gained traction, particularly for distributing malware.

ClickFix involves deceptive error messages or prompts that trick victims into executing malicious code themselves, often via PowerShell commands, ultimately leading to malware infections.

According to Microsoft's Threat Intelligence team, the attackers masquerade as South Korean government officials, gradually building trust with their targets. Once a certain level of rapport is established, the attacker sends a spear-phishing email with a PDF attachment. However, when targets attempt to read the document, they are redirected to a fake device registration link that instructs them to run PowerShell as an administrator and execute attacker-provided code.

https://www.bleepingcomputer.com/news/security/fake-google-meet-conference-errors-push-infostealing-malware/

![dialogs](https://github.com/user-attachments/assets/9d5a1b31-5479-4a67-826c-68195fb2c3a5)



1. social engineering: Create PDF file which will be sent spear-phishing.

2. ClickFix technique: (Fake-Captcha) to make the target run PowerShell as an administrator and paste attacker-provided code.

3. reverse shell: Make simple reverse shell payload to creates a TCP connection to a command and control (C2) server and listens for commands to execute on the target machine.
