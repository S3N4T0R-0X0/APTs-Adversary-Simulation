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



## Third Stage (Telegram-based Agent)

The initial objective of this stage is to enhance the realism of the adversary simulation by replacing the traditional direct command and control communication channel with a Telegram-based communication layer. Instead of requiring operators to interact with the payload through a dedicated control server, commands are exchanged through a Telegram bot, allowing the simulation to emulate an alternative communication workflow commonly observed in modern threat campaigns.

This phase focuses on demonstrating how a trusted cloud messaging platform can serve as an intermediary communication channel between the operator and the simulated implant. By leveraging Telegram as the transport layer, the simulation highlights how legitimate online services may be used to blend command and control traffic with normal network activity while maintaining reliable bidirectional communication.

<img width="1366" height="518" alt="Screenshot From 2026-08-02 04-42-05" src="https://github.com/user-attachments/assets/4daf915f-b729-4619-b3e2-a53e877ceab8" />


The operator communicates with the Telegram bot by sending commands through a private Telegram chat. The bot acts as the communication gateway, relaying operator instructions to the simulated implant and returning execution results through the same encrypted channel. From the operator's perspective, Telegram becomes the primary interface for tasking the implant, while the underlying communication remains transparent to the simulation workflow.

This enhancement demonstrates an alternative command and control architecture that relies on widely used cloud messaging infrastructure instead of dedicated C2 servers, enabling defenders to better understand, analyze, and detect communication patterns associated with cloud-based adversary simulations.

<img width="968" height="172" alt="Screenshot From 2026-08-02 04-50-00" src="https://github.com/user-attachments/assets/8fc7216b-3efe-4be0-bd01-0af313fac92c" />


---

To simulate the communication workflow observed during the analysis, I created a dedicated Telegram bot using **BotFather**, Telegram's official bot management service. Unlike traditional command and control infrastructures that depend on dedicated servers, fixed IP addresses, or custom domains, this simulation communicates exclusively through the Telegram Bot API over encrypted HTTPS connections.

Following the same communication model, I configured the bot with the **Display Name** `Olalampo` and the **Username** `stager_51_bot`. The bot token generated by BotFather serves as the authentication mechanism, allowing BEAR-C2 to establish communication with the Telegram Bot API without requiring a dedicated C2 server. This approach demonstrates how cloud messaging platforms can be incorporated into adversary simulations to emulate alternative command and control architectures.

<img width="2423" height="1920" alt="image_(1)" src="https://github.com/user-attachments/assets/96e06e84-3208-4b2c-935a-b48ca7dd0620" />


`Telegram API Setup`

To enable BEAR-C2 to communicate with the Telegram account controlling the bot, I generated a Telegram **API ID** and **API Hash** through Telegram's developer portal. After registering a new application, these credentials were configured inside BEAR-C2, allowing the framework to authenticate and interact with the Telegram account used throughout the simulation.


`CHAR.cpp Configuration`

The final stage consisted of configuring the Telegram communication module inside **CHAR.cpp**. The **Bot Token** generated by BotFather was embedded into the agent, allowing it to authenticate with the Telegram Bot API and establish bidirectional communication with the operator.

<img width="898" height="122" alt="image_(3)" src="https://github.com/user-attachments/assets/2a92081e-6158-4174-846e-e908e1074255" />

Since Telegram limits a single text message to **4096 characters**, the maximum message size was configured to **4000 characters** to provide a safe transmission margin. Whenever a command produces output larger than the configured limit, **CHAR.cpp** automatically splits the data into multiple sequential messages before transmitting them. This mechanism ensures reliable delivery of large command outputs while remaining fully compatible with the Telegram Bot API and maintaining continuous communication throughout the simulation.


---

**`main()`:**
Serves as the entry point of the payload and initializes the entire agent. After startup, it continuously communicates with the configured Telegram bot, periodically checking for new operator messages. Every incoming command is processed through the command dispatcher, while responses are returned back to the operator through the same communication channel. This continuous polling mechanism allows the agent to remain responsive throughout its execution.

