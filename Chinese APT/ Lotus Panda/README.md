#  Lotus Panda APT Adversary Simulation

This is a simulation of attack by (Lotus Panda) APT group targeting French diplomat based in Taipei (Taiwan) the attack campaign was active from November 10, 2015, The attack chain starts with send a spear-phishing email to an individual at the French Ministry of Foreign Affairs. The subject and the body of the email the targeted individual had been invited to a Science and Technology conference in Hsinchu, Taiwan. The email appears quite timely, as the conference was held on November 13, 2015, which is three days after the attack took place. The email body contained a link to the legitimate registration page for the conference, but the email also had two attachments with the filenames, Both attachments are malicious Word documents that attempt to exploit the Windows OLE Automation Array Remote Code Execution Vulnerability tracked by CVE-2014-6332. Upon successful exploitation, the attachments will install a Trojan named Emissary and open a Word document as a decoy. I relied on paloalto to figure out the details to make this simulation: https://unit42.paloaltonetworks.com/attack-on-french-diplomat-linked-to-operation-lotus-blossom/

![imageedit_1_6240568719](https://github.com/user-attachments/assets/4bba5e4d-879b-4cb7-9cce-d55cdf868033)

1.  Document attachment: send a spear-phishing email with the subject and the body of the email  the targeted individual had been invited to a Science and Technology conference.

## The first stage (delivery technique)

The first attachment opens a decoy document resembling an invitation to a Science and Technology conference held in Hsinchu, Taiwan. The second attachment opens a decoy registration form for attending the same conference. While the event was widely advertised online and on Facebook, this invitation includes a detailed itinerary that does not appear to have been published online.

![imageedit_2_2010643966](https://github.com/user-attachments/assets/104510f6-98dd-4859-9067-180d535bf35a)

The conference was primarily supported by Democratic Progressive Party (DPP) Chairwoman Tsai Ing wen and DPP caucus whip and Hsinchu representative Ker Chien ming, both of whom are longtime political allies. Tsai Ing-wen is the current front runner in the Taiwanese presidential race, and Ker Chien ming is a potential candidate for Speaker of the Legislative Yuan should she win.

![imageedit_3_4252276936](https://github.com/user-attachments/assets/4bfdcbca-7f88-4274-b752-6b96e6f90387)

The conference focused on leveraging open-source technology, international recruitment, and partnerships to further develop Hsinchu as Taiwan's Silicon Valley. It specifically highlighted France as a strategic ally in this effort France is Taiwan’s second largest technology partner and its fourth largest trading partner in Europe.





