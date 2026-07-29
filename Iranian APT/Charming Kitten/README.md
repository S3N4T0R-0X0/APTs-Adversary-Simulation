# Charming Kitten APT Adversary Simulation


This is a simulation of an attack by the APT group **Charming Kitten**, targeting multiple sectors including government, military, and critical infrastructure across the Middle East. The group’s targeting has expanded beyond government entities to include the maritime, aviation, and financial sectors, reflecting a growing interest in regional logistics and critical economic infrastructure.

<img width="696" height="433" alt="imageedit_6_4923792703" src="https://github.com/user-attachments/assets/cc8c6472-7ea5-455b-9303-29b84ad3fab7" />



Recent campaigns (Operation Olalampo) have targeted entities in Egypt, Saudi Arabia, the UAE, Turkey, Hungary, Turkmenistan, Israel, and South America. These attacks demonstrate the group’s ability to pivot between sectors while conducting multiple operations simultaneously. The campaign was active throughout 2025 and 2026. This simulation is based on research from 

Palo Alto Unit 42:
[https://unit42.paloaltonetworks.com/boggy-serpens-threat-assessment/](https://unit42.paloaltonetworks.com/boggy-serpens-threat-assessment/)

and Group-IB: https://www.group-ib.com/blog/muddywater-operation-olalampo/

<img width="1262" height="700" alt="word-image-341009-175304-1-1262x700" src="https://github.com/user-attachments/assets/78362455-3c93-4a60-88ec-6132d407f166" />

In this campaign, the blurred document lure delivers a new payload  as a custom HTTP backdoor. To maintain persistence, the group has evolved its development approach by leveraging AI-generated code and Rust-based tools, such as the BlackBeard backdoor, to rapidly deploy custom implants. Additionally, the group uses standard HTTP status codes, customized UDP based traffic, and the Telegram API for C2 communications.

<img width="689" height="578" alt="Screenshot 2026-05-03 at 17-06-02 Operation Olalampo Inside MuddyWater’s Latest Campaign Group-IB Blog" src="https://github.com/user-attachments/assets/dc9ea0ba-0c97-42ff-af21-fb6ab4f8c28c" />

## The first stage (delivery technique)

The initial campaign targeted project engineers using industry-specific terminology for subsea pipelines. The lure document was blurred in order to deceive targets into clicking “Enable Content” thereby triggering the execution of the embedded macro.


<img width="1177" height="700" alt="word-image-353730-175304-5-1177x700" src="https://github.com/user-attachments/assets/3c5ea612-6b8c-4b75-bca0-f0df1a23c0d0" />


In another wave the attack group delivered a malicious Excel and Word files designed to mimic the target’s internal financial records. The lure appeared as a legitimate spreadsheet containing payment details and cash flow projections.

The infected document included specific references to “Engineering, Construction & Marine Services” and used local currency (AED), along with realistic transaction descriptions such as “Payroll Payments via WPS” making it highly convincing to the target.

<img width="897" height="483" alt="Screenshot 2026-05-03 at 05-44-34 Boggy Serpens Threat Assessment" src="https://github.com/user-attachments/assets/b7e58230-ed40-4061-9896-a992c7e7336b" />

• Exploiting Trusted Relationships for Payload Delivery

Throughout the past year, Charming Kitten systematically abused trusted relationships by hijacking official government and corporate email accounts to bypass traditional email security controls and filtering mechanisms. This technique was observed in more than 15 targeted operations conducted across multiple regions worldwide.

In August 2025, the group leveraged a compromised mailbox belonging to the Omani Ministry of Foreign Affairs to distribute malicious documents to foreign ministries and diplomatic entities in several countries. The delivered files were disguised as legitimate diplomatic communications and official government correspondence, increasing the likelihood of recipient interaction and trust.

Following the regional conflicts in June 2025, the group also launched a themed phishing campaign using a “Sustainable Peace” seminar invitation as a lure. The invitation was crafted to appear as a legitimate geopolitical or diplomatic event, aiming to encourage engagement from targeted individuals within government, policy, and international relations sectors.

These operations demonstrate the group’s continued reliance on social engineering, trusted relationship abuse, and geopolitical themes to enhance credibility and improve payload delivery success rates.


<img width="822" height="322" alt="Screenshot 2026-05-11 at 15-30-46 Boggy Serpens Threat Assessment" src="https://github.com/user-attachments/assets/bedf84ba-db09-4ab1-9a69-99dc353dc02f" />

## The second stage (Malicious VBA Macro with Conditional Download and Execution)


A special thanks to **[Mohamed Montaser](https://www.linkedin.com/in/m-montaser?utm_source=share_via&utm_content=profile&utm_medium=member_android)** for his valuable contribution to this adversary simulation project. He was responsible for developing and refining the VBA macro components used throughout this simulation. His expertise and contributions played an important role in making this project possible.

**Sub `love_me_____()`:**
Acts as the primary controller for the entire macro, coordinating the payload workflow from start to finish. The routine begins by decoding an obfuscated file path from a hexadecimal string, concealing the actual location until runtime to reduce static detection. It then checks whether the target payload already exists on the system. If the file is found, the macro immediately transfers execution to the execution routine. If the file is not present, it invokes the download routine to retrieve the payload before executing it. This conditional logic allows the macro to support both first-time infections and subsequent executions without downloading the payload multiple times.

<img width="1366" height="768" alt="Screenshot From 2026-07-29 06-57-50" src="https://github.com/user-attachments/assets/a45fd121-8a27-4a51-b858-1873db6b8454" />

**Sub `DownloadAndRun`:**
Responsible for retrieving the payload when it is not already available on the system. The function decodes a hexadecimal-encoded URL at runtime and uses the Windows HTTP API (`WinHttp.WinHttpRequest`) to request the remote file. If the download succeeds, the response is written to disk in binary format at the previously decoded file path. Once the file has been successfully saved, the function immediately invokes the execution routine, allowing the newly downloaded payload to run without requiring additional user interaction.


<img width="1366" height="768" alt="Screenshot From 2026-07-29 06-58-52" src="https://github.com/user-attachments/assets/22d58511-7be2-495b-a238-b6efa81f6c58" />


**Function `DecodeHex`:**
Provides the string deobfuscation mechanism used throughout the macro. Rather than storing sensitive strings such as file paths or network locations in plain text, the macro represents them as hexadecimal data and reconstructs them only when needed. This technique is commonly used to obscure static indicators and reduce the visibility of embedded strings during basic inspection.


<img width="1366" height="768" alt="Screenshot From 2026-07-29 06-59-52" src="https://github.com/user-attachments/assets/d73ae561-541d-43cc-9343-8c151d1d499f" />



**Sub `ExecuteFile`:**
Serves as the execution component of the macro. After receiving the target file path, it attempts to launch the payload using multiple execution methods. The routine first executes the file through the VBA `Shell` function with a hidden window and then invokes a secondary execution method through Windows Script Host, providing an alternative launch mechanism. Using more than one execution technique increases execution reliability if one method encounters an error.

<img width="1366" height="768" alt="Screenshot From 2026-07-29 07-00-51" src="https://github.com/user-attachments/assets/e01c1101-90e9-4730-9e63-e7dd9b103b6f" />

**Sub `AutoOpen`:**
Acts as the automatic entry point for the macro. When the Office document is opened and macros are enabled, this procedure is triggered automatically and immediately calls `love_me_____()`, initiating the complete workflow without requiring any further interaction from the user.

https://github.com/user-attachments/assets/ac4ea6ce-da0f-4e0a-a20f-fcc6c13e3eab





