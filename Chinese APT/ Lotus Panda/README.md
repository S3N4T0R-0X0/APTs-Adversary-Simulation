#  Lotus Panda APT Adversary Simulation

This is a simulation of attack by (Lotus Panda) APT group targeting French diplomat based in Taipei (Taiwan) the attack campaign was active from November 10, 2015, The attack chain starts with send a spear-phishing email to an individual at the French Ministry of Foreign Affairs. The subject and the body of the email suggest the targeted individual had been invited to a Science and Technology conference in Hsinchu, Taiwan. The e-mail appears quite timely, as the conference was held on November 13, 2015, which is three days after the attack took place. The email body contained a link to the legitimate registration page for the conference, but the email also had two attachments with the filenames, Both attachments are malicious Word documents that attempt to exploit the Windows OLE Automation Array Remote Code Execution Vulnerability tracked by CVE-2014-6332. Upon successful exploitation, the attachments will install a Trojan named Emissary and open a Word document as a decoy. I relied on paloalto to figure out the details to make this simulation: https://unit42.paloaltonetworks.com/attack-on-french-diplomat-linked-to-operation-lotus-blossom/


![imageedit_1_6240568719](https://github.com/user-attachments/assets/4bba5e4d-879b-4cb7-9cce-d55cdf868033)

