# Charming Kitten APT Adversary Simulation

This is a simulation of attack by (Charming Kitten / APT35 / TA455 / Smoke Sandstorm) APT group targeting the aerospace, aviation, and defense industries across the Middle East, specifically Israel and the UAE, as well as possibly Turkey, India, and Albania. The campaign uses a fake job recruiting website called "Careers 2 Find" and a "Dream Job" social engineering lure to deliver "SnailResin" malware. The attack campaign has been active since at least September 2023 and was analyzed in late 2024. The attackers have relied on LinkedIn spoofing, ZIP archive delivery, and DLL Side-Loading for initial access and execution. The introduction of the SnailResin malware via a fake recruitment site represents a notable evolution toward high-level obfuscation, leveraging trusted platforms, and blending into legitimate cloud traffic. I relied on ClearSky Cyber Security to figure out the details to make this simulation: https://www.clearskysec.com/wp-content/uploads/2024/11/Iranian-Dream-Job-ver1.pdf


<img width="1280" height="796" alt="photo_2026-10-21_07-19-16" src="https://github.com/user-attachments/assets/ec640010-fd3b-448b-a63e-a38163c223bd" />


Throughout, the malware hides behind legitimate applications and cloud services to dodge static and dynamic detection, and the overall chain emphasizes low-noise execution with strong anti-analysis tricks built into the payload. This progression — LinkedIn recruiter lure → fake job website (careers2find[.]com) → malicious ZIP (SignedConnection[.]zip) → DLL Side-Loading (secur32.dll) → SnailResin malware → cloud-based C2 — shows Charming Kitten's shift toward more resilient, custom-built modular tooling and deliberate misattribution (mimicking North Korean Lazarus) in recent campaigns targeting aerospace and defense sectors.

![imageedit_3_7338603847](https://www.clearskysec.com/wp-content/uploads/2024/11/Iranian-Dream-Job-ver1.pdf)

**1. Delivery Technique:** Create a fake recruitment website (`careers2find[.]com`) and a LinkedIn recruiter profile to spear-phish targets in the aerospace industry, offering a "Dream Job" that leads them to download a malicious ZIP file (`SignedConnection[.]zip`).

**2. Malicious ZIP & DLL Side-Loading:** The ZIP file contains a legitimate application (`SignedConnection.exe`) alongside a malicious DLL (`secur32.dll`). When the executable is run, Windows DLL search order hijacking causes the malicious `secur32.dll` to be loaded instead of the legitimate one.

**3. SnailResin Malware:** The loaded DLL executes the SnailResin malware, which performs IP address discovery (via `api[.]ipify[.]org`) and fetches encoded C2 server information from a compromised GitHub account (`msdndegsupport`), hiding its infrastructure within legitimate cloud traffic.

**4. C2 infrastructure:** relies on GitHub (raw.githubusercontent.com) and Cloudflare to hide the real C2 servers. The malware uses ASCII-encoded data embedded in a README file to retrieve C2 configuration, communicating with domains like `xboxapicenter[.]com` and resolving to IPs like `89.221.225[.]249`.

---

## The first stage Kill Chain (delivery technique)

The attack kicks off with a spear-phishing campaign via LinkedIn. The threat actor TA455 creates fake recruiter profiles (e.g., "Tess Visser," "Adira Johnson," "Manon Roux") associated with a fabricated company called "Careers 2 Find." These profiles approach targets in the aerospace and aviation industries with a seemingly legitimate job offer for roles like "Aircraft Systems Design Manager."

The fake website `careers2find[.]com` is created, hiding its real IP under Cloudflare. The site contains a legitimate-looking job posting. When the victim engages, they are directed to download a PDF instruction file and a ZIP archive named `SignedConnection[.]zip`. The PDF provides step-by-step instructions on how to "safely" access the site, ensuring infection by guiding the user to extract the ZIP and run the `SignedConnection.exe` application.

![Fake Job Posting](https://www.clearskysec.com/wp-content/uploads/2024/11/Iranian-Dream-Job-ver1.pdf)

## The second stage (Malicious ZIP and DLL Side-Loading)

**SignedConnection[.]zip Analysis:**
The ZIP file contains a mix of legitimate and malicious files. The main executable is `SignedConnection.exe`, which acts as a front-end GUI displaying a "Careers2Find" login prompt. Alongside it are legitimate DLLs (`msvcp140.dll`, `vcruntime140.dll`, etc.) and a malicious `secur32.dll`.

**Execution Flow:**
1. The victim extracts the ZIP and runs `SignedConnection.exe`.
2. The executable loads `secur32.dll` via DLL side-loading (search order hijacking).
3. The legitimate `secur32.dll` is replaced with a malicious version.
4. The malicious DLL executes the SnailResin malware in the context of the trusted `SignedConnection.exe` process.

![Malicious ZIP Content](https://www.clearskysec.com/wp-content/uploads/2024/11/Iranian-Dream-Job-ver1.pdf)

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

## Infrastructure Analysis

- **Domain:** `careers2find[.]com` (created 4 months prior, hidden under Cloudflare, previously resolved to `185.186.244.130` - Webzilla B.V.)
- **C2 Domains:** `xboxapicenter[.]com`, `xboxplayservice[.]com`
- **C2 IPs:** `89.221.225[.]249`, and 20+ other IPs in the `89.221.225.0/24` range (AS44477 - STARK-INDUSTRIES)
- **GitHub Account:** `msdndegsupport`
- **SSL Certificate:** Common name `localhost`, Issuer `localhost`, SHA1: `21b0327e7ccb36d9ba00359e078acaa9a2320c83`

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

