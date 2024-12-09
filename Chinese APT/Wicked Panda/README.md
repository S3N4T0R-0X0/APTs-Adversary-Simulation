# Wicked Panda APT Adversary Simulation

This is a simulation of attack by the Wicked Panda group (APT-41) targeting U.S. state government networks the attack campaign was active between May 2021 and February 2022, in addition to attacks targeting Taiwanese media, the attack chain starts with the in-memory execution of MoonWalk backdoor. Once the MoonWalk backdoor is successfully loaded by DodgeBox, the malware decrypts and reflectively loads two embedded plugins (C2 and Utility). The C2 plugin uses a custom encrypted C2 protocol to communicate with the attacker-controlled Google Drive account.
I relied on zscaler to figure out the details to make this simulation: 
part 1: https://www.zscaler.com/blogs/security-research/dodgebox-deep-dive-updated-arsenal-apt41-part-1

part 2: https://www.zscaler.com/blogs/security-research/moonwalk-deep-dive-updated-arsenal-apt41-part-2

![imageedit_3_7808595478](https://github.com/user-attachments/assets/9e7691fa-0407-409a-bf71-e0f6ea00d19e)

This attack included several stages including DodgeBox, a reflective DLL loader written in C, showcases similarities to StealthVector in terms of concept but incorporates significant improvements in its implementation. It offers various capabilities, including decrypting and loading embedded DLLs, conducting environment checks and bindings, and executing cleanup procedures. What sets DodgeBox apart from other malware is its unique algorithms and techniques.

![imageedit_2_3915351931](https://github.com/user-attachments/assets/1ddd642e-4cd1-4bb5-bfc1-6a8e342d6364)

1. Employs DLL sideloading as a means of executing DodgeBox.
 
2. The malicious DLL, DodgeBox, serves as a loader and is responsible for decrypting a second stage payload from an encrypted DAT file (sbiedll.dat), The decrypted payload, MoonWalk functions as a backdoor.

3. Data exfiltration: over GoogleDrive API C2 Channe, This integrates GoogleDrive API functionality to facilitate communication between the compromised system and the attacker-controlled server thereby potentially hiding the traffic within legitimate GoogleDrive communication.
