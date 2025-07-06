# Famous Chollima APT Adversary Simulation

This is a simulation of attack by (Famous Chollima) APT group targeting job seekers to accomplish their goals and wide variety of United States (US) companies, the attack campaign was active early as December 2022, The attack chain starts with attackers invites the victim to participate in an online interview. The attackers likely uses video conferencing or other online collaboration tools for the interview. During the interview, the attackers convinces the victim to download and install an NPM-based package hosted on GitHub. The threat actor likely presents the package to the victim as software to review or analyze, but it actually contains malicious JavaScript designed to infect the victim’s host with backdoor malware. I relied on paloalto unit42 to figure out the details to make this https://unit42.paloaltonetworks.com/two-campaigns-by-north-korea-bad-actors-target-job-hunters/


![imageedit_4_8185813077](https://github.com/user-attachments/assets/cba7dd9b-d0e8-4b9c-b47a-7c413d8f91e5)


This attack included several stages including During the interview, the threat actor convinces the victim to download and install an NPM-based package hosted on GitHub. The attackers likely presents the package to the victim as software to review or analyze, but it actually contains malicious JavaScript designed to infect the victim’s host with backdoor.

HackerNews: https://thehackernews.com/2023/11/north-korean-hackers-pose-as-job.html


1. Social Engineering Technique: The Attackers attempts to infect software developers with malware through a fictitious job interview.


2. GitHub Abuse for Contagious Interview: Attackers exploited GitHub, a trusted platform used daily by developers, to deliver or distribute malicious packages, leveraging its legitimacy and widespread adoption.


3. NPM-based package hosted on GitHub: Create JavaScript-based payload hidden inside Node Package Manager (NPM) packages. InvisibleFerret is a simple but Python-based backdoor. Both are cross-platform malware that can run on Windows, Linux and macOS.


4. Python backdoor: The component for InvisibleFerret deploys remote control and information stealing capabilities. Once executed, it prepares the environment by installing the  Python packages, if they are not already present on the system.


5. TCP-C2 Server with XOR key: The C2 server returns JSON data instructing the backdoor with the next actions to take. The JSON response contains the same XOR key.  


![word-image-131292-1](https://github.com/user-attachments/assets/b24bee69-1301-4448-b424-052359dd033f)


## The first stage (Social Engineering Technique)

The attackers lure their victims by inviting them to job interviews. In other cases, the attackers themselves apply for jobs using fake identities. They exploit the idea that people are in need of work or are seeking better opportunities, impersonating individuals applying for a position at a company. This is a clever tactic, as exploiting resources is far more valuable than just simply using them.

![word-image-131292-9](https://github.com/user-attachments/assets/dbae96a7-4e40-4d41-a151-e42d85c45da7)





