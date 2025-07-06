# Famous Chollima APT Adversary Simulation

This is a simulation of attack by (Famous Chollima) APT group targeting job seekers to accomplish their goals and wide variety of United States (US) companies, the attack campaign was active early as December 2022, The attack chain starts with attackers invites the victim to participate in an online interview. The attackers likely uses video conferencing or other online collaboration tools for the interview. During the interview, the attackers convinces the victim to download and install an NPM-based package hosted on GitHub. The threat actor likely presents the package to the victim as software to review or analyze, but it actually contains malicious JavaScript designed to infect the victim’s host with backdoor malware. I relied on paloalto unit42 to figure out the details to make this https://unit42.paloaltonetworks.com/two-campaigns-by-north-korea-bad-actors-target-job-hunters/


![imageedit_4_8185813077](https://github.com/user-attachments/assets/cba7dd9b-d0e8-4b9c-b47a-7c413d8f91e5)


This attack included several stages including During the interview, the attackers convinces the victim to download and install an NPM-based package hosted on GitHub. The attackers likely presents the package to the victim as software to review or analyze, but it actually contains malicious JavaScript designed to infect the victim’s host with backdoor.

HackerNews: https://thehackernews.com/2023/11/north-korean-hackers-pose-as-job.html


1. Social Engineering Technique: The Attackers attempts to infect software developers with malware through a fictitious job interview.


2. GitHub Abuse (Supply-Chain): The Attackers exploited GitHub, a trusted platform used daily by developers, to deliver or distribute malicious packages, leveraging its legitimacy and widespread adoption. When developers clone and run the project, the malware executes in their environment.


3. NPM-based package hosted on GitHub: Create obfuscated JavaScript-based payload hidden inside Node Package Manager (NPM) packages. InvisibleFerret is a simple but Python-based backdoor. Both are cross-platform malware that can run on Windows, Linux and macOS. 


4. Python backdoor: The component for InvisibleFerret deploys remote control and information stealing capabilities. Once executed, it prepares the environment by installing the  Python packages, if they are not already present on the system.


5. TCP-C2 Server with XOR key: The C2 server returns JSON data instructing the backdoor with the next actions to take. The JSON response contains the same XOR key.  


![word-image-131292-1](https://github.com/user-attachments/assets/b24bee69-1301-4448-b424-052359dd033f)


## The first stage (Social Engineering Technique)

The attackers lure their victims by inviting them to job interviews. In other cases, the attackers themselves apply for jobs using fake identities. They exploit the idea that people are in need of work or are seeking better opportunities, impersonating individuals applying for a position at a company. This is a clever tactic, as exploiting resources is far more valuable than just simply using them.


![20250706_092112-Picsart-AiImageEnhancer](https://github.com/user-attachments/assets/d6088760-f8ba-4b34-99f9-634b35cbc98c)



![word-image-131292-13](https://github.com/user-attachments/assets/62200e9d-d953-4e9a-83d2-e5d244c4f4df)


## The second stage (delivery technique GitHub-Abuse)

The attackers took advantage of the fact that their victims were part of the software development and IT community, possessing technical expertise and regularly working with GitHub. At the same time, using an open source project during a technical interview doesn’t seem unusual. Asking the victim to share their screen and test some code to assess their technical skills appeared to be a reasonable and clever tactic, especially when targeting victims from the IT field.

![Screenshot From 2025-07-06 04-26-33](https://github.com/user-attachments/assets/95ddb76e-6a1c-4369-a3f2-89a2fe8c4ae2)

However, in some of the repositories created by the attackers, they forgot to disable comments on the project. As a result, some users and security researchers discovered the malicious technique and left comments on the repository warning that it contained malware and should not be used. This mistake was not identified early enough. Additionally, there were other repositories where the attackers should have deleted the comments after uploading the malicious code.

![word-image-131292-3](https://github.com/user-attachments/assets/9813a643-f29b-4969-b3ef-04772bcfe5ce)

## The third stage (implanting technique NPM-package)

The attackers created an NPM package that, in turn, executes obfuscated JavaScript code, `You can use these commands to create the NPM package.json file`.

```
sudo apt-get install npm

mkdir my-malicious-package
cd my-malicious-package
npm init -y

```
![Screenshot From 2025-07-06 05-14-44](https://github.com/user-attachments/assets/750f5360-e40c-4cec-b86c-8d03b388efb8)

Now i will create the JavaScript file using the command `touch payload.js`. Then, i paste the following code inside it, which contains only the `whoami` command just to test that everything is working correctly before adding the main payload and obfuscate Java Script code.

![Screenshot From 2025-07-06 05-42-21](https://github.com/user-attachments/assets/c0904b9f-2bc2-4718-8499-4e52280aeae9)

## The fourth stage (Python Backdoor)

The attackers created a simple payload that performs two main tasks:

![Screenshot From 2025-07-06 05-53-52](https://github.com/user-attachments/assets/97bf917c-b59d-48ef-843d-b991096c2997)


1.The first task is establishing a connection to a C2 server over TCP with XOR encryption.

2.The second task is stealing credentials from the victim's browser.

![Screenshot From 2025-07-06 06-00-57](https://github.com/user-attachments/assets/556f4607-9445-4c33-8f89-34a14dce1fa1)

This Python payload creates a reverse TCP shell that connects to a command-and-control (C2) server.

1. The script imports libraries for socket communication, subprocess execution, base64 encoding, and web browser interaction.

2. It defines XOR encryption/decryption functions to secure data exchange with a hardcoded key.

3. Upon execution, it opens url in a web browser and establishes a TCP connection to a specified C2 server (ip:port).

4. The script authenticates with the server, receives encrypted commands, executes them locally, and sends back encrypted results.

![Screenshot From 2025-07-06 10-07-50](https://github.com/user-attachments/assets/ae59d553-e8bd-4ce9-bd6c-be382d471c99)


## The fifth stage (execution technique with obfuscated JavaScript-based payload hidden inside NPM)

Now i will replace the whoami command with the actual payload inside the JavaScript file and obfuscate it using BEAR-C2.

![Screenshot From 2025-07-06 10-37-49](https://github.com/user-attachments/assets/6ce771c3-1637-43bb-8486-aabf5bcc55a2)


Now I will open the obfuscation tool included in BEAR-C2, select the JavaScript file to obfuscate it, then upload the payload to GitHub and begin the Command and Control operation.

![Screenshot From 2025-07-06 10-24-57](https://github.com/user-attachments/assets/8dee00be-5d26-4852-8560-d01c7b5def27)

The JavaScript file is now obfuscated using the built-in obfuscation tool provided by BEAR-C2.

![Pasted image](https://github.com/user-attachments/assets/e6074066-3e1a-4557-aba2-877e1452f5a7)

It's important to ensure that the payload file has the same name as defined inside the NPM package before uploading it to GitHub.

![IMG_20250706_113049_180](https://github.com/user-attachments/assets/1f89cf15-055d-4e82-93aa-0e267874ca81)




