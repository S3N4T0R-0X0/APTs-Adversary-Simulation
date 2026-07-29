# Venomous Bear APT Adversary Simulation

This is a simulation of attack by (Venomous Bear) APT group targeting U.S.A, Germany and Afghanista attack campaign was active since at least 2020, The attack chain starts with
installed the backdoor as a service on the infected machine. They attempted to operate under the radar by naming the service "Windows Time Service", like the existing Windows service. The backdoor can upload and execute files or exfiltrate files from the infected system, and the backdoor contacted the command and control (C2) server via an HTTPS encrypted channel every five seconds to check if there were new commands from the operator. I relied on ‏Cisco Talos Intelligence Group‏ tofigure out the details to make this simulation: https://blog.talosintelligence.com/tinyturla/

![imageedit_3_4790485345](https://github.com/S3N4T0R-0X0/Venomous-Bear-APT/assets/121706460/1a56bebb-927d-4286-8257-aa907f240017)

The attackers uses a .BAT file that resembles the Microsoft Windows Time Service, to install the backdoor. The backdoor comes in the form of a service dynamic link library (DLL) called w64time.dll. The description and filename make it look like a valid Microsoft DLL. Once up and running, it allows the attackers to exfiltrate files or upload and execute them, thus functioning as a second-stage postern when needed.

1. BAT file: The attackers used a .bat file similar to the one below to install the backdoor as a harmless-looking fake Microsoft Windows Time service.

2. DLL backdoor: I have developed a simulation of the backdoor that the attackers used in the actual attack.

3. Backdoor Listener: I was here developed a simple listener script that waits for the incoming connection from the backdoor when it is executed on the target machine.



According to what the Cisco team said, they were not able to identify the method by which this backdoor was installed on the victims’ systems.

![Screenshot from 2024-06-09 16-11-23](https://github.com/S3N4T0R-0X0/Venomous-Bear-APT/assets/121706460/3116c5e9-0476-4b93-a672-bc7436abfce0)


## The first stage (.BAT file)

The attackers used a .bat file similar to the one below to install the backdoor as a harmless-looking fake Microsoft Windows Time service, the .bat file is also setting the configuration parameters in the registry the backdoor is using.

![Screenshot from 2024-06-07 19-39-16](https://github.com/S3N4T0R-0X0/Venomous-Bear-APT/assets/121706460/381d1833-3f71-4278-aa56-60952e8d3f55)

I wrote a .bat file identical to the one the attackers used to the one below to install the backdoor as a fake Microsoft Windows Time service.

These commands add various configuration parameters for the W64Time service to the registry. 
   
    reg add "HKLM\SYSTEM\CurrentControlSet\services\W64Time\Parameters" /v ServiceDll /t REG_EXPAND_SZ /d "%SystemRoot%\system32\w64time.dll" /f
    reg add "HKLM\SYSTEM\CurrentControlSet\services\W64Time\Parameters" /v Hosts /t REG_SZ /d "REMOVED 5050" /f
    reg add "HKLM\SYSTEM\CurrentControlSet\services\W64Time\Parameters" /v Security /t REG_SZ /d "<REMOVED>" /f
    reg add "HKLM\SYSTEM\CurrentControlSet\services\W64Time\Parameters" /v TimeLong /t REG_DWORD /d 300000 /f
    reg add "HKLM\SYSTEM\CurrentControlSet\services\W64Time\Parameters" /v TimeShort /t REG_DWORD /d 5000 /f

    
ServiceDll: Specifies the DLL that implements the service.

Hosts: Sets the hosts and port (values removed for security).

Security: Configures security settings (value removed for security).

TimeLong: A time-related setting.
 
TimeShort: Another time-related setting.  


![Screenshot from 2024-06-08 07-18-07](https://github.com/S3N4T0R-0X0/Venomous-Bear-APT/assets/121706460/a1d9236a-12fc-4008-a9a1-0eedb818d0c9)

This means the malware is running as a service, hidden in the svchost.exe process. The DLL's ServiceMain startup function is doing not much more than executing.

## The Second stage (DLL backdoor)

"Here, I have developed a simulation of the backdoor that the attackers used in the actual attack."

First, the backdoor reads its configuration from the registry and saves it in the "result" structure, which is later on assigned to the "sConfig" structure.

![Screenshot from 2024-06-08 22-30-51](https://github.com/S3N4T0R-0X0/Venomous-Bear-APT/assets/121706460/b2164d44-bffd-4c9a-9ebe-574c28104eb0)


This backdoor includes the following components:

1.Service Control Handler: Registers a service control handler to manage the service's state.

2.Main Malware Function: Placeholder for the main logic of the backdoor.

3.Configuration Reading: Initializes the configuration with placeholders for actual values.

4.C2 Command Retrieval: Simulates retrieving commands from a Command and Control (C2) server.

5.Command Processing: Processes the retrieved commands (currently simulated).

6.Service Loop: Continuously connects to the C2 server and processes commands, with error handling and cleanup.

Adjust the placeholder values and add the actual logic for backdoor operations and C2 command processing as per your requirements.

![Screenshot from 2024-06-08 22-34-36](https://github.com/S3N4T0R-0X0/Venomous-Bear-APT/assets/121706460/1f3eb42d-b546-4d32-9166-851f0dd00fa6)

## The third stage (Backdoor Listener)

I was here developed a simple listener script that waits for the incoming connection from the backdoor when it is executed on the target machine.

Accepts incoming connections: When a client connects, it prints the client's IP address and port.

Sends the command: Encodes the command as bytes and sends it over the socket.

Prompts for a command: Asks the user to enter a command to send to the connected client.

Continues reading until no more data is received.

Receives output from the client: Reads data in chunks of 4096 bytes.

Accumulates the data into the output variable.

![Screenshot from 2024-06-09 10-44-07](https://github.com/S3N4T0R-0X0/Venomous-Bear-APT/assets/121706460/41bfa80d-a18a-4bd3-ad6d-f243bd29bece)

## MITRE ATT&CK Techniques

| Technique ID | Technique | Implementation |
|---------------|-----------|----------------|
| **T1059.003** | Command and Scripting Interpreter: Windows Command Shell | Uses a BAT script to install and configure the backdoor on the target system. |
| **T1543.003** | Create or Modify System Process: Windows Service | Installs the backdoor as a Windows service disguised as **Windows Time Service**. |
| **T1574.011** | Hijack Execution Flow: Services Registry Permissions Weakness | Loads the malicious service DLL through the Windows Service Control Manager. |
| **T1573** | Encrypted Channel | Encrypts Command and Control communications over HTTPS. |
| **T1071.001** | Application Layer Protocol: Web Protocols | Uses HTTPS for periodic communication with the C2 server. |
| **T1105** | Ingress Tool Transfer | Supports downloading additional files or payloads from the C2 server. |
| **T1105** | Ingress Tool Transfer | Supports uploading files from the infected host to the operator. |
| **T1112** | Modify Registry | Reads and stores configuration values in the Windows Registry. |
| **T1082** | System Information Discovery | Collects host configuration before entering the command-processing loop. |
| **T1106** | Native API | Interacts with the Windows Service Control Manager through native Windows APIs. |
| **T1041** | Exfiltration Over C2 Channel | Exfiltrates files through the established HTTPS command-and-control channel. |