<img width="1365" height="534" alt="Screenshot From 2026-08-02 03-58-13" src="https://github.com/user-attachments/assets/c8d0f5fe-12eb-4492-9fc1-745bba50c6ca" />


**`get_updates`:**
Acts as the communication receiver for the agent. It establishes secure HTTPS connections using the Windows WinHTTP API and continuously polls the Telegram Bot API for new messages. Each received update is parsed from JSON format before being forwarded to the message processing routine, allowing the payload to receive operator instructions remotely.

<img width="1365" height="577" alt="Screenshot From 2026-08-02 03-59-31" src="https://github.com/user-attachments/assets/c48d3675-b0b4-4089-ae9e-5fe4a388f229" />


**`process_message`:**
Functions as the command processor responsible for handling operator requests. It reconstructs multi-part messages, processes encoded commands when required, and forwards completed commands to the execution engine. Once execution is complete, large outputs are automatically divided into smaller chunks before being transmitted back to the operator, allowing lengthy responses to be delivered reliably through Telegram.

<img width="1365" height="577" alt="Screenshot From 2026-08-02 04-00-36" src="https://github.com/user-attachments/assets/83ab16a7-89db-49f0-84d9-ad7ccc252b7d" />

**`execute_command`:**
Serves as the central dispatcher for all incoming tasks. It determines which internal functionality should be used based on the received instruction and routes the request to the appropriate component. Depending on the command type, it can invoke built-in file management capabilities, directory browsing routines, or the command execution engine before returning the collected results to the communication layer.

<img width="1365" height="577" alt="Screenshot From 2026-08-02 04-02-08" src="https://github.com/user-attachments/assets/24164556-9087-4b96-abdc-93729ca3f258" />


**`run_command`:**
Acts as the local command execution engine. It creates hidden child processes, captures their standard output, and returns the generated results to the agent. The collected output is then passed back through the communication channel, allowing the operator to receive execution results remotely.

<img width="1366" height="517" alt="Screenshot From 2026-08-02 04-04-19" src="https://github.com/user-attachments/assets/8ff8e5b7-88d7-4062-aa13-29b6e1fa4933" />


**`browse_directory`:**
Provides remote directory enumeration capabilities by traversing the requested folder and collecting information about its contents. The gathered information is organized into structured JSON data before being returned to the communication layer, allowing the operator to remotely inspect directory structures.



<img width="1241" height="133" alt="Screenshot From 2026-08-02 04-06-11" src="https://github.com/user-attachments/assets/7fcf02f5-fcc7-40f2-9a94-ead6109ecc7e" />


**`download_file`, `upload_file`, `delete_file`, and `rename_file`:**
Together these routines provide the agent's file management capabilities. They are responsible for handling file transfer operations, filesystem modifications, and basic file management tasks. Data exchanged during file transfers is encoded to ensure it can be transmitted safely through the communication channel before being reconstructed on the receiving side.

<img width="1340" height="576" alt="Screenshot From 2026-08-02 04-07-54" src="https://github.com/user-attachments/assets/61cab43d-3762-4e01-891f-061f23921a5a" />



**`base64_encode`, `base64_decode`, and `decode_base64_command`:**
These helper functions provide the encoding and decoding mechanisms used by the agent. They convert data between its original binary representation and Base64 format, allowing commands, files, and structured data to be safely transmitted through the messaging channel while supporting message reconstruction for larger payloads.


<img width="1340" height="526" alt="Screenshot From 2026-08-02 04-10-13" src="https://github.com/user-attachments/assets/d09db77e-eb61-4c79-907c-b5a382a3c65e" />


**`send_telegram_message`:**
Acts as the outbound communication component of the agent. It formats execution results, prepares outgoing messages, and sends them back to the configured Telegram chat using HTTPS requests through the Telegram Bot API. Large outputs generated by the payload are transmitted in multiple parts to accommodate Telegram message size limitations.



<img width="1246" height="529" alt="Screenshot From 2026-08-02 04-11-00" src="https://github.com/user-attachments/assets/4401869e-41e1-4dc2-a576-e33cb47612e4" />

