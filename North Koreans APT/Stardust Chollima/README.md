# Stardust Chollima APT Adversary Simulation

This is a simulation of attack by (Stardust Chollima) APT group targeting Chilean interbank network, The attack
campaign was active in December 2018, have used PowerRatankba, a PowerShell-based malware variant that
closely resembles the original Ratankba implant. The Redbanc corporate network was infected with a version of
the PowerRatankba that was not detected by anti-malware. The way attackers delivered the malware, according to
Flashpoint a trusted Redbanc IT professional clicked to apply to a job opening found on social media (linkedin).
I relied on Security Affairs to figure out the details to make this: https://securityaffairs.com/79929/breaking-news/chilean-research-redbank-lazarus.html

<img width="640" height="360" alt="imageedit_2_7042384654" src="https://github.com/user-attachments/assets/d984834c-babb-4eeb-8f46-49aa62fa7817" />

The dropper used to deliver the malware is related to the PowerRatankba, a Microsoft Visual C#/ Basic .NET
compiled executable associated with Stardust Chollima APT. The dropper was used to download a PowerRatankba
PowerShell reconnaissance tool, the dropper displays a fake job application form while downloads and executes
PowerRatankba in the background by useing (Base64).

The PowerRatankba sample used in the Chilean interbank attack, differently from other variants, communicates to
the C&C server on HTTPS, This latter code is registered as a service through the “sc create” command as,
the malware gain persistence by setting an autostart.

BushidoToken Threat Intel: https://blog.bushidotoken.net/2021/08/the-lazarus-heist-where-are-they-now.html

<img width="640" height="484" alt="SWIFTphish" src="https://github.com/user-attachments/assets/c06b2f3e-9112-46b7-bccf-9123ae58eb1b" />


1. Social engineering technique: The attackers delivered the malware, according toFlashpoint a trusted Redbanc IT
professional clicked to apply to a job opening found on social media.

2. Fake job application form: The dropper displays a fake job application form while downloads and executes
PowerRatankba in the background by useing (Base64).

3. PowerRatankba.ps1: The main backdoor creates a connection between the targeted device and gives the attacker full
control via C2 server and latter code is registered as a service through the “sc create” command as,“ the malware gain
persistence by setting an autostart .

4. C&C server on HTTPS: When a command is received, it is executed using the PowerShell command in Windows.
The output of the command is captured and sent back to the C2 server.

## The first stage (social engineering technique)


